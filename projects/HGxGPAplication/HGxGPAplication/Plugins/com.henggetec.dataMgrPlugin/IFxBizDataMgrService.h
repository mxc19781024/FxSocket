/**************************************************************************************************
* @file IFxBizDataMgrService.h
* @note ҵ�����ݹ���
* @author z00050
* @date 2019-9-7
**************************************************************************************************/
#ifndef IBIZDATA_MGR_SERVICE_H
#define IBIZDATA_MGR_SERVICE_H

#include <QObject>
/**
  * @class IFxBizDataMgrService
  * @brief ҵ�����ݹ���
  * @author z00050
*/
class IFxBizDataMgrService
{
public:
	//���ر������� 
	//strPath �����ļ�·��
	virtual void LoadFile(std::string strPath) = 0;
	
	//�����ⲿ����
	//qbyteData ����UDP��TCP��WebService����
	virtual bool LoadData(QByteArray qbyteData) = 0;
	
	//��ճ�������
	virtual void ClearSceneData() = 0;	
	
};

Q_DECLARE_INTERFACE(IFxBizDataMgrService, "freex_bizdata_mgr_service")

#endif
