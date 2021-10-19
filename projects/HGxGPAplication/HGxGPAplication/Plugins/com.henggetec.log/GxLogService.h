#ifndef CGX_LOG_SERVICE_H
#define CGX_LOG_SERVICE_H

#include <QObject>

class CGxLogService
{
public:
	/*enum EmLogLevel
	{
		trace,
		debug,
		info,
		warn,
		error,
		critical,
		off,
	};*/

	virtual ~CGxLogService() {}

	virtual void trace(const std::string &msg, bool bExtra = false) = 0;

	virtual void debug(const std::string &msg, bool bExtra = false) = 0;

	virtual void info(const std::string &msg, bool bExtra = false) = 0;

	virtual void warn(const std::string &msg, bool bExtra = false) = 0;

	virtual void error(const std::string &msg, bool bExtra = false) = 0;

	virtual void critical(const std::string &msg, bool bExtra = false) = 0;

	virtual void setPattern(const std::string &pattern) = 0;

	//virtual void setLevel(EmLogLevel logLevel) = 0;
};

Q_DECLARE_INTERFACE(CGxLogService, "com_henggetec_service_log")

#endif // !FX_LOG_SERVICE_H

