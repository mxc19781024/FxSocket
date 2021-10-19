#pragma once

// 解析所有数据文件

#include <QString>

#include "SimStructDef.h"

class SimDataParse
{
public:
	SimDataParse(void);
	~SimDataParse(void);

	// 解析传感器数据
	void ParseSenorData(QString strFilePath, std::vector<Senor> &vecSenorData);

	// 解析事件数据
	void ParseEventData(QString strFilePath,std::vector<EventCommand> &vecEvent);

	// 解析视点数据
	void ParseViewData(QString strFilePath,std::vector<ViewInfoMsg> &vecViewCommandInfo);

	// 解析字幕数据
	void ParseScreenWord(QString strFilePath,std::vector<ScreenWordInfo> &vecScreenWordInfo);

	// 解析雷达侦测/跟踪数据
	void ParseDetectData(QString strFilePath,std::vector<DetectCommand> &vecTrackCommand);

	// 解析击中命令
	void ParseHitData(QString strFilePath, std::vector<HitCommand> &vecHitCommand);

	// 解析销毁命令
	void ParseDestryData(QString strFilePath,std::vector<DestoryCommand> &vecDestoryCommand);

	// 解析航迹数据
	void ParseTracLineData(QString strFilePath, std::vector<EntityPtPs> &vecTrackPath);

	// 解析瞄准数据
	void ParseAimMsgData(QString strFilePath, std::vector<AimInfoMsg> &vecAimInfos);

	// 解析空域
	void ParseAirSpaceData(QString strFilePath, std::vector<AirSpeceSectorInfo> &vecAirSpaceInfo);

protected:
	
	// 视点定位
	bool parseViewLocal(const QStringList &strList, ViewInfoMsg &viewInfoMsg);

	// 视点凝视
	bool parseViewPeer(const QStringList &strList, ViewInfoMsg &viewInfoMsg);

	// 
	bool parseViewTrave(const QStringList &strList, ViewInfoMsg &viewInfoMsg);
	// 
	bool parseViewTraveTarget(const QStringList &strList, ViewInfoMsg &viewInfoMsg);

	// 视点对看
	bool parseViewE2T(const QStringList &strList, ViewInfoMsg &viewInfoMsg);

	// 跟踪视角
	bool parseViewTrack(const QStringList &strList, ViewInfoMsg &viewInfoMsg);

};

