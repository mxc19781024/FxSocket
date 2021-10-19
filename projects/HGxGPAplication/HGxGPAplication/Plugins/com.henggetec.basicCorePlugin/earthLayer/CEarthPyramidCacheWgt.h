#ifndef CEarthPyramidCacheWgt_H
#define CEarthPyramidCacheWgt_H

#include <QWidget>
#include <QMouseEvent>
#include <QImage>
#include "ui_CEarthPyramidCacheWgt.h"
#include <QTimer>
/**************************************************************************************************
* @file CEarthPyramidCacheWgt 
* @note 金字塔缓存窗口
* @author w00025
* @date 2019-1-22 
**************************************************************************************************/
class CEarthPyramidCacheWgt : public QWidget
{
	Q_OBJECT

public:
	CEarthPyramidCacheWgt(QWidget *parent = 0);
	~CEarthPyramidCacheWgt();

	/**
	* @brief 设置缓存进度
	* @note
	* @param nValue[in] 缓存进度
	*/
	void setCacheProcess(int nValue);


	/**
	* @brief 设置剩余时间
	* @note
	* @param strRemainTime[in] 剩余时间
	*/
	void setRemainTime(const QString &strRemainTime);

	/**
	* @brief 设置已读取的时间
	* @note
	* @param dCostTime[in] 花费时间
	* @param iCostPercent[in]  读取的间隔时间的进度差值
	*/
	void setCostTime(double dCostTime, int iCostPercent);

protected:
	/**
	* @brief 鼠标按下事件
	* @note
	* @param event[in] 鼠标事件
	*/
	void mousePressEvent(QMouseEvent *event);

	/**
	* @brief 鼠标移动事件
	* @note
	* @param event[in] 鼠标事件
	*/
	void mouseMoveEvent(QMouseEvent *event);

	/**
	* @brief 鼠标释放事件
	* @note
	* @param event[in] 鼠标事件
	*/
	void mouseReleaseEvent(QMouseEvent *event);


private:
	/**
	* @brief 初始化信号和槽
	* @note
	*/
	void initSignalsAndSlots();

signals:
	/**
	* @brief 取消缓存信号
	* @note
	*/
	void cancelCacheSignal();

private slots:
	void valuedChangedSlot(int nValue);

	void timerChangedSlot( );

private:
	// 记录鼠标是否按下
	bool				m_bPressed;

	// 记录上一次的位置
	QPoint				m_bLastPos;

	// 渐变色
	QImage				m_image;

	Ui::CEarthPyramidCacheWgt ui;

	// 时分秒
	int m_nHour;
	int m_nMintue ;
	int m_nSecond ;

	//定时器
	QTimer *m_timer;

	//记录开始时间
	double m_TimeStart;

	//记录结束时间
	double m_TimeEnd;

	//记录耗费的时间
	double m_CostTime;

	//记录当前进度的百分比值
	int m_nValue;

	//开启进度加载的状态
	bool b_isStart;

};

#endif // CEarthPyramidCacheWgt_H
