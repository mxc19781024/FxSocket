#pragma once

#include <map>
#include <vector>
#include <list>
#include <QString>
#include <QFile>

/*********************************************************************************************/
/* 解析udp通信数据，然后将其数据通过 ， | #  ReferenceLongitude ReferenceLatitude RecordingTime */
/* 六种方式去进行数据的解析，然后将数据填写到 m_mapId2Type  m_pAcmiData  m_dHDelat m_dLonDelt    */
/* m_dLatDelt等中                                                                            */
/*********************************************************************************************/

/// 对应tacView 数据文件解析 只解析位置姿态

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

/// curTime-postion-posture  飞机飞行记录
struct AcmiFlyRecordItem
{
	/// 当前时间
	double dCurTime;

	/// 位置点 经纬高--单位 度
	C3dPoint postionPt;

	/// 姿态 Roll-Pitch-Yaw
	C3dPoint posturePt;
};

/// 一个装备记录 id，每帧记录
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

/// 文件头 只使用必要的
struct AcmiHead
{
	/// 相对经度
	float fReferenceLon;

	/// 相对纬度
	float fReferenceLat;

	/// 记录时间
	QString strRecordTime;
};

//飞行数据
struct AcmiData
{
	/// 文件头 经度-纬度-时间
	AcmiHead head;

	/// 记录 /// ID-curTime-postion-posture
	AcimFlyRecord records;
};

/// acmi 文件中标识
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

	/// “，”
	static QString &ItemSplitFlag();

	/// "|" transform
	static QString &TSplitFlag();

public:

	/// 相对经度
	static QString s_strReferenceLongitude;

	/// 相对纬度
	static QString s_strReferencLatitude;

	/// 记录时间
	static QString s_strRecordTime;

	/// 记录标志
	static QString s_strRecordFlag;

	/// 记录内容
	static QString s_strItemSplitFlag;

	/// transoform flag / pos posture   位置  姿态
	static QString s_strTSpliteFlag;
};

/// 
struct stZBInfo
{
	///类型
	QString strType;
	///名字
	QString strName;
	/// 阵营
	QString strColor;
	///？？
	QString strBD;
	///开始时间
	double  dStartTime;
	///结束时间
	double  dEndTime;
};

class ACMIDataParse
{
public:
	ACMIDataParse(void);
	~ACMIDataParse(void);

	// 文件读取
	bool ParseAcmiFile(QString strFilePath);

	//保存
	bool SaveFile(QString strDir);

	// 设置高度偏移 3000
	void SetHDelt(double dDelt){ m_dHDelat = dDelt;}
	//设置经度偏移  2
	void SetLonDelt(double dLonDelt){ m_dLonDelt = dLonDelt;}
	//设置维度偏移 -1.5
	void SetLatDelt(double dDelt){m_dLatDelt = dDelt;}

protected:
	/// AcmiData *m_pAcmiData.head 赋值 -> 经度 纬度 时间;
	void ParseHad(QString &strLine);

	/// m_pAcmiData->records  m_mapId2Type 赋值
 	void ParseRecord(QFile &file,QString &strLine);

	/// 根据传进去的 nIndex  进行返回 经纬高 飞机欧拉角的三个角
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
	/* ./文件头 head
			/-经度
			/-纬度
			/-时间

	   ./记录 records
			/-ID 
			/-记录 结构体
				//-curTime
				//-postion
				//-posture                                                                     */
	/************************************************************************/
	AcmiData *m_pAcmiData;

	// 高度偏移
	double m_dHDelat;
	//经度偏移
	double m_dLonDelt;
	//纬度偏移
	double m_dLatDelt;

};

