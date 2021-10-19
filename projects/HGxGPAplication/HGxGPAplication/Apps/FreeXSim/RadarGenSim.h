#ifndef RADAR_GENER_SIM_H
#define RADAR_GENER_SIM_H

#include <QThread>

#include "SimStructDef.h"

// 雷达传感器模拟
// 模拟雷达探测数据、雷达波束数据
class RadarGenSim : public QThread
{
	Q_OBJECT
public:

	RadarGenSim(QObject *parent = 0);
	
	~RadarGenSim(void);

	// 模拟生成数据
	void SimGenerData();

	// 生成管理对象
	void GenerSimObj();
	// 
	void Start();
	// 
	void Stop();

signals:

	// 场景持续时长
	void SimTimeLenSignal(double dSec);

	// 当前时间
	void CurTimeSignal(double dSec);
protected:

	//
	void run();


protected:
	//标志位
	SimInfo m_simInfo;

	//
	bool m_bStop;

};

#endif // RADAR_SIM_H
