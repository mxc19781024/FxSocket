#pragma once

#include <QWidget>

#include <QPen>
#pragma execution_character_set("utf-8")

// 容器比例
class ProgressContainer : public QWidget
{
	Q_OBJECT

public:
	enum BallType
	{
		SimpleCircle1,			// 圆形（单圆，有环，无边框, 有底）
		SimpleCircle2,			// 圆形（双圆，有环（渐变），无边框, 无底）
		SimpleSquare1,			// 方形（方角）
		SimpleSquare2			// 方形（圆角）
	};

public:
	ProgressContainer(QWidget *parent = nullptr);
	~ProgressContainer();

 	// 设置容器类型
 	void SetContainerType(BallType type);

	// 设置总数
	void SetTotal(double dTotal);
	// 设置数值
	void SetValue(double dValue);

	// 
	// 设置环颜色
	void SetRingColor(const QColor &ringBackColor, const QColor &ringColor);
	// 设置容器颜色
	void SetRippleColor(const QColor &rippleBackColor, const QColor &rippleColor);
	// 设置环边框颜色
	void SetRingBorderPen(const QPen &pen);
	// 设置容器边框颜色
	void SetRippleBorderPen(const QPen &pen);

	// 设置环渐变（从上到下）
	void SetRingGradient(bool bOpen, const QColor &startColor, const QColor &endColor);
	// 设置容器渐变（从上到下）
	void SetRippleGradient(bool bOpen, const QColor &startColor, const QColor &endColor);

	// 设置水波密度
	void setWaveDensity(int iDensity);

protected:
	void paintEvent(QPaintEvent *event);
	// 重新计算正弦波浪点集
	void resizeEvent(QResizeEvent *event);
	// 设置偏移量
	void timerEvent(QTimerEvent *event);

private:
	// 绘制圆形（单圆，有环，有边框，无底）
	void DrawSimpleCircle1(QPainter & painter);
	// 绘制圆形（单圆，有环，有边框，无底）
	void DrawSimpleCircle2(QPainter & painter);
	// 绘制方形（方角）
	void DrawSimpleSquare(QPainter & painter);

	// 计算贝塞尔控制点
	QVector<QPointF> calculateControlPoints(const QVector<QPointF> &points);

private:
	BallType m_type;		// 类型
	double m_dTotal;			// 总数
	double m_dValue;			// 数值

	// 圆环和容器
	QColor m_ringBackColor;
	QColor m_rippleBackColor;
	QColor m_ringColor;
	QColor m_rippleColor;

	QPen m_ringBorderPen;
	QPen m_rippleBorderPen;

	// 圆环渐变
	bool m_bRingGradient;
	QColor m_ringStartColor;
	QColor m_ringEndColor;

	// 波纹渐变
	bool m_bRippleGradient;
	QColor m_rippleStartColor;
	QColor m_rippleEndColor;

	QFont m_font;				// 字体

	int m_iDensity;				//< 波动密度
	QVector<QPointF> m_points1;	//< 水波1的数据点
	QVector<QPointF> m_points2; //< 水波2的数据点
	int m_iOffset;				//< 偏差值
	QPolygonF m_curve;
	int m_iActualRadius;		// 实际内部圆大小
	int m_rollIndex1;			// 波浪移动下标
	int m_rollIndex2;
};
