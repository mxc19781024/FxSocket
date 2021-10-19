#ifndef EQUIPMENTINFOWIDGET_H
#define EQUIPMENTINFOWIDGET_H

#include <QWidget>
#include <QHash>
#include "ctkPluginContext.h"
#include "service/event/ctkEventHandler.h"
#include "public/common/UnitEventCommon.h"

class CEquipmentAttibutes;
class CEquipmentSetting;

namespace Ui {
class CEquipmentInfoWidget;
}


class CEquipmentInfoWidget : public QWidget, public ctkEventHandler
{
    Q_OBJECT
	Q_INTERFACES(ctkEventHandler)
public:
    explicit CEquipmentInfoWidget(ctkPluginContext *context, QWidget *parent = 0);
    ~CEquipmentInfoWidget();

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
signals:
	void EquipmentInfoShowSignal();
	void EquipmentInfoClearSignal();
	void addSEquipmentInfoSignal(SEquipmentInfo);
	void RemoveUintSignal(SRemoveUintInfo);
private Q_SLOTS:
	void addSEquipmentInfoSlot(SEquipmentInfo) ;
	void RemoveUintSlot(SRemoveUintInfo);
	void switchEquipmentSlot(int index);
	void TimerUpdateSlot();
	void EquipmentInfoShowSlot();
	void EquipmentInfoClearSlot();
private:
    Ui::CEquipmentInfoWidget *ui;
	QMap<std::string,SEquipmentInfo>	  m_mapSEquipmentInfo;
};

#endif // NODEWIDGET_H
