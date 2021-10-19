//
// 
//

#include "AlarmInfoItem.h"
#include "ui_AlarmInfoItem.h"

#include <QTime>
#include <QMessageBox>
#include <QMouseEvent>
#include <QTimer>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>
#include <string>


CAlarmInfoItem::CAlarmInfoItem(QWidget *parent) : QWidget(parent), ui(new Ui::AlarmInfoItem)
{
	ui->setupUi(this);
	m_level = LEVEL_ORDINARY; 

	QFont timefont("Microsoft YaHei"); //第一个属性是字体（微软雅黑），第二个是大小，第三个是加粗（权重是75）
	timefont.setPixelSize(12);
	ui->TimeLabel->setFont(timefont);  //字体颜色
	ui->TimeLabel->setStyleSheet("QLabel#TimeLabel{color:#b4b0b0;}");
	ui->DataLabel->setStyleSheet("QLabel#DataLabel{color:#b4b0b0;}");

	QFont info_font("Microsoft YaHei"); //第一个属性是字体（微软雅黑），第二个是大小，第三个是加粗（权重是75）
	info_font.setPixelSize(12);
	ui->InfoLabel->setFont(info_font); //字体颜色
}

void CAlarmInfoItem::setData( QDate t)
{
	ui->DataLabel->setText(t.toString("yyyy/MM/dd"));
	update();
}

void CAlarmInfoItem::setTime(QTime t)
{
	std::string strTime = t.toString("hh:mm:ss:zzz").toStdString();
	std::string::size_type ipos = strTime.find_last_of(':');
	std::string strSubTime = strTime.substr(0,ipos);
	ui->TimeLabel->setText(strSubTime.c_str());
	update();
}

void CAlarmInfoItem::setInfo(QString info)
{
	ui->InfoLabel->setText(info);
	update();
}

void CAlarmInfoItem::setLevel(ELevel level)
{
	m_level = level;
	if (m_level == LEVEL_ORDINARY)
	{
		ui->InfoLabel->setStyleSheet("QLabel#InfoLabel{color:#b4b0b0;}");  //字体颜色
		ui->TypeLabel->setText(QString::fromLocal8Bit("常规消息"));
		ui->TypeLabel->setStyleSheet("QLabel#TypeLabel{color:#b4b0b0;}");
		ui->alarmInfoItemframe->setStyleSheet("QFrame#alarmInfoItemframe{background-image: url(:/img/resrc/images/heisi.png);\
											  border : 1px;} ");
	}
	if (m_level == LEVEL_IMPORTANCE)
	{
		ui->InfoLabel->setStyleSheet("QLabel#InfoLabel{color:#feffff;}");  //字体颜色
		ui->TypeLabel->setText(QString::fromLocal8Bit("重要消息"));
		ui->TypeLabel->setStyleSheet("QLabel#TypeLabel{color:#e85e39;}");
		ui->alarmInfoItemframe->setStyleSheet("QFrame#alarmInfoItemframe{background-image: url(:/img/resrc/images/hongse.png);\
											  border : 1px;} ");
	}
	update();
}

ELevel CAlarmInfoItem::getLevel() const
{
	return m_level;
}

CAlarmInfoItem::~CAlarmInfoItem()
{
	delete ui;
}