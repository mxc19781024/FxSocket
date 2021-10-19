#ifndef CGX_LOG_H
#define CGX_LOG_H

#include <QObject>
#include <GxLogService.h>
#include <spdlog/spdlog.h>

class CGxLog : public QObject, public CGxLogService
{
	Q_OBJECT
	Q_INTERFACES(CGxLogService)

public:
	CGxLog();

	~CGxLog();

	void trace(const std::string &, bool);

	void debug(const std::string &, bool);

	void info(const std::string &, bool);

	void warn(const std::string &, bool);

	void error(const std::string &, bool);

	void critical(const std::string &, bool);

	void setPattern(const std::string &pattern);

private:
	std::shared_ptr<spdlog::logger> m_pLogger;
};

#endif // !FX_LOG_H
