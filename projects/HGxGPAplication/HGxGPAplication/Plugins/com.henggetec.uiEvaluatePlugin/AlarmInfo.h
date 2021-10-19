//
// Created by chudonghao on 2020/2/13.
//

#ifndef HG_SRC_ALARMINFO_H
#define HG_SRC_ALARMINFO_H

#include <QWidget>
#include "ui_AlarmInfo.h"
#include "ctkPluginContext.h"
#include "service/event/ctkEventHandler.h"
#include "QList"
#include "QDateTime"
#include "common/UnitEventCommon.h"


class CAlarmInfoItem;


/**
 * 告警信息界面
 */
class CAlarmInfo : public QWidget, public ctkEventHandler
{
	Q_OBJECT
	Q_INTERFACES(ctkEventHandler)
public:
	explicit CAlarmInfo(QWidget *parent = nullptr);

	~CAlarmInfo() ;

	/**
	* 设置最多显示几条告警信息（默认值是12）
	* @param maxItemCount
	*/
	void setInfoData(SAlarmInfo sAlarminfo);
	/**
	 * 生成一条告警信息
	 * @param sAlarminfo 详细内容
	 * @return 告警信息子项
	 */
	CAlarmInfoItem *createItem(SAlarmInfo sAlarminfo);

	/**
	 * 设置最多显示几条告警信息（默认值是12）
	 * @param maxItemCount
	 */
	void setMaxItemCount(size_t maxItemCount);

	/**
	* @note 本窗体隐藏
	*/
	void Hide();
	/**
	* @note 本窗体显示
	*/
	void Show();

private slots:
	void createEventSlot(SAlarmInfo);

protected:
	// 事件处理
	void handleEvent(const ctkEvent &event) override;
private:
	Ui::AlarmInfo ui;
	QWidget               *m_pparent;
	QList<CAlarmInfoItem*>m_itemList;
	QList<SAlarmInfo>     m_AlarmInfoList;

	size_t m_maxItemCount;
	void removeExpiredItems();
};



#endif //HG_SRC_ALARMINFO_H
