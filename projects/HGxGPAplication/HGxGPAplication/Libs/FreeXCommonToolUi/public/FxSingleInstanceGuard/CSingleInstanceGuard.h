#ifndef SINGLE_INSTANCE_GUARD_H
#define SINGLE_INSTANCE_GUARD_H

#include <QObject>
#include <QSharedMemory>
//#include <FreeXSysConfig.h>

#include "freexcommontoolui_export.h"

/**
* @class	: CSingleInstanceGuard
* @brief	: 控制程序只有一个实例
* @note		: 
* @author	: k00026
**/
#if __unix__
#define FREEXCOMMONTOOLUI_EXPORT
#endif
class FREEXCOMMONTOOLUI_EXPORT CSingleInstanceGuard : public QObject
{
	Q_OBJECT

public:
	CSingleInstanceGuard(const QString &key, int iMaxDataSize = 1, QObject *parent = nullptr);
	~CSingleInstanceGuard();

	/**
	* @brief	: 启动
	*/
	bool start();

	bool started();

	void setSharedData(const QString &);

	QString sharedData();

private:
	bool runningInstanceExists();
	void detachSharedMem();
	QString generateKeyHash(const QString &key, const QString &salt) const;

private:
	QSharedMemory m_sharedMem;	// 共享内存
	int m_iMaxDataSize;			// 数据最大大小

};

#endif // SINGLE_INSTANCE_GUARD_H
