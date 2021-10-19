#ifndef EARTH_LAYER_ITEM_H
#define EARTH_LAYER_ITEM_H

#include "CCheckableTreeItem.h"

#include "layer_ui/CLayerManager.h"
//#include "earth_layer/IFxEarthLayer.h"

#include <osg/observer_ptr>

namespace HGxScene
{
	class CGxGlobalLayer;
}

/** 
* @class	: CEarthLayerItem
* @brief	: 三维图层树节点
* @note		: 三维图层树节点
* @author	: k00026
*/
class CEarthLayerItem : public CCheckableTreeItem
{
public:

	/** 
	* @brief 构造函数
	* @param treeItemType 树节点类型(一级、二级分类、三级图层)
	* @param feLayer 底层三维图层对象
	*/
	CEarthLayerItem(int treeItemType = Type, HGxScene::CGxGlobalLayer *pEarthLayer = nullptr);
	~CEarthLayerItem();

 	// 获取底层三维图层对象
	HGxScene::CGxGlobalLayer* earthLayer() const;

	// 设置图层显隐
	void setLayerVisible(bool) override;

	// 获取图层类型
	CLayerConfig::LayerType layerType() const;


private:

	osg::observer_ptr<HGxScene::CGxGlobalLayer> m_pEarthLayer;	// 底层三维图层对象
};

#endif // CEARTHLAYERITEM_H
