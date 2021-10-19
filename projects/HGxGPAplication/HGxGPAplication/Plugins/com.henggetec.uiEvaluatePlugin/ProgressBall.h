//
// Created by HG on 2020/3/27.
//

#ifndef SITUATION_DETAIL_SRC_UI_PROGRESSBALL_H
#define SITUATION_DETAIL_SRC_UI_PROGRESSBALL_H

#include <QWidget>
#include <QTimer>

class ProgressBall : public QWidget
{
	Q_OBJECT

public:
	enum ProgressShape { PS_ELLIPSE, PS_RECTANGLE };

	explicit ProgressBall(QWidget *parent);
	explicit ProgressBall(ProgressShape eShape = PS_ELLIPSE, QWidget *parent = NULL);

	void SetColor(const QColor &color) { m_color = color; update(); }
	QColor Color() const { return m_color; }
	void SetValue(int value) { m_iValue = value; update(); }
	int Value() const { return m_iValue; }
	void SetShape(ProgressShape shape){m_eShape = shape;}

	Q_SLOT void Roll();

protected:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);

private:
	void DrawEllipse(QPainter &painter);
	void DrawRectangle(QPainter &painter);

private:
	ProgressShape m_eShape;

	int m_iBodyRadius;
	int m_iProgressRadius;
	QColor m_color;
	int m_iValue;
	QTimer m_timer;
	QPolygonF m_curve;		// 波浪模版
	int m_rollIndex1;			// 波浪移动下标
	int m_rollIndex2;
	bool m_reset;


	bool m_bUseImage;
	QImage* m_pImage;

};

#endif //SITUATION_DETAIL_SRC_UI_PROGRESSBALL_H
