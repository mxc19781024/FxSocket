#include "ACMIDataParse.h"

#include <QFile>
#include <QStringList>
#include <QTextStream>

/// 如果当前秒没有位置数据 则和上一秒位置数据相同

/// 相对经度
QString AcmiFlags::s_strReferenceLongitude="ReferenceLongitude";

/// 相对纬度
QString AcmiFlags::s_strReferencLatitude="ReferenceLatitude";

/// 记录时间
QString AcmiFlags::s_strRecordTime="RecordingTime";

/// 记录标志
QString AcmiFlags::s_strRecordFlag = "#";

/// 各项之间分割标志
QString AcmiFlags::s_strItemSplitFlag=",";

/// transoform flag / pos posture
QString AcmiFlags::s_strTSpliteFlag="|";

AcmiFlags::AcmiFlags()
{

}

QString & AcmiFlags::ReferenceLongitude()
{
	return s_strReferenceLongitude;
}

QString & AcmiFlags::ReferencLatitude()
{
	return s_strReferencLatitude;
}

QString & AcmiFlags::RecordTime()
{
	return s_strRecordTime;
}

QString & AcmiFlags::RecordFlag()
{
	return s_strRecordFlag;
}


QString & AcmiFlags::ItemSplitFlag()
{
	return s_strItemSplitFlag;
}

QString & AcmiFlags::TSplitFlag()
{
	return s_strTSpliteFlag;
}

ACMIDataParse::ACMIDataParse(void)
	:m_pAcmiData(NULL)
	,m_dHDelat(0)
{

}


ACMIDataParse::~ACMIDataParse(void)
{
	if (m_pAcmiData != NULL)
	{
		delete m_pAcmiData;
		m_pAcmiData = NULL;
	}
}

