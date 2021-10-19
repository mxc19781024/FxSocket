#include "./CWriteBudder.h"
#include <cstdio>
#include <iostream>
#include "../CUdpSocket/CSocket.h"
#define enum_to_string(x) #x


CWriteBudder* CWriteBudder::m_WriteBuff = nullptr;
int CWriteBudder::m_count = 0;

CWriteBudder::CWriteBudder()
{
}

CWriteBudder::~CWriteBudder()
{
	while (!m_stdWritebuff.empty())
	{
		m_stdWritebuff.pop();
	}
}

CWriteBudder & CWriteBudder::getWriteBuff()
{
	// TODO: 在此处插入 return 语句

	if (m_WriteBuff == nullptr)
	{
		m_WriteBuff = new CWriteBudder();
	}
	return *m_WriteBuff;
}

bool CWriteBudder::isemoty()
{
	if (0 == m_count)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CWriteBudder::setWriteBuff(const std::string & msg)
{
	m_stdWritebuff.push(msg);
	CWriteBudder::m_count++;
}


void CWriteBudder::setWriteBuff(const SNodeInfo3D& msg)
{
	std::string cmd = 
		std::to_string(CMD_getAircaftInformtion) +  '#' 
		+ msg.name							+ '|'
		+ std::to_string(msg.time)			+ '|'
		+ std::to_string(msg.optionType)	+ '|'
		+ std::to_string(msg.postion.type)	+ '|'
		+ std::to_string(msg.postion.x)		+ '|'
		+ std::to_string(msg.postion.y)		+ '|'
		+ std::to_string(msg.postion.z);
	std::cout << "==================================" << std::endl;
	std::cout  << cmd << std::endl;
	std::cout << "==================================" << std::endl;
	setWriteBuff(cmd);
}

const std::string  CWriteBudder::getWriteBuffer()
{
	// TODO: 在此处插入 return 语句
	if (0 == CWriteBudder::m_count)
	{
		return NULL;
	}
	// TODO: 在此处插入 return 语句
	std::string msg = m_stdWritebuff.front();
	m_stdWritebuff.pop();
	CWriteBudder::m_count--;
	return msg;
}

