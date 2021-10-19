#ifndef C_READ_BUFF_H
#define C_READ_BUFF_H
/**************************************************************************************************
* @file <文件名> CReadBuff
* @brief <描述>  服务器得到的数据存储处
* @author <作者> m00410
* @date <创建时间>  2021年10月13日
* @note [备注/注意事项]  单例模式
**************************************************************************************************/
#pragma once

#include <string>
#include <iostream>
#include <queue>

class CReadBuff
{
public:
	/**
	* @brief <简要说明>获取任务队列队象
	* @details [详细说明]获取任务队列队象
	* @param 参数1 [in/out/inout] 
	* @param 参数2 [in/out/inout] 
	* @return 返回值以及说明
	* @note [备注/注意事项]
	*/
	static CReadBuff& getInstanceReadBuff();

	/**
	* @brief <简要说明>储存任务
	* @details [详细说明]
	* @param 参数1 [in/out/inout] [in] 存储的数据
	* @param 参数2 [in/out/inout] 
	* @return 返回值以及说明
	* @note [备注/注意事项]
	*/
	void setReadBuff(const std::string& msg);

	/**
	* @brief <简要说明>获取需要处理的任务
	* @details [详细说明]
	* @param 参数1 [in/out/inout] 参数1说明
	* @param 参数2 [in/out/inout] 【out】 获取的数据
	* @return 返回值以及说明
	* @note [备注/注意事项]
	*/
	const std::string getFirstReadbuffer();

	/**
	* @brief <简要说明>判断任务队列是否为空
	* @details [详细说明]
	* @param 参数1 [in/out/inout] 
	* @param 参数2 [in/out/inout] 【out】 没有需要处理的任务
	* @return 返回值以及说明   NULL返回FALSE
	* @note [备注/注意事项]
	*/
	bool emity();

private:

	CReadBuff();

	~CReadBuff();

private:
	static CReadBuff*  m_Readbuff;				//任务队列队象

	std::queue<std::string> m_stdReadbuff;		//任务队列

	static int m_count;							//任务队列的数量

};

#endif
