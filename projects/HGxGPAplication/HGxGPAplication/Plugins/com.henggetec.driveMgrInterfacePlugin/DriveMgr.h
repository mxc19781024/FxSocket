/**************************************************************************************************
* @file DriveMgr.h
* @note 实体驱动管理接口
* @author w00085
* @date 2021-5-11
**************************************************************************************************/
#ifndef FX_UNIT_3DMGR_H
#define FX_UNIT_3DMGR_H

#include <ctkPluginContext.h>
#include "IDriveMgrInterface.h"
#include <bdpObject/GxObjectManager.h>

namespace FxDrive
{
	//添加实体索引表
	typedef std::map<std::string, HGxObject::CGxObjectEle*>  MapObjectsIndex;
	//添加实体索引表
	typedef std::map<std::string, HGxObject::CGxPathModel*>  MapObjectsPathIndex;
	/**
	* @class CDriveMgr
	* @brief 实体驱动管理
	* @author 
	*/
	class CDriveMgr : public QObject,public IDriveMgrInterface
	{
		Q_OBJECT
		Q_INTERFACES(IDriveMgrInterface)

	public:
		CDriveMgr();

		~CDriveMgr();

		/**
		* @brief	 添加实体
		* @param	 objectData [in] 实体数据集
		* @return	 无
		* @note
		*/
		virtual void addUnit(SNewObjectData objectData);

		/**
		* @brief	 获取实体属性
		* @param	 strID [in] 实体id
		* @return	 实体属性指针
		* @note      
		*/
		virtual HGxObject::CGxObjectEle* getUnitEle(std::string strID);

		/**
		* @brief	 驱动实体
		* @param	 objectData [in] 实体数据集
		* @return	 无
		* @note
		*/
		virtual void driveUnit(SDriveObjectData objectData);

		/**
		* @brief	 驱动航天器实体
		* @param	 driverData [in] 航天器实体数据集
		* @return	 无
		* @note      
		*/
		virtual void driveAircraftUnit(VectorDriverAircraftObjectsData driverData);

		/**
		* @brief	 驱动航空器实体
		* @param	 driverData [in] 航空器实体数据集
		* @return	 无
		* @note      
		*/
		virtual void addSpacecraftUnit(VectorSpacecraftObjectsData driverData);

		///**
		//* @brief	 添加附着节点，包含传感器、标牌、尾迹、锁定、飘带、空域等
		//* @param     pAttachBase [in] 附着节点
		//* @return	 无
		//* @note
		//*/
		//virtual void addAttachUnit(AttachBase* pAttachBase);

		///**
		//* @brief	 更新附着节点，包含传感器、标牌、尾迹、锁定、飘带、空域等
		//* @param     pAttachBase [in] 附着节点
		//* @return	 无
		//* @note
		//*/
		//virtual void updateAttachUnit(AttachBase* pAttachBase);

		virtual void addSensorUnit(HGxObject::CGxEffectEle* sObjectEle);
		/**
		* @brief	 销毁实体
		* @param	 destroyID [in] 销毁实体id
		* @return	 无
		* @note      场景窗只能在中央核心区域
		*/
		virtual void destroyUnit(std::string destroyID);

		/**
		* @brief	 实体第一人称跟踪
		* @param	 strFirstTrackID [in] 实体id
		* @return	 无
		* @note      
		*/
		virtual void firstTrackUnit(const std::string strFirstTrackID);

		/**
		* @brief	 实体第三人称跟踪
		* @param	 strThirdTrackID [in] 实体id
		* @return	 无
		* @note      
		*/
		virtual void thirdTrackUnit(const std::string strThirdTrackID);

		/**
		* @brief	 清空场景
		* @return	 无
		* @note      无
		*/
		virtual void clearUnit();
	protected:
		HGxObject::CGxObjectManager *m_pObjectInstence; //对象管理器
		MapObjectsIndex              m_pObjectsIndexMap;//添加实体索引表,方便查找实体是否已添加
		//MapObjectsPathIndex          m_pObjectsPathIndexMap;//添加实体路径索引表,方便查找实体路径是否已添加
	};

}
#endif //FX_UNIT_3DMGR_H
