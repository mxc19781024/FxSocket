#ifndef MAP_LAYER_ITEM_H
#define MAP_LAYER_ITEM_H

#include <osg/ref_ptr>

#include "CCheckableTreeItem.h"

#include "layer_ui/CLayerManager.h"
//#include "map_layer/IFxMapLayer.h"

namespace HGxScene
{
	class CGxGlobalLayer;
}

/** 
* @class CMapLayerItem
* @brief 二维图层树节点
* @note 二维图层树节点
* @author kzm
*/
class CMapLayerItem : public CCheckableTreeItem
{
public:
	/** 
	* @brief 构造函数
	* @note 
	* @param layerType 图层类型(影像、高程、矢量)
	* @param treeItemType 树节点类型(分类节点、图层节点)
	* @param feLayer 底层二维图层对象
	*/

	CMapLayerItem(int treeItemType = Type, HGxScene::CGxGlobalLayer* pLayer = NULL);
	~CMapLayerItem();

 	// 获取底层二维图层对象
 	HGxScene::CGxGlobalLayer* mapLayer() const;

	// 设置图层显隐
	void setLayerVisible(bool) override;

	// 获取图层类型
	CLayerConfig::LayerType layerType() const;

private:

	osg::ref_ptr<HGxScene::CGxGlobalLayer> m_pMapLayer;		// 底层二维图层对象
};

#endif // CEARTHLAYERITEM_H
