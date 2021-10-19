#include <string>
#include <string.h>
#include <cstringt.h>
#include "./CParseReadData.h"
#include "../CWriteBuff/CWriteBudder.h"
#include "../CSeverMain/globalvars.h"

static std::vector<std::string> split(const std::string &str, const std::string &pattern)
{
	//const char* convert to char*
	char * strc = new char[strlen(str.c_str()) + 1];
	strcpy(strc, str.c_str());
	std::vector<std::string> resultVec;
	char* tmpStr = strtok(strc, pattern.c_str());
	while (tmpStr != NULL)
	{
		resultVec.push_back(std::string(tmpStr));
		tmpStr = strtok(NULL, pattern.c_str());
	}

	delete[] strc;

	return resultVec;
};

void CParseReadData::parseDataLogin()
{
	int cout = m_vNodeGather.size();
	std::string cmd= std::to_string(CMD_UserLogin_L) + '#' +std::to_string(cout);
	CWriteBudder::getWriteBuff().setWriteBuff(cmd);
}
void CParseReadData::parseDataAircaInfomtion()
{
	for (SNodeInfo3D node : m_vNodeGather)
	{
		CWriteBudder::getWriteBuff().setWriteBuff(node);
	}
}

void CParseReadData::parseDataInvalidCommand()
{
	CWriteBudder::getWriteBuff().setWriteBuff("Invalid command");
}
void CParseReadData::parseCommand( std::string& msg)
{
	std::string strParthString = msg;

	std::string cmd = split(strParthString, "#").at(0);

	const char *ch = cmd.c_str();

	int ncmd = atoi(ch);


	switch (ncmd)
	{
	case CMD_UserLogin_L:		parseDataLogin();			 break;
	case CMD_getAircaftInformtion:		parseDataAircaInfomtion();		break;

	default:		break;
	}
}

CParseReadData::CParseReadData()
{
	m_vNodeGather.clear();
	readXmlFile("D:/data.xml");
}

bool CParseReadData::parseData()
{
	if (CReadBuff::getInstanceReadBuff().emity())
	{
		std::string cmd = CReadBuff::getInstanceReadBuff().getFirstReadbuffer();
		parseCommand(cmd);
		return true;
	}
	else
	{
		return false;
	}
}

CParseReadData & CParseReadData::getInstance()
{
	static CParseReadData par;
	return par;
}

CParseReadData::~CParseReadData()
{
	m_vNodeGather.clear();
}



//解析xml文件
int CParseReadData::readXmlFile(std::string filepPath)
{
	TiXmlDocument mydoc(filepPath.c_str());//xml文档对象
	bool loadOk = mydoc.LoadFile();//加载文档
	if (!loadOk)
	{
		std::cout << "could not load the test file.Error:" << mydoc.ErrorDesc() << std::endl;
		exit(1);
	}

	TiXmlElement* xml_root = mydoc.RootElement();
	if (xml_root == NULL)
	{
		return -1;
	}
	TiXmlElement* xml_note = xml_root->FirstChildElement("node");

	xmlParse(xml_note);

	//{
	//	//遍历该结点
	//	for (TiXmlElement *StuElement = pEle->FirstChildElement();//第一个子元素
	//		StuElement != NULL;
	//		StuElement = StuElement->NextSiblingElement())//下一个兄弟元素
	//	{
	//		TiXmlAttribute *pAttr = StuElement->FirstAttribute();//第一个属性

	//		while (NULL != pAttr) //输出所有属性
	//		{
	//			std::cout << pAttr->Name() << ":" << pAttr->Value() << " ";
	//			pAttr = pAttr->Next();
	//		}

	//		//输出子元素的值
	//		for (TiXmlElement *sonElement = StuElement->FirstChildElement(); sonElement != NULL; sonElement = sonElement->NextSiblingElement())
	//		{
	//			TiXmlAttribute *pAttr = sonElement->FirstAttribute();//第一个属性

	//			while (NULL != pAttr) //输出所有属性
	//			{
	//				xmlParse(pAttr);
	//				pAttr = pAttr->Next();
	//			}
	//			std::cout << std::endl;
	//		}
	//	}
	//}

	return 1;
}

void CParseReadData::xmlParse(TiXmlElement *pEle)
{
	while (pEle)
	{

		SNodeInfo3D node;
		//获取他们的值
		TiXmlElement* xml_optionType = pEle->FirstChildElement("optionType");
		const char* optionType = xml_optionType->GetText();
		int optiontype = atoi(optionType);
		node.optionType = ENodeOptionType(optiontype);

		TiXmlElement* xml_serial = pEle->FirstChildElement("serial");
		const char* serial = xml_serial->GetText();

		TiXmlElement* xml_time = pEle->FirstChildElement("time");
		const char* time = xml_time->GetText();
		double dtime = atof(time);
		node.time = dtime;

		TiXmlElement* xml_name = pEle->FirstChildElement("name");
		const char* name = xml_name->GetText();
		node.name = name;

		TiXmlElement* xml_postion = pEle->FirstChildElement("postion");

		TiXmlElement* xml_posType = xml_postion->FirstChildElement("posType");
		const char* posType = xml_posType->GetText();
		int nposType = atoi(posType);
		node.postion.type = EPosType(nposType);

		TiXmlElement* xml_x = xml_postion->FirstChildElement("x");
		const char* x = xml_x->GetText();
		double nX = atof(x);
		node.postion.x = nX;

		TiXmlElement* xml_y = xml_postion->FirstChildElement("y");
		const char* posy = xml_y->GetText();
		double nY = atof(posy);
		node.postion.y = nY;

		TiXmlElement* xml_z = xml_postion->FirstChildElement("z");
		const char* z = xml_z->GetText();
		double nZ = atof(z);
		node.postion.z = nZ;

		m_vNodeGather.push_back(node);


		//只想下一个兄弟节点
		pEle = pEle->NextSiblingElement();
	}
}