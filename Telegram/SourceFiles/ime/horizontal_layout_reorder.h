// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#pragma once

#include "ui/effects/animations.h"
#include "ui/widgets/scroll_area.h"

namespace IME {

class RpWidget;
class HorizontalLayout;

class HorizontalLayoutReorder final {
public:
	enum class State : uchar {
		Started,
		Applied,
		Cancelled,
	};
	struct Single {
		not_null<Ui::RpWidget*> widget;
		int oldPosition = 0;
		int newPosition = 0;
		State state = State::Started;
	};

	HorizontalLayoutReorder(
		not_null<HorizontalLayout*> layout,
		not_null<Ui::ScrollArea*> scroll);
	HorizontalLayoutReorder(not_null<HorizontalLayout*> layout);

	void start();
	void cancel();
	void finishReordering();
	[[nodiscard]] rpl::producer<Single> updates() const;

private:
	struct Entry {
		not_null<Ui::RpWidget*> widget;
		Ui::Animations::Simple shiftAnimation;
		int shift = 0;
		int finalShift = 0;
		int deltaShift = 0;
	};

	void mouseMove(not_null<Ui::RpWidget*> widget, QPoint position);
	void mousePress(
		not_null<Ui::RpWidget*> widget,
		Qt::MouseButton button,
		QPoint position);
	void mouseRelease(Qt::MouseButton button);

	void checkForStart(QPoint position);
	void updateOrder(int index, QPoint position);
	void cancelCurrent();
	void finishCurrent();
	void cancelCurrent(int index);

	[[nodiscard]] int indexOf(not_null<Ui::RpWidget*> widget) const;
	void moveToShift(int index, int shift);
	void updateShift(not_null<Ui::RpWidget*> widget, int indexHint);

	void updateScrollCallback();
	void checkForScrollAnimation();
	int deltaFromEdge();

	const not_null<HorizontalLayout*> _layout;
	Ui::ScrollArea *_scroll = nullptr;

	Ui::Animations::Basic _scrollAnimation;

	Ui::RpWidget *_currentWidget = nullptr;
	int _currentStart = 0;
	int _currentDesiredIndex = 0;
	State _currentState = State::Cancelled;
	std::vector<Entry> _entries;
	rpl::event_stream<Single> _updates;
	rpl::lifetime _lifetime;

};

} // namespace Ui
