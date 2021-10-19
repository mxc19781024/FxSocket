/**************************************************************************************************
* @file UnitStructDef.h
* @note 实体驱动常量、结构体定义
* @author 
* @date 2021-7-10
**************************************************************************************************/
#ifndef FX_UNIT_STRUCT_DEF_H
#define FX_UNIT_STRUCT_DEF_H

#include <map>
#include <osg/Vec4>
#include <osg/Vec3d>
#include <osg/Vec2>
#include "SenorStructDef.h"

// 消息最大长度
#define MAX_MSG_LEN 1000
// 消息类型 
enum MsgType
{
	NewEntityMsg = 0,  // 新建实体
	EntityStuMsg = 1,  // 实体状态
	DetecTargetMsg = 2,// 跟踪目标
	DestoryMsg = 3,    // 销毁命令
	UiStartMsg = 4,    // 显示端开始命令
	EnventMsg = 5,     // 事件消息	
	ViewMsg = 6,       // 视点信息
	TimeCommandMsg = 7,// 时间信息
	ScreenWordMsg = 8, // 字幕消息	
	HitMsg = 9,        // 击中事件	
	SenorMsg = 10,     // 传感器消息	
	SimInfoMsg = 11,   // 场景信息	
	InitSimMsg = 12,   // 仿真初始化消息
	AimMsg = 13,       // 瞄准命令
	AirSpaceMsg = 14,  // 空域
	SpeSubMsg = 15     // 实体分离消息
};


// 消息头
struct MsgHead
{
	MsgType  type;    // 消息类型
	unsigned dataLen;// 数据区长度
	double   dTime;
};

struct ComMsg
{
	MsgHead msgHead;
	char    data[MAX_MSG_LEN];
};


//文字附加属性
struct STextAttach
{
	STextAttach()
	{
		fFontSize = 16;
		//memset(strID,'\0, sizeof(strID));
		//memset(strName,'\0', sizeof(strName));
		bOutLine = 1;
		bAutoScale = 1;
		bAutoRotate = 1;
		memset(vecColor, 4, sizeof(vecColor));
		memset(strFont, 1024, sizeof(strFont));
		memset(vecOutLineColor, 4, sizeof(vecOutLineColor));
		memset(vecOffset, 3, sizeof(vecOffset));
	}
	double                  vecColor[4];       //文字颜色
	char                    strFont[1024];     //文字字体
	float                   fFontSize;         //文字显示大小
	bool                    bOutLine;          //是否显示边框线
	double                  vecOutLineColor[4];//文字边框线颜色
	double                  vecOffset[3];      //文字相对中心点的偏移量
	bool                    bAutoScale;        //文字是否自动缩放
	bool                    bAutoRotate;       //文字是否自动旋转
};

//图片附加属性
struct SImageAttach
{
	double                  vecImageSize[2];   //图片大小（宽、高）
	float                   fOpacity;          //图片透明度
	bool                    bAutoScale;        //图片是否自动缩放
	bool                    bAutoRotate;	   //图片是否自动旋转
};

//模型附加属性
struct SModelAttach
{
	bool                    bAutoScale;        //模型是否自动缩放
	bool                    bAutoRotate;       //模型是否自动旋转
	bool                    bUseColor;         //模型是否使用颜色
	double                  vecColor[4];       //模型颜色
	bool					bBlend;            //模型颜色混合
};

//点附加属性
struct SPointAttach
{
	float        fPointSize;   //点的大小
	double       vecColor[4];  //点的颜色
};

//尾迹附加属性:线型
enum ELineType
{
	E_FULLLINE = 0,
	E_IMAGINRYLINE = 1,
	E_POINTS = 2,
	E_POINTLINE = 3,
};

//尾迹附加属性
struct SLineAttach
{
	float        fPointSize;   //尾迹点模式下，点的大小
	double       vecColor[4];  //尾迹颜色
	ELineType    eLineType;    //尾迹线的线型（点、线、点线）
	float        fLineWidth;   //尾迹线的线宽
	bool         bLoop;        //尾迹线是否闭环
};

// 所属阵营（附加属性）
enum EnCamp
{
	RED_CAMP = 1,     // 红方
	BLUE_CAMP = 2,    // 蓝方
	UNKNOWN_CAMP = 3  // 未知 不明
};

// 所属空域（附加属性）
enum EnAirspace
{
	SEA_SPACE = 1,   // 海
	GROUND_SPACE = 2,// 陆
	AIR_SPACE = 3    // 空
};

// 装备类型（附加属性）
enum EnEquType
{
	AIRCAFT_EUT = 1,         //飞机
	TANK_EUT = 2,            //坦克
	MISSILE_VECHICLE_EUT = 3,//导弹发射车
	MISSILE_EUT = 4,         //导弹
	CARRIER_EUT = 5,         //航母
	FRIGATE_EUT = 6,         //护卫舰
	COPTER_EUT = 7,          //直升机
	GROUND_EUT = 8           //地面站
};

