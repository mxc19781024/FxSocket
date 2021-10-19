#include "CReadBuff.h"


CReadBuff* CReadBuff::m_Readbuff = nullptr;

int CReadBuff::m_count = 0;



void CReadBuff::setReadBuff(const std::string & msg)
{
	m_stdReadbuff.push(msg);
	m_count++;
}

const std::string  CReadBuff::getFirstReadbuffer()
{

	// TODO: 在此处插入 return 语句
	std::string msg = m_stdReadbuff.front();
	m_stdReadbuff.pop();
	m_count--;
	return msg;
}

bool CReadBuff::emity()
{
	if (0 == m_count)
	{
		return false;
	}
	else
	{
		return true;
	}
}

CReadBuff::CReadBuff()
{
}

CReadBuff::~CReadBuff()
{
	while (!m_stdReadbuff.empty())
	{
		m_stdReadbuff.pop();
	}
}

CReadBuff & CReadBuff::getInstanceReadBuff()
{
	// TODO: 在此处插入 return 语句
	if (m_Readbuff == nullptr)
	{
		m_Readbuff = new CReadBuff();
	}
	return *m_Readbuff;
}
