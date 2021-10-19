#ifndef SENOR_STRUCT_DEF
#define SENOR_STRUCT_DEF

// 传感器类型
enum EnAttachType
{
	Radar_Beam = 1,      // 开机
	Radar_Senor = 2,     // 雷达	
	Air_SpeceSector = 3, // 空域
	Aim = 4,             // 瞄准
	Label = 5,           // 标牌
	Earth_Lock = 6,      // 锁定
	Track_Line = 7,      // 尾迹
	Wake_Ribbon = 8,     // 飘带
};

//传感器集合
class AttachBase
{
public:
	virtual void foo(){}
	EnAttachType eEnAttachType;//传感器类型
	std::string  strEntityID;       // 附着对象ID
	std::string  strId;             // 附着对象自身ID
};

// 连接状态
enum EnConeSenorState
{
	LINK = 1,// 连接
	UNLINkE = 2// 未连接
};

// 雷达波束信息
class RadarBeamSenorData : public AttachBase
{
public:
	std::string   strSenorId;   // 传感器ID	
	std::string   strBeamId;    // 波速ID
	double dHorWidth;  // 水平宽度
	double dVerWidth;  // 垂直宽度
	double dAzAngle;   // 中心航向角
	double dPitchAngle;// 中心俯仰角
	char   snTurn;
};

// 传感器状态
enum EnMachSenorState
{	
	OPEN = 1, // 开机
	CLOSE = 2,// 关机	
	FAULT = 3 // 故障
};

// 雷达状态单独发
class RadarSenorData : public AttachBase
{
public:
	double          dTime;
	double          dAzStartAngle;   // 包络开始航向角
	double          dAzEndAngle;     // 包络结束航向角
	double          dPicthStartAngle;// 包络开始俯仰角 
	double          dPicthEndAngle;  // 包络结束俯仰角
	double          dRaduis;         // 雷达半径
	int             nBeamNum;
	EnMachSenorState     enState;         // 传感器状态 1-开机 2-关机 3-故障 	
	double          dScanPeriod;     // 波束扫描周期
	char            nPrefer;         // 1-角度相对于实体姿态 2-角度相对于绝对绝对角度 航向角相对于正北，俯仰角水平为0向下为正 向上为正
	RadarBeamSenorData   beam;
};

enum EnAirSpeceSectorOperateType
{
	ADD = 1,
	MOD = 2,
	DEL = 3
};

//空域
class AirSpeceSectorData : public AttachBase
{
public:
	double        dTime;
	std::string   strName;   // 名称
	double        vecPosition[3];//中心点位置（经纬高）;
	double        dStartAngle;	 // 开始角度 方位--- 单位:°
	double        dSwingAngle;	 // 摇摆范围 单位:°
	double        dRadius;	     // 半径--米
	double        dH;	         // 空域高度
	double        vecColor[4];   // 颜色
	EnAirSpeceSectorOperateType enType;
};

class AimSenorData : public AttachBase
{
public:
	double dTime;        // 时戳	
	std::string   strTargetId;    // 目标Id	
	bool   bAim;         // 开始/结束瞄准	
	double dContinueTime;// 持续时长 根据加减速计算好
};

class LabelInfo : public AttachBase
{
public:
	std::string  strName;            // 名字
	std::string  strPara;            // 显示信息	
	osg::Vec4    vecLineColor;       // 边框线颜色	
	osg::Vec4    vecBackgroundColor; // 背景色	
	osg::Vec4    vecTextColor;       // 文字颜色
	float        fWidth;             // 线宽	
	float        fFontSize;          // 字体大小	
};

#endif //FXSIM_STRUCT_DEF