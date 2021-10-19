#ifndef HGX_UDPUNICASE_PARAM_H
#define HGX_UDPUNICASE_PARAM_H 1

#include <string>
#include <bdpNet/Common.h>

#define  UDP_UNICAST   0
#define  UDP_MULTICAST 1
//#define  UDP_BORADCAST 2

#define  UDP_SEND  0x1
#define  UDP_RECV  0x2

#define  UDP_SERVICER_TYPE  "UDP"

struct SUdpParam : public HGxNet::SNetworkParam
{
	int  iCommuMode; // 通信模式，发送或接收
	int  iUdpType;   // UDP类型，单播、组播、广播

	std::string  strURL;  // 自身的URL

	std::string  strGroupURL;  // 组播的组URL 
	std::string  strInterURL;  // 组播的URL
	bool bEchoBack;  // 组播回环

	SUdpParam()
		: iCommuMode(UDP_RECV),
		  iUdpType(UDP_UNICAST),
		  bEchoBack(false)
	{}

	const char* getServicerType() const { return UDP_SERVICER_TYPE; }
};

#endif // HGX_UDPUNICASE_PARAM_H
