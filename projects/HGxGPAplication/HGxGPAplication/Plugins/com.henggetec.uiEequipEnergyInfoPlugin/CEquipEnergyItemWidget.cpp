//

#include "CEquipEnergyItemWidget.h"
#include <QStyleOption>
#include <QPainter>
#include "ui_CEquipEnergyItemWidget.h"
#include "QFile"
#include "QTimer"
#include "bdpDB\GxRegister.h"
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
//qt5 中文乱码
#pragma execution_character_set("utf-8")
#endif

CEquipEnergyItemWidget::CEquipEnergyItemWidget(QWidget *parent) : QWidget(parent), ui(new Ui::CEquipEnergyItemWidget)
{
    ui->setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_Hover);
	//this->setFixedSize(711,45);

	//setStyleSheet("color:rgb(110,0,0)");
	//样式表，需要注意路径
	QFile mainwndStyleFile(":/qss/resrc/qss/equip_energy_info.css");
	if (mainwndStyleFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString str =  mainwndStyleFile.readAll();
		setStyleSheet(str);
	}
	init();
}

CEquipEnergyItemWidget::~CEquipEnergyItemWidget()
{
    delete ui;
}

void CEquipEnergyItemWidget::paintEvent(QPaintEvent *event)
{
    
}

void CEquipEnergyItemWidget::init()
{
	m_pEnergyCustomBar=new ProgressBar(20);
	QHBoxLayout* layout=new QHBoxLayout(ui->EnergyWidget);
	layout->setContentsMargins(0,4,0,0);
	m_pEnergyCustomBar->setFixedWidth(170);
	m_pEnergyCustomBar->setFixedHeight(22);
	m_pEnergyCustomBar->setProgressValue(0.5);
	layout->addWidget(m_pEnergyCustomBar);

	m_pLoadCustomBar=new ProgressBar(20);
	QHBoxLayout* layout_2=new QHBoxLayout(ui->LoadWidget);
	layout_2->setContentsMargins(0,4,0,0);
	m_pLoadCustomBar->setFixedWidth(170);
	m_pLoadCustomBar->setFixedHeight(22);
	m_pLoadCustomBar->setProgressValue(0.33);
	layout_2->addWidget(m_pLoadCustomBar);
}

void CEquipEnergyItemWidget::setEquipEnergyInfo( SEquipInfo sEquipInfo )
{
	ui->labelIndex->setText(sEquipInfo.strIndex.c_str());
	ui->labelType->setText(sEquipInfo.strType.c_str());
	ui->labelID->setText(sEquipInfo.strID.c_str());


	QImage* pImage =  new QImage;
	pImage->load(QString::fromStdString(HGxDB::CGxRegister::getInstance()->getConfig().getFullPath(sEquipInfo.strIconPath)));
	if (pImage)
	{
		ui->labelLogo->setPixmap(QPixmap::fromImage(*pImage));
	}
	m_pEnergyCustomBar->setProgressValue(sEquipInfo.dEnergyNum);
	m_pLoadCustomBar->setProgressValue(sEquipInfo.dLoadNum);
	ui->label_para1->setText(QString::number(sEquipInfo.dParaA));
	ui->label_para2->setText(QString::number(sEquipInfo.dParaB));
	ui->label_para3->setText(QString::number(sEquipInfo.dParaC));
}


ProgressBar::ProgressBar( int count, QWidget* parent)
	: QWidget(parent),
	m_allCount(count)
{
	init();
	m_progressValue->setStyleSheet("color:rgb(255,122,50) ");
}
void ProgressBar:: setProgressValue(double value)
{

	for ( int  i = 0 ; i < m_allCount ; i++)
	{
		if( i  < value*m_allCount )
		{
			m_listProgress[i]->setStyleSheet("background:rgb(255,122,50) ; max-height:2px; max-width:5px");
		}
		else
		{
			m_listProgress[i]->setStyleSheet("background:rgb(110,110,110) ; max-height:2px; max-width:5px");
		}
	}

	m_progressValue->setText(QString::number(value*100,'f',0)+"%");
}

void ProgressBar::init()
{
	QWidget* wgt=new QWidget(this);
	QHBoxLayout* layout=new QHBoxLayout(wgt);
	QHBoxLayout* layout_2=new QHBoxLayout(wgt);
	layout->setContentsMargins(0,0,0,0);
	layout_2->setSpacing(2);
	layout_2->setContentsMargins(0,0,0,0);

	for(int i=0 ; i < m_allCount ; i++)
	{
		QLabel*  lab = new QLabel;
		lab->setStyleSheet("background:rgb(110,110,110) ; max-height:2px; max-width:5px");
		layout_2->addWidget(lab);
		m_listProgress.push_back(lab);
	}

	m_progressValue=new QLabel;

	layout->addLayout(layout_2);
	layout->addWidget(m_progressValue);
}
