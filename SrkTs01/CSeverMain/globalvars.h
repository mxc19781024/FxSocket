#ifndef GLOBALVARS_H
#define GLOBALVARS_H
#include <iostream>
#pragma once

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
	E_LLH_COOR,   //经纬度坐标
	E_WORLD_COOR   //世界坐标
};

/*
*@enum 三维节点操作类型
*/
enum ENodeOptionType
{
	E_SHOW = 0,    //显示
	E_HIDE = 1,    //隐藏
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