/**************************************************************************************************
* @file UnitStructDef.h
* @note 实体驱动常量、结构体定义
* @author 
* @date 2019-8-26
**************************************************************************************************/
#ifndef FX_UNIT_STRUCT_DEF_H
#define FX_UNIT_STRUCT_DEF_H

#include <map>
#include <osg/Vec4>
#include <osg/Vec3d>
#include <osg/Vec2>
#include "StructDef.h"

//单元角色及队形指令
struct RoleCommand
{
	short C1;//大类指令。1:定位；2：对抗；3：雷达协同；7:队形；10：开关指令；相关机理确定，用于任务系统和交感网控制  当值50时为网络相关指令，才处理。
	short C2;//子类指令。0：关，1：建网，2:入网，4:退网，5：子网功能
	std::string recplatfmID;//接收平台
	std::string WorkState;//
	double SimTimeSec;	//下达指令的系统仿真时间，秒
	std::string strNumbCo;//子网内飞机数
};
//队形指令集合
typedef std::vector<RoleCommand> RoleCommandVec;

	//载机雷达状态记录
	struct ZaijiRadarState
	{
		double SimuTime;	//仿真时间，秒
		short ValidRadiatFlag;//有辐射标志，0无，1有
		short Shapetype;	//数据类型，1：双圆锥结构，2：双圆垂直割结构
		std::string IDbelong;//雷达所在飞机的飞机标识号
		float yawCurline;	//雷达波束瞬时指向，方位角（以机体纵轴为中心），弧度，顺时针正，值域（-pi，+pi]，
		float picthCurline;	//雷达波束瞬时指向，水平俯仰角，弧度，（-pi/2，+pi/2]
		float yawCnline;	//雷达中心线极坐标，方位角，值域（-pi，+pi]，以雷达中心线的极坐标
		float picthCnline;	//雷达中心线极坐标，仰角，（-pi/2，+pi/2]
		float BeamNearDis;//临近界，米
		float dectin;	//近界，米
		float BeamMidDis;//中界，米
		float dectout;	//远界，米。威力在基准RCS=1平米时的值，距离-米。
		float alpha;	//双圆锥结构时，半顶锥角，[0，pi)。
		//双圆垂直平面割结构时，为水平角，[0，pi/2）
		float beta;		//双圆锥结构时，无效。双圆垂直平面割结构时，为垂直角,[0,pi/2)。	
	};
	//载机雷达状态集合
	typedef std::vector<ZaijiRadarState> vecZaijiRadarState;

//装备毁伤程度
enum EDegreeDamage
{

};

//单个装备能力
struct SPreEquipCap
{
	double      dHit;       //装备打击能力
	double      dCruise;    //装备巡航能力
	double      dPerformance;//装备性能
	double      dEndurance;  //装备续航能力
	double      dCoordinate; //装备协调能力
	double      dProbe;      //装备探测能力
};

//单个装备属性信息
struct SPreEquipInfo
{
	std::string name;            //装备名称
	std::string strtype;         //装备类型
	std::string description;     //装备描述
	std::string model_path;      //装备三维模型路径
	std::string image_path;      //装备二维图片路径
	double      maxVoyage;       //装备最大航程
	double      maxSpeed;        //装备最大速度
	double      maxOil;          //装备最大油量
	int         ammunitionNumber;//装备携带弹药个数
	std::string combatCapability;//作战能力
	std::string degreeDamage;    //装备毁伤程度
	SPreEquipCap sPreEquipCap;   //作战能力
};
//装备单元数据集合
typedef std::map<std::string,SPreEquipInfo> MapEquipInfoData;

//传感器集合
struct SSenor
{
	// 雷达
	Senor senor[SENOR_MAX_NUM];
};

