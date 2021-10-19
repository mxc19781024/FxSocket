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

// ������������࣬���ڽ���ҵ�����ݵĽ�����
class CSimControlCenter : public QObject, public osg::Referenced
{
	Q_OBJECT

public:
	CSimControlCenter();

	// ����ip�Ͷ˿ں�
	void setIpAndPort(char* ipAddr, unsigned int port);
	// ��ʼ���ݴ����߳�
	void startThreads();
	// ֹͣ���ݴ����߳�
	void stopThreads();

signals :

	void sig_eventCommandNotify(std::string eventCmd);
	// ��λ��
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

	bool						  m_bRadarVisible;  //�״�
	bool						  m_bTrackVisible;  //����
	bool						  m_bLinkLineVisible;//������
	bool						  m_bTraksVisible;  //����
	bool						  m_bLabel;			//����
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
