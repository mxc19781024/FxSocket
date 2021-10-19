#ifndef FXSIM_STRUCT_DEF
#define FXSIM_STRUCT_DEF

#pragma pack(push,1)

#define NAME_MAX_LEN 100
#define DESCRIPT_TXT_MAX_LEN 200

#define BEAM_MAX_NUM 2
#define SENOR_MAX_NUM 2

// 消息最大长度
#define MAX_MSG_LEN 1000

// 视点结构最大长度
#define MAX_VIEW_LEN 300

// 分离组件最大数量
#define MAX_SEPSUB_NUM 4 

// 连接状态
enum EnConeState
{
	// 连接
	LINK_CON = 1,
	// 未连接
	UNLINkE_CON = 2
};

enum EnTrackState
{
	TRACK_STATE = 1,

	UNTRACK_STATE = 2
};
// 跟踪命令
struct DetectCommand
{
	DetectCommand()
	{
		// 
		dTime = 0;
		nEntityID = 0;
		nTargetId = 0;
		enTrackState = EnTrackState::UNTRACK_STATE;
	}

	~DetectCommand()
	{
		dTime = 0;
		nEntityID = 0;
		nTargetId = 0;
	}
	double dTime;
	int nEntityID;
	int nTargetId;
	// 1 - 跟踪 2-取消跟踪 
	EnTrackState enTrackState;
};


// 心跳包
struct TimeMsg
{
	double dTime;
};

// 雷达波束信息
struct RadarBeamInfo
{
	RadarBeamInfo()
	{
		// 传感器ID
		nSenorId = 0;
		// 波速ID
		nBeamId = 0;
		// 水平宽度
		dHorWidth = 0;
		// 垂直宽度
		dVerWidth = 0;
		// 中心航向角
		dAzAngle = 0;
		// 中心俯仰角
		dPitchAngle = 0;

		// 转向1- 正 2-逆
		snTurn = 1;
	}

	// 传感器ID
	int nSenorId;
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

	char snTurn;

};

// 机器状态
enum EnMachState
{
	// 开机
	OPEN_STATE = 1,
	// 关机
	CLOSE_STATE = 2,
	// 故障
	FAULT_STATE = 3

};

// 雷达状态单独发
struct Senor
{	
	Senor()
	{
		// 实体ID
		nEntityID =0;
		// 雷达自身ID
		nSeorId =0;
		// 包络开始航向角
		dAzStartAngle = 0;
		// 包络结束航向角
		dAzEndAngle = 0;
		// 包络开始俯仰角 
		dPicthStartAngle = 0;
		// 包络结束俯仰角
		dPicthEndAngle = 0;
		// 雷达半径
		dRaduis = 0;	
		// 
		nBeamNum = 0;
		// 传感器状态 1-开机 2-关机 3-故障 
		enState = EnMachState::OPEN_STATE;
		// 波束扫描周期
		dScanPeriod = 0;

		// 1-角度相对于实体姿态 2-角度相对于绝对绝对角度 航向角相对于正北，俯仰角水平为0向下为正 向上为正
		nPrefer = 1;

	}

	~Senor()
	{
		// 实体ID
		nEntityID =0;
		// 雷达自身ID
		nSeorId =0;

		// 包络开始航向角
		dAzStartAngle = 0;
		// 包络结束航向角
		dAzEndAngle = 0;
		// 包络开始俯仰角 
		dPicthStartAngle = 0;
		// 包络结束俯仰角
		dPicthEndAngle = 0;
		// 雷达半径
		dRaduis = 0;	
		// 
		nBeamNum = 0;
		// 传感器状态 1-开机 2-关机 3-故障 
		enState = EnMachState::OPEN_STATE;
		// 波束扫描周期
		dScanPeriod = 0;

		// 1-角度相对于实体姿态 2-角度相对于绝对绝对角度 航向角相对于正北，俯仰角水平为0向下为正 向上为正
		nPrefer = 1;

		dTime = 0;
	}

	double dTime;
	// 实体ID
	int nEntityID;
	// 雷达自身ID
	int nSeorId;

	// 包络开始航向角
	double dAzStartAngle;
	// 包络结束航向角
	double dAzEndAngle;
	// 包络开始俯仰角 
	double dPicthStartAngle;
	// 包络结束俯仰角
	double dPicthEndAngle;
	// 雷达半径
	double dRaduis;	
	// 
	int nBeamNum;
	// 传感器状态 1-开机 2-关机 3-故障 
	EnMachState enState;
	// 波束扫描周期
	double dScanPeriod;

