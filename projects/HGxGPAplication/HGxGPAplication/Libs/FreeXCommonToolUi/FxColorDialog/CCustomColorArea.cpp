/**************************************************************************************************
* @file		: CCustomColorArea.cpp
* @note		: 自定义颜色区域实现文件
* @author	: kzm
* @data		: 2018-8
**************************************************************************************************/

#include "CCustomColorArea.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMap>
#include "CCustomColorItem.h"


class CCustomColorAreaPrivate
{
	Q_DECLARE_PUBLIC(CCustomColorArea)
public:
	explicit CCustomColorAreaPrivate(CCustomColorArea *parent);
	void initColorItems();							// 初始化所有自定义颜色项

public:
	CCustomColorArea * const q_ptr;					// q指针
	QMap<int, CCustomColorItem *> m_mapIndexToItem;	// 所以自定义颜色项
	int m_iCurIndex;								// 当前编号
};

CCustomColorAreaPrivate::CCustomColorAreaPrivate(CCustomColorArea *parent)
	: q_ptr(parent)
	, m_iCurIndex(0)
{
	initColorItems();
}


void CCustomColorAreaPrivate::initColorItems()
{
	Q_Q(CCustomColorArea);

	QHBoxLayout *pLayout1 = new QHBoxLayout(q);
	pLayout1->setMargin(0);
	pLayout1->setSpacing(5);
	for (int i = 0; i < 9; ++i)
	{
		CCustomColorItem *pColorItem = new CCustomColorItem(QColor(255, 255, 255));
		pLayout1->addWidget(pColorItem);
		m_mapIndexToItem.insert(i, pColorItem);
		QObject::connect(pColorItem, SIGNAL(ItemClickedSignal(const QColor &)), q, SIGNAL(ColorItemSelcSignal(const QColor &)));
	}
}

CCustomColorArea::CCustomColorArea(QWidget *parent)
	: QWidget(parent)
	, d_ptr(new CCustomColorAreaPrivate(this))
{
}

CCustomColorArea::~CCustomColorArea()
{
	
}

void CCustomColorArea::AddToCustomColor(const QColor &c)
{
	Q_D(CCustomColorArea);
	int iIndex = d->m_iCurIndex % 9;
	CCustomColorItem *pCurItem = d->m_mapIndexToItem[iIndex];
	pCurItem->SetColor(c);
	d->m_iCurIndex++;
}
