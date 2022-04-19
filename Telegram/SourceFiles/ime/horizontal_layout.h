// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#pragma once

#include "ui/rp_widget.h"
#include "base/object_ptr.h"

namespace IME {

class HorizontalLayout : public Ui::RpWidget {
public:
	using Ui::RpWidget::RpWidget;

	[[nodiscard]] int count() const {
		return _columns.size();
	}
	[[nodiscard]] not_null<Ui::RpWidget*> widgetAt(int index) const {
		Expects(index >= 0 && index < count());

		return _columns[index].widget.data();
	}

	template <
		typename Widget,
		typename = std::enable_if_t<
			std::is_base_of_v<Ui::RpWidget, Widget>>>
	Widget *insert(
			int atPosition,
			object_ptr<Widget> &&child,
			const style::margins &margin = style::margins()) {
		return static_cast<Widget*>(insertChild(
			atPosition,
			std::move(child),
			margin));
	}

	template <
		typename Widget,
		typename = std::enable_if_t<
			std::is_base_of_v<Ui::RpWidget, Widget>>>
	Widget *add(
			object_ptr<Widget> &&child,
			const style::margins &margin = style::margins()) {
		return insert(count(), std::move(child), margin);
	}

	QMargins getMargins() const override;
	int naturalHeight() const;

	void setHorizontalShift(int index, int shift);
	void reorderColumns(int oldIndex, int newIndex);

protected:
	int resizeGetWidth(int newWidth);
	void visibleLeftBottomUpdated(
		int visibleLeft,
		int visibleRight);

private:
	Ui::RpWidget *insertChild(
		int addPosition,
		object_ptr<Ui::RpWidget> child,
		const style::margins &margin);
	void removeChild(Ui::RpWidget *child);
	void updateChildGeometry(
		const style::margins &margins,
		Ui::RpWidget *child,
		const style::margins &margin,
		int width,
		int top) const;

	struct Column {
		object_ptr<Ui::RpWidget> widget;
		style::margins margin;
		int horizontalShift = 0;
	};
	std::vector<Column> _columns;
	bool _inResize = false;

};

} // namespace IME