//单个实体对象驱动数据
struct SNewObjectData
{
	SNewObjectData()
	{
		dTime = 0.0;
		//memset(strID,'\0, sizeof(strID));
		//memset(strName,'\0', sizeof(strName));
		nameVisible = 1;
		nameFontSize = 20.0;
		memset(nameColor, 1, sizeof(nameColor));
		//memset(strModelFilePath,'\0', sizeof(strModelFilePath));
		nType = 0;
		modelAuto = 1;
		nDriveStyle = 0;
		memset(modelColor, 1, sizeof(modelColor));
		//memset(strImageFilePath,'\0', sizeof(strImageFilePath));	
		memset(imageImageSize, 64, sizeof(imageImageSize));
		memset(modelScale, 10, sizeof(modelScale));
		memset(vecPositionLLH, 0, sizeof(vecPositionLLH));
		memset(vecPosture, 0, sizeof(vecPosture));
	}
	double		 dTime;
	char	     strID[64];            //实体唯一Id
	char	     parentID[64];         //上级ID 通信指挥关系
	char	     strName[128];         //实体名字
	int          nameVisible;		   //实体名字显影，0：不显示，1：显示
	int          nameFontSize;		   //实体名字字号
	double	     nameColor[4];		   //实体名字颜色

	EnCamp enCamp;// 阵营 1-红方 2-蓝方 3-不明	 
	EnAirspace enSpaceType;// 实体所属空域 1-海上装备 2-陆地装备 3-空中装备	
	EnEquType enEquType;// 1-飞机 2-坦克 3-导弹发射车 4-导弹 5-航母 6-护卫舰	
	SSenor    sSenor;// 传感器
	EnConeState enCommuState;// 通信状态 1-联通 2-断开				 
	SepareSubInfo sepInfo[MAX_SEPSUB_NUM];// 分离实体id
	SPreEquipInfo sPreEquipInfo;      //实体配置信息


	char   	     strModelFilePath[512];//实体模型路径
	int          nType;                //实体类型，0：白方，1：红方，2：蓝方
	int          modelAuto;			   //实体是否自动缩放，0：静止缩放，1：启动缩放
	double	     modelColor[4];		   //实体染色颜色
	char   	     strImageFilePath[512];//实体模型图片路径
	double	     imageImageSize[2];	   //实体模型图片大小

	double       modelScale[3];		   //实体模型缩放大小
	double       vecPositionLLH[3];    //实体当前位置（ 经纬高）
	double       vecPosture[3];        //实体当前姿态,偏航、俯仰、横滚

	int          nDriveStyle;          //实体驱动方式

};

// 状态实时位置姿态
struct SDriveObjectData
{
	SDriveObjectData()
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

//单个实体对象驱动数据
struct SDriverAircraftObjectData
{
	SDriverAircraftObjectData()
	{
		dTime = 0.0; 
		//memset(strID,'\0, sizeof(strID));
		//memset(strName,'\0', sizeof(strName));
		nameVisible = 1;		  
		nameFontSize = 20.0;		  
		memset(nameColor,1, sizeof(nameColor));
		//memset(strModelFilePath,'\0', sizeof(strModelFilePath));
		nType = 0;               
		modelAuto = 1;		
		nDriveStyle = 0;
		memset(modelColor,1, sizeof(modelColor));
		//memset(strImageFilePath,'\0', sizeof(strImageFilePath));	
		memset(imageImageSize,64, sizeof(imageImageSize));
		memset(modelScale,10, sizeof(modelScale));
		memset(vecPositionLLH,0, sizeof(vecPositionLLH));
		memset(vecPosture,0, sizeof(vecPosture));
	}
	double		 dTime; 
	char	     strID[64];            //实体唯一Id
	char	     parentID[64];         //上级ID 通信指挥关系
	char	     strName[128];         //实体名字
	int          nameVisible;		   //实体名字显影，0：不显示，1：显示
	int          nameFontSize;		   //实体名字字号
	double	     nameColor[4];		   //实体名字颜色
									   
	EnCamp enCamp;// 阵营 1-红方 2-蓝方 3-不明	 
	EnAirspace enSpaceType;// 实体所属空域 1-海上装备 2-陆地装备 3-空中装备	
	EnEquType enEquType;// 1-飞机 2-坦克 3-导弹发射车 4-导弹 5-航母 6-护卫舰	
	SSenor    sSenor;// 传感器
	EnConeState enCommuState;// 通信状态 1-联通 2-断开				 
	SepareSubInfo sepInfo[MAX_SEPSUB_NUM];// 分离实体id
	SPreEquipInfo sPreEquipInfo;      //实体配置信息


	char   	     strModelFilePath[512];//实体模型路径
	int          nType;                //实体类型，0：白方，1：红方，2：蓝方
	int          modelAuto;			   //实体是否自动缩放，0：静止缩放，1：启动缩放
	double	     modelColor[4];		   //实体染色颜色
	char   	     strImageFilePath[512];//实体模型图片路径
	double	     imageImageSize[2];	   //实体模型图片大小

