//#include <ICore.h>
//#include <CToolManager.h>

#include <QHBoxLayout>

#include "CMapLayerPropertyMgr.h"

#include <gdpScene/CommonGeo.h>
#include <gdpScene/layer/GxGlobalLayer.h> 
#include <gdpScene/layer/GxLayerModel.h> 

#include "GxUiFrameworkServiceInterface.h"

#include <GxPluginFrameworkCore.h>
#include <GxPluginFwLauncherInterface.h>
#include <GxEventHelperInterface.h>

#include "GxBasicCorePlugin_p.h"

CMapLayerPropertyMgr::CMapLayerPropertyMgr(QWidget *parent)
	: QWidget(parent)
	, m_pImgLayerProp(nullptr)
	, m_pDotLayerProp(nullptr)
	, m_pLineLayerProp(nullptr)
	, m_pFacetLayerProp(nullptr)
	, m_pUiFrameworkService(nullptr)
	, m_pHLayout(nullptr)
{
	m_pHLayout = new QHBoxLayout;
	m_pHLayout->setContentsMargins(0, 0, 0, 0);
	m_pHLayout->setMargin(0);

	this->setLayout(m_pHLayout);
}

CMapLayerPropertyMgr::~CMapLayerPropertyMgr()
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
}

void CMapLayerPropertyMgr::showLayerProperty(HGxScene::CGxGlobalLayer* pLayer)
{
  	if (pLayer == nullptr || m_pHLayout == nullptr)
  	{
  		return;
  	}

	if (NULL == m_pUiFrameworkService)
	{
		m_pUiFrameworkService = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(CGxBasicCorePlugin::getInstance()->getPluginContext());
	}

	// 清空布局中的窗口
	if (m_pHLayout->count() > 0)
	{
		while (m_pHLayout->count())
		{
			QLayoutItem* pLayoutItem = m_pHLayout->takeAt(0);

			QWidget* pWgt = pLayoutItem->widget();

			if (NULL == pWgt)
			{
				continue;
			}

			pWgt->setParent(NULL);
			delete pWgt;
			pWgt = nullptr;
		}
	}

 	// 影像
 	if (pLayer->getLayerType() == HGxScene::E_IMAGE_LAYER)
 	{
 		m_pImgLayerProp = new CMapImageLayerPropertyWgt;
 		connect(m_pImgLayerProp, SIGNAL(layerNameChangedSignal(const QString &)), this, SIGNAL(layerNameChangedSignal(const QString &)));
		m_pHLayout->addWidget(m_pImgLayerProp);

 		m_pImgLayerProp->setCurrentLayer(pLayer);
 	}
 	// 矢量
 	else if (pLayer->getLayerType() == HGxScene::E_VECTOR_LAYER)
 	{
 		HGxScene::CGxLayerModel* pModelLayer = static_cast<HGxScene::CGxLayerModel*>(pLayer);
 		// 矢量面
 		if (pModelLayer->hasPolygonFeature())
 		{

 			m_pFacetLayerProp = new CMapFacetLayerPropertyWgt();
 			connect(m_pFacetLayerProp, SIGNAL(layerNameChangedSignal(const QString &)), this, SIGNAL(layerNameChangedSignal(const QString &)));
			m_pHLayout->addWidget(m_pFacetLayerProp);

 			m_pFacetLayerProp->setCurrentLayer(pLayer);
 		}
 		// 矢量线
 		else if (pModelLayer->hasLineFeature())
 		{
			m_pLineLayerProp = new CMapLineLayerPropertyWgt();
			connect(m_pLineLayerProp, SIGNAL(layerNameChangedSignal(const QString &)), this, SIGNAL(layerNameChangedSignal(const QString &)));
			m_pHLayout->addWidget(m_pLineLayerProp);

 			m_pLineLayerProp->setCurrentLayer(pLayer);
 		}
 		// 矢量点
 		else if (pModelLayer->hasPointFeature())
 		{
 			m_pDotLayerProp = new CMapDotLayerPropertyWgt();
 			connect(m_pDotLayerProp, SIGNAL(layerNameChangedSignal(const QString &)), this, SIGNAL(layerNameChangedSignal(const QString &)));
			m_pHLayout->addWidget(m_pDotLayerProp);

 			m_pDotLayerProp->setCurrentLayer(pLayer);
 		}
 	}
//W00085
/*	m_pUiFrameworkService->showDockPanel("MapLayerProperty");*/
}

void CMapLayerPropertyMgr::changeCurrentLayer(HGxScene::CGxGlobalLayer* pLayer)
{
// 	if (!ICore::toolManager()->propertyWidgetCreatedAndVisible())
// 	{
// 		return;
// 	}

	if (NULL == m_pUiFrameworkService)
	{
		m_pUiFrameworkService = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(CGxBasicCorePlugin::getInstance()->getPluginContext());
	}

	// Todo 判断属性面板是否显示

 	showLayerProperty(pLayer);
}

void CMapLayerPropertyMgr::deleteLayerProperty(HGxScene::CGxGlobalLayer* pLayer)
{
	if (NULL == m_pUiFrameworkService)
	{
		m_pUiFrameworkService = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(CGxBasicCorePlugin::getInstance()->getPluginContext());
	}

// 	if (!m_pUiFrameworkService->isDockExists("MapLayerProperty"))
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

	if (bShow)
	{
		// 清空布局中的窗口
		while (m_pHLayout->count())
		{
			QLayoutItem* pLayoutItem = m_pHLayout->takeAt(0);

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
	}
}
