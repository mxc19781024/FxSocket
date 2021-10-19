#ifndef CSCENETOOLFOR2D_H
#define CSCENETOOLFOR2D_H

#pragma execution_character_set("utf-8")

#include <QWidget>

#include <service/event/ctkEventHandler.h>
#include <ctkServiceRegistration.h>

//#include <map/IMapManipulatorManager.h>
//#include <map/IFxMapScene.h>
//#include <scene/IFxSceneService.h>
//#include <scene/IFxScene.h>

#include "ui_CSceneToolFor2d.h"


struct ctkEventAdmin;
class ctkPluginContext;
class FxColorPickerWidget;
class CSceneToolItem;
class CSceneToolSelectWidget;

namespace HGxScene
{
	class CGxManipulatorManager;
	class CGxSceneBase;
	class CGxController;
}

class CSceneToolFor2d : public QWidget,
	public ctkEventHandler
{
	Q_OBJECT
	Q_INTERFACES(ctkEventHandler)
public:
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
	CSceneToolFor2d(ctkPluginContext *context, QWidget *parent = 0, Qt::WFlags flags = 0);
#else
	CSceneToolFor2d(ctkPluginContext *context, QWidget *parent = 0, Qt::WindowFlags flags = 0);
#endif
	~CSceneToolFor2d();

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
	void setKitsStatue(bool bVisiabled, const std::string strKey, std::string sceneId = "");

	/**  
	  * @note 序列化工具ui界面
	*/
	void initToolUI(QStringList strTools);

protected:
	/**  
		* @brief 接收事件管理对象发来的事件
		* @note add by w00025
		* @param event[in] 事件对象
	*/
	void handleEvent(const ctkEvent& event);
private:

	/** 
	* @brief 界面初始化
	* @note
	* @return 
	*/
	void initWgt();

 	/** 
 	* @brief 获取操作器管理
 	* @note
 	* @return 
 	*/
	HGxScene::CGxManipulatorManager* getMapManipulatorManager();

 	/** 
 	* @brief 获取二维场景指针
 	* @note
 	* @return 
 	*/
	HGxScene::CGxSceneBase* getMapScene();

	/** 
	* @brief 获取场景渲染窗口指针
	* @note
	* @return 返回场景渲染窗口指针
	*/
	QWidget* getMapSceneRenderWgt();

	/** 
	* @brief 进行地图操作的时候停止标绘动作
	* @note
	* @return 
	*/
	void stopPlot();

	/** 
	* @brief 初始化视点同步按钮的状态
	* @note
	* @return 
	*/
	void initSyncBtnStatus();

	/** 
	* @brief 开启某一功能前关闭标会和测量服务
	* @note
	* @return 
	*/
	void preStart();

signals:
	/** 
	* @brief 视点同步信号
	* @note
	*/
	void synchViewpointSignal(const ctkDictionary& dic);

	// 初始化地图操作为漫游状态
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
	  * @note 复位按钮点击槽
	*/
	void sceneResetSlot();

	/**  
	  * @note 放大按钮点击槽
	*/
	void zoomInSlot(bool); 

	/**  
	  * @note 缩小按钮点击槽
	*/
	void zoomOutSlot(bool);

	/**  
	  * @note 漫游按钮点击槽
	*/
	void roamSlot(bool); 

	/**  
	  * @note 选择按钮点击槽
	*/
	void selectSlot(bool);

	/**  
	  * @note 地图背景色按钮点击槽
	*/
	void backgroundColorSlot();

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
	  * @note 设置视点同步按钮状态
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
	* @brief 晨昏线槽函数
	*/
	void twilightBtnSlot(bool bVisiabled);

	/**
	* @brief 视点同步激活场景槽函数
	*/
	void activeSceneSlot(std::string str);

private:
	Ui::CSceneToolFor2dClass ui;

	// 服务引用
	ctkServiceReference				m_serviceReference;

	// 插件上下文
	ctkPluginContext*				m_pContext;

	// 服务注册对象
	ctkServiceRegistration			m_sceneCreateRegist;

	// 服务注册对象
	ctkServiceRegistration			m_synchRegist;

	// 地图背景色选择对话框
	FxColorPickerWidget *			m_pColorDialog;

	// 漫游按钮
	CSceneToolItem*					m_pRoamBtn;

	// 场景工具选择窗口
	CSceneToolSelectWidget*			m_pSelectWgt;

	// 场景工具按钮
	QVector<CSceneToolItem*>		m_vecToolItems;

	std::string						m_strSceneID;		// 场景ID
};

#endif // CSCENETOOLFOR2D_H