	double       modelScale[3];		   //实体模型缩放大小
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
	char	     strID[64];            //实体唯一Id
	char	     nTargetId[64];            //实体唯一Id
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

	double dTime;
	char chDescript[200];
};

// enum ECamp
// {
// 	InitObject = 0,
// 	RedObject = 1,
// 	BlueObject = 2,
// };


struct SObjectEleData
{
	bool                    bVisible;
	osg::Vec3d		        vecPositionLLH;
	osg::Vec3d		        vecPosture;
};

struct STextObjectEleData
{
	std::string		        strText;      //实体名称
	osg::Vec4               vecColor;
	std::string             strFont;
	float                   fFontSize;
	bool                    bOutLine;
	osg::Vec4               vecOutLineColor;
	osg::Vec3d              vecOffset;
	bool                    bAutoScale;
	bool                    bAutoRotate;
};

struct SImageObjectEleData
{
	std::string		        strImageFilePath;   //实体图片
	osg::Vec2               vecImageSize;
	float                   fOpacity;
	bool                    bAutoScale;
	bool                    bAutoRotate;
};

/**
* @brief 模型类型
*/
enum EModelType
{
	GENARAL = 0,
	ANIMATIONPATH = 1,
	COMBINED = 2,
};
struct SModelObjectEleData
{
	std::string		        strFilePath;   //实体模型
	osg::Vec3d              vecScale;//模型缩放
	osg::Vec3d              vecPosition;//模型缩放
	osg::Vec3d              vecPosture;//模型缩放
	bool                    bAutoScale;
	bool                    bAutoRotate;
	bool                    bUseColor;
	osg::Vec4               vecColor;
	EModelType				eModelType;
	bool					bBlend; //颜色混合
};

struct SPointObjectEleData
{
	float        fPointSize;
	osg::Vec4    vecColor;
};

/**
* @brief 线型
*/
enum ELineType
{
	E_FULLLINE = 0,
	E_IMAGINRYLINE = 1,
	E_POINTS = 2,
	E_POINTLINE = 3,
};

struct SLineObjectEleData
{
	bool         bVisible;
	float        fPointSize;
	osg::Vec4    vecColor;
	ELineType    eLineType;
	float        fLineWidth;
	bool         bLoop;

};

#include <gdpAlgor/GxAstronomyCommon.h>

enum ESpacecraftOrbitMode
{
	SatStaticOrbit = 0,
	SatDynamicOrbit = 1,
};
struct SSpacecraftObjectEleData
{
	ESpacecraftOrbitMode     eOrbitMode;
	HGxAlgor::SSgp4Data      stSgp4Data;
};

struct SEntityObjectEleData
{
	std::string		        strID;	    //实体ID
	std::string		        strParentID;//父节点ID
	std::string             strEntityType;    //实体类型
	std::string             strEntityClass;    //实体类型
	bool                    bDynamic;
	EnCamp                  eCamp;      //阵营
	SObjectEleData          sObjectEleData;
	STextObjectEleData      sTextObjectEleData;
	SImageObjectEleData     sImageObjectEleData;
	SModelObjectEleData     sModelObjectEleData;
	SPointObjectEleData     sPointObjectEleData;
	SLineObjectEleData      sLineObjectEleData;
};

//单个实体对象驱动数据
struct SNewSpacecraftObjectEleData
{
	SSpacecraftObjectEleData sSpacecraftObjectEleData;
	SEntityObjectEleData sEntityObjectEleData;
};

typedef std::vector<std::string> VectorDestroyUnitData;

//实体对象驱动数据集合
typedef std::vector<SDriverAircraftObjectData> VectorDriverAircraftObjectsData;
//实体对象驱动数据集合
typedef std::vector<SNewSpacecraftObjectEleData> VectorSpacecraftObjectsData;
//实体对象驱动数据集合
typedef std::vector<SDriverAircraftObjectData> VectorDriverAircraftObjectsData;
//航空器实体对象驱动数据集合
typedef std::vector<SDriverAircraftObjectData> VectorDriverSpacecraftObjectsData;


#endif // FX_EFFECT_COMMON_H
