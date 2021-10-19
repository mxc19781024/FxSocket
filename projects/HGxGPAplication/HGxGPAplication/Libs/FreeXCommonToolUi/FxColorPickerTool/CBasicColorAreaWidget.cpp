/**************************************************************************************************
* @file		: CBasicColorAreaWidget.cpp
* @note		: 基本颜色区域实现文件
* @author	: k00026
* @data		: 2018-8
**************************************************************************************************/

#include "CBasicColorAreaWidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include "CSingleColorPickerItem.h"


CBasicColorAreaWidget::CBasicColorAreaWidget(QWidget *parent)
	: QWidget(parent)
{
	InitColorItems();
}

CBasicColorAreaWidget::~CBasicColorAreaWidget()
{

}

void CBasicColorAreaWidget::InitColorItems()
{
	QList<QColor> listColorItems;
	listColorItems
		<< QColor("#fe686a") << QColor("#ff9734") << QColor("#ffc700") << QColor("#609e4b") << QColor("#8fbc21") 
		<< QColor("#c3dd00") << QColor("#2476fe") << QColor("#35a0ff") << QColor("#48cbff")
		<< QColor("#ff4b4b") << QColor("#ff6924") << QColor("#ff8c01") << QColor("#3e8829") << QColor("#5cae0b") 
		<< QColor("#7cd501") << QColor("#005bfe") << QColor("#0182ff") << QColor("#00affe")
		<< QColor("#c13639") << QColor("#db4f1c") << QColor("#f16702") << QColor("#326721") << QColor("#489008")
		<< QColor("#5fbb00") << QColor("#0042c4") << QColor("#0065ab") << QColor("#018793")
		<< QColor("#000000") << QColor("#2c2c2c") << QColor("#3f3f3f") << QColor("#606060") << QColor("#7d7d7d")
		<< QColor("#919191") << QColor("#a6a6a6") << QColor("#d2d2d2") << QColor("#ffffff");

	int iCountPerRow = 9;
	int iRow = listColorItems.size() / iCountPerRow;

	QVBoxLayout *pVLayout = new QVBoxLayout(this);
	pVLayout->setMargin(0);
	pVLayout->setSpacing(3);
	for (int i = 0; i < iRow; ++i)
	{
		QHBoxLayout *pHLayout = new QHBoxLayout;
		pHLayout->setMargin(0);
		pHLayout->setSpacing(3);
		for (int j = 0; j < iCountPerRow; ++j)
		{
			CSingleColorPickerItem *pItem = new CSingleColorPickerItem(listColorItems[iCountPerRow * i + j]);
			pHLayout->addWidget(pItem);
			connect(pItem, SIGNAL(ItemClickedSignal(const QColor &)), this, SIGNAL(ColorItemSelcSignal(const QColor &)));
		}
		pVLayout->addLayout(pHLayout);
	}
}