	// 1-角度相对于实体姿态 2-角度相对于绝对绝对角度 航向角相对于正北，俯仰角水平为0向下为正 向上为正
	char nPrefer;

	RadarBeamInfo beam[BEAM_MAX_NUM];
};


// 阵营
enum EnCamp
{
	// 红方
	RED_CAMP = 1,
	// 蓝方
	BLUE_CAMP = 2,
	// 未知 不明
	UNKNOWN_CAMP = 3
};

enum EnAirspace
{
	// 海
	SEA_SPACE = 1,
	// 陆
	GROUND_SPACE = 2,
	// 空
	AIR_SPACE = 3
};

//1-飞机 2-坦克 3-导弹发射车 4-导弹 5-航母 6-护卫舰 7-直升机
// 装备类型
enum EnEquType
{
	AIRCAFT_EUT = 1,

	TANK_EUT = 2,

	MISSILE_VECHICLE_EUT = 3,

	MISSILE_EUT = 4,

	CARRIER_EUT = 5,

	FRIGATE_EUT = 6,

	COPTER_EUT = 7,

	GROUND_EUT = 8
};

// 分离组件基本信息
struct SepareSubInfo
{
	SepareSubInfo()
	{
		chLevel = 0;
		nSubId = 0;
		dTime = 0;
	}

	// 分离级别
	char chLevel;

	// 组件id
	int nSubId;

	// 分离时间
	double dTime;
};

struct SpeareSubMsg
{
	SpeareSubMsg()
	{
		dTime = 0;
		nPrimId = 0;
		nSubId = 0;
		chLevel = 0;
	}
	// 时刻
	double dTime;
	// 主实体ID
	int nPrimId;
	// 需要分离的实体ID
	int nSubId;
	// 分离级别
	char chLevel;
};

// 新建实体
struct NewEntity
{
	NewEntity()
	{
		dTime = 0;
		// 上级ID 通信指挥关系
		parentId = 0;	
		// 自身ID
		nEntityID = 0;
		// 阵营 1-红方 2-蓝方 3-不明
		enCamp = EnCamp::UNKNOWN_CAMP;
		// 实体所属空域 1-海上装备 2-陆地装备 3-空中装备 
		enSpaceType = EnAirspace::AIR_SPACE; 
		// 1-飞机 2-坦克 3-导弹发射车 4-导弹 5-航母 6-护卫舰
		enEquType = EnEquType::AIRCAFT_EUT;
		// 传感器数量
		nSenorNum = 0;
		// 实体初始姿态
		// 偏航、俯仰、横滚
		dAzAngle = 0;
		dPicthAngle = 0;
		dRolAngle = 0;

		// 实体初始位置
		// 经纬高-度
		dLon = 0;
		dLat = 0;
		dAlt = 0; 	

		sepNum = 0;

		dOffsetX = 0;
		dOffsetY = 0;
		dOffSetZ = 0;

		// 姿态调整
		dRoteX = 0;
		dRoteY = 0;
		dRoteZ = 0;

		sepNum = 0;
		nComPriId = 0;
		sepLevel = 0;

		// 实体名称
		memset(cName,0,NAME_MAX_LEN);
	}

	~NewEntity()
	{
		dTime = 0;
		// 上级ID 通信指挥关系
		parentId = 0;	
		// 自身ID
		nEntityID = 0;
		// 阵营 1-红方 2-蓝方 3-不明
		enCamp = EnCamp::UNKNOWN_CAMP;
		// 实体所属空域 1-海上装备 2-陆地装备 3-空中装备 
		enSpaceType = EnAirspace::AIR_SPACE; 
		// 1-飞机 2-坦克 3-导弹发射车 4-导弹 5-航母 6-护卫舰
		enEquType = EnEquType::AIRCAFT_EUT;
		// 传感器数量
		nSenorNum = 0;
		// 实体初始姿态
		// 偏航、俯仰、横滚
		dAzAngle = 0;
		dPicthAngle = 0;
		dRolAngle = 0;

		// 实体初始位置
		// 经纬高-度
		dLon = 0;
		dLat = 0;
		dAlt = 0; 	

		sepNum = 0;
		nComPriId = 0;
		sepLevel = 0;

		// 实体名称
		memset(cName,0,NAME_MAX_LEN);
	}

