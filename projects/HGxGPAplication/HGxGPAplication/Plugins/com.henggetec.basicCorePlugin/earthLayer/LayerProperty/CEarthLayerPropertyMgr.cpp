//#include <ICore.h>
//#include <CToolManager.h>

#include <QHBoxLayout>

#include "earthLayer/LayerProperty/CEarthLayerPropertyMgr.h"

#include "gdpScene/layer/GxGlobalLayer.h"
#include "gdpScene/layer/GxLayerModel.h"
#include <gdpScene/CommonGeo.h>

#include <GxPluginFrameworkCore.h>
#include <GxPluginFwLauncherInterface.h>
#include <GxServiceQuerier.h>

#include "GxUiFrameworkServiceInterface.h"

#include <GxPluginFrameworkCore.h>
#include <GxPluginFwLauncherInterface.h>
#include <GxEventHelperInterface.h>

#include "GxBasicCorePlugin_p.h"

using namespace HGxPFWCore;

CEarthLayerPropertyMgr::CEarthLayerPropertyMgr(QWidget *parent)
	: QWidget(parent)
	, m_pImgLayerProp(nullptr)
	, m_pDotLayerProp(nullptr)
	, m_pLineLayerProp(nullptr)
	, m_pFacetLayerProp(nullptr)
	, m_pEleLayerProp(nullptr)
	, m_pLiveLayerProp(nullptr)
	, m_pUiFrameworkService(nullptr)
	, m_pHBoxLayout(nullptr)
{
	m_pHBoxLayout = new QHBoxLayout;
	m_pHBoxLayout->setContentsMargins(0, 0, 0, 0);
	m_pHBoxLayout->setSpacing(0);

	this->setLayout(m_pHBoxLayout);
}

CEarthLayerPropertyMgr::~CEarthLayerPropertyMgr()
{
	if (m_pImgLayerProp != nullptr)
	{
		m_pImgLayerProp->deleteLater();
		m_pImgLayerProp = nullptr;
	}
	if (m_pDotLayerProp != nullptr)
	{
		m_pDotLayerProp->deleteLater();
		m_pDotLayerProp = nullptr;
	}
	if (m_pLineLayerProp != nullptr)
	{
		m_pLineLayerProp->deleteLater();
		m_pLineLayerProp = nullptr;
	}
	if (m_pFacetLayerProp != nullptr)
	{
		m_pFacetLayerProp->deleteLater();
		m_pFacetLayerProp = nullptr;
	}
	if (m_pEleLayerProp != nullptr)
	{
		m_pEleLayerProp->deleteLater();
		m_pEleLayerProp = nullptr;
	}
	if (m_pLiveLayerProp != nullptr)
	{
		m_pLiveLayerProp->deleteLater();
		m_pLiveLayerProp = nullptr;
	}
}

