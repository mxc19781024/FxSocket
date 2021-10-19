#pragma once

#include <QMutex>
#include <QString>

// 仿真时钟
class SimTimer
{
public:

	// 获取单例
	static SimTimer *getInstance();

	// 销毁单例
	static void destroyInstance();

	// 秒转时分秒
	static QString secondToStrHMS(double dSecond);

	// 获取当前系统时间 单位秒
	static double getCurSysTime();

	//// 延时单位 s
	static void delay( long double Delt_Time );
public:

	SimTimer(void);
	~SimTimer(void);

	// 单位秒
	void setCurLogicTime(double dCurTime);
	
	// 设置数据计算周期
	void setDeriod(double dPeriod);

	// 获取逻辑时间
	double getCurLogicTime();

	// 获取仿真周期
	double getPeriod();

	// 设置延时是否停止
	static void setDelayStop(bool bStop);

	// 设置时间缩放因子 用于加减速
	void setTimeFactor(double dFactor);

	double getTimeFactor();

	// 设置开始绝对时间
	void setStartAbsTime(double dStartAbs);

	// 获取开始绝对时间
	double getStartAbsTime();

	// 获取时钟真实逝去时间
	double getRelElapseTime();
	
	void  setRelElapseTime(double dElapseTime);


protected:
	// 数据采集周期 单位秒 0.100 100毫秒采集一次 默认100毫秒采集一次数据
	// 
	double m_dPeriod;

	// 当前逻辑时间
	double m_dCurLogicTime;

	// 时间因子
	double m_dTimeFactor;

	// 开始绝对时间
	double m_dStartAbsTime;

	// 时延时间 一次模拟总共逝去时间根据时钟走
	// 真实时间 
	double m_dRelElaspTime;

	// 互斥体
	static QMutex s_mutex;

	// 仿真时钟控制器
	static SimTimer *s_pSimTimer;

	static bool m_bDelyStop;
};

