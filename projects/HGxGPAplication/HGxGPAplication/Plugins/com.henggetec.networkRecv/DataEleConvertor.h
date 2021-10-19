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
// ͨѶ����ת��Ϊʵ������
class CDataEleConvertor : public osg::Referenced
{
public:
	CDataEleConvertor();

	/**
	* @brief �������ӿ�
	* @return ���������
	*/
	static CDataEleConvertor* getInstance();

	// ��ȡʵ��ID
	std::string getEntityID(int id);
	// ��ȡ������ID
	std::string getSensorID(int sensorOwnerID, int sensorID);
	// ��ȡ����Id
	std::string getAirSpaceID(int enId);
	// ��ȡ�ϼ�ͨѶ��ϵ��ЧID
	std::string getCommAttriID(int nCommEffectOwnerEntityID);
	// ��ȡ����������ЧID
	std::string getDetectAttriID(int nEntityID, int nTargetID);
	// ��ȡ������ЧID
	std::string getHitEffectID(int nTargetId);
	// ͨ��״̬ 1-��ͨ 2-�Ͽ�
	bool isCommEffectVisible(unsigned short usCommuState);
	//������ɫ  1-��ɫ 2-��ɫ 3-��ɫ
	osg::Vec4 getPathColor(int camp);
	//ģ����ɫ 1-��ɫ 2-��ɫ 3-��ɫ
	osg::Vec4 getModelColor(int camp);

	SNewObjectData getEntityAttri(const NewEntity &newEntityInfo);
	// ��ȡ��׼��ЧID
	std::string getAimEffectID(int nEntityID, int nTargetID);
protected:
	//CMsgDataParserThread*		  m_dataParserThread;
};

#endif //SIM_CONTROL_H
