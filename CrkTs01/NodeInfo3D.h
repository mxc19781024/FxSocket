#ifndef NODE_INFO_3D_H
#define NODE_INFO_3D_H

#include <iostream>
#define LOCAL_ADDRESS "234.2.2.2"
#define LOCAL_POST 4412
#define REMOTE_ADDRESS "234.2.2.4"
#define REMOTE_POST 4414

enum ORDER {
	CMD_UserLogin_L = 'L',          //登陆
	CMD_getAircaftInformtion = 'A', //飞机信息
	CMD_DataCount = 'D',
};

/*
*@enum 坐标系类型
*/
enum EPosType
{
	E_LLH_COOR ,   //经纬度坐标
	E_WORLD_COOR   //世界坐标
};

/*
*@enum 三维节点操作类型
*/
enum ENodeOptionType
{
	E_SHOW,    //显示
	E_HIDE,    //隐藏
};

/*
*@struct SPosition
*@brief 坐标值
*@details 包含坐标系类型以及对应的坐标值
*@author g00427
*/
struct SPosition
{
	EPosType type;  //坐标类型
	double x;
	double y;
	double z;
};

/*
*@struct SNodeInfo3D
*@brief 三维节点信息
*@details 节点名字、坐标、操作类型等
*@author g00427
*/
struct SNodeInfo3D
{
	std::string name;            //名称
	SPosition postion;           //坐标
	ENodeOptionType optionType;  //操作
	double time;                 //时间
};

#endif
