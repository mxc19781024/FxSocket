/**************************************************************************************************
* @file		: CCustomColorArea.h
* @note		: 自定义颜色区域头文件
* @author	: kzm
* @data		: 2018-8
**************************************************************************************************/

#ifndef CUSTOMCOLORAREA_H
#define CUSTOMCOLORAREA_H

#include <QWidget>
//#include <FreeXSysConfig.h>

class CCustomColorAreaPrivate;

/**
* @class	: CCustomColorArea
* @brief	: 自定义颜色区域
* @note		: 自定义颜色区域
* @author	: k00026
**/
class CCustomColorArea : public QWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(CCustomColorArea)

public:
	explicit CCustomColorArea(QWidget *parent = nullptr);
	~CCustomColorArea();
	// 添加到自定义颜色
	void AddToCustomColor(const QColor &color);

signals:
	// 颜色项选择信号
	void ColorItemSelcSignal(const QColor &);

private:
	Q_DISABLE_COPY(CCustomColorArea)			// 禁止复制
	CCustomColorAreaPrivate * const d_ptr;		// d指针
};

#endif // CUSTOMCOLORAREA_H
