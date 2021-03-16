/*
    Added by Evgeny Sychev
*/
#pragma once

#include "boxes/abstract_box.h"
#include "mtproto/sender.h"

namespace Ui {
class UsernameInput;
class LinkButton;
} // namespace Ui

namespace Main {
class Session;
} // namespace Main

class WalletBox : public Section {
public:
	WalletBox(QWidget*, not_null<Main::Session*> session);

protected:
	void prepare() override;
	void setInnerFocus() override;

	void paintEvent(QPaintEvent *e) override;
	void resizeEvent(QResizeEvent *e) override;

private:
	// void updateDone(const MTPUser &result);
	// void updateFail(const RPCError &error);

	// void checkDone(const MTPBool &result);
	// void checkFail(const RPCError &error);

	// void save();

	// void check();
	// void changed();

	// void linkClick();

	// QString getName() const;
	// void updateLinkText();

	const not_null<Main::Session*> _session;
	// MTP::Sender _api;

	// object_ptr<Ui::UsernameInput> _username;
	// object_ptr<Ui::LinkButton> _link;

	// mtpRequestId _saveRequestId = 0;
	// mtpRequestId _checkRequestId = 0;
	// QString _sentUsername, _checkUsername, _errorText, _goodText;

	// Ui::Text::String _about;
	// object_ptr<QTimer> _checkTimer;

	not_null<Ui::FlatLabel*> _walletTitle;

};
