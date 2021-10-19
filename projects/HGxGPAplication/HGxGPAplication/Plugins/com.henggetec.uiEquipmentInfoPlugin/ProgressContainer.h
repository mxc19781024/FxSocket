#pragma once

#include <QWidget>

#include <QPen>
#pragma execution_character_set("utf-8")

// ��������
class ProgressContainer : public QWidget
{
	Q_OBJECT

public:
	enum BallType
	{
		SimpleCircle1,			// Բ�Σ���Բ���л����ޱ߿�, �еף�
		SimpleCircle2,			// Բ�Σ�˫Բ���л������䣩���ޱ߿�, �޵ף�
		SimpleSquare1,			// ���Σ����ǣ�
		SimpleSquare2			// ���Σ�Բ�ǣ�
	};

public:
	ProgressContainer(QWidget *parent = nullptr);
	~ProgressContainer();

 	// ������������
 	void SetContainerType(BallType type);

	// ��������
	void SetTotal(double dTotal);
	// ������ֵ
	void SetValue(double dValue);

	// 
	// ���û���ɫ
	void SetRingColor(const QColor &ringBackColor, const QColor &ringColor);
	// ����������ɫ
	void SetRippleColor(const QColor &rippleBackColor, const QColor &rippleColor);
	// ���û��߿���ɫ
	void SetRingBorderPen(const QPen &pen);
	// ���������߿���ɫ
	void SetRippleBorderPen(const QPen &pen);

	// ���û����䣨���ϵ��£�
	void SetRingGradient(bool bOpen, const QColor &startColor, const QColor &endColor);
	// �����������䣨���ϵ��£�
	void SetRippleGradient(bool bOpen, const QColor &startColor, const QColor &endColor);

	// ����ˮ���ܶ�
	void setWaveDensity(int iDensity);

protected:
	void paintEvent(QPaintEvent *event);
	// ���¼������Ҳ��˵㼯
	void resizeEvent(QResizeEvent *event);
	// ����ƫ����
	void timerEvent(QTimerEvent *event);

private:
	// ����Բ�Σ���Բ���л����б߿��޵ף�
	void DrawSimpleCircle1(QPainter & painter);
	// ����Բ�Σ���Բ���л����б߿��޵ף�
	void DrawSimpleCircle2(QPainter & painter);
	// ���Ʒ��Σ����ǣ�
	void DrawSimpleSquare(QPainter & painter);

	// ���㱴�������Ƶ�
	QVector<QPointF> calculateControlPoints(const QVector<QPointF> &points);

private:
	BallType m_type;		// ����
	double m_dTotal;			// ����
	double m_dValue;			// ��ֵ

	// Բ��������
	QColor m_ringBackColor;
	QColor m_rippleBackColor;
	QColor m_ringColor;
	QColor m_rippleColor;

	QPen m_ringBorderPen;
	QPen m_rippleBorderPen;

	// Բ������
	bool m_bRingGradient;
	QColor m_ringStartColor;
	QColor m_ringEndColor;

	// ���ƽ���
	bool m_bRippleGradient;
	QColor m_rippleStartColor;
	QColor m_rippleEndColor;

	QFont m_font;				// ����

	int m_iDensity;				//< �����ܶ�
	QVector<QPointF> m_points1;	//< ˮ��1�����ݵ�
	QVector<QPointF> m_points2; //< ˮ��2�����ݵ�
	int m_iOffset;				//< ƫ��ֵ
	QPolygonF m_curve;
	int m_iActualRadius;		// ʵ���ڲ�Բ��С
	int m_rollIndex1;			// �����ƶ��±�
	int m_rollIndex2;
};
