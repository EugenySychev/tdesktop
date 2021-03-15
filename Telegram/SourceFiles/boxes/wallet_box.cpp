/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#include "boxes/wallet_box.h"
#include "mainwidget.h"
#include "mainwindow.h"
#include "ui/widgets/buttons.h"
#include "ui/special_fields.h"
#include "ui/toast/toast.h"
#include "core/application.h"
#include "main/main_session.h"
#include "data/data_session.h"
#include "data/data_user.h"
#include "styles/style_layers.h"
#include "styles/style_boxes.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QClipboard>


WalletBox::WalletBox(QWidget*, not_null<Main::Session*> session)
: _session(session)
, _walletTitle(Ui::CreateChild<Ui::FlatLabel>(
		this,
		st::confirmInviteTitle))
{
}

void WalletBox::prepare() {

 	setTitle(tr::lng_wallet_is());

	// addButton(tr::lng_settings_save(), [=] { save(); });
	// addButton(tr::lng_cancel(), [=] { closeBox(); });

	// connect(_username, &Ui::MaskedInputField::changed, [=] { changed(); });
	// connect(_username, &Ui::MaskedInputField::submitted, [=] { save(); });
	// _link->addClickHandler([=] { linkClick(); });

	// _about.setText(st::usernameTextStyle, tr::lng_username_about(tr::now));
	setDimensions(st::boxWidth, st::usernamePadding.top() + st::boxWidth + st::usernamePadding.bottom());

	// _checkTimer->setSingleShot(true);
	// connect(_checkTimer, &QTimer::timeout, [=] { check(); });

	// updateLinkText();
}

void WalletBox::setInnerFocus() {
	// _walletTitle->setFocusFast();
}

void WalletBox::paintEvent(QPaintEvent *e) {
	BoxContent::paintEvent(e);

}

void WalletBox::resizeEvent(QResizeEvent *e) {
	BoxContent::resizeEvent(e);

	_walletTitle->resize(width() - st::usernamePadding.left() - st::usernamePadding.right(), _walletTitle->height());
	_walletTitle->moveToLeft(st::usernamePadding.left(), st::usernamePadding.top());

}

