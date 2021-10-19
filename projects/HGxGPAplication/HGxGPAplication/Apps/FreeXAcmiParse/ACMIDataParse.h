#pragma once

#include <map>
#include <vector>
#include <list>
#include <QString>
#include <QFile>

/*********************************************************************************************/
/* ����udpͨ�����ݣ�Ȼ��������ͨ�� �� | #  ReferenceLongitude ReferenceLatitude RecordingTime */
/* ���ַ�ʽȥ�������ݵĽ�����Ȼ��������д�� m_mapId2Type  m_pAcmiData  m_dHDelat m_dLonDelt    */
/* m_dLatDelt����                                                                            */
/*********************************************************************************************/

/// ��ӦtacView �����ļ����� ֻ����λ����̬

struct C3dPoint
{
	C3dPoint()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	float x;
	float y;
	float z;
};

/// curTime-postion-posture  �ɻ����м�¼
struct AcmiFlyRecordItem
{
	/// ��ǰʱ��
	double dCurTime;

	/// λ�õ� ��γ��--��λ ��
	C3dPoint postionPt;

	/// ��̬ Roll-Pitch-Yaw
	C3dPoint posturePt;
};

/// һ��װ����¼ id��ÿ֡��¼
//struct AcimFlyRecord
//{
//	int nId;
//
//	std::vector<AcmiFlyRecordItem> vecRecordItem;
//};

/// curTime-postion-posture
typedef std::vector<AcmiFlyRecordItem> AcimRecordItems;

/// ID--ItemContent=curTime-postion-posture
typedef  std::map<QString,AcimRecordItems > AcimFlyRecord;

/// �ļ�ͷ ֻʹ�ñ�Ҫ��
struct AcmiHead
{
	/// ��Ծ���
	float fReferenceLon;

	/// ���γ��
	float fReferenceLat;

	/// ��¼ʱ��
	QString strRecordTime;
};

//��������
struct AcmiData
{
	/// �ļ�ͷ ����-γ��-ʱ��
	AcmiHead head;

	/// ��¼ /// ID-curTime-postion-posture
	AcimFlyRecord records;
};

/// acmi �ļ��б�ʶ
class AcmiFlags
{
	///
public:
	AcmiFlags();

	///ReferenceLongitude
	static QString &ReferenceLongitude();

	///ReferenceLatitude
	static QString &ReferencLatitude();

	///RecordingTime
	static QString &RecordTime();

	/// " # "
	static QString &RecordFlag();

	/// ������
	static QString &ItemSplitFlag();

	/// "|" transform
	static QString &TSplitFlag();

public:

	/// ��Ծ���
	static QString s_strReferenceLongitude;

	/// ���γ��
	static QString s_strReferencLatitude;

	/// ��¼ʱ��
	static QString s_strRecordTime;

	/// ��¼��־
	static QString s_strRecordFlag;

	/// ��¼����
	static QString s_strItemSplitFlag;

	/// transoform flag / pos posture   λ��  ��̬
	static QString s_strTSpliteFlag;
};

/// 
struct stZBInfo
{
	///����
	QString strType;
	///����
	QString strName;
	/// ��Ӫ
	QString strColor;
	///����
	QString strBD;
	///��ʼʱ��
	double  dStartTime;
	///����ʱ��
	double  dEndTime;
};

class ACMIDataParse
{
public:
	ACMIDataParse(void);
	~ACMIDataParse(void);

	// �ļ���ȡ
	bool ParseAcmiFile(QString strFilePath);

	//����
	bool SaveFile(QString strDir);

	// ���ø߶�ƫ�� 3000
	void SetHDelt(double dDelt){ m_dHDelat = dDelt;}
	//���þ���ƫ��  2
	void SetLonDelt(double dLonDelt){ m_dLonDelt = dLonDelt;}
	//����ά��ƫ�� -1.5
	void SetLatDelt(double dDelt){m_dLatDelt = dDelt;}

protected:
	/// AcmiData *m_pAcmiData.head ��ֵ -> ���� γ�� ʱ��;
	void ParseHad(QString &strLine);

	/// m_pAcmiData->records  m_mapId2Type ��ֵ
 	void ParseRecord(QFile &file,QString &strLine);

	/// ���ݴ���ȥ�� nIndex  ���з��� ��γ�� �ɻ�ŷ���ǵ�������
	float GetTValue(const AcimRecordItems & items, const QStringList &splits,int nIndex);

protected:
	/************************************************************************/
	/* ./string
	   ./stZBInfo
			/-strType;
			/-strName;
			/-strColor;
			/-strBD;
			/-dStartTime;
			/-dEndTime;
	 */
	/************************************************************************/
	std::map<QString,stZBInfo> m_mapId2Type;

	/************************************************************************/
	/* ./�ļ�ͷ head
			/-����
			/-γ��
			/-ʱ��

	   ./��¼ records
			/-ID 
			/-��¼ �ṹ��
				//-curTime
				//-postion
				//-posture                                                                     */
	/************************************************************************/
	AcmiData *m_pAcmiData;

	// �߶�ƫ��
	double m_dHDelat;
	//����ƫ��
	double m_dLonDelt;
	//γ��ƫ��
	double m_dLatDelt;

};

