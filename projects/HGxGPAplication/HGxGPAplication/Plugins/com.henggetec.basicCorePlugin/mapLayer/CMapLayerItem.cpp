#include "CMapLayerItem.h"

#include <gdpScene/layer/GxGlobalLayer.h>

CMapLayerItem::CMapLayerItem(int treeItemType, HGxScene::CGxGlobalLayer* pMapLayer)
	: CCheckableTreeItem(treeItemType)
	, m_pMapLayer(pMapLayer)
{

}

CMapLayerItem::~CMapLayerItem()
{

}

HGxScene::CGxGlobalLayer* CMapLayerItem::mapLayer() const
{
 	return m_pMapLayer.get();
}

void CMapLayerItem::setLayerVisible(bool bVisible)
{
 	if (m_pMapLayer.valid())
 	{
 		m_pMapLayer->setVisible(bVisible);
 	}
}

CLayerConfig::LayerType CMapLayerItem::layerType() const
{
	if (m_pMapLayer.valid())
 	{
 		if (m_pMapLayer->getLayerType() == HGxScene::E_LIVE_LAYER)
 		{
 			return CLayerConfig::ImageLayer;
 		}
		else if (m_pMapLayer->getLayerType() == HGxScene::E_VECTOR_LAYER)
 		{
 			return CLayerConfig::VectorLayer;
 		}
 	}
	return CLayerConfig::UndefinedLayer;
}