/*
created as example for iMe
*/

#pragma once

#include "ui/wrap/padding_wrap.h"
#include "ui/widgets/checkbox.h"
#include "base/timer.h"


namespace Ui {
class UserpicButton;
class FlatLabel;
template <typename Widget>
class SlideWrap;
} // namespace Ui

namespace IMe
{


class AicAccountWidget : public Ui::FixedHeightWidget
{
public:
    AicAccountWidget(
        QWidget *parent,
        not_null<PeerData*> peer,
        not_null<Window::SessionController*> controller);

private:

	object_ptr<Ui::UserpicButton> _userpic;
	object_ptr<Ui::FlatLabel> _name = { nullptr };
	object_ptr<Ui::RpWidget> _verifiedCheck = { nullptr };
	object_ptr<Ui::RpWidget> _scamFakeBadge = { nullptr };
	object_ptr<Ui::FlatLabel> _status = { nullptr };
};


} // namespace IMe
