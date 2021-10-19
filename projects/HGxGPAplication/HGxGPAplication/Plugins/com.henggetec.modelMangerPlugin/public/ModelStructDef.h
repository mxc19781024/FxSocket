/**************************************************************************************************
* @file ModelStructDef.h
* @note 实体属性常量、结构体定义
* @author w00085
* @date 2021-6-26
**************************************************************************************************/
#ifndef FX_MODEL_STRUCT_DEF_H
#define FX_MODEL_STRUCT_DEF_H

#include <QMap>

//传感器种类
enum ESenor
{

};

//实体装备搭载传感器集合
typedef std::vector<ESenor> VectorSenor;

//单个装备作战能力（附加属性）
struct SPreEquipCombatCap
{
	double      dHit;        //装备打击能力
	double      dCruise;     //装备巡航能力
	double      dPerformance;//装备性能
	double      dEndurance;  //装备续航能力
	double      dCoordinate; //装备协调能力
	double      dProbe;      //装备探测能力
};

//单个装备能力（附加属性）
struct SPreEquipCap
{
	double      maxVoyage;       //装备最大航程
	double      maxSpeed;        //装备最大速度
	double      maxOil;          //装备最大油量
	int         ammunitionNumber;//装备携带弹药个数
	std::string combatCapability;//作战能力
	std::string degreeDamage;    //装备毁伤程度
};

//单个装备附加属性信息
struct SPreEquipAttachInfo
{
	SPreEquipCap       sEquipCap;     //基本能力
	SPreEquipCombatCap sCombatCap;    //作战能力
	VectorSenor        vecESenor;     //传感器集合
};

//单个装备基础属性信息
struct SPreEquipBsaeInfo
{
	std::string name;            //装备名称
	std::string strtype;         //装备类型
	std::string description;     //装备描述
	std::string model_path;      //装备三维模型路径
	std::string image_path;      //装备二维图片路径
};

//单个装备属性信息
struct SModelAttribute
{
	SPreEquipBsaeInfo sSPreEquipBsaeInfo;      //装备基础属性信息
	SPreEquipAttachInfo sSPreEquipAttachInfo;  //装备附加属性信息
};


//实体对象驱动数据集合
typedef QMap<std::string, SModelAttribute> MapModelAttributeData;  //key:模型名称，value：装备属性信息

#endif // FX_MODEL_STRUCT_DEF_H
