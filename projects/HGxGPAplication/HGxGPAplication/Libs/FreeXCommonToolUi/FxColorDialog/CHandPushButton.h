/**************************************************************************************************
* @file		: CHandPushButton.h
* @note		: 鼠标样式为手指形状的按钮头文件
* @author	: k00026
* @data		: 2018-8-13
**************************************************************************************************/

#ifndef HANDBUTTON_H
#define HANDBUTTON_H

#include <QPushButton>
//#include <FreeXSysConfig.h>

/**
* @class	: CHandPushButton
* @brief	: 鼠标样式为手指形状的按钮
* @note		: 鼠标浮于按钮上，形状为手指形状
* @author	: k00026
**/
class CHandPushButton : public QPushButton
{
	Q_OBJECT

public:
	explicit CHandPushButton(QWidget *parent = nullptr);
	~CHandPushButton();

protected:
	// 鼠标进入
	void enterEvent(QEvent *event);
	// 鼠标离开
	void leaveEvent(QEvent *event);
};

#endif // !HANDBUTTON_H
