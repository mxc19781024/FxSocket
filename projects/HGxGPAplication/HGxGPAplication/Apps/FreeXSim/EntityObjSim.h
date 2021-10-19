#ifndef ENTITYOBJSIM_H
#define ENTITYOBJSIM_H

#include <QObject>

#include "SimStructDef.h"

#include <QFile>

#include <QSet>


// 实体数据仿真
class EntityObjSim : public QObject
{
	Q_OBJECT

public:
	EntityObjSim();

	~EntityObjSim();

	SimEntityObj &getSimEntity();

	static double s_curLogicTime;

	// 真实周期 周期经过缩放因子缩放后
	static double s_dRelPeriod;

	// 参考开始时间
	static double s_dReferenTime;

public:

	// 模拟探测跟踪数据
	void GenerDetectData( SimEntityObj & simObj);

	// 根据实体的位置判断当前目标是否被探测到
	EnTrackState DetecTrarget(SimEntityObj &simTargetObj,EntityPtPs entiyPtps);

	// 模拟雷达波束数据
	void GenreRadarBeamData(  SimEntityObj & simObj);

	// 停止生成数据
	void StopGenrateData();

	// 获取在当前时刻所有Comsg包
	bool getObjComMsg(std::vector<ComMsg> &vecComsg);

	bool getObjComMsg(std::map<double ,std::vector<ComMsg> >  &mapVecComsg);

	void startSim();

protected:

	// 是否存活
	bool IsExsit(const SimEntityObj & simObj);

	// 获取当前新建实体包 // 每个实体只有一个
	bool getNewEntity(NewEntity &newEntity);

	// 获取状态包
	bool getEntityStu(std::vector<EntityStu> &entityStu);

	// 获取当前传感器数据
	bool getCurSenor(SimEntityObj &simObj, std::vector<Senor> &vecSenor);

	// 获取当前探测消息
	bool getCurDetect(SimEntityObj &simObj, std::vector<DetectCommand> &vecComand);

	// 获取当前通信状态
	bool getCurCommonState(SimEntityObj &simObj,EnConeState &nCommState,double dTime);

	// 获取当前分离事件信息
	bool getCurSubEvent(SimEntityObj &simObj,std::vector<SpeareSubMsg> &vecSepMsg);
protected:

	// 计算传感器状态
	int CaclSenorState(SimEntityObj &simObj, Senor & senorData);

	// 计算实体位置 不做插值只是按照数据文件中的点进行获取 插值后期考虑
	// bNeed 必须返回一个点 在计算时使用 实时发送时该时刻没有点没必要发送 
	bool CaclEntityPtPs(SimEntityObj &simObj,std::vector<EntityPtPs> &outPtPs,bool bNeed = false);

	// 数据中的时间是否在该仿真周期内
	bool DataTimeInPeriod(double dPeriod,double dDataTime);

	// 根据模拟探测结构生成对应ID
	QString getDetectStrId(SimDetect &simDetect);
public:

private:

	SimEntityObj m_simObj;

	// 当前实体航迹下标
	unsigned int m_unCurTrackIndex;

	// 当前雷达传感器实体下标
	unsigned int m_unSenorIndex;

	// 当前探测下标
	unsigned int m_unDetectIndex;

	// 当前分离消息下标
	unsigned int m_unSepIndex;

	// 传感器文件信息
	QFile m_fileSenor;

	// 探测信息存储文件 
	QFile m_fileDetect;

	// 生成探测目标时需要判断 每个雷达发现某个目标时需要记录状态 如果状态变化则发送消息 否则不发送
	std::map<QString,SimDetect> m_mapDetectState;


};


// 仿真实体管理
class EnityObjSimMrg : public QObject
{
	Q_OBJECT

public:

	// 单例
	static EnityObjSimMrg *instance();

	static void destroyInstance();

public:

	EnityObjSimMrg(){};

	~EnityObjSimMrg(){};

	//
public:

	// 添加仿真实体
	void addObjSim(EntityObjSim *pObjSim);

	// 销毁所有实体
	void destroyObjSim();

	// 获取所有仿真实体
	std::map<int, std::vector<EntityObjSim*> > &getObjSims();

	// 生成仿真数据
	void geneSimData();

	// 仿真前处理 将不在生命后期内的objSim全部移除
	void preSimObjLife();

	void getObjComMsg(std::vector<ComMsg> &vecMsg);

	// 关闭全部
	void clearAll();
protected:

	// 阵营-- EntityObjSim*
	std::map<int, std::vector<EntityObjSim*> > m_mapObjSim;

	// 
	static EnityObjSimMrg * s_pEnityObjSimMrg;

};

#endif // ENTITYOBJSIM_H
