#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/wincolor_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>

#include "GxLog.h"

CGxLog::CGxLog()
{
	auto pConsoleSink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
	auto pRotateFileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("CGxLog.txt", 1024000 * 10, 3);
	m_pLogger = std::make_shared<spdlog::logger>("Message", pConsoleSink);
	spdlog::register_logger(m_pLogger);
	m_pLogger->sinks().push_back(pRotateFileSink);
	m_pLogger->set_level(spdlog::level::trace);
	m_pLogger->flush_on(spdlog::level::trace);
	spdlog::set_default_logger(m_pLogger);
}

CGxLog::~CGxLog()
{
	spdlog::drop_all();
}

void CGxLog::trace(const std::string &strMsg, bool bExtra)
{
	if (bExtra)
	{
		SPDLOG_TRACE(strMsg);
	}
	else
	{
		spdlog::trace(strMsg);
	}
}

void CGxLog::debug(const std::string &strMsg, bool bExtra)
{
	if (bExtra)
	{
		int a = 0;
		SPDLOG_DEBUG(strMsg);
	}
	else
	{
		spdlog::debug(strMsg);
	}
}

void CGxLog::info(const std::string &strMsg, bool bExtra)
{
	if (bExtra)
	{
		SPDLOG_INFO(strMsg);
	}
	else
	{
		spdlog::info(strMsg);
	}
}

void CGxLog::warn(const std::string &strMsg, bool bExtra)
{
	if (bExtra)
	{
		SPDLOG_WARN(strMsg);
	}
	else
	{
		spdlog::warn(strMsg);
	}
}

void CGxLog::error(const std::string &strMsg, bool bExtra)
{
	if (bExtra)
	{
		SPDLOG_ERROR(strMsg);
	}
	else
	{
		spdlog::error(strMsg);
	}
}

void CGxLog::critical(const std::string &strMsg, bool bExtra)
{
	if (bExtra)
	{
		SPDLOG_CRITICAL(strMsg);
	}
	else
	{
		spdlog::critical(strMsg);
	}
}

void CGxLog::setPattern(const std::string &strPattern)
{
	spdlog::set_pattern(strPattern);
}
