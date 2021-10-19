//#include <FreeXFramework/FreeXFramework.h>
//#include <FreeXFramework/LoadInfo.h>

#include <scene_ui/ISceneUiService.h>
#include <scene_ui/ISceneInterface.h>

#include "CEffectsUiPlugin.h"
#include <QTime>

//#include <ICore.h>
//#include <CMainWindowManager.h>
#include <QFutureWatcher>
#include <qtconcurrentrun.h>
#include <functional>
//#include <FxLoadIndicator/CLoadIndicator.h>
#include <bdpCommon/GxStrUtil.h>

#include "FxLoadIndicator/CLoadIndicator.h"

#include "bdpScene/scene/GxControlManager.h"


CEffectsUiPlugin::CEffectsUiPlugin(ctkPluginContext *context)
	: m_pEffectWgt(NULL)
	, m_pContext(context)

{
	context->registerService<IEffectsUiService>(this);

	if (!m_pEffectWgt)
	{
		m_pEffectWgt = new CSpecialEffectsControl(context);
	}
}

CEffectsUiPlugin::~CEffectsUiPlugin()
{
	if(m_pEffectWgt)
	{
		delete m_pEffectWgt;
		m_pEffectWgt = NULL;
	}
}

// bool CEffectsUiPlugin::load(const FxCommonUtils::TiXmlElement* pEle, FxCore::IFxWorkspaceProcess* pProcess/*=NULL*/)
// {
// 	const FxCommonUtils::TiXmlElement* pElePlugin = pEle->FirstChildElement("plugin_EarthEnv_ui");
// 	if (!pElePlugin)
// 	{
// 		FxCore::CLoadInfo::Instance()->removeCount();
// 		return false;
// 	}
// 
// 	if (!m_pEffectWgt)
// 	{
// 		FxCore::CLoadInfo::Instance()->removeCount();
// 		return false;
// 	}
// 
// 	// 加载各个场景的环境效果		
// 	const FxCommonUtils::TiXmlElement* pElement = NULL;
// 	for (pElement = pElePlugin->FirstChildElement("scene"); pElement; pElement = pElement->NextSiblingElement("scene"))
// 	{
// 
// 		CLoadIndicator::addUseCount(ICore::mainWindowManager()->mainWindow());
// 
// 		std::string strSceneName = pElement->Attribute("SceneName");
// 
// 		// 设置三维场景名称
// 		if (m_pEffectWgt)
// 		{
// 			m_pEffectWgt->setSceneName(strSceneName);
// 		}
// 
// 		m_pEnvService = m_pEffectWgt->getEnvService(strSceneName);
// 		if (!m_pEnvService)
// 		{
// 			FxCore::CLoadInfo::Instance()->removeCount();
// 			return false;
// 		}
// 
// 		//加载晕渲、阴影
// 		//(w00224 2019-10-28)
// 		//begin
// 		const FxCommonUtils::TiXmlElement* pElementTemp = pElement->FirstChildElement("layerEffect");
// 		if (pElementTemp)
// 		{
// 			// 通过三维图层服务设置地球阴影和晕渲效果的初始状态
// 			FxEarth::IFxEarthLayerService* pLayerService = getEarthLayerService();
// 			if (pLayerService)
// 			{
// 				//w00224 2020-5-27
// 				FxEarth::IFeLayerEffect* normalEffectLayer = pLayerService->getEffectLayer(FxEarth::g_strNormMapEffect);
// 				if (normalEffectLayer)
// 				{
// 					normalEffectLayer->setVisible(FxCommonUtils::StringToBool(pElementTemp->Attribute("normalEffect")));
// 				}
// 
// 				FxEarth::IFeLayerEffect* contourEffectLayer = pLayerService->getEffectLayer(FxEarth::g_strContourMapEffect);
// 				if (contourEffectLayer)
// 				{
// 					contourEffectLayer->setVisible(FxCommonUtils::StringToBool(pElementTemp->Attribute("contourEffect")));
// 				}
// 			}
// 
// 		}
// 		//end
// 
// 		//z00011 增加任务数量
// 		FxCore::CLoadInfo::Instance()->addCount();
// 		//end 
// 		QFutureWatcher<std::string> *watcher = new QFutureWatcher<std::string>();
// 		connect(watcher, SIGNAL(finished()), this, SLOT(loadFinishedSlot()));
// 		//x00038 qt5不支持多线程渲染 makeCurrent报错,所以海洋需要在此处初始化一下
// 		m_pEffectWgt->initOceanForQtFive();
// 
// 		//end
// 		watcher->setFuture(QtConcurrent::run(std::bind(&CEffectsUiPlugin::loadInThread, this, pElement, strSceneName)));
// 	}
// 
// 	return true;
// }

