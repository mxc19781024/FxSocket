#ifndef CEFFECTSUIPLUGIN_H
#define CEFFECTSUIPLUGIN_H

#include <QObject>
#include <ctkPluginContext.h>
//#include <FreeXFramework/FreeXFramework.h>


//#include <FreeXCore/IFxDocument.h>

//#include <scene/IFxSceneService.h>
#include <earth_effects_ui/IEffectsUiService.h>

#include "CSpecialEffectsControl.h"

class CSpecialEffectsControl;

namespace HGxDB
{
	class CGxTiXmlElement;
	class CGxTiXmlDocument;
}

class CEffectsUiPlugin : public QObject
	, public IEffectsUiService
/*	, public FxCore::IFxDocument*/
{
	Q_OBJECT
	Q_INTERFACES(IEffectsUiService)
//	Q_INTERFACES(FxCore::IFxDocument)
	

public:
	CEffectsUiPlugin(ctkPluginContext *context);
	~CEffectsUiPlugin();

// 	/**
// 	* @brief 加载文档
// 	* @param pEle [in] xml节点，包含需要解析的内容
// 	* @param pProcess [in] 用于监测处理进度
// 	* @return 是否加载成功
// 	*/
// 	bool load(const FxCommonUtils::TiXmlElement* pEle, FxCore::IFxWorkspaceProcess* pProcess = NULL) override;
// 
// 	/**
// 	* @brief 保存文档
// 	* @param pEle [in] xml节点，包含需要解析的内容
// 	* @param pProcess [in] 用于监测处理进度
// 	* @return 是否保存成功
// 	*/
// 	bool save(FxCommonUtils::TiXmlElement* pEle, FxCore::IFxWorkspaceProcess* pProcess = NULL) override;
// 
// 	/**
// 	* @brief 获取文档级别
// 	* @return 文档级别
// 	*/
// 	int getDocLevel() override { return FxCore::IFxDocument::DOC_FUNC; }

	/**  
	* @brief 获取三维环境界面指针
	* @return 三维环境界面指针
	*/
	QWidget* getWgt();

private: 

 	//std::string loadInThread(const FxCommonUtils::TiXmlElement*, const std::string &);

private slots:
	/** 
 	* @brief 线程环境节点反序列化结束后调用此槽函数
 	*/
	void loadFinishedSlot();

private:
	// 地球环境特效界面类
	CSpecialEffectsControl*				m_pEffectWgt;

	// 插件上下文
	ctkPluginContext*					m_pContext;

/***************************此处需要环境管理成员变量*******************************/

	//FxEarthEnv::IFxEnvironmentManager*	m_pEnvService;
};
#endif // CEFFECTSUIPLUGIN_H
