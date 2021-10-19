/**************************************************************************************************
* @file FxGanttChart.h
* @note ����ͼ
* @author g00068
* @data 2020-10-22
**************************************************************************************************/
#ifndef FX_GANTT_WIDGET_H
#define FX_GANTT_WIDGET_H 
#include <FxGanttChartWidget.h>
class CFxGanttWidget :  public QWidget
{
public:
	CFxGanttWidget(CFxGanttChart* pGanttChart);

	~CFxGanttWidget();

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

	/**
	* @note  ���˫���¼�
	* @param ev [in] ����¼�
	*/
	virtual void mouseDoubleClickEvent(QMouseEvent* ev);

protected:
	/**
	* @note �����¼���
	* @param painter [in] ����
	*/
	void drawEvent(QPainter& painter);

	/**
	* @note ����ָ��
	* @param painter [in] ����
	*/
	void drawPointer(QPainter& painter);

	/**
	* @note ���ƹ�����
	* @param painter [in] ����
	*/
	void drawScrollBar(QPainter& painter);

protected:
	CFxGanttChart* m_pGanttChart;
};
#endif //FX_GANTT_WIDGET_H