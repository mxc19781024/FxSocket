//

#ifndef EQUIPENERGYITEMWIDGET_H
#define EQUIPENERGYITEMWIDGET_H

#include <QWidget>
#include <string>
#include <QLabel>
#include "QList"
#include "common\UnitEventCommon.h"

namespace Ui
{
class CEquipEnergyItemWidget;
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

private:
	//当前进度
	int   m_curValue;
	//总进度个数
	int   m_allCount;
	QList<QLabel*> m_listProgress;
	QLabel*  m_progressValue;

};

class CEquipEnergyItemWidget : public QWidget
{
Q_OBJECT
public:
    explicit CEquipEnergyItemWidget(QWidget *parent = nullptr);

    ~CEquipEnergyItemWidget() ;

	void setEquipEnergyInfo(SEquipInfo sEquipInfo);
private:
	void init();
private Q_SLOTS:

protected:
    void paintEvent(QPaintEvent *event) ;
private:

    Ui::CEquipEnergyItemWidget *ui;

	//进度条
	ProgressBar*  m_pEnergyCustomBar;
	ProgressBar*  m_pLoadCustomBar;
};


#endif //EQUIPENERGYITEMWIDGET_H
