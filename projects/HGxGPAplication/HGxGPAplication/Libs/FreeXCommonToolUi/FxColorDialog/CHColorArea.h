/**************************************************************************************************
* @file		: CHColorArea.h
* @note		: 色调区域
* @author	: k00026
* @data		: 2018-8
**************************************************************************************************/

#ifndef HCOLOR_AREA_H
#define HCOLOR_AREA_H

#include <QWidget>
//#include <FreeXSysConfig.h>

class CHColorAreaPrivate;

/**
* @class	: CHColorArea
* @brief	: 色调控制区域
* @note		: 色调Hue控制区域
* @author	: k00026
**/
class CHColorArea : public QWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(CHColorArea)

public:
	CHColorArea(QWidget *parent = nullptr, int hue = 0);
	~CHColorArea();
	// 设置色调值
	void SetHueValue(int hvalue);
	// 获得色调值
	int GetHueValue();

protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

signals:
	// 手动改变色调值发出信号
	void HueManualChangedSignal(int);

private:
	Q_DISABLE_COPY(CHColorArea)						// 禁止复制
	QScopedPointer<CHColorAreaPrivate> const d_ptr;	// d指针
};

#endif // !HCOLOR_AREA_H
