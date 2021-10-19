/**************************************************************************************************
* @file IFxBizDataMgrService.h
* @note 业务数据管理
* @author z00050
* @date 2019-9-7
**************************************************************************************************/
#ifndef IBIZDATA_MGR_SERVICE_H
#define IBIZDATA_MGR_SERVICE_H

#include <QObject>
/**
  * @class IFxBizDataMgrService
  * @brief 业务数据管理
  * @author z00050
*/
class IFxBizDataMgrService
{
public:
	//加载本地数据 
	//strPath 本地文件路径
	virtual void LoadFile(std::string strPath) = 0;
	
	//加载外部数据
	//qbyteData 接收UDP、TCP或WebService数据
	virtual bool LoadData(QByteArray qbyteData) = 0;
	
	//清空场景数据
	virtual void ClearSceneData() = 0;	
	
};

Q_DECLARE_INTERFACE(IFxBizDataMgrService, "freex_bizdata_mgr_service")

#endif
