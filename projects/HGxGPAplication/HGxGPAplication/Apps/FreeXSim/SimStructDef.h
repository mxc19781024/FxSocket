#ifndef SIM_STRUCT_DEF
#define SIM_STRUCT_DEF
/**************************************************************************************************
* @file simStructDef.h
* @note 模拟结构体
* @author 
* @data 
**************************************************************************************************/
#include <map>
#include <vector>


#include "StructDef.h"

// 时段状态信息
struct TimeSegState
{
	// 
	double dStartTime;

	double dEndTime;

	// 状态
	int nState;
};

// 3D 向量
struct Vector3D
{

	double dX;

	double dY;

	double dZ;
};

// 实体位置姿态数据
struct EntityPtPs
{
	// 时间戳
	double dTime;
	// 经-deg 纬-deg 高-米
	Vector3D pt;

	// 姿态数据 俯仰-deg 横滚-deg 偏航-deg
	Vector3D posture;
};

// 波束数据
struct BeamData
{
	BeamData()
	{
		// 
		snTurn = 1;
	}
	// 波速ID
	int nBeamId;

	// 水平宽度
	double dHorWidth;

	// 垂直宽度
	double dVerWidth;

	// 中心航向角
	double dAzAngle;

	// 中心俯仰角
	double dPitchAngle;

	// 转动方向 1-正向 2-逆向
	short snTurn;
};


struct SenorData
{
	// 时戳
	double dTime;

	int nSenorId;

	double dAzStartAngle;

	double dAzEndAngle;

	double dPitchStartAngle;

	double dPicthEndAngle;

	int nState;

	// 波束信息
	std::vector<BeamData> vecBeamData;

};

// 仿真实体对象
struct SimEntityObj
{
	SimEntityObj()
	{
		dExsitStartTime = 0;
		dExsitEndTime = 0;
	}

	// 实体信息
	NewEntity entiyInfo;

	// 开始时间
	double dExsitStartTime;

	// 结束时间
	double dExsitEndTime;

	// 通信时段信息
	std::vector<TimeSegState> vecCommusState;
	// 雷达状态时段信息 id-时段
	std::map<int , std::vector<TimeSegState> > mapSenorsState;

	// 传感器数据路径
	std::vector<QString> vecSenorDataPath;

	// 传感器数据 暂时是一个传感器 一个雷达波束 不会有问题
	std::vector<Senor>  vecSenorData;

	QString strTrackFilePath;
	// 航迹数据
	std::vector<EntityPtPs> vecTrackData;

	// 跟踪探测消息
	QString m_strDetectFilePath;
	std::vector<DetectCommand> vecDetectCommand;

	// 分离级别信息
	std::vector<SepareSubInfo> vecSepSubInfo;
	
};

struct SimDetect
{
	// 实体Id
	int nEntityId;

	// 目标Id
	int nTargetId;

	// 跟踪状态
	EnTrackState enTrackState;
};



#endif //SIM_STRUCT_DEF