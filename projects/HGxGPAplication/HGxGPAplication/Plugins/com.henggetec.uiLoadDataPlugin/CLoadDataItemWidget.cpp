#include "CLoadDataItemWidget.h"
#include "ui_CLoadDataItemWidget.h"
#include <QStandardItemModel>
#include <QPixmapCache>
#include <QHBoxLayout>
#include <QGraphicsScene>
#include <QFile>
#include "QLinkedList"
#include "QPainter"
#include "qlistwidget.h"
#include "qlabel.h"
#include <io.h>



#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
//qt5 中文乱码
#pragma execution_character_set("utf-8")
#endif

CLoadDataItemWidget::CLoadDataItemWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CLoadDataItemWidget)
{
	ui->setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_Hover);
	//ui->DescribeText->setFocusPolicy(Qt::NoFocus);
	ui->DescribeText->setAttribute(Qt::WA_TranslucentBackground);
	ui->DescribeText->setStyleSheet("QTextBrowser#DescribeText{border: 0px;color:rgb(255,255,255) }");
	//ui->Type_Image->setStyleSheet("QLabel#Type_Image{border-image: url(:/images/resrc/images/icon_evaluation .png);}");
}

CLoadDataItemWidget::~CLoadDataItemWidget()
{
    delete ui;
}

void CLoadDataItemWidget::setItemInfo( SItemInfo sItemInfo )
{
	ui->DescribeText->setText(QString::fromStdString(sItemInfo.strDescribe));
	ui->StartTimeLabel->setText(QString::fromStdString(sItemInfo.strStartTime));
	ui->EndTimeLabel->setText(QString::fromStdString(sItemInfo.strEndTime));
	ui->NameLabel->setText(QString::fromStdString(sItemInfo.strName));
	ui->TypeLabel->setText(QString::fromStdString(sItemInfo.strType));
	QPixmap pPixmap;
	if (pPixmap.load(QString::fromStdString(sItemInfo.strImage)))
	{
		ui->PictureLabel->setPixmap(pPixmap);
	}
}

