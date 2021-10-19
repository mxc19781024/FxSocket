/**************************************************************************************************
* @file		: CBasicColorArea.h
* @note		: 基本颜色区域头文件
* @author	: k00026
* @data		: 2018-8
**************************************************************************************************/

#ifndef BASIC_COLOR_AREA_H
#define BASIC_COLOR_AREA_H

#include <QWidget>
//#include <FreeXSysConfig.h>

/**
* @class	: CBasicColorArea
* @brief	: 基本颜色区域
* @note		: 
* @author	: k00026
**/
class CBasicColorArea : public QWidget
{
	Q_OBJECT

public:
	explicit CBasicColorArea(QWidget *parent = nullptr);
	~CBasicColorArea();
	// 初始化所有基本颜色项
	void InitColorItems();

signals:
	void ColorItemSelcSignal(const QColor &);
};

#endif // BASICCOLORAREA_H
