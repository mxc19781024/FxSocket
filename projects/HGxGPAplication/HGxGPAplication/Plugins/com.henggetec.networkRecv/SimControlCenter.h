#ifndef SIM_CONTROL_H
#define SIM_CONTROL_H

#include <QThread>
#include <QByteArray>
#include <QMutex>
#include <QObject>
#include "Apps\FreeXSim\StructDef.h"
#include <iostream>
#include "osg\Vec4"
#include "osg\Referenced"
#include "common\UnitCommon.h"

class CMsgDataParserThread;

// 仿真控制中心类，用于进行业务数据的解析等
class CSimControlCenter : public QObject, public osg::Referenced
{
	Q_OBJECT

public:
	CSimControlCenter();

	// 设置ip和端口号
	void setIpAndPort(char* ipAddr, unsigned int port);
	// 开始数据处理线程
	void startThreads();
	// 停止数据处理线程
	void stopThreads();

signals :

	void sig_eventCommandNotify(std::string eventCmd);
	// 单位秒
	void sig_timeUpdate(double dTime);

	void sig_simInfo(SimInfo simInfo);
	void sig_init();

protected slots:
	//void slot_receiveData(QByteArray byteArray);
	void slot_msgDataHandler(const ComMsg&);

protected:
	void executeMsg(NewEntity& msg);
	void executeMsg(Senor& msg);
	void executeMsg(EntityStu& msg);
	void executeMsg(DestoryCommand& msg);
	void executeMsg(HitCommand& msg);
	void executeMsg(EventCommand& msg);
	void executeMsg(AimInfoMsg& msg);
	void executeMsg(AirSpeceSectorInfo &msg);

protected:
	CMsgDataParserThread*		  m_dataParserThread;

	bool						  m_bRadarVisible;  //雷达
	bool						  m_bTrackVisible;  //跟踪
	bool						  m_bLinkLineVisible;//连接线
	bool						  m_bTraksVisible;  //航迹
	bool						  m_bLabel;			//标牌
	bool                          m_bStopThreads;
};

Q_DECLARE_METATYPE(ViewLocatCmd);
Q_DECLARE_METATYPE(PeerView);
Q_DECLARE_METATYPE(PeerE2TView);
Q_DECLARE_METATYPE(TracView);
Q_DECLARE_METATYPE(SimInfo)
Q_DECLARE_METATYPE(TraveView)
Q_DECLARE_METATYPE(TraveTargetView)

#endif //SIM_CONTROL_H
