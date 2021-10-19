#ifndef MSG_DATA_THREAD_H
#define MSG_DATA_THREAD_H

#include <QThread>
#include <QByteArray>
#include <QMutex>
#include <QObject>
#include "Apps\FreeXSim\StructDef.h"
#include <iostream>
#include "osg\Vec4"
#include "osg\Referenced"
#include "common\UnitCommon.h"


// 数据接收和处理线程

class CMsgDataParserThread : public QThread
{
	Q_OBJECT
public:
	CMsgDataParserThread();
	~CMsgDataParserThread();

	void Start();

	void Stop();
signals :
	void sig_parseData(const ComMsg&);
protected slots:
    void slot_receiveData(QByteArray byteArray);

	void pushData(const std::vector<ComMsg>& msgList);
protected:

	void run();

	virtual void msgDataHandler(const ComMsg&) {}
protected:
	bool m_bStop;
	bool m_bHasData; // todo： 改为信号
	QMutex m_mutextDataCopy;
	std::vector<ComMsg> m_vectDataPending;
	std::vector<ComMsg> m_vectDataProcessing;
};

#endif //SIM_CONTROL_H
