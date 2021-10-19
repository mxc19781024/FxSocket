#ifndef C_WRITE_BUFFER_H
#define C_WRITE_BUFFER_H
/**************************************************************************************************
* @file <文件名>  C_WRITE_BUFFER_H
* @brief <描述>   发送数据队列
* @author <作者>  m00410
* @date <创建时间> 2021年10月13日
* @note [备注/注意事项] 
**************************************************************************************************/
#include <string>
#include <iostream>
#include <queue>
#include "../CSeverMain/globalvars.h"
#pragma once

class CWriteBudder
{
private:
	std::queue<std::string> m_stdWritebuff;				//发送任务队列

	CWriteBudder();

	~CWriteBudder();

	static CWriteBudder* m_WriteBuff;					//发送任务队列队象操作器

	static int m_count;									//任务数								//
public:
	/**
	* @brief <简要说明>  获取写时任务队列队象
	* @details [详细说明] 发送任务队列队象操作器
	* @param 参数1 [in/out/inout] 参数1说明
	* @param 参数2 [in/out/inout] 参数2说明
	* @return 返回值以及说明  发送任务队列队象操作器
	* @note [备注/注意事项]
	*/
	static CWriteBudder& getWriteBuff();
	/**
	* @brief <简要说明>  判断是否为空
	* @details [详细说明]
	* @param 参数1 [in/out/inout] 参数1说明
	* @param 参数2 [in/out/inout] 参数2说明
	* @return 返回值以及说明  NULL 返回 TRUE  NOTNULL 为 FALSE 
	* @note [备注/注意事项]
	*/
	static bool  isemoty();
	/**
	* @brief <简要说明> 添加发送任务
	* @details [详细说明]
	* @param 参数1 [in/out/inout]  【in】  准备发送的数据
	* @param 参数2 [in/out/inout] 参数2说明
	* @return 返回值以及说明
	* @note [备注/注意事项]
	*/
	void setWriteBuff(const std::string& msg);
	/**
	* @brief <简要说明>  添加发送任务
	* @details [详细说明]
	* @param 参数1 [in/out/inout] 【in】  准备发送的数据
	* @param 参数2 [in/out/inout] 参数2说明
	* @return 返回值以及说明
	* @note [备注/注意事项]
	*/
	void setWriteBuff(const SNodeInfo3D & msg);
	/**
	* @brief <简要说明>  获取发送任务
	* @details [详细说明]
	* @param 参数1 [in/out/inout] 参数1说明
	* @param 参数2 [in/out/inout] 参数2说明
	* @return 返回值以及说明 返回发送的消息
	* @note [备注/注意事项]
	*/
	const std::string getWriteBuffer();
	/**
	* @brief <简要说明>  数据的数量
	* @details [详细说明]
	* @param 参数1 [in/out/inout] 参数1说明
	* @param 参数2 [in/out/inout] 参数2说明
	* @return 返回值以及说明
	* @note [备注/注意事项]
	*/
	static int& getCount()
	{
		return m_count;
	}
};
#endif
