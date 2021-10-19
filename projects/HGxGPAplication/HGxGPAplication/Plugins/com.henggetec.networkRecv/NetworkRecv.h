#ifndef HGX_NETWORK_RECV_H
#define HGX_NETWORK_RECV_H

#include <bdpNet/GxNetworkManager.h>
#include <QObject>

namespace HGxNet
{
	class CGxService;
}

struct SUdpReceiver : public QObject,public HGxNet::SNetworkReceiver
{
	Q_OBJECT
public:
	SUdpReceiver(HGxNet::CGxServicer* pServicer);

	~SUdpReceiver();

	int recvData(char* pBuf, unsigned int uiLen);
signals:
	void recvDataSignal(QByteArray);
protected:
	HGxNet::CGxServicer*  m_pServicer;

	long    m_pMemoryID;
	char*   m_pBuf;
};

#endif // HGX_TEST_NETWORK_H