	double dTime;
	// 上级ID 通信指挥关系
	int parentId;	
	// 自身ID
	int nEntityID;
	// 阵营 1-红方 2-蓝方 3-不明
	EnCamp enCamp;
	// 实体所属空域 1-海上装备 2-陆地装备 3-空中装备 
	EnAirspace enSpaceType; 
	// 1-飞机 2-坦克 3-导弹发射车 4-导弹 5-航母 6-护卫舰
	EnEquType enEquType;
	// 传感器数量
	int nSenorNum;
	// 雷达
	Senor senor[SENOR_MAX_NUM];
	// 实体初始姿态
	// 偏航、俯仰、横滚
	double dAzAngle;
	double dPicthAngle;
	double dRolAngle;

	// 实体初始位置
	// 经纬高-度
	double dLon;
	double dLat;
	double dAlt;	

	// 通信状态 1-联通 2-断开
	EnConeState enCommuState;

	// 分离实体id
	SepareSubInfo sepInfo[MAX_SEPSUB_NUM];

	// 子组件实体数量
	char sepNum;

	// 分离级别 0 为无效 --
	char sepLevel;

	// 组合主题ID 为大于0 的数字 为零则无效
	int nComPriId;

	// 实体名称
	char cName[NAME_MAX_LEN];

	// 组合实体使用
	// 相对于主实体偏移 位置
	double dOffsetX;

	double dOffsetY;

	double dOffSetZ;

	// 姿态调整
	double dRoteX;

	double dRoteY;

	double dRoteZ;
};

enum EnOperateType
{
	ADD_OP = 1,

	MOD_OP = 2,

	DEL_OP = 3
};


struct AirSpeceSectorInfo
{
	AirSpeceSectorInfo()
	{
		// 时刻
		dTime = 0;
		// 
		camp = UNKNOWN_CAMP;
		// 
		nEntityId = 0;
		// 中心点 地理位置:LLH--LL:度,H:米
		// 经纬高-度
		dLon = 0;
		dLat = 0;
		dAlt = 0;	
		// 开始角度 方位--- 单位:°
		dStartAngle = 0;
		// 摇摆范围 单位:°
		dSwingAngle = 0;
		// 半径--米
		dRadius = 0;

		memset(chName,0,NAME_MAX_LEN);
	}
	// 
	double dTime;
	// 
	// 
	int nEntityId;
	// 名称
	char chName[NAME_MAX_LEN];

	EnCamp camp;
	// 中心点 地理位置:LLH--LL:度,H:米
	// 经纬高-度
	double dLon;
	double dLat;
	double dAlt;	
	// 开始角度 方位--- 单位:°
	double dStartAngle;
	// 摇摆范围 单位:°
	double dSwingAngle;
	// 半径--米
	double dRadius;
	// 空域高度
	double dH;

	EnOperateType enType;
};

// 状态实时位置姿态
struct EntityStu
{
	EntityStu()
	{
		dTime = 0;
		// 实体ID
		nEntityID = 0;
		// 姿态
		dAzAngle = 0;
		dPicthAngle = 0;
		dRolAngle = 0;

		// 方位
		dLon = 0;
		dLat = 0;
		dAlt  = 0;	
		enCommuState = EnConeState::UNLINkE_CON;
	}

	~EntityStu()
	{
		dTime = 0;
		// 实体ID
		nEntityID = 0;
		// 姿态
		dAzAngle = 0;
		dPicthAngle = 0;
		dRolAngle = 0;

		// 方位
		dLon = 0;
		dLat = 0;
		dAlt  = 0;	

		enCommuState = EnConeState::UNLINkE_CON;
	
	}

	double dTime;

	// 实体ID
	int nEntityID;

	// 姿态
	double dAzAngle;

	double dPicthAngle;

	double dRolAngle;

	// 方位
	double dLon;

	double dLat;

	double dAlt;	


	// 通信状态 1-联通 2-断开
	EnConeState enCommuState;

};

// 销毁
struct DestoryCommand
{
	DestoryCommand()
	{
		dTime = 0;
		nEntityID = 0;
	}

	~DestoryCommand()
	{
		dTime = 0;
	}

	double dTime;
	int nEntityID;
};

// 击中命令
struct HitCommand
{
	HitCommand()
	{
		dTime = 0;
		nEntityID = 0;
		nTargetId = 0;
	}

	~HitCommand()
	{
		dTime = 0;
		nEntityID = 0;
		nTargetId = 0;
	}

	double dTime;
	int nEntityID;
	int nTargetId;
};

