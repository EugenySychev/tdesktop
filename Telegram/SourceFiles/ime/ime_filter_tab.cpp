#include "ime/ime_filter_tab.h"
#include "ime/ime_settings.h"

#include "data/data_chat_filters.h"
#include "styles/style_menu_icons.h"
#include "styles/style_widgets.h"
#include "styles/style_window.h"
#include "ui/filter_icons.h"
#include "ui/widgets/side_bar_button.h"

namespace IME {

FiltersTab::FiltersTab(not_null<Ui::RpWidget *> parent)
    : _parent(parent), _outer(_parent) {
  _container = std::make_unique<HorizontalLayout>(_parent);
  setup();
}

FiltersTab::~FiltersTab() {}

void FiltersTab::refresh() {

  auto imeList = IME::ImeSettings::getDefaultFilters();
  auto now = base::flat_map<int, base::unique_qptr<Ui::SideBarButton>>();

  for (const auto &filter : imeList) {
    qDebug() << "Default Filter " << filter.id() << " title: " << filter.title()
             << " icon " << filter.iconEmoji();
    now.emplace(filter.id(), prepareButton(_list, filter.id(), filter.title(),
                                           Ui::ComputeFilterIcon(filter)));
  }
  _filters = std::move(now);
}

void FiltersTab::setup() {
  _outer.setAttribute(Qt::WA_OpaquePaintEvent);
  _outer.show();
  _outer.paintRequest() | rpl::start_with_next(
                              [=](QRect clip) {
                                auto p = QPainter(&_outer);
                                p.setPen(Qt::NoPen);
                                p.setBrush(st::windowFiltersButton.textBg);
                                p.drawRect(clip);
                              },
                              _outer.lifetime());
}

base::unique_qptr<Ui::SideBarButton>
FiltersTab::prepareButton(not_null<HorizontalLayout *> container, FilterId id,
                          const QString &title, Ui::FilterIcon icon) {
  auto button = base::unique_qptr<Ui::SideBarButton>(
      container->add(object_ptr<Ui::SideBarButton>(container, title,
                                                   st::windowFiltersButton)));
  const auto raw = button.get();
  const auto &icons = Ui::LookupFilterIcon(icon);
  raw->setIconOverride(icons.normal, icons.active);
  // if (id >= 0) {
  // 	UnreadStateValue(
  // 		&_session->session(),
  // 		id
  // 	) | rpl::start_with_next([=](const Dialogs::UnreadState &state) {
  // 		const auto count = (state.chats + state.marks);
  // 		const auto muted = (state.chatsMuted + state.marksMuted);
  // 		const auto string = !count
  // 			? QString()
  // 			: (count > 99)
  // 			? "99+"
  // 			: QString::number(count);
  // 		raw->setBadge(string, count == muted);
  // 	}, raw->lifetime());
  // }
  // raw->setActive(_session->activeChatsFilterCurrent() == id);
  // raw->setClickedCallback([=] {
  // 	if (_reordering) {
  // 		return;
  // 	} else if (id >= 0) {
  // 		_session->setActiveChatsFilter(id);
  // 	} else {
  // 		const auto filters = &_session->session().data().chatsFilters();
  // 		if (filters->suggestedLoaded()) {
  // 			_session->showSettings(Settings::Type::Folders);
  // 		} else if (!_waitingSuggested) {
  // 			_waitingSuggested = true;
  // 			filters->requestSuggested();
  // 			filters->suggestedUpdated(
  // 			) | rpl::take(1) | rpl::start_with_next([=] {
  // 				_session->showSettings(Settings::Type::Folders);
  // 			}, _outer.lifetime());
  // 		}
  // 	}
  // });
  // if (id > 0) {
  // 	raw->events(
  // 	) | rpl::filter([=](not_null<QEvent*> e) {
  // 		return e->type() == QEvent::ContextMenu;
  // 	}) | rpl::start_with_next([=] {
  // 		showMenu(QCursor::pos(), id);
  // 	}, raw->lifetime());
  // }
  return button;
}

} // namespace IME