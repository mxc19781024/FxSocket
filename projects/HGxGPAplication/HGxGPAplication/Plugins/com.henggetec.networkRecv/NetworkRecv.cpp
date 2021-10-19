#include "NetworkRecv.h"

#include <bdpNet/GxServicer.h>
#include <bdpUtil/GxShareMemory.h>
#include <iostream>
#include "QByteArray"

#define  SEND_BUF_SIZE  4096

SUdpReceiver::SUdpReceiver(HGxNet::CGxServicer* pServicer)
	: m_pServicer(pServicer)
{
	m_pMemoryID = HGxUtil::CGxShareMemory::Instance()->createShareMemory(SEND_BUF_SIZE);
	if(m_pMemoryID >= 0)
	{
		m_pBuf = HGxUtil::CGxShareMemory::Instance()->getData(m_pMemoryID, 0);
	}
}

SUdpReceiver::~SUdpReceiver()
{
	if(m_pMemoryID >= 0)
	{
		HGxUtil::CGxShareMemory::Instance()->clearShareMemory(m_pMemoryID);
	}
}

int SUdpReceiver::recvData(char* pBuf, unsigned int uiLen)
{
	if(uiLen > 0)
	{
		std::string strTest(pBuf, uiLen);

		if(m_pServicer && m_pBuf)
		{
			char chData[SEND_BUF_SIZE] = { 0 };
			if (uiLen > 0)
			{
				QByteArray bytedata(pBuf, uiLen);
				emit recvDataSignal(bytedata);
			}
		}
	}

	return uiLen;
}
