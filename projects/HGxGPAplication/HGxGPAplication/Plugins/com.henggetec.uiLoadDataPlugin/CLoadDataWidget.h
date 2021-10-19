#ifndef LOADDATAWIDGET_H
#define LOADDATAWIDGET_H

#include <QWidget>
#include <QHash>
#include "qlistwidget.h"
#include "CLoadDataItemWidget.h"
#include <service/event/ctkEventHandler.h>

namespace Ui {
class CLoadDataWidget;
class CTestWidget;
}


class CLoadDataWidget : public QWidget, public ctkEventHandler
{
    Q_OBJECT
	Q_INTERFACES(ctkEventHandler)
public:
    explicit CLoadDataWidget(ctkPluginContext *context, QWidget *parent = nullptr);
    ~CLoadDataWidget();

protected:
	/**  
	  * @brief 初始化图表1
	  * @param 参数 无
	  * @return 返回值  无
	*/
	void InitListItem();

	/**  
	  * @brief 读取脚本信息
	  * @param 参数 无
	  * @return 返回值  无
	*/
	void ReadFileInfo();

	void parseChart(std::string strPath);

protected:
	/**
	* @brief 接收事件管理对象发来的事件
	* @note add by w00025
	* @param event[in] 事件对象
	*/
	void handleEvent(const ctkEvent& event);

Q_SIGNALS:
	// 
	void SignalSwitchMP(int);
private Q_SLOTS:
	void SlotLoadScene();
	void SlotCloseWidget();
private:
    Ui::CLoadDataWidget *ui;
	Ui::CTestWidget *Testui;
	std::vector<SItemInfo>			m_vecItemInfo;
	std::map<std::string , QListWidgetItem*>    m_mapPath2Item;
	std::map<std::string , std::string>    m_mapPath2Type;
	// 插件上下文
	ctkPluginContext*				m_pContext;
};

#endif // LOADDATAWIDGET_H
