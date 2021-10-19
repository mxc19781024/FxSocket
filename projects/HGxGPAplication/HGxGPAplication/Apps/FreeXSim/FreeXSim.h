#ifndef FREEXSIMISDGET_H
#define FREEXSIMISDGET_H

#include <qglobal.h>
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
#include <QtGui/QWidget>
#else
#include <QWidget>
#endif
#include "RadarGenSim.h"

#include "SimData.h"
#include "Apps/FreeXSim/ui_FreeXSimWidget.h"

class FreeXSim : public QWidget
{
	Q_OBJECT

public:
	FreeXSim(QWidget *parent = 0);
	~FreeXSim();

	protected slots:

		// 生成数据
		void generDataSlot();

		// 模拟仿真数据
		void simDataSlot();

		// 时长
		void timeLenSlot(double dTimeLen);

		// 当前时间
		void curTimeSlot(double dCurTime);

		// 加速
		void increSpeedSlot();
		// 减速
		void desSpeedSlot();
		// 设置场景名称
		void setSceneNameSlot(QString );

		// 停止模拟
		void stopSimSlot();

		// 暂停
		void pasueSlot();

		// 设置当前倍速
		void setCurSpeedLbl();

private:
	Ui::FreeXSimClass ui;

	// 模拟雷达数据
	RadarGenSim *m_pRadarSim;

	double m_dSimTimeLen;

	// 数据模拟
	SimData *m_pSimData;
};

#endif // FREEXSIM_H