// bool CEffectsUiPlugin::save(FxCommonUtils::TiXmlElement* pEle, FxCore::IFxWorkspaceProcess* pProcess/*=NULL*/)
// {
// 	// 新建节点( 通过场景名称关联标绘所在场景，load的时候使用 )
// 	FxCommonUtils::TiXmlElement* pElePlugin = new FxCommonUtils::TiXmlElement("plugin_EarthEnv_ui");
// 
// 	if (!m_pEffectWgt)
// 	{
// 		return false;
// 	}
// 
// 	// 获取所有的三维场景
// 	ISceneUiService *pSceneService = getService<ISceneUiService>();
// 	if (pSceneService == nullptr)
// 	{
// 		return false;
// 	}
// 	QList<ISceneInterface *> listEarthScenes = pSceneService->earthScenes();
// 
// 	// 保存每个三维场景中的环境
// 	FxCommonUtils::TiXmlElement *pSceneEle = nullptr;
// 	foreach(auto *pScene, listEarthScenes)
// 	{
// 		pSceneEle = new FxCommonUtils::TiXmlElement("scene");
// 		pSceneEle->SetAttribute("SceneName", pScene->sceneName().toUtf8().data());
// 		pElePlugin->LinkEndChild(pSceneEle);
// 
// 		FxEarthEnv::IFxEnvironmentManager* pEnvService = m_pEffectWgt->getEnvService(pScene->sceneName().toUtf8().data());
// 		if (pEnvService != nullptr)
// 		{
// 			pEnvService->save(pSceneEle, pProcess);
// 		}
// 	}
// 	pEle->LinkEndChild(pElePlugin);
// 
// 	//晕渲、阴影在底层不归属与环境，但是在UI层是一起的，所以在这里单独记录这两个效果
// 	//(w00224 2019-10-28)
// 	//begin
// 	// 通过三维图层服务获取地球阴影和晕渲效果的初始状态
// 	FxEarth::IFxEarthLayerService* pLayerService = getEarthLayerService();
// 	if (pLayerService)
// 	{
// 		FxCommonUtils::TiXmlElement* pLayerEffect = new FxCommonUtils::TiXmlElement("layerEffect");
// 		//w00224 2020-5-27
// 		FxEarth::IFeLayerEffect* normalEffectLayer = pLayerService->getEffectLayer(FxEarth::g_strNormMapEffect);
// 		if (normalEffectLayer)
// 		{
// 			pLayerEffect->SetAttribute("normalEffect", FxCommonUtils::BoolToString(normalEffectLayer->getVisible()).c_str());
// 		}
// 
// 		FxEarth::IFeLayerEffect* contourEffectLayer = pLayerService->getEffectLayer(FxEarth::g_strContourMapEffect);
// 		if (contourEffectLayer)
// 		{
// 			pLayerEffect->SetAttribute("contourEffect", FxCommonUtils::BoolToString(contourEffectLayer->getVisible()).c_str());
// 		}
// 
// 		pSceneEle->LinkEndChild(pLayerEffect);
// 	}
// 	//end
// 
// 	return true;
// }

QWidget* CEffectsUiPlugin::getWgt()
{
	if (m_pEffectWgt)
	{
		return m_pEffectWgt;
	}
	return NULL;
}

//  std::string CEffectsUiPlugin::loadInThread(const FxCommonUtils::TiXmlElement*pElement, const std::string &strSceneName)
//  {
//  	ISceneUiService *pSceneService = getService<ISceneUiService>();
//  	if (!pSceneService)
//  	{
//  		//FxCore::CLoadInfo::Instance()->removeCount();
//  		return "";
//  	}
//  	pSceneService->pauseRender(true);
//  	m_pEnvService->load(pElement, nullptr);
//  	return strSceneName;
//  }

void CEffectsUiPlugin::loadFinishedSlot()
{
// 	QFutureWatcher<std::string> *pWatcher = static_cast<QFutureWatcher<std::string> *>(sender());
// 	std::string strSceneName = pWatcher->result();
// 	if (strSceneName.empty())
// 	{
// 		return;
// 	}
// 	
// 	HGxScene::CGxControllerMgr::getInstance()->pauseSimulation("3D", false);
// 	
// 	if (m_pEffectWgt)
// 	{
// 		SEarthEnvInfo stEarthEnvInfo;
// 		bool bSuccess = m_pEffectWgt->getEarthEnvData(stEarthEnvInfo);
// 		if (bSuccess)
// 		{
// 			m_pEffectWgt->updateUI(stEarthEnvInfo);
// 		}
// 	}

	// 引用计数减一
	//CLoadIndicator::removeUseCount();

	//FxCore::CLoadInfo::Instance()->removeCount();
}
