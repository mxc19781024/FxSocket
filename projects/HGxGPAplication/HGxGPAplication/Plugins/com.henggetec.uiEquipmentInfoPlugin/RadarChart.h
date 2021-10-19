#pragma once

#include <QWidget>

#include <QPen>
#pragma execution_character_set("utf-8")
#include "public\common\UnitEventCommon.h"

struct RadarNode
{
	double dScale;		// ����
	QString qName;		// ����
	QColor fontColor;	// ������ɫ
	QColor borderColor;	// �߿���ɫ
	QColor backColor;	// ����ɫ
};

// �״�ͼ
class RadarChart : public QWidget
{
	Q_OBJECT

public:
// 	enum RadarType
// 	{
// 		SimpleRadar1,		// ���״�ͼ�����ֲ����߿�
// 		SimpleRadar2		// ���״�ͼ�����ִ��߿�
// 	};

public:
	RadarChart(QWidget *parent);
	~RadarChart();

	// ���û�����ɫ	���������color1��color2��	color3�����һ��
	void SetRingBackColor(const QColor &color1, const QColor &color2, const QColor &color3);
	// ���û���	pen1���ڲ�		�����߿�
	void SetPen(const QPen &pen1, const QPen &pen2);
	// ���ñ������䣨���µ����ϣ�
	void SetBackGradient(const QColor &startColor, const QColor &endColor);
	// �����߽��䣨���µ����ϣ�
	void SetLineGradient(const QColor &startColor, const QColor &endColor);
	// ���������Ƿ��б߿�
	void SetTextBorderOpen(bool bOpen);

	// �������ݣ�������ĿΪ6��
	void SetData(const QVector<RadarNode> &vecData);

	// ���û���
	void SetRingCount(int iCount);

	// ���û���
	void SetData(SEquipCap sEquipCap);

protected:
	void paintEvent(QPaintEvent *event);

private:
	QPointF CalPosition(double dAngle, int iRadius);

private:
	// ������ɫ
	QColor m_ringColor1;
	QColor m_ringColor2;
	QColor m_lastColor;

	// ����
	QPen m_insidePen;
	QPen m_latPen;

	// ����
	QColor m_backStartColor;
	QColor m_backEndColor;
	QColor m_lineStartColor;
	QColor m_lineEndColor;

	bool m_bTextBorderOpen;		// �����Ƿ��б߿�

	QVector<RadarNode> m_vecData;	// ����

	int m_iRingCount;		// ����Ŀ
};
