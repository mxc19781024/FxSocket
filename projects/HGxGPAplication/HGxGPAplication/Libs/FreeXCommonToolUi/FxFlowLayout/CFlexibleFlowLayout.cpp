#include <QWidget>

#include "FxFlowLayout/CFlexibleFlowLayout.h"
#include <qmath.h>


CFlexibleFlowLayout::CFlexibleFlowLayout(QWidget *parent, int iHSpacing, int iVSpacing)
	: QLayout(parent)
	, m_iHSpace(iHSpacing)
	, m_iVSpace(iVSpacing)
	, m_iMinimumItemWidth(160)
	, m_bHiddenEnabled(false)
{
}

CFlexibleFlowLayout::CFlexibleFlowLayout(int hSpacing, int vSpacing)
	: m_iHSpace(hSpacing)
	, m_iVSpace(vSpacing)
	, m_iMinimumItemWidth(160)
	, m_bHiddenEnabled(false)
{
}

CFlexibleFlowLayout::~CFlexibleFlowLayout()
{
	QLayoutItem *item;
	while ((item = takeAt(0)))
		delete item;
}

void CFlexibleFlowLayout::setMinimumItemWidth(int iWidth)
{
	m_iMinimumItemWidth = iWidth;
}

void CFlexibleFlowLayout::setHiddenEnabled(bool bEnabled)
{
	m_bHiddenEnabled = bEnabled;
}

void CFlexibleFlowLayout::addItem(QLayoutItem *item)
{
	m_itemList.append(item);
}
//! [3]

//! [4]
int CFlexibleFlowLayout::horizontalSpacing() const
{
	if (m_iHSpace >= 0)
	{
		return m_iHSpace;
	}
	else
	{
		return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
	}
}

int CFlexibleFlowLayout::verticalSpacing() const
{
	if (m_iVSpace >= 0)
	{
		return m_iVSpace;
	}
	else
	{
		return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
	}
}

int CFlexibleFlowLayout::count() const
{
	return m_itemList.size();
}

QLayoutItem *CFlexibleFlowLayout::itemAt(int index) const
{
	return m_itemList.value(index);
}

QLayoutItem *CFlexibleFlowLayout::takeAt(int index)
{
	if (index >= 0 && index < m_itemList.size())
		return m_itemList.takeAt(index);
	else
		return 0;
}

Qt::Orientations CFlexibleFlowLayout::expandingDirections() const
{
	return 0;
}

bool CFlexibleFlowLayout::hasHeightForWidth() const
{
	return true;
}

int CFlexibleFlowLayout::heightForWidth(int width) const
{
	int height = doLayout(QRect(0, 0, width, 0), true);
	return height;
}

void CFlexibleFlowLayout::setGeometry(const QRect &rect)
{
	QLayout::setGeometry(rect);
	doLayout(rect, false);
}

QSize CFlexibleFlowLayout::sizeHint() const
{
	return minimumSize();
}

QSize CFlexibleFlowLayout::minimumSize() const
{
	QSize size;
	QLayoutItem *item;
	foreach(item, m_itemList)
		size = size.expandedTo(item->minimumSize());

	size += QSize(2 * margin(), 2 * margin());
	return size;
}

int CFlexibleFlowLayout::doLayout(const QRect &rect, bool testOnly) const
{
	int iItemCount = m_itemList.size();
	if (iItemCount == 0)
	{
		return 0;
	}

	int iLeft, iTop, iRight, iBottom;
	getContentsMargins(&iLeft, &iTop, &iRight, &iBottom);
	QRect effectiveRect = rect.adjusted(+iLeft, +iTop, -iRight, -iBottom);
	int x = effectiveRect.x();
	int y = effectiveRect.y();
	int lineHeight = 0;

	int iSpaceX = horizontalSpacing();
	int iSpaceY = verticalSpacing();

	int iCountPerRow = 1;
	while (iCountPerRow * m_iMinimumItemWidth + (iCountPerRow - 1) * iSpaceX < effectiveRect.width())
	{
		iCountPerRow++;
	}
	if (--iCountPerRow == 0)
	{
		iCountPerRow = 1;
	}
	int iRealWidth = qCeil((double)(effectiveRect.width() + iSpaceX - iCountPerRow * iSpaceX) / iCountPerRow);

	if (m_bHiddenEnabled)
	{
		QList<QLayoutItem *> listItems;
		for (int i = 0; i < m_itemList.size(); ++i)
		{
			if (m_itemList[i]->widget()->isVisible())
			{
				listItems.append(m_itemList[i]);
			}
		}

		if (listItems.size() == 0)
		{
			return 0;
		}

		for (int i = 0; i < listItems.size(); ++i)
		{
			int iRow = i / iCountPerRow;
			int iCol = i % iCountPerRow;
			int iItemHeight = listItems[i]->sizeHint().height();
			//逻辑错误 l180修改
			/*if (!testOnly)
			{*/
			listItems[i]->setGeometry(QRect(iLeft + iCol * iRealWidth + iCol * iSpaceX, iTop + iRow * iItemHeight + iRow * iSpaceY, iRealWidth, iItemHeight));
			/*}*/
		}
		int iRowCount = qCeil(listItems.size() * 1.0 / iCountPerRow);
		return iTop + iRowCount * listItems.at(0)->sizeHint().height() + (iRowCount - 1) * iSpaceY;
	}

	for (int i = 0; i < m_itemList.size(); ++i)
	{
		int iRow = i / iCountPerRow;
		int iCol = i % iCountPerRow;
		int iItemHeight = m_itemList.at(i)->sizeHint().height();
		if (!testOnly)
		{
			m_itemList.value(i)->setGeometry(QRect(iLeft + iCol * iRealWidth + iCol * iSpaceX, iTop + iRow * iItemHeight + iRow * iSpaceY, iRealWidth, iItemHeight));
		}
	}
	int iRowCount = qCeil(m_itemList.size() * 1.0 / iCountPerRow);
	return iTop + iRowCount * m_itemList.at(0)->sizeHint().height() + (iRowCount - 1) * iSpaceY;
}

int CFlexibleFlowLayout::smartSpacing(QStyle::PixelMetric pm) const
{
	QObject *parent = this->parent();
	if (!parent)
	{
		return -1;
	}
	else if (parent->isWidgetType())
	{
		QWidget *pw = static_cast<QWidget *>(parent);
		return pw->style()->pixelMetric(pm, 0, pw);
	}
	else
	{
		return static_cast<QLayout *>(parent)->spacing();
	}
}
