//
// Created by HG on 2020/3/26.
//

#ifndef EQUIPMENTATTIBUTES_H
#define EQUIPMENTATTIBUTES_H

#include <QWidget>
#include <QLabel>
namespace Ui
{
class EquipmentAttibutes;
} // namespace Ui

class ProgressBar :public QWidget
{
	Q_OBJECT
public:
	//count 进度条总个数
	ProgressBar(int count,QWidget* parent=0);
public:
	void init();
	//设置进度
	void setProgressValue(double value);
	//设置个数
	void setProgressCount(int value);

private:
	//当前进度
	int   m_curValue;
	//总进度个数
	int   m_allCount;
	QList<QLabel*> m_listProgress;
};

class CEquipmentAttibutes : public QWidget
{
Q_OBJECT
public:
    explicit CEquipmentAttibutes(QWidget *parent = nullptr);

    ~CEquipmentAttibutes() ;

    void setType(QString type);
	/**
	* @brief 设置装备携带弹药个数
	* @param 参数 nNumber  个数
	* @return 返回值  无
	*/
	void setAmmunitionNumber(int nNumber);
	/**
	* @brief 设置装备最大速度
	* @param 参数 dSpeed  速度
	* @return 返回值  无
	*/
	void setMaxSpeed(double dSpeed);
	/**
	* @brief 设置装备最大油量
	* @param 参数 dMaxOil  油量
	* @return 返回值  无
	*/
	void setMaxOil(double dMaxOil);
private:
	void InitProgressBar();
private Q_SLOTS:

	void TimerUpdateSlot();
protected:
    void paintEvent(QPaintEvent *event) ;
private:

    Ui::EquipmentAttibutes *ui;
	ProgressBar*  m_pAmmuBar;
	int           m_pIndex;
	double        m_dSpeed;
	double        m_dMaxOil;
};


#endif //SITUATION_DETAIL_SRC_UI_SITUATIONCLARITY_H
