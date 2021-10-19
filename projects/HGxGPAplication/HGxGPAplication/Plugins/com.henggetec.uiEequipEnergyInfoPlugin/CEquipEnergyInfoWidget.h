#ifndef EQUIPENERGYINFOWIDGET_H
#define EQUIPENERGYINFOWIDGET_H

#include <QWidget>
#include <QHash>
#include <QLabel>
#include "ctkPluginContext.h"
#include "service/event/ctkEventHandler.h"
#include "CEquipEnergyItemWidget.h"


namespace Ui {
class CEquipEnergyInfoWidget;
}

class CEquipEnergyInfoWidget : public QWidget, public ctkEventHandler
{
    Q_OBJECT
	Q_INTERFACES(ctkEventHandler)
public:
    explicit CEquipEnergyInfoWidget(ctkPluginContext *context, QWidget *parent = 0);
    ~CEquipEnergyInfoWidget();

	void setMaxItemCount(int nCount);
protected:
	/**
	* @brief	事件处理
	* @param	event [in] 事件
	*/
	virtual void handleEvent(const ctkEvent& event);

	/**
	* @note 绘制事件
	* @param p [in] 绘制事件
	*/
	virtual void paintEvent(QPaintEvent* p);

private:
	void removeExpiredItems();
protected:
	void test();
signals:
	void EquipEnergyInfoShowSignal();
	void addSEquipInfoSignal(SEquipInfo);
	void EquipEnergyInfoClearSignal();
private Q_SLOTS:
	void EquipEnergyInfoShowSlot();
	void TimerUpdateSlot();
	void addSEquipInfoSlot(SEquipInfo);
	void EquipEnergyInfoClearSlot();
private:
    Ui::CEquipEnergyInfoWidget *ui;
	QList<CEquipEnergyItemWidget*>m_itemList;
	QList<SEquipInfo>             m_equipInfoList;
	int			m_nMaxItemCount;
};


/**************************************************************************************************
* @file		: ProgressBar.h
* @note		: 自定义进度条
* @author	: j00270
* @date		: 2020-10-9
**************************************************************************************************/
//class ProgressBar :public QWidget
//{
//	Q_OBJECT
//public:
//	//count 进度条总个数
//	ProgressBar(int count,QWidget* parent=0);
//public:
//	void init();
//	//设置进度
//	void setProgressValue(double value);
//
//private:
//	//当前进度
//	int   m_curValue;
//	//总进度个数
//	int   m_allCount;
//	QList<QLabel*> m_listProgress;
//	QLabel*  m_progressValue;
//
//};

#endif // NODEWIDGET_H
