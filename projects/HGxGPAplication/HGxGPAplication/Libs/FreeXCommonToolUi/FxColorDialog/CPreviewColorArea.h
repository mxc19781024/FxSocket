/**************************************************************************************************
* @file		: CPreviewColorArea.h
* @note		: 颜色预览区域头文件
* @author	: k00026
* @data		: 2018-8
**************************************************************************************************/

#ifndef C_PREVIEW_COLOR_AREA_H
#define C_PREVIEW_COLOR_AREA_H

#include <QWidget>
//#include <FreeXSysConfig.h>

/**
* @class	: CPreviewColorArea
* @brief	: 颜色预览区域
* @note		: 预览当前颜色以及新的颜色
* @author	: k00026
**/
class CPreviewColorArea : public QWidget
{
	Q_OBJECT

public:
	explicit CPreviewColorArea(QWidget *parent = nullptr);
	~CPreviewColorArea();
	// 设置当前颜色
	void SetCurrentColor(const QColor &color);
	// 设置新的颜色
	void SetNewColor(const QColor &color);

protected:
	void paintEvent(QPaintEvent *);

signals:
	// 饱和度、亮度改变信号
	void svChangedSignal(int, int, int);

private:
	QColor m_curColor;		// 当前颜色
	QColor m_newColor;		// 新的颜色
	int m_iRoundRadius;		// 圆角半径
};

#endif // C_PREVIEW_COLOR_AREA_H