void CEarthLayerPropertyMgr::showLayerProperty(HGxScene::CGxGlobalLayer* pLayer)
{
 	if (pLayer == nullptr)
 	{
 		return;
 	}

	if (NULL == m_pUiFrameworkService)
	{
		m_pUiFrameworkService = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(CGxBasicCorePlugin::getInstance()->getPluginContext());
	}

	// 清空布局中的窗口
	while (m_pHBoxLayout->count())
	{
		QLayoutItem* pLayoutItem = m_pHBoxLayout->takeAt(0);

		QWidget* pWgt = pLayoutItem->widget();

		if (NULL == pWgt)
		{
			continue;
		}

		pWgt->setParent(NULL);
		delete pWgt;
		pWgt = nullptr;
	}
 
 	// 影像
 	if (pLayer->getLayerType() == HGxScene::E_IMAGE_LAYER)
 	{
 		m_pImgLayerProp = new CEarthImageLayerPropertyWgt;
 		connect(m_pImgLayerProp, SIGNAL(layerNameChangedSignal(const QString &)), this, SIGNAL(layerNameChangedSignal(const QString &)));
		m_pHBoxLayout->addWidget(m_pImgLayerProp);

 		m_pImgLayerProp->setCurrentLayer(pLayer);
 	}
 	// 矢量
 	else if (pLayer->getLayerType() == HGxScene::E_VECTOR_LAYER)
 	{
		HGxScene::CGxLayerModel* pModelLayer = static_cast<HGxScene::CGxLayerModel *>(pLayer);

 		// 矢量面
 		if (pModelLayer->hasPolygonFeature())
 		{
 			m_pFacetLayerProp = new CEarthFacetLayerPropertyWgt();
 			connect(m_pFacetLayerProp, SIGNAL(layerNameChangedSignal(const QString &)), this, SIGNAL(layerNameChangedSignal(const QString &)));
			m_pHBoxLayout->addWidget(m_pFacetLayerProp);

 			m_pFacetLayerProp->setCurrentLayer(pLayer);
 		}
 		// 矢量线
 		else if (pModelLayer->hasLineFeature())
 		{
 			m_pLineLayerProp = new CEarthLineLayerPropertyWgt();
 			connect(m_pLineLayerProp, SIGNAL(layerNameChangedSignal(const QString &)), this, SIGNAL(layerNameChangedSignal(const QString &)));
			m_pHBoxLayout->addWidget(m_pLineLayerProp);

 			m_pLineLayerProp->setCurrentLayer(pLayer);
 		}
 		// 矢量点
 		else if (pModelLayer->hasPointFeature())
 		{
 			m_pDotLayerProp = new CEarthDotLayerPropertyWgt();
 			connect(m_pDotLayerProp, SIGNAL(layerNameChangedSignal(const QString &)), this, SIGNAL(layerNameChangedSignal(const QString &)));
			m_pHBoxLayout->addWidget(m_pDotLayerProp);

 			m_pDotLayerProp->setCurrentLayer(pLayer);
 		}
 	}
 	// 高程
 	else if (pLayer->getLayerType() == HGxScene::E_ELEVA_LAYER)
 	{
 		m_pEleLayerProp = new CEleLayerPropertyWgt;
 		connect(m_pEleLayerProp, SIGNAL(layerNameChangedSignal(const QString &)), this, SIGNAL(layerNameChangedSignal(const QString &)));
		m_pHBoxLayout->addWidget(m_pEleLayerProp);

 		m_pEleLayerProp->setCurrentLayer(pLayer);
 	}
 	// 效果
 	else if (pLayer->getLayerType() == HGxScene::E_EFFECT_LAYER)
 	{
 		// 暂无
 	}
 	// 倾斜摄影
 	else if (pLayer->getLayerType() == HGxScene::E_LIVE_LAYER)
 	{
 		m_pLiveLayerProp = new CTiltPropertyWgt();
 		connect(m_pLiveLayerProp, SIGNAL(layerNameChangedSignal(const QString &)), this, SIGNAL(layerNameChangedSignal(const QString &)));
		m_pHBoxLayout->addWidget(m_pLiveLayerProp);

 		m_pLiveLayerProp->setFeLiveLayer(pLayer);
 	}
	//W00085
/*	m_pUiFrameworkService->showDockPanel("EarthLayerProperty");*/
}

void CEarthLayerPropertyMgr::changeCurrentLayer(HGxScene::CGxGlobalLayer* pLayer)
{
//  	if (!ICore::toolManager()->propertyWidgetCreatedAndVisible())
//  	{
//  		return;
//  	}

	if (NULL == m_pUiFrameworkService)
	{
		m_pUiFrameworkService = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(CGxBasicCorePlugin::getInstance()->getPluginContext());
	}

	//Todo 属性面板是否显示

	showLayerProperty(pLayer);
}

