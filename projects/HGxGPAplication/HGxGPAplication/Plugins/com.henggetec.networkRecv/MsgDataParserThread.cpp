#include "MsgDataParserThread.h"


CMsgDataParserThread::CMsgDataParserThread()
	: m_bStop(false),
	m_bHasData(false)
{

}


CMsgDataParserThread::~CMsgDataParserThread()
{
	if (isRunning())
	{
		Stop();
		quit();
		wait();
	}
}

void CMsgDataParserThread::slot_receiveData(QByteArray byteArray)
{
	// 数据包含的命令块个数
	unsigned int numMsg = byteArray.size() / sizeof(ComMsg);
	if ((numMsg * sizeof(ComMsg)) != byteArray.size())
	{
		std::cout << "!!!!!! 数据大小异常 !!!!!!!" << std::endl;
		return;
	}

	std::vector<ComMsg> vectMsg(numMsg);
	memcpy(vectMsg.data(), byteArray.data(), byteArray.size());
	pushData(vectMsg);
}

void CMsgDataParserThread::pushData(const std::vector<ComMsg>& msgList)
{
	m_mutextDataCopy.lock();
	m_vectDataPending.insert(m_vectDataPending.end(), msgList.begin(), msgList.end());
	m_bHasData = true;
	m_mutextDataCopy.unlock();
}

void CMsgDataParserThread::Start()
{
	m_bStop = false; 
	start();
}

void CMsgDataParserThread::Stop()
{
	m_bStop = true;
	m_bHasData = true;
}


void CMsgDataParserThread::run()
{
	while (!m_bStop)
	{
		// 等待数据
		while (!m_bHasData)
		{
			msleep(10);
		}

		m_mutextDataCopy.lock();
		m_vectDataProcessing.assign(m_vectDataPending.begin(), m_vectDataPending.end());
		m_vectDataPending.clear();
		m_bHasData = false;
		m_mutextDataCopy.unlock();

		for (int n = 0; n < m_vectDataProcessing.size(); ++n)
		{
			emit sig_parseData(m_vectDataProcessing.at(n));
			//msgDataHandler(m_vectDataProcessing.at(n));
		}
	}
}
