#include "CEarthLayerItem.h"

#include "gdpScene/layer/GxGlobalLayer.h"

CEarthLayerItem::CEarthLayerItem(int type, HGxScene::CGxGlobalLayer *pEarthLayer)
	: CCheckableTreeItem(type)
	, m_pEarthLayer(pEarthLayer)
{

}

CEarthLayerItem::~CEarthLayerItem()
{

}

HGxScene::CGxGlobalLayer* CEarthLayerItem::earthLayer() const
{
	return m_pEarthLayer.get();
}

void CEarthLayerItem::setLayerVisible(bool bVisible)
{
	if (m_pEarthLayer.valid())
	{
		m_pEarthLayer->setVisible(bVisible);
	}
}

CLayerConfig::LayerType CEarthLayerItem::layerType() const
{
	if (m_pEarthLayer.valid())
	{
		if (m_pEarthLayer->asImageLayer())
		{
			return CLayerConfig::ImageLayer;
		}
		else if (m_pEarthLayer->asVectorLayer())
		{
			return CLayerConfig::VectorLayer;
		}
		else if (m_pEarthLayer->asElevationLayer())
		{
			return CLayerConfig::ElevationLayer;
		}
		else if (m_pEarthLayer->asLiveLayer())
		{
			return CLayerConfig::LiveLayer;
		}
	}
	return CLayerConfig::UndefinedLayer;
}