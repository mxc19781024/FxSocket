#ifndef EQUIPMENTINFOWIDGET_H
#define EQUIPMENTINFOWIDGET_H

#include <QWidget>
#include <QHash>
#include "ctkPluginContext.h"
#include "service/event/ctkEventHandler.h"
#include "common/UnitEventCommon.h"
#include "AlarmInfo.h"


class CEquipmentAttibutes;
class CEquipmentSetting;

namespace Ui {
class CEvaluateInfoWidget;
}


class CEvaluateInfoWidget : public QWidget, public ctkEventHandler
{
    Q_OBJECT
	Q_INTERFACES(ctkEventHandler)
public:
    explicit CEvaluateInfoWidget(ctkPluginContext *context, QWidget *parent = 0);
    ~CEvaluateInfoWidget();

protected:
	/**
	* @brief	事件处理
	* @param	event [in] 事件
	*/
	virtual void handleEvent(const ctkEvent& event);

protected:
	/**  
	  * @brief 初始化图表1
	  * @param 参数 无
	  * @return 返回值  无
	*/
	void InitFrameChart1();


	bool eventFilter(QObject * obj, QEvent *ev);
	virtual void paintEvent(QPaintEvent *ev);
signals:
	void EvaluateInfoShowSignal();
	void createEventSignal(SAlarmInfo);
private Q_SLOTS:
	void createEventSlot(SAlarmInfo sInfo);
	void TimerUpdateSlot();
	void EvaluateInfoShowSlot();

private:
	void painterTopk();

private:
    Ui::CEvaluateInfoWidget *ui;
	ctkPluginContext *m_pContext;
	QList<QPointF> m_listpoints;
	QStringList m_listTopk;
	CAlarmInfo* m_pAlarmInfo;

};
#endif//EQUIPMENTINFOWIDGET_H