//传感器集合
struct SSenor
{
	RadarSenor       sRadarSenor; //雷达信息
};

//实体基本属性
struct SPreUnitBsaeInfo
{
	char		            strID[64];               //实体ID,该属性具有唯一性
	char	                parentID[64];            //上级ID 通信指挥关系
	char		            strNameText[128];        //实体名称
	char		            strImageFilePath[1024];  //实体图片路径
	char		            strModelFilePath[1024];  //实体模型路径
	double                  vecModelScale[3];        //实体模型缩放
	double                  vecModelPosition[3];     //实体模型位置（经纬高）
	double                  vecModelPosture[3];      //实体模型姿态（俯仰，滚转，偏航，单位：度）
	bool                    bUnitVisible;            //实体显隐
	bool                    bNameVisible;            //实体名称显隐
	bool                    bImageVisible;           //实体图片显隐
	bool                    bModelVisible;           //实体模型显隐
	bool                    bPointVisible;           //实体点显隐
	bool                    bLineVisible;            //实体尾迹显隐
	bool                    bLabelVisible;           //实体标牌显隐
	bool                    bDynamic;                //
};

//实体附加属性
struct SPreUnitAttachInfo
{
	EnCamp           eCamp;       //所属阵营（附加属性）
	SSenor           sSSenor;     //传感器信息（附加属性）
	EnAirspace       eEnAirspace; //所属空域（附加属性）
	EnEquType        eEnEquType;  //装备类型（附加属性）
	STextAttach      sTextAttach; //文字附加属性（附加属性）
	SImageAttach     sImageAttach;//图片附加属性（附加属性）
	SModelAttach     sModelAttach;//模型附加属性（附加属性）
	SPointAttach     sPointAttach;//点附加属性（附加属性）
	SLineAttach      sLineAttach; //线附加属性（附加属性）
}

//新建实体对象数据信息
struct SNewUnitData
{
	SPreUnitBsaeInfo        sPreUnitBsaeInfo;  //基础信息，包含基本信息后可直接创建实体
	SPreUnitAttachInfo      sPreUnitAttachInfo;//实体特性附加信息，用于修改实体属性
};

// 状态实时位置姿态
struct SDriveUnitData
{
	SDriveUnitData()
	{
		dTime = 0.0;
		nDriveStyle = 0;
		memset(modelColor, 1, sizeof(modelColor));
		memset(vecPositionLLH, 0, sizeof(vecPositionLLH));
		memset(vecPosture, 0, sizeof(vecPosture));
	}
	double		 dTime;
	char	     strID[64];            //实体唯一Id

	EnCamp enCamp;// 阵营 1-红方 2-蓝方 3-不明	 
	EnAirspace enSpaceType;// 实体所属空域 1-海上装备 2-陆地装备 3-空中装备	
	EnEquType enEquType;// 1-飞机 2-坦克 3-导弹发射车 4-导弹 5-航母 6-护卫舰	
	SSenor    sSenor;// 传感器
	EnConeState enCommuState;// 通信状态 1-联通 2-断开				 
	SepareSubInfo sepInfo[MAX_SEPSUB_NUM];// 分离实体id

	double	     modelColor[4];		   //实体染色颜色
	double       vecPositionLLH[3];    //实体当前位置（ 经纬高）
	double       vecPosture[3];        //实体当前姿态,偏航、俯仰、横滚
	int          nDriveStyle;          //实体驱动方式

};

// 击中命令
struct HitData
{
	HitData()
	{
		dTime = 0;
	}

	~HitData()
	{
		dTime = 0;
	}

	double		 dTime;
	char	     strID[64];                //实体唯一Id
	char	     nTargetId[64];            //被击中实体唯一Id
};

// 事件命令
struct EventData
{
	EventData()
	{
		dTime = 0;
		memset(chDescript, 0, 200);
	}

	~EventData()
	{
		dTime = 0;
		memset(chDescript, 0, 200);
	}

	double dTime;          //事件时间
	char   chDescript[200];//事件信息
};

#include <gdpAlgor/GxAstronomyCommon.h>

//卫星轨道模式
enum ESpacecraftOrbitMode
{
	SatStaticOrbit = 0,   //静态轨道
	SatDynamicOrbit = 1,  //动态轨道
};

//卫星轨道数据：轨道模式及轨道根数
struct SSpacecraftObjectEleData
{
	ESpacecraftOrbitMode     eOrbitMode;
	HGxAlgor::SSgp4Data      stSgp4Data;
};
//单个实体对象驱动数据(航天器：卫星)
struct SNewSpacecraftObjectEleData
{
	SSpacecraftObjectEleData sSpacecraftObjectEleData;  //卫星轨道数据信息
	SNewUnitData             sNewUnitData;              //实体对象卫星属性信息，在非星历模式下无需设置模型位置信息
};

typedef std::vector<std::string> VectorDestroyUnitData; //销毁实体的ID集


#endif // FX_UNIT_STRUCT_DEF_H
