#ifndef CSPECIALEFFECTSCONTROL_H
#define CSPECIALEFFECTSCONTROL_H


#include <QWidget>
#include <QMessageBox>

#include <ctkServiceRegistration.h>
#include <service/event/ctkEventHandler.h>
#include <ctkPluginContext.h>

//#include <earth_layer/IFxEarthLayer.h>
//#include <earth_environment/IFxEnvironmentManager.h>
//#include <earth_environment/IFxSkyEnvironment.h>
//#include <earth_environment/IFxWeatherEnvironment.h>
//#include <earth_ocean/IFxOceanEnvironment.h>
//#include <earth_ocean/IFxOceanManager.h>
//#include <scene/IFxSceneService.h>


#include "ui_CSpecialEffectsControl.h"
#include "CEffectsControlItem.h"
#include "CEnvirAttrSetting.h"
#include <utility>

#include <osg/ref_ptr>
#include <bdpTool/environment/GxEnviManager.h>

namespace HGxTool
{
	class CGxOceanOption;
	class CGxWeatherOption;
	class CGxSkyOption;
}

/**
	* @class CSpecialEffectsControl
	* @brief 三维地球环境信息
	* @author 田义俊
*/
typedef struct SEarthEnvInfo
{
	// 太阳光照强度
	float				fSunIntensity;
	// 雾化因子
	float				fFogFactor;
	// 云图最低显示高度
	float				fCloudLowHeight;
	// 雨浓度
	float				fRainConcentration;
	// 雨高度
	float				fRainHeight;
	// 雪浓度
	float				fSnowConcentration;
	// 雪高度
	float				fSnowHeight;
	// 海面高度
	float				fSeaHeight;
	// 海风速
	float				fSeaWindSeep;
	// 海风向
	float				fSeaWindDirection;

	// 三维环境名称与开启状态的映射
	QMap<QString, bool> mapEnvSet;

}SEarthEnvInfo;


/**
	* @class CSpecialEffectsControl
	* @brief 三维场景特效窗口
	* @author 田义俊
*/
struct ctkEventAdmin;
class ISceneUiService;

class CSpecialEffectsControl : public QWidget, 
	public ctkEventHandler
{
	Q_OBJECT
	Q_INTERFACES(ctkEventHandler)
public:
	//特效控制窗口，默认为3d模式，设置参数is3dModel为false，则置为2d模式
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
	CSpecialEffectsControl(ctkPluginContext *context, QWidget *parent = 0, Qt::WFlags flags = 0);
#else
	CSpecialEffectsControl(ctkPluginContext *context, QWidget *parent = 0, Qt::WindowFlags flags = 0);
#endif
	~CSpecialEffectsControl();

	/** 
	* @brief 获取环境属性窗口
	* @note add by w00025
	* @return 环境属性窗口指针
	*/
	CEnvirAttrSetting* getAttrWgt(); 

	/** 
	* @brief 获取地球环境数据
	* @note add by w00025
	* @param stEarthEnvInfo[out] 三维地球环境信息
	* @return true 获取成功 false获取失败
	*/
	bool getEarthEnvData(SEarthEnvInfo &stEarthEnvInfo);

	/** 
	* @brief 刷新三维环境界面显示
	* @note add by w00025
	* @param stEarthEnvInfo[in] 三维地球环境信息
	* @return
	*/
	void updateUI(const SEarthEnvInfo &stEarthEnvInfo);

	/** 
 	* @brief 调用底层数据接口获取界面初始化数据
 	* @note add by w00025
 	*/
	void initData();

	/** 
 	* @brief 设置场景名称
 	* @note add by w00025
 	* @param strSceneName[in] 场景名称
 	*/
	void setSceneName(const std::string &strSceneName);
 
	std::string getSceneID();

	/** 
 	* @brief 获取场景名称
 	* @note add by w00025
 	* @return 场景名称
 	*/
 	std::string getSceneName();

 	/** 
  	* @brief 三维环境插件启动时调用此接口进行初始化
  	*/
 	void initEnvWhenStart();

	/**
	* @brief: 初始化海洋
	* @note add by w000224 2020-6-1
	* Qt5不支持多线程渲染 makeCurrent报错,此处提供可单独初始化海洋的接口，便于概念上理解
	*/
	void initOceanForQtFive();

	HGxTool::CGxOceanOption* getOceanOption();

	HGxTool::CGxWeatherOption* getWeatherOption();

	HGxTool::CGxSkyOption* getSkyOption();

protected:
	/**  
	* @brief 接收事件管理对象发来的事件
	* @note add by w00025
	* @param event[in] 事件对象
	*/
	void handleEvent(const ctkEvent& event);

private:
	/** 
 	* @brief 根据场景ID获取场景名称
 	* @note add by w00025
 	* @param nSceneID[in] 场景ID
 	* @return 场景名称
 	*/
 	std::string getSceneName( std::string nSceneID);

	/** 
 	* @brief 在线程中获取天空节点
 	* @note add by w00025
 	*/
	bool getNodeInThread();

	//初始化窗口
	void initWgt();
	//添加3d模式下的Item
	void addControlItem(QList<std::pair<QString, QString>>);
	//清空Item
	void clearControlItem();

	// 发送通知软件变化的事件
	void sendModifiedEvent(bool bModified);

signals:
	void initEnvSignal();

	void settingWidgetHideSignal();

private slots:
	
	//3dItem开关槽
	void swichedSlotForItem(const QString &, bool);
	
	//设置按钮点击槽
	void settingBtnClickedSlot(); 

	/**
	* @brief: 初始化环境槽函数
	* @note add by w00025
	*/
	void initEnvSlot();

	/**
	* @brief: 环境发生变化的槽函数
	* @note add by w00025
	*/
	void envirChangedSlot();
	
	/**
	* @brief: 线程获取天空节点以及海洋节点结束时调用的槽函数
	* @note add by w00025
	* @return 获取节点成功返回true 获取失败返回false
	*/
	bool getNodeFinishedSlot();

	/**  
	  * @note 隐藏环境设置界面
	*/
	void settingWidgetHideSlot();

private:
	Ui::CSpecialEffectsControlClass ui;

	// 场景名称
	std::string							m_strSceneID;

	//所有控制项
	QVector<CEffectsControlItem *>		m_vecControlItem; 

	//环境属性设置窗口
	CEnvirAttrSetting*					m_pAttrWgt; 

	osg::ref_ptr<HGxTool::CGxEnviManager>		m_rpEnviManager;

	HGxTool::CGxOceanOption* m_pOceanOption;

	HGxTool::CGxWeatherOption* m_pWeatherOption;
	
	HGxTool::CGxSkyOption* m_pSkyOption;
};

#endif // CSPECIALEFFECTSCONTROL_H
