#include "SimTimer.h"
#include <QMutexLocker>

#ifdef _WIN32
#include "qt_windows.h"
#else
#include <time.h>
#endif

#include <QTime>

// 互斥体
QMutex SimTimer::s_mutex;
// 仿真时钟控制器
SimTimer *SimTimer::s_pSimTimer = NULL;

bool SimTimer::m_bDelyStop = false;

SimTimer * SimTimer::getInstance()
{
	QMutexLocker locker(&s_mutex);

	if (s_pSimTimer == NULL)
	{
		s_pSimTimer = new SimTimer;
	}

	return s_pSimTimer;
}

void SimTimer::destroyInstance()
{	
	QMutexLocker locker(&s_mutex);

	if (s_pSimTimer != NULL)
	{
		delete s_pSimTimer;
		s_pSimTimer = NULL;
	}

}

SimTimer::SimTimer(void)
	:m_dPeriod(0.1)      //数据采集周期
	,m_dCurLogicTime(0)	 //当前逻辑时间
	,m_dStartAbsTime(0)	 //开始绝对时间
	,m_dTimeFactor(1)	 //时间因子
	, m_dRelElaspTime(0) // 时延时间 一次模拟总共逝去时间根据时钟走 ------ 真实时间 
{

}


SimTimer::~SimTimer(void)
{

}

//设置当前逻辑时间
void SimTimer::setCurLogicTime( double dCurTime )
{
	QMutexLocker locker(&s_mutex);
	m_dCurLogicTime = dCurTime;
}

//设置数据计算周期
void SimTimer::setDeriod( double dPeriod )
{
	QMutexLocker locker(&s_mutex);
	m_dPeriod = dPeriod;
}

//获得逻辑时间
double SimTimer::getCurLogicTime()
{
	QMutexLocker locker(&s_mutex);
	return m_dCurLogicTime;
}

//获取仿真周期
double SimTimer::getPeriod()
{
	QMutexLocker locker(&s_mutex);
	return m_dPeriod;
}

//延时单位
void SimTimer::delay( long double Delt_Time )
{
#ifdef WIN32

	LARGE_INTEGER Freq, timeStart, timeEnd;
	ULONGLONG TimetoWait;

	//// 时钟频率一秒多少下   检索性能计数器的频率。性能计数器的频率固定在系统启动处，并且在所有处理器中保持一致。因此，频率仅在应用初始化时查询，结果可以缓存。
	if (!QueryPerformanceFrequency(&Freq))
	{
		return;
	}

	//// 总共等待的时间
	TimetoWait  = Freq.QuadPart*Delt_Time;

	//检索性能计数器的当前值，这是一个高分辨率（<1us）时间戳，可用于时间间隔测量。
	QueryPerformanceCounter(&timeStart);

	timeEnd = timeStart;

	while((timeEnd.QuadPart - timeStart.QuadPart) <= TimetoWait)
	{
		if (m_bDelyStop)
		{
			break;
		}

		QueryPerformanceCounter(&timeEnd);
	}

#else
	struct timespec time_start ={0,0},time_end={0,0};

	/// 转为纳秒
	long long lNs = Delt_Time * 1000 * 1000 * 1000;
	clock_gettime(CLOCK_REALTIME, &time_start);
	while(true)
	{
		if(m_bStop)
		{
			break;
		}
		clock_gettime(CLOCK_REALTIME, &time_end);

		long long deltNs = (time_end.tv_sec - time_start.tv_sec) * 1000 * 1000 * 1000 + time_end.tv_nsec - time_start.tv_nsec;

		if(deltNs > lNs)
		{
			break;
		}
	}
#endif
	return;
}

//设置延时是否停止
void SimTimer::setDelayStop( bool bStop )
{
	QMutexLocker locker(&s_mutex);
	m_bDelyStop = bStop;
}

// 设置时间缩放因子 用于加减速
void SimTimer::setTimeFactor( double dFactor )
{
	QMutexLocker locker(&s_mutex);
	m_dTimeFactor = dFactor;
}

//获取时间系数
double SimTimer::getTimeFactor()
{
	QMutexLocker locker(&s_mutex);
	return m_dTimeFactor;
}

//设置开始绝对时间
void SimTimer::setStartAbsTime( double dStartAbs )
{
	QMutexLocker locker(&s_mutex);
	m_dStartAbsTime = dStartAbs;
}

//获取开始绝对时间
double SimTimer::getStartAbsTime()
{
	QMutexLocker locker(&s_mutex);
	return m_dStartAbsTime;
}

// 获取时钟真实逝去时间
double SimTimer::getRelElapseTime()
{
	QMutexLocker locker(&s_mutex);
	return m_dRelElaspTime;
}
// 设置时钟真实逝去时间
void SimTimer::setRelElapseTime( double dElapseTime )
{
	QMutexLocker locker(&s_mutex);
	m_dRelElaspTime = dElapseTime;
}

//获取当前系统时间
double SimTimer::getCurSysTime()
{
	qint64 curTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

	return curTime / 1000.0;
}

//时间转换
QString SimTimer::secondToStrHMS( double dSecond )
{
	QTime time(0,0,0,0);
	time = time.addMSecs(dSecond * 1000);
	QString strTime = time.toString("hh:mm:ss");
	return strTime;
}
