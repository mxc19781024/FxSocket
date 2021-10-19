#include "ACMIDataParse.h"

#include <QFile>
#include <QStringList>
#include <QTextStream>

/// �����ǰ��û��λ������ �����һ��λ��������ͬ

/// ��Ծ���
QString AcmiFlags::s_strReferenceLongitude="ReferenceLongitude";

/// ���γ��
QString AcmiFlags::s_strReferencLatitude="ReferenceLatitude";

/// ��¼ʱ��
QString AcmiFlags::s_strRecordTime="RecordingTime";

/// ��¼��־
QString AcmiFlags::s_strRecordFlag = "#";

/// ����֮��ָ��־
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
	//�Զ��Ž��зָ�
	QStringList strSplitList = strLine.split(AcmiFlags::ItemSplitFlag());

	//�ж����ݵ�������
	if (strSplitList.size() < 2)
	{
		return;
	}

	//�õ����ݲ���
	QString strContent = strSplitList.at(1);

	//�ԣ����и�
	strSplitList = strContent.split("=");

	//�ж����ݵ�����
	if (strSplitList.size() < 2)
	{
		//���� 0
		strContent = "0";
	}
	else
	{
		//�������ݲ���
		strContent = strSplitList.at(1);
	}

	//�ж��ַ������Ƿ���� ReferenceLongitude
	if (strLine.contains(AcmiFlags::ReferenceLongitude()))
	{
		//�����ȸ�ֵ
		m_pAcmiData->head.fReferenceLon = strContent.toFloat();
	}
	else if (strLine.contains(AcmiFlags::ReferencLatitude()))
	{
		//��γ�ȸ�ֵ
		m_pAcmiData->head.fReferenceLat = strContent.toFloat();
	}
	else if (strLine.contains(AcmiFlags::RecordTime()))
	{
		//��ֵʱ��
		m_pAcmiData->head.strRecordTime = strContent;
	}
}

void ACMIDataParse::ParseRecord(QFile &file, QString &strLine )
{
	/// ����ʱ��   �Ƴ� #
	QString  strCurTime = strLine.remove(AcmiFlags::RecordFlag());

	//Qstringתfloat  ��ǰʱ��
	float fCurTiem = strCurTime.toFloat();

	//�ṹ�� curTime-postion-posture  
	AcmiFlyRecordItem  recoderItem;
	
	//�и��
	QStringList strRecordSplit;

	//���� id
	QString strContent;

	//Id
	QString  strId;

	do 
	{
		//��ȡ�ļ�
		strLine = file.readLine();

		//�ж� # �Ƿ���� ���ڷ���TRUE  û�з���FALSE
		if (strLine.contains(AcmiFlags::RecordFlag()))
		{
			//��#ȥ��  ��ǰʱ����ַ���
			strCurTime = strLine.remove(AcmiFlags::RecordFlag());
			//ת��Ϊfloat
			fCurTiem = strCurTime.toFloat();
		}
		else
		{
			//��ֹǰ���жϴ���
			if (strLine.contains(AcmiFlags::RecordFlag()))
			{
				continue;
			}

			/// ÿ����¼�� �������и�
			strRecordSplit = strLine.split(AcmiFlags::ItemSplitFlag());

			//���ݲ�����
			if (strRecordSplit.size() < 2)
			{
				continue;
			}

			//���������ĵ�ǰʱ����д洢
			recoderItem.dCurTime = fCurTiem;
			/// ����Id
			strContent = strRecordSplit.at(0);

			//���ַ����еĿո�ɾ��
			strId = strContent.trimmed();

			//������ĩβ��\0ȥ��  32.2  --> 32.  
			strId = strId.left(strId.size() - 2);

			//����ֵ
			bool bOk;

			//���ַ���ת��Ϊ�޷��������� 16����
			unsigned int nt = strId.toUInt(&bOk,16);

			//ת��Ϊ�ַ���
			strId = QString::number(nt);

			/************************************************************************/
			/* ///
					struct stZBInfo
					{
						QString strType;
					
						QString strName;
						/// ��Ӫ
						QString strColor;
					
						QString strBD;
					
						double  dStartTime;
					
						double  dEndTime;
					}; */
			/************************************************************************/

			/// ��������
			///	if (strLine.contains("Type"))
			{
				stZBInfo info;
				bool bTr = false;
				//�����и��ַ�list
				for (int i = 0; i < strRecordSplit.size(); ++i)
				{
					QString strTypeContent = strRecordSplit.at(i);
					//�鿴�Ƿ��� Type �ַ�
					if (strTypeContent.contains("Type"))								//����
					{
						//�� = �и�
						QStringList strRecordSplit = strTypeContent.split("=");

						//ֻ��һ���ַ� ����  ��ʶΪType
						if (strRecordSplit.size() == 2 && strRecordSplit.at(0) == "Type")
						{
							info.strType = strRecordSplit.at(1);
						}
						bTr = true;
					}
					else if (strTypeContent.contains("Name"))							//����
					{
						QStringList strRecordSplit = strTypeContent.split("=");
						if (strRecordSplit.size() == 2 && strRecordSplit.at(0) == "Name")
						{
							info.strName = strRecordSplit.at(1);
						}
					}
					else if (strTypeContent.contains("Color"))							//��ɫ
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
					//ͨ�� ID ����Ϣ���д洢
					m_mapId2Type[strId] = info;
				}

			}

			/// ����λ����̬
			if (strContent.isEmpty())
			{
				/// idΪ��
				continue;
			}

			strRecordSplit = strRecordSplit.at(1).split("=");
			/// 
			if (strRecordSplit.size() < 2)
			{
				continue;
			}

			/// λ�ñ�ʶ
			strContent = strRecordSplit.at(0);

			if (strContent != "T")
			{
				/// ����ڶ�������λ�� ��λ������һ����ͬ
				AcimRecordItems  &items = m_pAcmiData->records[strId];

				if (items.size() < 1)
				{
					//��γ��  �����ǡ�ˮƽ�ǡ������Ǿ�Ϊ��
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
					//�ڿڿڿڿ� ->��
					recoderItem.postionPt = items[items.size() - 1].postionPt;
					recoderItem.posturePt = items[items.size() - 1].posturePt;
					items.push_back(recoderItem);
				}

				continue;
			}
			//�� | �����и�
			strRecordSplit = strRecordSplit.at(1).split(AcmiFlags::TSplitFlag());

			//
			AcimRecordItems  &items = m_pAcmiData->records[strId];

			/// ��ȡ��һ��
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

		// ������ֵ��ֵ  

		switch(nIndex)
		{
		case 0: // ����
			{
				fValue = item.postionPt.x;
			}
			break;
		case 1: // γ��
			{
				fValue = item.postionPt.y;
			}
			break;
		case 2: // �߶�
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