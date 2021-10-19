#ifndef CCOMMONITEM_H
#define CCOMMONITEM_H

#include "CCheckableTreeItem.h"

/**
* @class	: CCommonItem
* @brief	: 普通非图层节点
* @note		: 
* @author	: kzm_00026
**/
class CCommonItem : public CCheckableTreeItem
{
public:
	CCommonItem(int type = Type);
	~CCommonItem();

	void setLayerVisible(bool) override;

private:
};

#endif // CCOMMONITEM_H
