/**************************************************************************************************
* @file IModelSTMgrInterface.h
* @note 实体管理接口
* @author w00085
* @date 2021-5-11
**************************************************************************************************/
#ifndef IMODEL_MGR_SERVICE_H
#define IMODEL_MGR_SERVICE_H

#include "public/ModelStructDef.h"
#include <QObject>


/**
  * @class IDriveMgrInterface
  * @brief 实体驱动接口
  * @author w00085
*/
class IModelMgrInterface
{
public:

	/**
	* @brief	 获取装备属性信息
	* @param	 strName [in] 装备名称
	* @param	 modelAttribute [out] 装备属性
	* @return	 无
	* @note
	*/
	virtual void getModelAttribute(std::string strName,SModelAttribute& modelAttribute) = 0;
};

Q_DECLARE_INTERFACE(IModelMgrInterface, "freex_model_mgr_service")

#endif
