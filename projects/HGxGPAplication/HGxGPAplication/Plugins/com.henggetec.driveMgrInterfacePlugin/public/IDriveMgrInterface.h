/**************************************************************************************************
* @file IDriveMgrInterface.h
* @note 实体驱动接口
* @author w00085
* @date 2021-5-11
**************************************************************************************************/
#ifndef IDRIVE_MGR_SERVICE_H
#define IDRIVE_MGR_SERVICE_H

#include <QObject>
#include "common/UnitCommon.h"
#include "bdpObject/GxEffectEle.h"
#include "common/SenorStructDef.h"
#include "bdpObject/GxObjectEle.h"


/**
  * @class IDriveMgrInterface
  * @brief 实体驱动接口
  * @author w00085
*/
class IDriveMgrInterface
{
public:

	/**
	* @brief	 添加实体
	* @param	 objectData [in] 实体数据集
	* @return	 无
	* @note
	*/
	virtual void addUnit(SNewObjectData objectData) = 0;

	/**
	* @brief	 获取实体属性
	* @param	 strID [in] 实体id
	* @return	 实体属性指针
	* @note
	*/
	virtual HGxObject::CGxObjectEle* getUnitEle(std::string strID) = 0;

	/**
	* @brief	 驱动实体
	* @param	 objectData [in] 实体数据集
	* @return	 无
	* @note
	*/
	virtual void driveUnit(SDriveObjectData objectData) = 0;

	/**
	* @brief	 驱动航天器实体
	* @param	 driverData [in] 航天器实体数据集
	* @return	 无
	* @note
	*/
	virtual void driveAircraftUnit(VectorDriverAircraftObjectsData driverData) = 0;

	/**
	* @brief	 驱动航空器实体
	* @param	 driverData [in] 航空器实体数据集
	* @return	 无
	* @note
	*/
	virtual void addSpacecraftUnit(VectorSpacecraftObjectsData driverData) = 0;

	///**
	//* @brief	 添加附着节点，包含传感器、标牌、尾迹、锁定、飘带、空域等
	//* @param     pAttachBase [in] 附着节点
	//* @return	 无
	//* @note
	//*/
	//virtual void addAttachUnit(AttachBase* pAttachBase) = 0;

	///**
	//* @brief	 更新附着节点，包含传感器、标牌、尾迹、锁定、飘带、空域等
	//* @param     pAttachBase [in] 附着节点
	//* @return	 无
	//* @note
	//*/
	//virtual void updateAttachUnit(AttachBase* pAttachBase) = 0;

	/**
	* @brief	 添加传感器
	* @param	 
	* @return	 无
	* @note
	*/
	virtual void addSensorUnit(HGxObject::CGxEffectEle* sObjectEle) = 0;

	/**
	* @brief	 销毁实体
	* @param	 destroyID [in] 销毁实体id
	* @return	 无
	* @note      场景窗只能在中央核心区域
	*/
	virtual void destroyUnit(std::string destroyID) = 0;

	/**
	* @brief	 实体第一人称跟踪
	* @param	 strFirstTrackID [in] 实体id
	* @return	 无
	* @note
	*/
	virtual void firstTrackUnit(const std::string strFirstTrackID) = 0;

	/**
	* @brief	 实体第三人称跟踪
	* @param	 strThirdTrackID [in] 实体id
	* @return	 无
	* @note
	*/
	virtual void thirdTrackUnit(const std::string strThirdTrackID) = 0;

	/**
	* @brief	 清空场景
	* @return	 无
	* @note      无
	*/
	virtual void clearUnit() = 0;
};

Q_DECLARE_INTERFACE(IDriveMgrInterface, "freex_drive_mgr_service")

#endif
