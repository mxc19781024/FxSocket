/**************************************************************************************************
* @file FxGanttChart.h
* @note ����ͼ
* @author g00068
* @data 2020-10-22
**************************************************************************************************/
#ifndef FX_GRADUATION_WIDGET_H
#define FX_GRADUATION_WIDGET_H 
#include <FxGanttChartWidget.h>
class CFxGraduationWidget : public QWidget
{
public:
	CFxGraduationWidget(CFxGanttChart* pGanttChart);

	~CFxGraduationWidget();

private:

protected:
	/**
	* @note �����¼�
	* @param p [in] �����¼�
	*/
	virtual void paintEvent(QPaintEvent* p);

	/**
	* @note  ��껬���¼�
	* @param ev [in] ��껬���¼�
	*/
	virtual void wheelEvent(QWheelEvent* ev);

	/**
	* @note  ��갴���¼�
	* @param ev [in] ����¼�
	*/
	virtual void mousePressEvent(QMouseEvent* ev);

	/**
	* @note  ����ͷ��¼�
	* @param ev [in] ����¼�
	*/
	virtual void mouseReleaseEvent(QMouseEvent* ev);

	/**
	* @note  ����ƶ��¼�
	* @param ev [in] ����¼�
	*/
	virtual void mouseMoveEvent(QMouseEvent* ev);

protected:
	/**
	* @note  ����ʱ��̶�
	* @param painter [in] ����
	*/
	void drawGraduation(QPainter& painter);

	/**
	* @note ����ָ��
	* @param painter [in] ����
	*/
	void drawPointer(QPainter& painter);

	/**  
	* @note ����ʵ���б�
	*/
	void drawEventPoint(QPainter& painter);

protected:
	CFxGanttChart* m_pGanttChart;
};
#endif //FX_GRADUATION_WIDGET_H