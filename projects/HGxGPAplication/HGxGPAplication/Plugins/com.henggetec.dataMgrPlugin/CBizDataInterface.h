/**************************************************************************************************
* @file CBizDataInterface.h
* @note 界面信号槽
* @author z00050
* @date 2019-6-10
**************************************************************************************************/
#ifndef CBIZDATA_DATA_INTERFACE_H
#define CBIZDATA_DATA_INTERFACE_H

#include <QMutex>
#include <ctkPluginContext.h>
#include <CBizDataParse.h>
#include <IFxBizDataMgrService.h>

class CBizDataSend;
class CBizDataInterface : public QObject , public IFxBizDataMgrService
{
	Q_OBJECT
		Q_INTERFACES(IFxBizDataMgrService)

public:
	CBizDataInterface(ctkPluginContext *context,QObject *parent = nullptr);
	~CBizDataInterface();
	
	//加载本地数据 
	//strPath 本地文件路径
	virtual void LoadFile(std::string strPath);

	//加载外部数据
	//qbyteData 接收UDP、TCP或WebService数据
	virtual bool LoadData(QByteArray qbyteData);

	//清空场景数据
	virtual void ClearSceneData();	

protected:
	//清空场景
	void ClearScene();

signals:
	void SignalAllTimeAixBegin(bool bState);

protected slots:
	void SlotFileParseFinish(bool bState);
	void SlotReceiveDriverData();
	void TimerUpdateSlot();
	void LoadFlyPathData();
private:
	ctkPluginContext*		m_pContext;	
	CBizDataParse*			m_pSceneParser;
	CBizDataSend*           m_pBizDataSend;
	std::map<int,VectorDriverAircraftObjectsData> m_mapIndexToVecDriverData;
};

/**
* @class CBizDataParse
* @brief 部署文件解析类
* @author g00037
*/
class CBizDataSend : public QThread
{  
	Q_OBJECT
public:  
	CBizDataSend(CBizDataParse*	pSceneParser); 
	~CBizDataSend();  

public:	
	void SetData(std::map<int,VectorDriverAircraftObjectsData> mapIndexToVecDriverData,
		std::map<float, RoleCommandVec> mapIndexRoleCommandData,
		std::map<float, vecZaijiRadarState> mapZaijiRadarState);

	//清空已有的数据
	void ClearData();

protected:
	//解析线程
	virtual void run();
private:  
	QMutex m_mutex;
	CBizDataParse*			m_pSceneParser;
	bool                    m_bDown;
	std::map<int,VectorDriverAircraftObjectsData> m_mapIndexToVecDriverData;
	std::map<float, RoleCommandVec>               m_mapIndexRoleCommandData;
	std::map<float, vecZaijiRadarState>           m_mapZaijiRadarStateData;

}; 

#endif //CBIZDATA_DATA_INTERFACE_H
