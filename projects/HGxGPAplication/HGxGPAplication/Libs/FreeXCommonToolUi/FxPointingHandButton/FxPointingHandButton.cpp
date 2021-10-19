#include "FxPointingHandButton.h"

FxPointingHandButton::FxPointingHandButton(QWidget *parent)
	: QPushButton(parent)
{

}

FxPointingHandButton::~FxPointingHandButton()
{

}

void FxPointingHandButton::enterEvent(QEvent *event)
{
	Q_UNUSED(event);
	setCursor(Qt::PointingHandCursor);
}

void FxPointingHandButton::leaveEvent(QEvent *event)
{
	Q_UNUSED(event);
	setCursor(Qt::ArrowCursor);
}
