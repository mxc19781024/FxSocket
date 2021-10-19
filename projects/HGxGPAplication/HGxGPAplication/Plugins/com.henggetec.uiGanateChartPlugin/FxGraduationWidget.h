/**************************************************************************************************
* @file FxGanttChart.h
* @note 甘特图
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

protected:
	/**
	* @note  绘制时间刻度
	* @param painter [in] 画家
	*/
	void drawGraduation(QPainter& painter);

	/**
	* @note 绘制指针
	* @param painter [in] 画家
	*/
	void drawPointer(QPainter& painter);

	/**  
	* @note 解析实体列表
	*/
	void drawEventPoint(QPainter& painter);

protected:
	CFxGanttChart* m_pGanttChart;
};
#endif //FX_GRADUATION_WIDGET_H