// 事件命令
struct EventCommand
{
	EventCommand()
	{
		dTime = 0;
		memset(chDescript,0,DESCRIPT_TXT_MAX_LEN);
	}

	~EventCommand()
	{
		dTime = 0;
		memset(chDescript,0,DESCRIPT_TXT_MAX_LEN);
	}

	double dTime;
	char chDescript[DESCRIPT_TXT_MAX_LEN];
};

// 字幕消息
struct ScreenWordInfo
{
	ScreenWordInfo()
	{
		dTime = 0;
		memset(chMsg,0,DESCRIPT_TXT_MAX_LEN);
	}

	~ScreenWordInfo()
	{
		dTime = 0;
		memset(chMsg,0,DESCRIPT_TXT_MAX_LEN);
	}

	double dTime;
	char chMsg[DESCRIPT_TXT_MAX_LEN];
};

// 视点信息 
// 定位
struct ViewLocatCmd
{
	ViewLocatCmd()
	{
		dTimeLen = 0;
		// 参数focal_point：焦点（指向点），使用map的SRS或参数指定的SRC
		dFocalLon = 0;
		dFocalLat = 0;
		dFocalAlt = 0;

		// 航向角（Heading，顺时针方向，单位度)，相机相对于焦点所在的切平面
		dHeading = 0;
		// 俯仰角（Pitch，单位度 ），相机相对于焦点所在的切平面
		dPitch = 0;
		//从相机到指向点（焦点）的直线距离
		dRange = 0;
	}

	~ViewLocatCmd()
	{
		dTimeLen = 0;
		// 参数focal_point：焦点（指向点），使用map的SRS或参数指定的SRC
		dFocalLon = 0;
		dFocalLat = 0;
		dFocalAlt = 0;

		// 航向角（Heading，顺时针方向，单位度)，相机相对于焦点所在的切平面
		dHeading = 0;
		// 俯仰角（Pitch，单位度 ），相机相对于焦点所在的切平面
		dPitch = 0;
		//从相机到指向点（焦点）的直线距离
		dRange = 0;
	}

	// 参数focal_point：焦点（指向点），使用map的SRS或参数指定的SRC
	double dFocalLon;
	double dFocalLat;
	double dFocalAlt;

	// 航向角（Heading，顺时针方向，单位度)，相机相对于焦点所在的切平面
	double dHeading;
	// 俯仰角（Pitch，单位度 ），相机相对于焦点所在的切平面
	double dPitch;
	//从相机到指向点（焦点）的直线距离
	double dRange;

	// 时长-- s 
	double dTimeLen;
};

// 凝视
struct PeerView
{
	PeerView()
	{
		// 视点位置
		dFocalLon = 0;
		dFocalLat = 0;
		dFocalAlt = 0;

		// 凝视实体 
		nEntityId = 0;
	}

	// 视点位置
	double dFocalLon;
	double dFocalLat;
	double dFocalAlt;
	
	// 凝视实体 
	int nEntityId;
};

// 谁看谁
struct PeerE2TView
{
	PeerE2TView()
	{
		// 
		nEntitId = 0;
		// 
		nTargetId = 0;
	}
	 // 
	int nEntitId;
	// 
	int nTargetId;
};

// 跟踪类型
enum EnTrackType
{
	// 只跟踪位置 
	EN_TRACK_POSTION = 1,

	// 跟踪位置姿态
	EN_TRACK_POSTURE_POSTION = 2,

	// 跟踪速度
	EN_TRACK_SPEED = 3,

};

// 视点跟踪
struct TracView
{
	TracView()
	{
		// 只跟踪位置 位置姿态 速度跟踪
		enTrackType =EN_TRACK_POSTION;
		nTargetId = 0;
		// 俯仰角
		dPitch = 0;
		// 偏航
		dAngle = 0;
		// 距离
		dDis = 0;
	}

	// 只跟踪位置 位置姿态 速度跟踪
	EnTrackType enTrackType;
	int nTargetId;
	// 俯仰角
	double dPitch;
	// 偏航
	double dAngle;
	// 距离
	double dDis;
};

// 漫游视点
struct TraveView
{
	TraveView()
	{
		// 视点位置 // 单位均为度
		dFocalLon = 0;
		dFocalLat = 0;
		dFocalAlt = 0;
		dHeading = 0;
		dPitch = 0;
		dRoll = 0;

		dFocalLon1 = 0;
		dFocalLat1 = 0;
		dFocalAlt1 = 0;
		dHeading1 = 0;
		dPitch1 = 0;
		dRoll1 = 0;

		// 时长
		dLenTime = 0;
	}
	// 视点位置 // 单位均为度
	double dFocalLon;
	double dFocalLat;
	double dFocalAlt;
	double dHeading;
	double dPitch;
	double dRoll;

