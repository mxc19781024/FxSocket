#pragma once
/**************************************************************************************************
* @file ConfigMgr.h
* @note 配置管理
* @author Y00012
* @data 2021-1-4
**************************************************************************************************/

#include <QString>

/**
  * @class 文件路径管理
  * @brief 管理文件路径
  * @note 详细说明
  * @author Y00012
*/
class FilePathMgr
{
public:
	FilePathMgr();

	~FilePathMgr();

	// 获取数据文件路径
	static QString getDataFileDir();

	// 获取获取仿真数据集文件目录
	static QString getSimDataSetDir();

	// 获取航迹数据目录
	static QString getTrackDataDir();

	// 获取传感器/雷达数据目录
	static QString getSenorDataDir();

	// 探测数据路径
	static QString getDetectDataDir();

	// 获取传感器数据路径
	static QString getSenorDataPath(int entityId);

	// 获取探测数据文件路径
	static QString getDetectDataPath(int entityId);

};

