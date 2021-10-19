/**************************************************************************************************
* @file		: CBasicColorArea.cpp
* @note		: 基本颜色区域实现文件
* @author	: kzm
* @data		: 2018-8
**************************************************************************************************/


#include "CBasicColorArea.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include "CSingleColorItem.h"


CBasicColorArea::CBasicColorArea(QWidget *parent)
	: QWidget(parent)
{
	InitColorItems();
}

CBasicColorArea::~CBasicColorArea()
{

}

void CBasicColorArea::InitColorItems()
{
	// TODO 
	CSingleColorItem *pItem11 = new CSingleColorItem(QColor("#fe686a"));
	CSingleColorItem *pItem12 = new CSingleColorItem(QColor("#ff9734"));
	CSingleColorItem *pItem13 = new CSingleColorItem(QColor("#ffc700"));
	CSingleColorItem *pItem14 = new CSingleColorItem(QColor("#609e4b"));
	CSingleColorItem *pItem15 = new CSingleColorItem(QColor("#8fbc21"));
	CSingleColorItem *pItem16 = new CSingleColorItem(QColor("#c3dd00"));
	CSingleColorItem *pItem17 = new CSingleColorItem(QColor("#2476fe"));
	CSingleColorItem *pItem18 = new CSingleColorItem(QColor("#35a0ff"));
	CSingleColorItem *pItem19 = new CSingleColorItem(QColor("#48cbff"));
	QHBoxLayout *pLayout1 = new QHBoxLayout;
	pLayout1->setMargin(0);
	pLayout1->setSpacing(5);
	pLayout1->addWidget(pItem11);
	pLayout1->addWidget(pItem12);
	pLayout1->addWidget(pItem13);
	pLayout1->addWidget(pItem14);
	pLayout1->addWidget(pItem15);
	pLayout1->addWidget(pItem16);
	pLayout1->addWidget(pItem17);
	pLayout1->addWidget(pItem18);
	pLayout1->addWidget(pItem19);

	CSingleColorItem *pItem21 = new CSingleColorItem(QColor("#ff4b4b"));
	CSingleColorItem *pItem22 = new CSingleColorItem(QColor("#ff6924"));
	CSingleColorItem *pItem23 = new CSingleColorItem(QColor("#ff8c01"));
	CSingleColorItem *pItem24 = new CSingleColorItem(QColor("#3e8829"));
	CSingleColorItem *pItem25 = new CSingleColorItem(QColor("#5cae0b"));
	CSingleColorItem *pItem26 = new CSingleColorItem(QColor("#7cd501"));
	CSingleColorItem *pItem27 = new CSingleColorItem(QColor("#005bfe"));
	CSingleColorItem *pItem28 = new CSingleColorItem(QColor("#0182ff"));
	CSingleColorItem *pItem29 = new CSingleColorItem(QColor("#00affe"));
	QHBoxLayout *pLayout2 = new QHBoxLayout;
	pLayout2->setMargin(0);
	pLayout2->setSpacing(5);
	pLayout2->addWidget(pItem21);
	pLayout2->addWidget(pItem22);
	pLayout2->addWidget(pItem23);
	pLayout2->addWidget(pItem24);
	pLayout2->addWidget(pItem25);
	pLayout2->addWidget(pItem26);
	pLayout2->addWidget(pItem27);
	pLayout2->addWidget(pItem28);
	pLayout2->addWidget(pItem29);

	CSingleColorItem *pItem31 = new CSingleColorItem(QColor("#c13639"));
	CSingleColorItem *pItem32 = new CSingleColorItem(QColor("#db4f1c"));
	CSingleColorItem *pItem33 = new CSingleColorItem(QColor("#f16702"));
	CSingleColorItem *pItem34 = new CSingleColorItem(QColor("#326721"));
	CSingleColorItem *pItem35 = new CSingleColorItem(QColor("#489008"));
	CSingleColorItem *pItem36 = new CSingleColorItem(QColor("#5fbb00"));
	CSingleColorItem *pItem37 = new CSingleColorItem(QColor("#0042c4"));
	CSingleColorItem *pItem38 = new CSingleColorItem(QColor("#0065ab"));
	CSingleColorItem *pItem39 = new CSingleColorItem(QColor("#018793"));
	QHBoxLayout *pLayout3 = new QHBoxLayout;
	pLayout3->setMargin(0);
	pLayout3->setSpacing(5);
	pLayout3->addWidget(pItem31);
	pLayout3->addWidget(pItem32);
	pLayout3->addWidget(pItem33);
	pLayout3->addWidget(pItem34);
	pLayout3->addWidget(pItem35);
	pLayout3->addWidget(pItem36);
	pLayout3->addWidget(pItem37);
	pLayout3->addWidget(pItem38);
	pLayout3->addWidget(pItem39);

	CSingleColorItem *pItem41 = new CSingleColorItem(QColor("#000000"));
	CSingleColorItem *pItem42 = new CSingleColorItem(QColor("#2c2c2c"));
	CSingleColorItem *pItem43 = new CSingleColorItem(QColor("#3f3f3f"));
	CSingleColorItem *pItem44 = new CSingleColorItem(QColor("#606060"));
	CSingleColorItem *pItem45 = new CSingleColorItem(QColor("#7d7d7d"));
	CSingleColorItem *pItem46 = new CSingleColorItem(QColor("#919191"));
	CSingleColorItem *pItem47 = new CSingleColorItem(QColor("#a6a6a6"));
	CSingleColorItem *pItem48 = new CSingleColorItem(QColor("#d2d2d2"));
	CSingleColorItem *pItem49 = new CSingleColorItem(QColor("#ffffff"));
	QHBoxLayout *pLayout4 = new QHBoxLayout;
	pLayout4->setMargin(0);
	pLayout4->setSpacing(5);
	pLayout4->addWidget(pItem41);
	pLayout4->addWidget(pItem42);
	pLayout4->addWidget(pItem43);
	pLayout4->addWidget(pItem44);
	pLayout4->addWidget(pItem45);
	pLayout4->addWidget(pItem46);
	pLayout4->addWidget(pItem47);
	pLayout4->addWidget(pItem48);
	pLayout4->addWidget(pItem49);

	QVBoxLayout *pVLayout = new QVBoxLayout(this);
	pVLayout->setMargin(0);
	pVLayout->setSpacing(10);
	pVLayout->addLayout(pLayout1);
	pVLayout->addLayout(pLayout2);
	pVLayout->addLayout(pLayout3);
	pVLayout->addLayout(pLayout4);

	connect(pItem11, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem12, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem13, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem14, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem15, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem16, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem17, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem18, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem19, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));

	connect(pItem21, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem22, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem23, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem24, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem25, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem26, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem27, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem28, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem29, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));

	connect(pItem31, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem32, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem33, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem34, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem35, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem36, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem37, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem38, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem39, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));

	connect(pItem41, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem42, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem43, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem44, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem45, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem46, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem47, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem48, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
	connect(pItem49, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));

	
}