bool ACMIDataParse::ParseAcmiFile( QString strFilePath )
{
	QFile file(strFilePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	m_pAcmiData = new AcmiData;

	QString strLine;
	while (!file.atEnd()) 
	{
		strLine = file.readLine();

		if (strLine.contains(AcmiFlags::RecordFlag()))
		{
			/// 
			ParseRecord(file,strLine);
		}
		else
		{
			ParseHad(strLine);
		}
	}

	return true;
}

bool ACMIDataParse::SaveFile( QString strDir )
{
	/// 

	if (!m_pAcmiData)
	{
		return false;
	}
	QString strFilePath;
	for (AcimFlyRecord::iterator itr = m_pAcmiData->records.begin(); 
		itr != m_pAcmiData->records.end(); ++itr)
	{
		stZBInfo &info = m_mapId2Type[itr->first];
		if (info.strType.contains("Decoy"))
		{
			continue;
		}
		/// 
		strFilePath = strDir + info.strType.trimmed()+ info.strColor.trimmed()+ itr->first + ".txt";
		QFile file(strFilePath);

		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
			continue;

		QString strText;
		for (int i = 0; i < itr->second.size(); ++i)
		{
			AcmiFlyRecordItem &curRecord = itr->second[i];

			QTextStream out(&file);

			strText = QString("%1 %2 %3 %4 %5 %6 %7\n").arg(curRecord.dCurTime,0,'f',8)
				.arg(curRecord.postionPt.x + m_pAcmiData->head.fReferenceLon +m_dLonDelt,0,'f',8)
				.arg(curRecord.postionPt.y + m_pAcmiData->head.fReferenceLat + m_dLatDelt,0,'f',8)
				.arg(curRecord.postionPt.z + m_dHDelat,0,'f',8)
				.arg(curRecord.posturePt.x,0,'f',8)
				.arg(curRecord.posturePt.y,0,'f',8)
				.arg(curRecord.posturePt.z,0,'f',8);

			out << strText;
		}


		if (itr->second.size() > 0)
		{
			m_mapId2Type[itr->first].dStartTime = itr->second[0].dCurTime;
			m_mapId2Type[itr->first].dEndTime = itr->second[itr->second.size() - 1].dCurTime;
		}

	}

	strFilePath = strDir + "SceneInfo" + ".txt";
	QFile file(strFilePath);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;
	QTextStream out(&file);

	for (std::map<QString,stZBInfo>::iterator itr = m_mapId2Type.begin(); 
		itr != m_mapId2Type.end(); ++itr)
	{
		stZBInfo info = itr->second;
		if (info.strType.contains("Decoy"))
		{
			continue;
		}
		QString strText;
		strText = itr->first + "," + info.strName.trimmed() + "," + info.strType.trimmed() + ","+info.strColor.trimmed() +"," + info.strBD.trimmed() + "," + QString::number(info.dStartTime,'f',11) + "," + QString::number(info.dEndTime,'f',11)+ "\n"; 
		out << strText;
	}

	return true;
}

void ACMIDataParse::ParseHad(QString &strLine)
{
	//以逗号进行分割
	QStringList strSplitList = strLine.split(AcmiFlags::ItemSplitFlag());

	//判断数据的完整性
	if (strSplitList.size() < 2)
	{
		return;
	}

	//得到数据部分
	QString strContent = strSplitList.at(1);

	//以＝号切割
	strSplitList = strContent.split("=");

	//判断数据的数量
	if (strSplitList.size() < 2)
	{
		//等于 0
		strContent = "0";
	}
	else
	{
		//等于数据部分
		strContent = strSplitList.at(1);
	}

	//判断字符串中是否包含 ReferenceLongitude
	if (strLine.contains(AcmiFlags::ReferenceLongitude()))
	{
		//给经度赋值
		m_pAcmiData->head.fReferenceLon = strContent.toFloat();
	}
	else if (strLine.contains(AcmiFlags::ReferencLatitude()))
	{
		//给纬度赋值
		m_pAcmiData->head.fReferenceLat = strContent.toFloat();
	}
	else if (strLine.contains(AcmiFlags::RecordTime()))
	{
		//赋值时间
		m_pAcmiData->head.strRecordTime = strContent;
	}
}

void ACMIDataParse::ParseRecord(QFile &file, QString &strLine )
{
	/// 解析时间   移除 #
	QString  strCurTime = strLine.remove(AcmiFlags::RecordFlag());

	//Qstring转float  当前时间
	float fCurTiem = strCurTime.toFloat();

	//结构体 curTime-postion-posture  
	AcmiFlyRecordItem  recoderItem;
	
	//切割？？
	QStringList strRecordSplit;

	//内容 id
	QString strContent;

	//Id
	QString  strId;

	do 
	{
		//读取文件
		strLine = file.readLine();

		//判断 # 是否存在 存在返回TRUE  没有返回FALSE
		if (strLine.contains(AcmiFlags::RecordFlag()))
		{
			//将#去除  当前时间的字符串
			strCurTime = strLine.remove(AcmiFlags::RecordFlag());
			//转化为float
			fCurTiem = strCurTime.toFloat();
		}
		else
		{
			//防止前方判断错误
			if (strLine.contains(AcmiFlags::RecordFlag()))
			{
				continue;
			}

			/// 每条记录以 ，进行切割
			strRecordSplit = strLine.split(AcmiFlags::ItemSplitFlag());

			//数据不完整
			if (strRecordSplit.size() < 2)
			{
				continue;
			}

			//将解析到的当前时间进行存储
			recoderItem.dCurTime = fCurTiem;
			/// 解析Id
			strContent = strRecordSplit.at(0);

			//将字符串中的空格删除
			strId = strContent.trimmed();

			//将数据末尾和\0去除  32.2  --> 32.  
			strId = strId.left(strId.size() - 2);

			//返回值
			bool bOk;

			//将字符串转化为无符号整型数 16进制
			unsigned int nt = strId.toUInt(&bOk,16);

			//转化为字符串
			strId = QString::number(nt);

			/************************************************************************/
			/* ///
					struct stZBInfo
					{
						QString strType;
					
						QString strName;
						/// 阵营
						QString strColor;
					
						QString strBD;
					
						double  dStartTime;
					
						double  dEndTime;
					}; */
			/************************************************************************/

			/// 包含类型
			///	if (strLine.contains("Type"))
			{
				stZBInfo info;
				bool bTr = false;
				//遍历切割字符list
				for (int i = 0; i < strRecordSplit.size(); ++i)
				{
					QString strTypeContent = strRecordSplit.at(i);
					//查看是否有 Type 字符
					if (strTypeContent.contains("Type"))								//类型
					{
						//以 = 切割
						QStringList strRecordSplit = strTypeContent.split("=");

						//只有一个字符 并且  标识为Type
						if (strRecordSplit.size() == 2 && strRecordSplit.at(0) == "Type")
						{
							info.strType = strRecordSplit.at(1);
						}
						bTr = true;
					}
					else if (strTypeContent.contains("Name"))							//名字
					{
						QStringList strRecordSplit = strTypeContent.split("=");
						if (strRecordSplit.size() == 2 && strRecordSplit.at(0) == "Name")
						{
							info.strName = strRecordSplit.at(1);
						}
					}
					else if (strTypeContent.contains("Color"))							//颜色
					{
						QStringList strRecordSplit = strTypeContent.split("=");
						if (strRecordSplit.size() == 2 && strRecordSplit.at(0) == "Color")
						{
							info.strColor = strRecordSplit.at(1);
						}
					}
					else if (strTypeContent.contains("Pilot"))							// ? ?
					{
						QStringList strRecordSplit = strTypeContent.split("=");
						if (strRecordSplit.size() == 2 && strRecordSplit.at(0) == "Pilot")
						{
							info.strBD = strRecordSplit.at(1);
						}
					}
				}

				if (bTr)
				{
					//通过 ID 将信息就行存储
					m_mapId2Type[strId] = info;
				}

			}

			/// 解析位置姿态
			if (strContent.isEmpty())
			{
				/// id为空
				continue;
			}

			strRecordSplit = strRecordSplit.at(1).split("=");
			/// 
			if (strRecordSplit.size() < 2)
			{
				continue;
			}

			/// 位置标识
			strContent = strRecordSplit.at(0);

			if (strContent != "T")
			{
				/// 如果第二个不是位置 则位置与上一个相同
				AcimRecordItems  &items = m_pAcmiData->records[strId];

				if (items.size() < 1)
				{
					//经纬高  俯仰角、水平角、翻滚角均为零
					recoderItem.postionPt.x = 0;
					recoderItem.postionPt.y = 0;
					recoderItem.postionPt.z = 0;
					recoderItem.posturePt.x = 0;
					recoderItem.posturePt.y = 0;
					recoderItem.posturePt.z = 0;
					items.push_back(recoderItem);
				}
				else
				{
					//口口口口口 ->口
					recoderItem.postionPt = items[items.size() - 1].postionPt;
					recoderItem.posturePt = items[items.size() - 1].posturePt;
					items.push_back(recoderItem);
				}

				continue;
			}
			//以 | 进行切割
			strRecordSplit = strRecordSplit.at(1).split(AcmiFlags::TSplitFlag());

			//
			AcimRecordItems  &items = m_pAcmiData->records[strId];

			/// 获取上一个
			if (strRecordSplit.size() == 3)
			{
				strRecordSplit.push_back("");
				strRecordSplit.push_back("");
				strRecordSplit.push_back("");
				///LLH
				float fValue = GetTValue(items,strRecordSplit,0);
				recoderItem.postionPt.x = fValue;
				fValue =GetTValue(items,strRecordSplit,1);
				recoderItem.postionPt.y = fValue;
				fValue =GetTValue(items,strRecordSplit,2);
				recoderItem.postionPt.z = fValue;
				fValue =GetTValue(items,strRecordSplit,3);
				recoderItem.posturePt.x = fValue;
				fValue =GetTValue(items,strRecordSplit,4);
				recoderItem.posturePt.y = fValue;
				fValue =GetTValue(items,strRecordSplit,5);
				recoderItem.posturePt.z = fValue;

			}
			else if (strRecordSplit.size() == 5)
			{
				/// LLHUV
				float fValue = GetTValue(items,strRecordSplit,0);
				recoderItem.postionPt.x = fValue;
				fValue =GetTValue(items,strRecordSplit,1);
				recoderItem.postionPt.y = fValue;
				fValue =GetTValue(items,strRecordSplit,2);
				recoderItem.postionPt.z = fValue;

				///  
			}
			else if (strRecordSplit.size() == 6)
			{
				/// LLH roll pitch yaw
				float fValue = GetTValue(items,strRecordSplit,0);
				recoderItem.postionPt.x = fValue;
				fValue =GetTValue(items,strRecordSplit,1);
				recoderItem.postionPt.y = fValue;
				fValue =GetTValue(items,strRecordSplit,2);
				recoderItem.postionPt.z = fValue;
				fValue =GetTValue(items,strRecordSplit,3);
				recoderItem.posturePt.x = fValue;
				fValue =GetTValue(items,strRecordSplit,4);
				recoderItem.posturePt.y = fValue;
				fValue =GetTValue(items,strRecordSplit,5);
				recoderItem.posturePt.z = fValue;
			}
			else if (strRecordSplit.size() == 9)
			{
				/// LLH roll pitch yaw U V heading
				float fValue = GetTValue(items,strRecordSplit,0);
				recoderItem.postionPt.x = fValue;
				fValue =GetTValue(items,strRecordSplit,1);
				recoderItem.postionPt.y = fValue;
				fValue =GetTValue(items,strRecordSplit,2);
				recoderItem.postionPt.z = fValue;
				fValue =GetTValue(items,strRecordSplit,3);
				recoderItem.posturePt.x = fValue;
				fValue =GetTValue(items,strRecordSplit,4);
				recoderItem.posturePt.y = fValue;
				fValue =GetTValue(items,strRecordSplit,5);
				recoderItem.posturePt.z = fValue;
			}
			else
			{
				continue;
			}

			m_pAcmiData->records[strId].push_back(recoderItem);			
		}
	} 
	while (!file.atEnd());

}


float ACMIDataParse::GetTValue(const AcimRecordItems & items, const QStringList &splits,int nIndex)
{

	float fValue = 0.0;

	if (nIndex < 0 || nIndex >5)
	{
		return fValue;
	}

	QString strContent = "";

	if (nIndex > splits.size())
	{
		strContent = "";
	}
	else
	{
		strContent = splits.at(nIndex);
	}

	if (strContent.trimmed().isEmpty())
	{
		if (items.size() == 0)
		{
			return 0;
		}

		const AcmiFlyRecordItem & item = *(items.end() - 1);

		// 给返回值赋值  

		switch(nIndex)
		{
		case 0: // 经度
			{
				fValue = item.postionPt.x;
			}
			break;
		case 1: // 纬度
			{
				fValue = item.postionPt.y;
			}
			break;
		case 2: // 高度
			{
				fValue = item.postionPt.z;
			}
			break;
		case 3: // Roll
			{
				fValue = item.posturePt.x;
			}
			break;
		case 4: // pitch
			{
				fValue = item.posturePt.y;
			}
			break;
		case 5: // yaw
			{
				fValue = item.posturePt.z;
			}
			break;
		default:
			break;
		}
	}
	else
	{
		fValue = strContent.toFloat();
	}

	return fValue;
}