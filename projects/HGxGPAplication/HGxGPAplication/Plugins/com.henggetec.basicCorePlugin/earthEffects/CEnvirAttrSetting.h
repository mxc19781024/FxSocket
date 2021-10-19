#ifndef CENVIRATTRSETTING_H
#define CENVIRATTRSETTING_H

#include <QWidget>
#include <QTimer>
#include <QDateTime>
#include <QList>
//#include <earth_environment/IFxSkyEnvironment.h>
#include "ui_CEnvirAttrSetting.h"
#include "FxBaseWindow/CFxBaseWindow.h"


#define DEFAULT_INTERVAL  1000  //默认计时器间隔时间
#define DEFAULT_SIMUSPEED  1.0  //默认仿真速率
#define MINI_INTERVAL  100  //最小计时器间隔时间
class CSpecialEffectsControl;

class CEnvirAttrSetting : public CFxBaseWindow
{
	Q_OBJECT

public:
	CEnvirAttrSetting(QWidget *parent = 0);
	~CEnvirAttrSetting();

	//是否开启计时器
	void startTimer(bool bStart);

	/** 
	* @brief 设置光照强度
	* @note add by w00025
	* @param fIntensity[in] 光照强度
	*/
	void setSunLightIntensity(const float fIntensity);

private:
	//初始化窗口
	void initWgt();
	//信号槽连接
	void initConnection();
	//仿真速率列表重新排序并去重
	void sortList();
	//设置当前列表索引值为DEFAULT_SIMUSPEED对应的索引号
	bool resetCurrentIndex();

signals:
	/** 
	* @brief 环境设置发生变化的信号
	* @note add by w00025
	*/
	void envirAttrChangedSignal();

public slots:
	//仿真计时器槽
	void simuTimerTimeOutSlot();

	//系统时间计时器槽
	void sysTimerTimeOutSlot(); 
	
	//重置仿真时间槽
	void resetSlot(); 

	//加速槽
	void speedUpSlot(); 

	//减速槽
	void slowDownSlot(); 

	//滑动条操作槽
	void sliderChangedSlot(int);

	//spinBox操作槽
	void spinBoxChangedSlot(int);

	//雨浓度操作槽
	void spinBoxRainConcentrationSlot(int);
	void spinBoxRainConcentrationSlot(double);
	//雨高度操作槽
	void spinBoxRainHeightSlot(int);

	//雪浓度操作槽
	void spinBoxSnowConcentrationSlot(int);
	void spinBoxSnowConcentrationSlot(double);

	//雪高度操作槽
	void spinBoxSnowHeightSlot(int);

	//雾浓度操作槽
	//雾高度操作槽

	//海洋高度操作槽
	void spinBoxOceanHeightSlot(int);

	//海洋风速操作槽
	void spinBoxWindVecSlot(int);

	//海洋风向操作槽
	void spinBoxWindDirSlot(int);

	//雾化因子设置
	void spinBoxWuHuaSlot(int);
	void spinBoxWuHuaSlot(double);

	//云图最低距离
	void spinBoxYunTuSlot(int);
	void sliderYunTuSlot(int);

private:
	Ui::CEnvirAttrSetting	ui;

	CSpecialEffectsControl*	m_pEffectControl;

	QTimer*					m_pSimuTimer; //仿真计时器
	QTimer*					m_pSysTimer; //系统计时器
	QDateTime				m_timeSimuTime; //仿真时间
	QList<double>			m_lstSimuVel; //所有可用的仿真速率
	int						m_iListIndex; //当前列表索引
};

#endif // CENVIRATTRSETTING_H