void CEarthLayerPropertyMgr::deleteLayerProperty(HGxScene::CGxGlobalLayer* pLayer)
{
	if (NULL == m_pUiFrameworkService)
	{
		m_pUiFrameworkService = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(CGxBasicCorePlugin::getInstance()->getPluginContext());
	}

// 	if (!m_pUiFrameworkService->isDockExists("EarthLayerProperty"))
// 	{
// 		return;
// 	}

	bool bShow = false;

	// 影像
	if (pLayer->getLayerType() == HGxScene::E_IMAGE_LAYER)
	{
		if (m_pImgLayerProp)
		{
			if (m_pImgLayerProp->getCurrentLayer() == pLayer)
			{
				bShow = true;
			}
		}
	}
	// 矢量
	else if (pLayer->getLayerType() == HGxScene::E_VECTOR_LAYER)
	{
		HGxScene::CGxLayerModel* pModelLayer = static_cast<HGxScene::CGxLayerModel *>(pLayer);

		// 矢量面
		if (pModelLayer->hasPolygonFeature())
		{
			if (m_pFacetLayerProp)
			{
				if (m_pFacetLayerProp->getCurrentLayer() == pLayer)
				{
					bShow = true;
				}
			}
		}
		// 矢量线
		else if (pModelLayer->hasLineFeature())
		{
			if (m_pLineLayerProp)
			{
				if (m_pLineLayerProp->getCurrentLayer() == pLayer)
				{
					bShow = true;
				}
			}
		}
		// 矢量点
		else if (pModelLayer->hasPointFeature())
		{
			if (m_pDotLayerProp)
			{
				if (m_pDotLayerProp->getCurrentLayer() == pLayer)
				{
					bShow = true;
				}
			}
		}
	}
	// 高程
	else if (pLayer->getLayerType() == HGxScene::E_ELEVA_LAYER)
	{
		if (m_pEleLayerProp)
		{
			if (m_pEleLayerProp->getCurrentLayer() == pLayer)
			{
				bShow = true;
			}
		}
	}
	// 效果
	else if (pLayer->getLayerType() == HGxScene::E_EFFECT_LAYER)
	{
		// 暂无
	}
	// 倾斜摄影
	else if (pLayer->getLayerType() == HGxScene::E_LIVE_LAYER)
	{
		if (m_pLiveLayerProp)
		{
			if (m_pLiveLayerProp->getCurrentLayer() == pLayer)
			{
				bShow = true;
			}
		}
	}

	if (bShow)
	{
		// 清空布局中的窗口
		while (m_pHBoxLayout->count())
		{
			QLayoutItem* pLayoutItem = m_pHBoxLayout->takeAt(0);

			QWidget* pWgt = pLayoutItem->widget();

			if (NULL == pWgt)
			{
				continue;
			}

			pWgt->setParent(NULL);
			delete pWgt;
			pWgt = nullptr;
		}

		// 影像
		if (pLayer->getLayerType() == HGxScene::E_IMAGE_LAYER)
		{
			m_pImgLayerProp = NULL;
		}
		// 矢量
		else if (pLayer->getLayerType() == HGxScene::E_VECTOR_LAYER)
		{
			HGxScene::CGxLayerModel* pModelLayer = static_cast<HGxScene::CGxLayerModel *>(pLayer);

			// 矢量面
			if (pModelLayer->hasPolygonFeature())
			{
				m_pFacetLayerProp = NULL;
			}
			// 矢量线
			else if (pModelLayer->hasLineFeature())
			{
				m_pLineLayerProp = NULL;
			}
			// 矢量点
			else if (pModelLayer->hasPointFeature())
			{
				m_pDotLayerProp = NULL;
			}
		}
		// 高程
		else if (pLayer->getLayerType() == HGxScene::E_ELEVA_LAYER)
		{
			m_pEleLayerProp = NULL;
		}
		// 效果
		else if (pLayer->getLayerType() == HGxScene::E_EFFECT_LAYER)
		{
			// 暂无
		}
		// 倾斜摄影
		else if (pLayer->getLayerType() == HGxScene::E_LIVE_LAYER)
		{
			m_pLiveLayerProp = NULL;
		}
	}
}
