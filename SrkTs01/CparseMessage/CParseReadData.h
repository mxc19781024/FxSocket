#ifndef C_PARSE_READ_DATA_H
#define C_PARSE_READ_DATA_H

/**************************************************************************************************
* @file <CParseMessage>
* @brief <数据解析>
* @author <m00410>
* @date <2021年10月13日>
* @note [目前只有.xml数据的发送]
**************************************************************************************************/

#pragma once
#include "../CReadbuff/CReadBuff.h"
#include "../CSeverMain/globalvars.h"
#include <string>
#include <iostream>
#include <windows.h>
#include <tinyxml.h>


/**
* @class <CParseReadData>
* @brief <数据解析>
* @details [只有添加解析命令时，才会使用本文件添加解析命令]
* @author <m00410>
* @note [目前只有.xml数据的发送]
*/
class CParseReadData
{
public:
	/**
	* @brief <等待读缓冲区接收到命令，然后启动解析>
	* @details [解析读缓冲中的数据]
	* @param 参数1 [in/out/inout] NULL
	* @param 参数2 [in/out/inout] NULL
	* @return 成功返回TRUE
	* @note [在后期可以使用多线程进行优化]
	*/
	bool parseData();

	/**
	* @brief <获取解析命令的处理类>
	* @details [采用单例模式，使全局可以使用]
	* @param 参数1 [in/out/inout] NULL
	* @param 参数2 [in/out/inout] NULL
	* @return 返回解析的类
	* @note [备注/注意事项]
	*/
	static CParseReadData& getInstance();
protected:
	/**
	* @brief <将读取到的.xml文件进行解析>
	* @details [对应属性值进行读取]
	* @param 参数1 [in] 元素节点
	* @param 参数2 [in/out/inout] 
	* @return 返回值以及说明 
	* @note [对数据结构体进行修改时，需要对解析文件进行修改]
	*/
	void xmlParse(TiXmlElement *pEle);
	/**
	* @brief <读取.xml文件，将其转换为文档>
	* @details [采用C++源码实现]
	* @param 参数1 [in/out/inout] in   路径
	* @param 参数2 [in/out/inout] NULL
	* @return  返回值以及说明
	* @note [可移植，需要同时移植C++.xml源码]
	*/
	int readXmlFile(std::string filepPath);
	/**
	* @brief <解析命令>
	* @details [将收到的数据进行拆分，对应对应的任务进行处理]
	* @param 参数1 [in/out/inout] 【in】 完整的一条数据
	* @param 参数2 [in/out/inout] 参数2说明
	* @return 返回值以及说明
	* @note [在后期需要服务器发送时，需要添加对应的解析命令]
	*/
	void parseCommand(std::string & msg);
	/**
	* @brief <数据条数>
	* @details [在刚开始得到.xml数据时，需要先发送数据的大小]
	* @param 参数1 [in/out/inout] 参数1说明
	* @param 参数2 [in/out/inout] 参数2说明
	* @return 返回值以及说明
	* @note [备注/注意事项]
	*/
	void parseDataLogin();
	/**
	* @brief <将数据一条一条的发送>
	* @details [结合 parseDataLogin（） 将数据一条一条发送]
	* @param 参数1 [in/out/inout] 参数1说明
	* @param 参数2 [in/out/inout] 参数2说明
	* @return 返回值以及说明
	* @note [备注/注意事项]
	*/
	void parseDataAircaInfomtion();
	/**
	* @brief <解析无效的命令>
	* @details [详细说明]
	* @param 参数1 [in/out/inout] 参数1说明
	* @param 参数2 [in/out/inout] 参数2说明
	* @return 返回值以及说明
	* @note [备注/注意事项]
	*/
	void parseDataInvalidCommand();
private:
	CParseReadData();

	~CParseReadData();

	std::vector<SNodeInfo3D> m_vNodeGather;			//储存.xml数据的容器
};

#endif


