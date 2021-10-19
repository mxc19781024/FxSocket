/**************************************************************************************************
* @file FxGanttChart.h
* @note 甘特图
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
	* @note 绘制事件
	* @param p [in] 绘制事件
	*/
	virtual void paintEvent(QPaintEvent* p);

	/**
	* @note  鼠标滑轮事件
	* @param ev [in] 鼠标滑轮事件
	*/
	virtual void wheelEvent(QWheelEvent* ev);

	/**
	* @note  鼠标按下事件
	* @param ev [in] 鼠标事件
	*/
	virtual void mousePressEvent(QMouseEvent* ev);

	/**
	* @note  鼠标释放事件
	* @param ev [in] 鼠标事件
	*/
	virtual void mouseReleaseEvent(QMouseEvent* ev);

	/**
	* @note  鼠标移动事件
	* @param ev [in] 鼠标事件
	*/
	virtual void mouseMoveEvent(QMouseEvent* ev);

	/**
	* @note  鼠标双击事件
	* @param ev [in] 鼠标事件
	*/
	virtual void mouseDoubleClickEvent(QMouseEvent* ev);

protected:
	/**
	* @note 绘制事件条
	* @param painter [in] 画家
	*/
	void drawEvent(QPainter& painter);

	/**
	* @note 绘制指针
	* @param painter [in] 画家
	*/
	void drawPointer(QPainter& painter);

	/**
	* @note 绘制滚动条
	* @param painter [in] 画家
	*/
	void drawScrollBar(QPainter& painter);

protected:
	CFxGanttChart* m_pGanttChart;
};
#endif //FX_GANTT_WIDGET_H