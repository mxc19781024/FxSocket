#pragma once
/**************************************************************************************************
* @file simData.h
* @note 模拟数据
* @author 
* @data 
**************************************************************************************************/
#include <QThread>
#include "SimStructDef.h"
#include <QMutex>

#include "MultiCast.h"

bool DataTimeInPeriod( double dLogicTime,double dPeriod,double dDataTime);

template<typename T>
struct VecIndex
{
	VecIndex()
	{
		nIndex = 0;
	}

	void resetIndex(unsigned index)
	{
		nIndex = index;
	}

	bool getCurT(double dLogicTime,double dPeriod,std::vector<T> &ret)
	{
		// 
		unsigned  vecSize = vecT.size();

		if (vecSize > 0)
		{
			double dNextTime = dLogicTime + dPeriod / SimTimer::getInstance()->getTimeFactor();
			if (vecT[0].dTime  > dNextTime || vecT[vecSize - 1].dTime < dLogicTime )
			{
				return false;
			}
		}
		// 找第一个大于该时间点的点
		for (;nIndex < vecSize; ++nIndex)
		{
			if (DataTimeInPeriod(dLogicTime,dPeriod ,vecT[nIndex].dTime))
			{
				ret.push_back(vecT[nIndex]);
			}
			else if (vecT[nIndex].dTime >= dLogicTime + SimTimer::getInstance()->getPeriod() / SimTimer::getInstance()->getTimeFactor())
			{
				break;
			}
		}

		if (nIndex == vecSize)
		{
			nIndex = 0;
		}

		return ret.size() > 0;
	}

	std::vector<T> vecT;
	unsigned nIndex;
};

// 网络信息
struct NetInfo
{
	// IP
	QString strIp;
	// 端口
	int nPort;
};

// 先在一个线程中处理 处理数据与UDP发送数据
// 后期再优化 数据缓冲区与 UDP发送 UDP发送也可采用抽点的形式在多个线程中进行发送但是意义不大
// 
class SimData : public QThread 
{
	Q_OBJECT

public:
	SimData(void);
	~SimData(void);

public:

	// 
	void Start();
	// 
	void Stop();
	// 数据仿真
	void DataSim();

	// 设置是否暂停
	void SetPause(bool bPause);

	// 获取暂停状态
	bool GetPause();

	// 时间越大速度越慢
	void SetTimeFactor(double dFactor);

signals:
	// 场景持续时长
	void SimTimeLenSignal(double dSec);
	// 当前时间
	void CurTimeSignal(double dSec);

	// 场景名称
	void CurSceneNameSignal(QString strName);

protected:

	// 生成管理对象
	void GenerSimObj();
	// 
	void run();
	// 解析网络信息
	void ParseNetInfo();

	// 处理通用信息
	void HandlCommInfo(std::vector<ComMsg> &vecMsg);

	// 同一时刻好多数据 处理通用信息
	void HandlCommInfo(std::map<double, std::vector<ComMsg> > &mapVecComsg);

	// 发送场景信息消息
	void SendSceneInfoMsg();

	// 发送场景初始化消息
	void SendSceneInitMsg();

protected:

	bool m_bStop;

	SimInfo m_simInfo;

	// 事件信息数组
	QString m_strEvenFilePath;
	VecIndex<EventCommand> m_vecEnventInfo;

	// 击中事件
	QString m_strHitFilePath;
	VecIndex<HitCommand> m_vecHitCommand;

	// 销毁命令
	QString m_strDesFilePath;
	VecIndex<DestoryCommand> m_vecDestoryCommand;

	// 视点信息
	QString m_strViewFilePath;
	VecIndex<ViewInfoMsg> m_vecViewCommandInfo;

	// 字幕消息
	QString m_strScrInfoFilePath;
	VecIndex<ScreenWordInfo> m_vecScreenWordInfo;

	//
	QString m_strAimDataFilePath;
	VecIndex<AimInfoMsg> m_vecAimInfo;

	QString m_strAirSpaceFilePath;

	VecIndex<AirSpeceSectorInfo> m_vecAirSpaceInfo;

	// 发送数据网络信息
	NetInfo m_sendNetInfo;

	// 接收数据网络信息
	NetInfo m_recvNetInfo;

	// 
	std::vector<ComMsg> m_vecComMsg;

	// 
	std::map<double, std::vector<ComMsg> > m_mapComsg;

	// 仿真开始时间
	long double m_dStartSysTime;

	// 当前逝去时间
	long double m_dCurEplaseTime;

	// 暂停
	bool m_bPause;

	CMultiCast *m_pSendUdp;

	// 记录暂停时刻
	long double m_dStopTime;
	// 互斥锁
	QMutex m_mutex;
};

