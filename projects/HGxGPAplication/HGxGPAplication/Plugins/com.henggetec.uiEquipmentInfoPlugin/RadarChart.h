#pragma once

#include <QWidget>

#include <QPen>
#pragma execution_character_set("utf-8")
#include "public\common\UnitEventCommon.h"

struct RadarNode
{
	double dScale;		// 比例
	QString qName;		// 比例
	QColor fontColor;	// 字体颜色
	QColor borderColor;	// 边框颜色
	QColor backColor;	// 背景色
};

// 雷达图
class RadarChart : public QWidget
{
	Q_OBJECT

public:
// 	enum RadarType
// 	{
// 		SimpleRadar1,		// 简单雷达图（文字不带边框）
// 		SimpleRadar2		// 简单雷达图（文字带边框）
// 	};

public:
	RadarChart(QWidget *parent);
	~RadarChart();

	// 设置环背景色	两两间隔（color1、color2）	color3：最后一环
	void SetRingBackColor(const QColor &color1, const QColor &color2, const QColor &color3);
	// 设置画笔	pen1：内部		最外层边框
	void SetPen(const QPen &pen1, const QPen &pen2);
	// 设置背景渐变（坐下到右上）
	void SetBackGradient(const QColor &startColor, const QColor &endColor);
	// 设置线渐变（左下到右上）
	void SetLineGradient(const QColor &startColor, const QColor &endColor);
	// 设置文字是否有边框
	void SetTextBorderOpen(bool bOpen);

	// 设置数据（限制数目为6）
	void SetData(const QVector<RadarNode> &vecData);

	// 设置环数
	void SetRingCount(int iCount);

	// 设置环数
	void SetData(SEquipCap sEquipCap);

protected:
	void paintEvent(QPaintEvent *event);

private:
	QPointF CalPosition(double dAngle, int iRadius);

private:
	// 环背景色
	QColor m_ringColor1;
	QColor m_ringColor2;
	QColor m_lastColor;

	// 画笔
	QPen m_insidePen;
	QPen m_latPen;

	// 渐变
	QColor m_backStartColor;
	QColor m_backEndColor;
	QColor m_lineStartColor;
	QColor m_lineEndColor;

	bool m_bTextBorderOpen;		// 文字是否有边框

	QVector<RadarNode> m_vecData;	// 数据

	int m_iRingCount;		// 环数目
};
