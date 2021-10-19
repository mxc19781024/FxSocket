#include <QCryptographicHash>
#include <QDataStream>
#include <QBuffer>

#include "FxSingleInstanceGuard/CSingleInstanceGuard.h"



CSingleInstanceGuard::CSingleInstanceGuard(const QString &strKey, int iMaxDataSize, QObject *parent)
	: m_sharedMem(strKey)
{
	if (iMaxDataSize < 1)
	{
		iMaxDataSize = 1;
	}
	m_iMaxDataSize = iMaxDataSize;
}

CSingleInstanceGuard::~CSingleInstanceGuard()
{
	detachSharedMem();
}

bool CSingleInstanceGuard::runningInstanceExists()
{
	if (m_sharedMem.isAttached())
		return false;

	if (m_sharedMem.attach())
	{
		m_sharedMem.detach();
		return true;
	}
	return false;
}

bool CSingleInstanceGuard::start()
{
	// 如果已有运行中的实例，则启动失败
	if (runningInstanceExists())
		return false;

	bool bSuccess = m_sharedMem.create(m_iMaxDataSize);

	if (!bSuccess)
	{
		detachSharedMem();
		return false;
	}

	return true;
}


bool CSingleInstanceGuard::started()
{
	if (m_sharedMem.attach())
	{
		m_sharedMem.detach();
		return true;
	}
	return false;
}

void CSingleInstanceGuard::setSharedData(const QString &strData)
{
	if (!m_sharedMem.attach())
	{
		return;
	}

	QBuffer buffer;
	buffer.open(QBuffer::ReadWrite);
	QDataStream out(&buffer);
	out << strData;
	int iSize = buffer.size();

	m_sharedMem.lock();
	char *to = (char *)m_sharedMem.data();
	const char *from = buffer.data().data();
	memcpy(to, from, qMin(m_sharedMem.size(), iSize));
	m_sharedMem.unlock();
	m_sharedMem.detach();
}

QString CSingleInstanceGuard::sharedData()
{
	if (!m_sharedMem.attach())
	{
		return "";
	}

	QBuffer buffer;
	QDataStream in(&buffer);
	QString strSharedData;

	m_sharedMem.lock();
	buffer.setData((char *)m_sharedMem.constData(), m_sharedMem.size());
	buffer.open(QBuffer::ReadOnly);
	in >> strSharedData;
	m_sharedMem.unlock();
	m_sharedMem.detach();

	return strSharedData;
}

void CSingleInstanceGuard::detachSharedMem()
{
	if (m_sharedMem.isAttached())
		m_sharedMem.detach();
	bool bExists = m_sharedMem.attach();
	if (bExists)
		m_sharedMem.detach();
}

QString CSingleInstanceGuard::generateKeyHash(const QString &strKey, const QString &strSalt) const
{
	QByteArray data;

	data.append(strKey.toUtf8());
	data.append(strSalt.toUtf8());
	data = QCryptographicHash::hash(data, QCryptographicHash::Sha1).toHex();

	return data;
}
