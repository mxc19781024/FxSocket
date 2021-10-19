/**************************************************************************************************
* @file ModelMgr.h
* @note 实体管理
* @author w00085
* @date 2021-5-11
**************************************************************************************************/
#ifndef FX_MODEL_3DMGR_H
#define FX_MODEL_3DMGR_H

#include <ctkPluginContext.h>
#include "IModelSTMgrInterface.h"

namespace FxDrive
{
	//添加实体索引表
	//typedef std::map<std::string, HGxObject::CGxObjectEle*>  MapObjectsIndex;

	/**
	* @class CDriveMgr
	* @brief 实体驱动管理
	* @author 
	*/
	class CModelMgr : public QObject,public IModelMgrInterface
	{
		Q_OBJECT
		Q_INTERFACES(IModelMgrInterface)

	public:
		CModelMgr();

		~CModelMgr();

		/**
		* @brief	 获取装备属性信息
		* @param	 strName [in] 装备名称
		* @param	 modelAttribute [out] 装备属性
		* @return	 无
		* @note
		*/
		void getModelAttribute(std::string strName, SModelAttribute& modelAttribute);
	protected:
		MapModelAttributeData          m_modelAttributeObjectsIndexMap;//装备索引表
	};

}
#endif //FX_UNIT_3DMGR_H
