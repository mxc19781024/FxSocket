#include "ConfigMgr.h"
#include <QApplication>

/**************************************************************************************************
* @file 
* @note 返回当前数据文件对应的文件路径
**************************************************************************************************/

#define DATA_FOLDER_NAME "SimData"
#define DATA_SIMDATASET_NAME "DataSet"
#define TRACDATA_FOLDER_NAME "TrackData"
#define SENORDATA_FOLDER_NAME "SenorData"
#define DETECTDATA_FOLDER_NAME "DetectData"

FilePathMgr::FilePathMgr()
{
	// 
}

FilePathMgr::~FilePathMgr()
{
	// 
}

QString FilePathMgr::getDataFileDir()
{
	// 
	return QApplication::applicationDirPath() + "/"+DATA_FOLDER_NAME + "/";
}

QString FilePathMgr::getSimDataSetDir()
{
	// 
	return getDataFileDir() + DATA_SIMDATASET_NAME + "/";
}

QString FilePathMgr::getTrackDataDir()
{
	// 
	return getSimDataSetDir() + TRACDATA_FOLDER_NAME + "/";
}

QString FilePathMgr::getSenorDataDir()
{
	//
	return getSimDataSetDir() + SENORDATA_FOLDER_NAME + "/";
}

QString FilePathMgr::getDetectDataDir()
{
	return getSimDataSetDir() + DETECTDATA_FOLDER_NAME + "/";
}

QString FilePathMgr::getSenorDataPath( int entityId )
{
	QString strFileName = QString("%1_SENOR.txt").arg(entityId);
	return getSenorDataDir() + strFileName;
}

QString FilePathMgr::getDetectDataPath( int entityId )
{
	QString strFileName = QString("%1_Detect.txt").arg(entityId);

	return getDetectDataDir() + strFileName;
}
