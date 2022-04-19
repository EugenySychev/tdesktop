// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#include "ime/horizontal_layout.h"

#include "ui/ui_utility.h"

namespace IME {

QMargins HorizontalLayout::getMargins() const {
	auto result = QMargins();
	if (!_columns.empty()) {
		auto &left = _columns.front();
		auto leftMargin = left.widget->getMargins().left();
		result.setLeft(
			qMax(leftMargin - left.margin.left(), 0));
		auto &right = _columns.back();
		auto rightMargin = right.widget->getMargins().right();
		result.setBottom(
			qMax(rightMargin - right.margin.right(), 0));
		for (auto &column : _columns) {
			auto margins = column.widget->getMargins();
			result.setTop(qMax(
				margins.top() - column.margin.top(),
				result.top()));
			result.setBottom(qMax(
				margins.bottom() - column.margin.bottom(),
				result.bottom()));
		}
	}
	return result;
}

int HorizontalLayout::naturalHeight() const {
	auto result = 0;
	for (auto &column : _columns) {
		const auto natural = column.widget->height();
		if (natural < 0) {
			return natural;
		}
		accumulate_max(
			result,
			column.margin.top() + natural + column.margin.bottom());
	}
	return result;
}

void HorizontalLayout::setHorizontalShift(int index, int shift) {
	Expects(index >= 0 && index < _columns.size());

	auto &column = _columns[index];
	if (const auto delta = shift - column.horizontalShift) {
		column.horizontalShift = shift;
		column.widget->move(column.widget->x() + delta, column.widget->y());
		column.widget->update();
	}
}

void HorizontalLayout::reorderColumns(int oldIndex, int newIndex) {
	Expects(oldIndex >= 0 && oldIndex < _columns.size());
	Expects(newIndex >= 0 && newIndex < _columns.size());
	Expects(!_inResize);

	base::reorder(_columns, oldIndex, newIndex);
	// TODO: turn back
	// resizeToHeight(height());
}

int HorizontalLayout::resizeGetWidth(int newHeight) {
	_inResize = true;
	auto guard = gsl::finally([&] { _inResize = false; });

	auto margins = getMargins();
	auto result = 0;
	for (auto &column : _columns) {
		updateChildGeometry(
			margins,
			column.widget,
			column.margin,
			newHeight,
			result + column.horizontalShift);
		result += column.margin.left()
			+ column.widget->widthNoMargins()
			+ column.margin.right();
	}
	return result;
}

void HorizontalLayout::visibleLeftBottomUpdated(
		int visibleLeft,
		int visibleRight) {
	for (auto &column : _columns) {
		setChildVisibleTopBottom(
			column.widget,
			visibleLeft,
			visibleRight);
	}
}

void HorizontalLayout::updateChildGeometry(
		const style::margins &margins,
		Ui::RpWidget *child,
		const style::margins &margin,
		int height,
		int left) const {
	auto availcolumnHeight = height
		- margin.top()
		- margin.bottom();
	// TODO: turn back
	// child->resizeToNaturalHeight(availcolumnHeight);
	// child->moveToTop(
	// 	margins.left() + margin.left() + left,
	// 	margins.top() + margin.top(),
	// 	height);
}

Ui::RpWidget *HorizontalLayout::insertChild(
		int atPosition,
		object_ptr<Ui::RpWidget> child,
		const style::margins &margin) {
	Expects(atPosition >= 0 && atPosition <= _columns.size());
	Expects(!_inResize);

	if (const auto weak = AttachParentChild(this, child)) {
		_columns.insert(
			begin(_columns) + atPosition,
			{ std::move(child), margin });
		// weak->heightValue(
		// ) | rpl::start_with_next_done([=] {
		// 	if (!_inResize) {
		// 		childHeightUpdated(weak);
		// 	}
		// }, [=] {
		// 	removeChild(weak);
		// }, lifetime());
		return weak;
	}
	return nullptr;
}

void HorizontalLayout::removeChild(Ui::RpWidget *child) {
	auto it = ranges::find_if(_columns, [child](const Column &column) {
		return (column.widget == child);
	});
	auto end = _columns.end();
	Assert(it != end);

	auto margins = getMargins();
	auto left = [&] {
		if (it == _columns.begin()) {
			return margins.left();
		}
		auto prev = it - 1;
		return margins.left() + margins.right();
	}() - margins.left();
	for (auto next = it + 1; next != end; ++next) {
		auto &column = *next;
		auto margin = column.margin;
		auto widget = column.widget.data();
		widget->moveToLeft(
			margins.left() + left, margins.top());
		left += widget->width();
	}
	it->widget = nullptr;
	_columns.erase(it);

	// resize(margins.left() + left + margins.right(), height());
}

} // namespace IME