	double dFocalLon1;
	double dFocalLat1;
	double dFocalAlt1;
	double dHeading1;
	double dPitch1;
	double dRoll1;

	// 时长
	double dLenTime;
};


// 漫游视点
struct TraveTargetView
{
	TraveTargetView()
	{
		// 
		// 视点位置
		dFocalLon = 0;
		dFocalLat = 0;
		dFocalAlt = 0;

		dFocalLon1 = 0;
		dFocalLat1 = 0;
		dFocalAlt1 = 0;

		nTargetId = 0;
		// 时长
		dLenTime = 0; 
	}
	// 
	// 视点位置
	double dFocalLon;
	double dFocalLat;
	double dFocalAlt;

	double dFocalLon1;
	double dFocalLat1;
	double dFocalAlt1;

	int nTargetId;
	// 时长
	double dLenTime;
};

// 视点类型
enum EnViewType
{
	// 视点定位
	EN_LOCAT_VIEW = 1,
	// 视点凝视
	EN_PEER_VIEW = 2,
	// 对看视点
	EN_PEERE2T_VIEW = 3,
	// 视点跟踪
	EN_TRACK_VIEW = 4,
	// 漫游视点
	EN_TRAVE_VIEW = 5,
	// 漫游目标
	EN_TRAVETARGET_VIEW = 6

};

struct ViewInfoMsg
{

	ViewInfoMsg()
	{
		dTime = 0;
		enViewType = EN_LOCAT_VIEW;
		memset(chMsg,0,MAX_VIEW_LEN);
	}

	double dTime;

	EnViewType enViewType;

	char chMsg[MAX_VIEW_LEN];
};

struct AimInfoMsg
{
	AimInfoMsg()
	{
		// 时戳
		dTime = 0;
		// 实体Id
		nEntityId = 0;
		// 目标Id
		nTargetId = 0;
		// 持续时长 根据加减速计算好
		dContinueTime = 0;
		//
		bAim = false;
	}

	// 时戳
	double dTime;

	// 实体Id
	int nEntityId;

	// 目标Id
	int nTargetId;

	// 开始/结束瞄准
	bool bAim;

	// 持续时长 根据加减速计算好
	double dContinueTime;
};

// 消息类型 
enum MsgType
{
	// 新建实体
	NewEntityMsg = 0,
	// 实体状态
	EntityStuMsg = 1,
	// 跟踪目标
	DetecTargetMsg = 2,
	// 销毁命令
	DestoryMsg = 3,
	// 显示端开始命令
	UiStartMsg = 4,
	// 事件消息
	EnventMsg = 5,
	// 视点信息
	ViewMsg = 6,
	// 时间信息
	TimeCommandMsg = 7,
	// 字幕消息
	ScreenWordMsg = 8,
	// 击中事件
	HitMsg = 9,
	// 传感器消息
	SenorMsg = 10,
	// 场景信息
	SimInfoMsg = 11,
	// 仿真初始化消息
	InitSimMsg = 12,
	// 瞄准命令
	AimMsg = 13,
	// 空域
	AirSpaceMsg = 14,
	// 实体分离消息
	SpeSubMsg = 15
};

// 想定信息
struct SimInfo
{
	SimInfo()
	{
		dTime = 0;

		// 参考时间
		dPreferTime = 0;

		// 总共时长
		dTimeLen = 0;

		// 场景名称
		memset(chBattleName,0,DESCRIPT_TXT_MAX_LEN);
	}

	~SimInfo()
	{
		dTime = 0;

		// 参考时间
		dPreferTime = 0;

		// 总共时长
		dTimeLen = 0;

		// 场景名称
		memset(chBattleName,0,DESCRIPT_TXT_MAX_LEN);
	}

	double dTime;

	// 参考时间
	double dPreferTime;

	// 总共时长
	double dTimeLen;

	// 场景名称
	char chBattleName[DESCRIPT_TXT_MAX_LEN];
};

// 消息头
struct MsgHead
{
	// 消息类型
	MsgType type;
	// 数据区长度
	unsigned dataLen;	
	// 
	double dTime;
};

struct ComMsg
{
	MsgHead msgHead;
	char data[MAX_MSG_LEN];
};

#pragma pack(pop)
#endif //FXSIM_STRUCT_DEF