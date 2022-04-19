/*
    This file is a part of iMe code client
*/
#pragma once

#include "data/data_types.h"
#include "horizontal_layout_reorder.h"
#include "ime/horizontal_layout.h"
#include "ui/widgets/scroll_area.h"
#include "window/window_session_controller.h"

namespace Ui {
class SideBarButton;
enum class FilterIcon : uchar;
}
namespace Data {
class ChatFilter;
} // namespace Data

namespace IME {
class FiltersTab final {
public:
  FiltersTab(not_null<Ui::RpWidget *> parent);
  ~FiltersTab();

private:
  void setup();
  void refresh();
  const not_null<Ui::RpWidget *> _parent;
  Ui::RpWidget _outer;
  std::unique_ptr<HorizontalLayout> _container;
  HorizontalLayout *_list = nullptr;
  std::unique_ptr<HorizontalLayoutReorder> _reorder;

  base::unique_qptr<Ui::SideBarButton>
  prepareButton(not_null<HorizontalLayout *> container, FilterId id,
                const QString &title, Ui::FilterIcon icon);
  FilterId _activeFilterId = 0;
  // int _reordering = 0;
  // bool _ignoreRefresh = false;
  // bool _waitingSuggested = false;

  base::flat_map<FilterId, base::unique_qptr<Ui::SideBarButton>> _filters;
};

} // namespace IME