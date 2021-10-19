#ifndef DATA_CONVERTOR_H
#define DATA_CONVERTOR_H

#include <QThread>
#include <QByteArray>
#include <QMutex>
#include <QObject>
#include "Apps\FreeXSim\StructDef.h"
#include <iostream>
#include "osg\Vec4"
#include "osg\Referenced"
#include "common\UnitCommon.h"
#include "MsgDataParserThread.h"

//////////////////////////////////////////////////////////////////////////
// 通讯数据转换为实体数据
class CDataEleConvertor : public osg::Referenced
{
public:
	CDataEleConvertor();

	/**
	* @brief 对象管理接口
	* @return 对象管理器
	*/
	static CDataEleConvertor* getInstance();

	// 获取实体ID
	std::string getEntityID(int id);
	// 获取传感器ID
	std::string getSensorID(int sensorOwnerID, int sensorID);
	// 获取空域Id
	std::string getAirSpaceID(int enId);
	// 获取上级通讯关系特效ID
	std::string getCommAttriID(int nCommEffectOwnerEntityID);
	// 获取跟踪锁定特效ID
	std::string getDetectAttriID(int nEntityID, int nTargetID);
	// 获取击中特效ID
	std::string getHitEffectID(int nTargetId);
	// 通信状态 1-联通 2-断开
	bool isCommEffectVisible(unsigned short usCommuState);
	//航迹颜色  1-红色 2-蓝色 3-白色
	osg::Vec4 getPathColor(int camp);
	//模型颜色 1-红色 2-蓝色 3-白色
	osg::Vec4 getModelColor(int camp);

	SNewObjectData getEntityAttri(const NewEntity &newEntityInfo);
	// 获取瞄准特效ID
	std::string getAimEffectID(int nEntityID, int nTargetID);
protected:
	//CMsgDataParserThread*		  m_dataParserThread;
};

#endif //SIM_CONTROL_H
