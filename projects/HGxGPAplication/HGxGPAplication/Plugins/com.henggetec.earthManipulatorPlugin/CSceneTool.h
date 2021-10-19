#ifndef CSCENETOOL_H
#define CSCENETOOL_H

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>
#include <QStringList>

#include <service/event/ctkEventHandler.h>
#include <ctkServiceRegistration.h>

//#include <earth/IFxEarthManipulatorManager.h>

#include "ui_CSceneTool.h"
#include "CSceneToolSelectWidget.h"
#include "CSceneToolItem.h"

struct ctkEventAdmin;
class ctkPluginContext;
class CSceneToolSelectWidget;

namespace HGxScene
{
	class CGxManipulatorManager;
	class CGxSceneBase;
	class CGxController;
}

//三维场景工具
class CSceneTool : public QWidget,
	public ctkEventHandler
{
	Q_OBJECT
	Q_INTERFACES(ctkEventHandler)

public:
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
	CSceneTool(ctkPluginContext *context, QWidget *parent = 0, Qt::WFlags flags = 0);
#else
	CSceneTool(ctkPluginContext *context, QWidget *parent = 0, Qt::WindowFlags flags = 0);
#endif
	~CSceneTool();

	/**
	* @brief	: 停止场景操作
	*/
	void stopManipulation();
	
	/**  
	  * @note 获取开启的场景工具列表
	  * @renturn 所有开启的场景工具list 以,分割
	*/
	std::string getToolsOpen();

	/**  
	  * @brief 设置场景工具状态，需要传递状态和挂件工具的唯一key
	*/
	void setKitsStatue(bool bVisiabled, const std::string strKey, std::string sceneId = "", bool bThreadCall=false);

	/**  
	  * @note 序列化工具ui界面
	*/
	void initToolUI(QStringList strTools);

	/**
	* @brief 初始化视点同步按钮的状态
	* @note
	* @return
	*/
	void initSyncBtnStatus();

protected:
	/**  
		* @brief 接收事件管理对象发来的事件
		* @note add by w00025
		* @param event[in] 事件对象
	*/
	void handleEvent(const ctkEvent& event);

private:
	/** 
	* @brief 发送事件到二维场景工具插件，保持界面同步更新
	* @note add by w00025
	* @param bSynchInfo[in] 视点同步信息 true同步 false不同步
	*/
	void sendEvent(bool bSynchInfo);

	/** 
	* @brief 获取场景渲染窗口指针
	* @note
	* @return 返回场景渲染窗口指针
	*/
	QWidget* getEarthSceneRenderWgt();

	/** 
	* @brief 进行地图操作的时候停止标绘动作
	* @note
	* @return 
	*/
	void stopPlot();

signals:
	/** 
	* @brief 视点同步信号
	* @note
	*/
	void synchViewpointSignal(const ctkDictionary& dic);

	/** 
	* @brief 初始化地图操作为漫游状态
	* @note
	*/
	void initRoamStatusSignal();

	/**  
	  * @note 设置视点同步按钮状态信号
	*/
	void setSyncBtnSignal();

	/**
	* @note 设置视点同步激活场景信号
	*/
	void activeSceneSignal(std::string strSceneID);

private slots:
	/** 
	* @brief 复位按钮点击槽
	* @note 
	*/
	void sceneResetSlot();

	/** 
	* @brief 选择按钮点击槽
	* @note add by w00025
	*/
	void selectSlot(bool);

	/** 
	* @brief 视点同步
	* @note add by w00025
	* @param synch[in] 是否同步 true同步 false不同步
	*/
	void viewSynchSlot(bool bSynch);

	/** 
	* @brief 初始化地图操作为漫游状态
	* @note add by w00025
	*/
	void initRoamStatusSlot();

	/**  
	  * @note 初始化视点同步按钮是否可用状态
	*/
	void setSyncBtnSlot();

	/**  
	  * @brief 鸟瞰图槽函数
	*/
	void airViewBtnSlot(bool bVisiabled);

	/**  
	  * @brief 导航器槽函数
	*/
	void navigatorBtnSlot(bool bVisiabled);

	/**  
	  * @brief 比例尺槽函数
	*/
	void mapScaleBtnSlot(bool bVisiabled);

	/**  
	  * @brief 经纬网格槽函数
	*/
	void graticuleBtnSlot(bool bVisiabled);

	/**  
	  * @brief 场景信息槽函数
	*/
	void sceneInfoBtnSlot(bool bVisiabled);

	/**
	* @brief 视点同步激活场景槽函数
	*/
	void activeSceneSlot(std::string str);

private:
	Ui::CSceneToolClass ui;

	// 漫游按钮
	CSceneToolItem*					m_pRoamBtn;

	// 服务引用
	ctkServiceReference				m_serviceReference;

	// 插件上下文
	ctkPluginContext*				m_pContext;
	
	// 服务注册对象
	ctkServiceRegistration			m_sceneCreateRegist;

	// 服务注册对象
	ctkServiceRegistration			m_synchRegist;

	// 场景工具选择窗口
	CSceneToolSelectWidget*			m_pSelectWgt;

	// 场景工具按钮
	QVector<CSceneToolItem*>		m_vecToolItems;

	std::string						m_strSceneID;
};

#endif // CSCENETOOL_H
