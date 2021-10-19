/**************************************************************************************************
* @file		: CBasicColorArea.h
* @note		: 基本颜色区域头文件
* @author	: kzm
* @data		: 2018-8
**************************************************************************************************/

#ifndef BASIC_COLOR_AREA_WIDGET_H
#define BASIC_COLOR_AREA_WIDGET_H

#include <QWidget>
////#include <FreeXSysConfig.h>

/**
* @class	: CBasicColorAreaWidget
* @brief	: 基本颜色区域
* @note		: 
* @author	: k00026
**/
class CBasicColorAreaWidget : public QWidget
{
	Q_OBJECT

public:
	explicit CBasicColorAreaWidget(QWidget *parent = nullptr);
	~CBasicColorAreaWidget();
	// 初始化所有基本颜色项
	void InitColorItems();

signals:
	void ColorItemSelcSignal(const QColor &);
};

#endif // BASIC_COLOR_AREA_WIDGET_H
