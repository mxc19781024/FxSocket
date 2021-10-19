/**************************************************************************************************
* @file		: CHandPushButton.app
* @note		: 鼠标样式为手指形状的按钮实现文件
* @author	: k00026
* @data		: 2018-8-13 9:19:35
**************************************************************************************************/

#include "CHandPushButton.h"


CHandPushButton::CHandPushButton(QWidget *parent)
	: QPushButton(parent)
{
}

CHandPushButton::~CHandPushButton()
{
}

void CHandPushButton::enterEvent(QEvent *event)
{
	Q_UNUSED(event);
	setCursor(Qt::PointingHandCursor);
}

void CHandPushButton::leaveEvent(QEvent *event)
{
	Q_UNUSED(event);
	setCursor(Qt::ArrowCursor);
}
