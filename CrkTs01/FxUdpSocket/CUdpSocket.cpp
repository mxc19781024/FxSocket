#include "CUdpSocket.h"
CUdpSocket* CUdpSocket::m_udpSocket = nullptr;
int CUdpSocket::length = 1;
CBuffer CBuffer::m_cBuff;
int CBuffer::m_nVecNum = 0;

CUdpSocket& CUdpSocket::getIpmreqUdpSocket()
{
	// TODO: 在此处插入 return 语句
	if (m_udpSocket == nullptr)
	{
		m_udpSocket = new CUdpSocket();
	}
	else
	{
		return *m_udpSocket;
	}
}

bool CUdpSocket::NodeCount()
{
	if (m_vNodeCount < 1)
	{
		return false;
	}
	else
	{
		return true;
	}
}

CUdpSocket::CUdpSocket()
{
	m_iRet = 0;
	m_vNodeCount = 0;
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	m_socket = socket(AF_INET, SOCK_DGRAM, 0);

	//本 AF_INET     234.2.2.2   4412
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	addr.sin_port = htons(LOCAL_POST);

	//外部AF_INET   234.2.2.4     4414
	addr1.sin_family = AF_INET;
	addr1.sin_addr.S_un.S_addr = inet_addr(REMOTE_ADDRESS);
	addr1.sin_port = htons(REMOTE_POST);

	int iFlag = 1;	// 0-同一台主机 1-夸主机
	m_iRet = setsockopt(m_socket, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&iFlag, sizeof(iFlag));
	if (m_iRet != 0) {
		printf("setsockopt fail:%d", WSAGetLastError());
		return ;
	}

	m_iRet = bind(m_socket, (sockaddr*)&addr, sizeof(addr));
	if (m_iRet != 0) {
		printf("bind fail:%d", WSAGetLastError());
		return ;
	}
	printf("socket:%d bind success\n", m_socket);

	// 加入组播  
	ip_mreq multiCast;

	multiCast.imr_interface.S_un.S_addr = INADDR_ANY;
	//multiCast.imr_interface.S_un.S_addr = inet_addr("192.168.101.68");
	multiCast.imr_multiaddr.S_un.S_addr = inet_addr(LOCAL_ADDRESS);
	m_iRet = setsockopt(m_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&multiCast, sizeof(multiCast));
	if (m_iRet != 0) {
		printf("setsockopt fail:%d", WSAGetLastError());
		return ;
	}

	printf("udp group start\n");

	loop();

}

CUdpSocket::~CUdpSocket()
{
}

void CUdpSocket::loop()
{

	std::string cmd = std::to_string(CMD_UserLogin_L) + '#';
	CBuffer::getObject().addTheTask(cmd);
	//write(cmd);
		std::string msg;
		while (length)
		{
			int len = sizeof(sockaddr);
			char strRecv[1024] = { 0 };
			memset(strRecv, 0, sizeof(strRecv));
			m_iRet = recvfrom(m_socket, strRecv, sizeof(strRecv) - 1, 0, (sockaddr*)&addr1, &len);
			if (m_iRet <= 0) {
				printf("recvfrom fail:%d", WSAGetLastError());
				return;
			}
			printf("recv data:%s\n", strRecv);
			msg = strRecv;
			parseCommand(msg);
			length--;
		}
}

std::vector<std::string> CUdpSocket::split(const std::string &str, const std::string &pattern)
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

void CUdpSocket::parseDataLogin(std::string list)
{
	std::string cmd = std::to_string(CMD_getAircaftInformtion) + '#';
	//write(cmd);
	CBuffer::getObject().addTheTask(cmd);
	length = atoi(list.c_str()) + 1;
}

void CUdpSocket::parseDataAircaInfomtion(std::string& list)
{
	/************************************************************************/
	/*        string	  + msg.name							          	*
	 *        double	  + std::to_string(msg.time)			          	*
	 *        SPosition   + std::to_string(msg.optionType)		          	*
	 *        EPosType    + std::to_string(msg.postion.type)	          	*
	 *        double	  + std::to_string(msg.postion.x)		          	*
	 *        double	  + std::to_string(msg.postion.y)		          	*
	 *        double      + std::to_string(msg.postion.z);                  */
	/************************************************************************/
	std::vector<std::string> msgList= split(list, "|");
	SNodeInfo3D node;
	node.name = msgList.at(0);

	std::string strTime = msgList.at(1);
	double dtime = atof(strTime.c_str());
	node.time = dtime;

	std::string strPosition = msgList.at(2);
	int nPosition = atoi(strPosition.c_str());
	node.optionType = ENodeOptionType(nPosition);

	std::string strPosType = msgList.at(3);
	int nPosType = atoi(strPosType.c_str());
	node.postion.type = EPosType(nPosType);

	std::string strX = msgList.at(4);
	int nX = atoi(strX.c_str());
	node.postion.x = EPosType(nX);

	std::string strY = msgList.at(5);
	int nY = atoi(strY.c_str());
	node.postion.y = EPosType(nY);

	std::string strZ = msgList.at(6);
	int nZ = atoi(strZ.c_str());
	node.postion.z = EPosType(nZ);

	m_vNodeGather.push_back(node);

}

void CUdpSocket::parseCommand(std::string& msg)
{
	std::string strParthString = msg;

	std::string cmd = split(strParthString, "#").at(0);

	const char *ch = cmd.c_str();

	int ncmd = atoi(ch);

	std::string list = split(strParthString, "#").at(1);

	switch (ncmd)
	{
	case CMD_UserLogin_L:
		parseDataLogin(list); break;
	case CMD_getAircaftInformtion:
		parseDataAircaInfomtion(list); break;

	default:		break;
	}
}

bool CUdpSocket::write(const std::string& msg)
{
	m_iRet = sendto(m_socket, msg.c_str(), strlen(msg.c_str()) + 1, 0, (sockaddr*)&addr1, sizeof(sockaddr));
	if (m_iRet <= 0) {
		printf("send fail:%d", WSAGetLastError());
		return false;
	}
	else {
		printf("send data:%s\n", msg.c_str());
		return true;
	}
}

const std::vector<SNodeInfo3D>& CUdpSocket::getNodeGather() const
{
	// TODO: 在此处插入 return 语句
	return m_vNodeGather;
}

CBuffer & CBuffer::getObject()
{
	// TODO: 在此处插入 return 语句
	return m_cBuff;
}

bool CBuffer::isempty()
{
	if (m_nVecNum > 0)
	{
		return true;
	} 
	else
	{
		return false;
	}
}

CBuffer::CBuffer()
{
	m_vecBffer.clear();
}

CBuffer::~CBuffer()
{
	m_vecBffer.clear();
	m_nVecNum = 0;
}

void CBuffer::addTheTask(const std::string & task)
{
	m_vecBffer.push_back(task);
	m_nVecNum++;
}

const std::string CBuffer::getTheTask()
{
	// TODO: 在此处插入 return 语句
	std::string msg = m_vecBffer.front();
	m_vecBffer.pop_back();
	m_nVecNum--;
	return msg;
}
