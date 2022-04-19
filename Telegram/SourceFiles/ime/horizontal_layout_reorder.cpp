// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#include "ime/horizontal_layout_reorder.h"

#include "ime/horizontal_layout.h"
#include "styles/style_basic.h"

#include <QtGui/QtEvents>
#include <QtWidgets/QApplication>

namespace IME {

namespace {

constexpr auto kScrollFactor = 0.05;

} // namespace

HorizontalLayoutReorder::HorizontalLayoutReorder(
	not_null<HorizontalLayout*> layout,
	not_null<Ui::ScrollArea*> scroll)
: _layout(layout)
, _scroll(scroll)
, _scrollAnimation([=] { updateScrollCallback(); }) {
}

HorizontalLayoutReorder::HorizontalLayoutReorder(not_null<HorizontalLayout*> layout)
: _layout(layout) {
}

void HorizontalLayoutReorder::cancel() {
	if (_currentWidget) {
		cancelCurrent(indexOf(_currentWidget));
	}
	_lifetime.destroy();
	for (auto i = 0, count = _layout->count(); i != count; ++i) {
		_layout->setHorizontalShift(i, 0);
	}
	_entries.clear();
}

void HorizontalLayoutReorder::start() {
	const auto count = _layout->count();
	if (count < 2) {
		return;
	}
	for (auto i = 0; i != count; ++i) {
		const auto widget = _layout->widgetAt(i);
		widget->events(
		) | rpl::start_with_next_done([=](not_null<QEvent*> e) {
			switch (e->type()) {
			case QEvent::MouseMove:
				mouseMove(
					widget,
					static_cast<QMouseEvent*>(e.get())->globalPos());
				break;
			case QEvent::MouseButtonPress:
				mousePress(
					widget,
					static_cast<QMouseEvent*>(e.get())->button(),
					static_cast<QMouseEvent*>(e.get())->globalPos());
				break;
			case QEvent::MouseButtonRelease:
				mouseRelease(static_cast<QMouseEvent*>(e.get())->button());
				break;
			}
		}, [=] {
			cancel();
		}, _lifetime);
		_entries.push_back({ widget });
	}
}

void HorizontalLayoutReorder::mouseMove(
		not_null<Ui::RpWidget*> widget,
		QPoint position) {
	if (_currentWidget != widget) {
		return;
	} else if (_currentState != State::Started) {
		checkForStart(position);
	} else {
		updateOrder(indexOf(_currentWidget), position);
	}
}

void HorizontalLayoutReorder::checkForStart(QPoint position) {
	const auto shift = position.y() - _currentStart;
	const auto delta = QApplication::startDragDistance();
	if (std::abs(shift) <= delta) {
		return;
	}
	_currentWidget->raise();
	_currentState = State::Started;
	_currentStart += (shift > 0) ? delta : -delta;

	const auto index = indexOf(_currentWidget);
	_currentDesiredIndex = index;
	_updates.fire({ _currentWidget, index, index, _currentState });

	updateOrder(index, position);
}

void HorizontalLayoutReorder::updateOrder(int index, QPoint position) {
	const auto shift = position.x() - _currentStart;
	auto &current = _entries[index];
	current.shiftAnimation.stop();
	current.shift = current.finalShift = shift;
	_layout->setHorizontalShift(index, shift);

	checkForScrollAnimation();

	const auto count = _entries.size();
	const auto currentWidth = current.widget->width();
	const auto currentMiddle = current.widget->x() + currentWidth / 2;
	_currentDesiredIndex = index;
	if (shift > 0) {
		auto left = current.widget->x() - shift;
		for (auto next = index + 1; next != count; ++next) {
			const auto &entry = _entries[next];
			left += entry.widget->width();
			if (currentMiddle < left) {
				moveToShift(next, 0);
			} else {
				_currentDesiredIndex = next;
				moveToShift(next, -currentWidth);
			}
		}
		for (auto prev = index - 1; prev >= 0; --prev) {
			moveToShift(prev, 0);
		}
	} else {
		for (auto next = index + 1; next != count; ++next) {
			moveToShift(next, 0);
		}
		for (auto prev = index - 1; prev >= 0; --prev) {
			const auto &entry = _entries[prev];
			if (currentMiddle >= entry.widget->x() - entry.shift + currentWidth) {
				moveToShift(prev, 0);
			} else {
				_currentDesiredIndex = prev;
				moveToShift(prev, currentWidth);
			}
		}
	}
}

void HorizontalLayoutReorder::mousePress(
		not_null<Ui::RpWidget*> widget,
		Qt::MouseButton button,
		QPoint position) {
	if (button != Qt::LeftButton) {
		return;
	}
	cancelCurrent();
	_currentWidget = widget;
	_currentStart = position.x();
}

void HorizontalLayoutReorder::mouseRelease(Qt::MouseButton button) {
	if (button != Qt::LeftButton) {
		return;
	}
	finishReordering();
}

void HorizontalLayoutReorder::cancelCurrent() {
	if (_currentWidget) {
		cancelCurrent(indexOf(_currentWidget));
	}
}

void HorizontalLayoutReorder::cancelCurrent(int index) {
	Expects(_currentWidget != nullptr);

	if (_currentState == State::Started) {
		_currentState = State::Cancelled;
		_updates.fire({ _currentWidget, index, index, _currentState });
	}
	_currentWidget = nullptr;
	for (auto i = 0, count = int(_entries.size()); i != count; ++i) {
		moveToShift(i, 0);
	}
}

void HorizontalLayoutReorder::finishReordering() {
	if (_scroll) {
		_scrollAnimation.stop();
	}
	finishCurrent();
}

void HorizontalLayoutReorder::finishCurrent() {
	if (!_currentWidget) {
		return;
	}
	const auto index = indexOf(_currentWidget);
	if (_currentDesiredIndex == index || _currentState != State::Started) {
		cancelCurrent(index);
		return;
	}
	const auto result = _currentDesiredIndex;
	const auto widget = _currentWidget;
	_currentState = State::Cancelled;
	_currentWidget = nullptr;

	auto &current = _entries[index];
	const auto width = current.widget->width();
	if (index < result) {
		auto sum = 0;
		for (auto i = index; i != result; ++i) {
			auto &entry = _entries[i + 1];
			const auto widget = entry.widget;
			entry.deltaShift += width;
			updateShift(widget, i + 1);
			sum += widget->width();
		}
		current.finalShift -= sum;
	} else if (index > result) {
		auto sum = 0;
		for (auto i = result; i != index; ++i) {
			auto &entry = _entries[i];
			const auto widget = entry.widget;
			entry.deltaShift -= width;
			updateShift(widget, i);
			sum += widget->width();
		}
		current.finalShift += sum;
	}
	if (!(current.finalShift + current.deltaShift)) {
		current.shift = 0;
		_layout->setHorizontalShift(index, 0);
	}
	base::reorder(_entries, index, result);
	_layout->reorderColumns(index, _currentDesiredIndex);
	for (auto i = 0, count = int(_entries.size()); i != count; ++i) {
		moveToShift(i, 0);
	}

	_updates.fire({ widget, index, result, State::Applied });
}

void HorizontalLayoutReorder::moveToShift(int index, int shift) {
	auto &entry = _entries[index];
	if (entry.finalShift + entry.deltaShift == shift) {
		return;
	}
	const auto widget = entry.widget;
	entry.shiftAnimation.start(
		[=] { updateShift(widget, index); },
		entry.finalShift,
		shift - entry.deltaShift,
		st::slideWrapDuration);
	entry.finalShift = shift - entry.deltaShift;
}

void HorizontalLayoutReorder::updateShift(
		not_null<Ui::RpWidget*> widget,
		int indexHint) {
	Expects(indexHint >= 0 && indexHint < _entries.size());

	const auto index = (_entries[indexHint].widget == widget)
		? indexHint
		: indexOf(widget);
	auto &entry = _entries[index];
	entry.shift = base::SafeRound(
		entry.shiftAnimation.value(entry.finalShift)
	) + entry.deltaShift;
	if (entry.deltaShift && !entry.shiftAnimation.animating()) {
		entry.finalShift += entry.deltaShift;
		entry.deltaShift = 0;
	}
	_layout->setHorizontalShift(index, entry.shift);
}

int HorizontalLayoutReorder::indexOf(not_null<Ui::RpWidget*> widget) const {
	const auto i = ranges::find(_entries, widget, &Entry::widget);
	Assert(i != end(_entries));
	return i - begin(_entries);
}

auto HorizontalLayoutReorder::updates() const -> rpl::producer<Single> {
	return _updates.events();
}

void HorizontalLayoutReorder::updateScrollCallback() {
	if (!_scroll) {
		return;
	}
	const auto delta = deltaFromEdge();
	const auto oldTLeft = _scroll->scrollLeft();
	// _scroll->scrollToX(oldTLeft + delta);
	const auto newLeft = _scroll->scrollLeft();

	_currentStart += oldTLeft - newLeft;
	if (newLeft == 0 || newLeft == _scroll->scrollTopMax()) {
		_scrollAnimation.stop();
	}
}

void HorizontalLayoutReorder::checkForScrollAnimation() {
	if (!_scroll || !deltaFromEdge() || _scrollAnimation.animating()) {
		return;
	}
	_scrollAnimation.start();
}

int HorizontalLayoutReorder::deltaFromEdge() {
	Expects(_currentWidget != nullptr);
	Expects(_scroll);

	const auto globalPosition = _currentWidget->mapToGlobal(QPoint(0, 0));
	const auto localLeft = _scroll->mapFromGlobal(globalPosition).x();
	const auto localRight = localLeft
		+ _currentWidget->width()
		- _scroll->width();

	const auto isLeftEdge = (localLeft < 0);
	const auto isRightEdge = (localRight > 0);
	if (!isLeftEdge && !isRightEdge) {
		_scrollAnimation.stop();
		return 0;
	}
	return int((isRightEdge ? localRight : localLeft) * kScrollFactor);
}

} // namespace IME
