#include "FxTreeSearchTool/CTreeSearchTool.h"

#include <QTreeWidget>
#include <QTreeView>
#include <QStandardItemModel>
#include "Hanz2Piny.h"



struct StPinyin
{
	std::string quanPin;
	std::string firstLetter;
};

enum ESearchMode
{
	// QTreeWidget
	WIDGET = 255,

	// QTreeView + QStandardItemModel
	MODELVIEW = 254
};

class CTreeSearchToolPrivate
{
public:
	CTreeSearchToolPrivate();
	~CTreeSearchToolPrivate();

	// 通过文本内容搜索
	void searchByText(const QString &);

	// 通过拼音搜索
	void searchByPinyin(const QString &);

private:
	// 显示所有父节点
	void showTotalParentItems(QTreeWidgetItem *pCurItem);
	// 显示所有子节点
	void showTotalChildItems(QTreeWidgetItem *pCurItem);
	// 通过文本搜索子节点
	void searchChildByText(QTreeWidgetItem *pCurItem, const QString &);
	// 通过拼音搜索子节点
	void searchChildByPinyin(QTreeWidgetItem *pCurItem, const QString &);

	// 显示所有父节点
	void showTotalParentItems(QStandardItem *pCurItem);
	// 显示所有子节点
	void showTotalChildItems(QStandardItem *pCurItem);
	// 通过文本搜索子节点
	void searchChildByText(QStandardItem *pCurItem, const QString &);
	// 通过拼音搜索子节点
	void searchChildByPinyin(QStandardItem *pCurItem, const QString &);

	// 拼接拼音(每个汉字可能对应多个拼音，因为有多音字)
	void jointPinyin(const std::vector<std::vector<std::string>> &pinyinsOfEachCharacter, std::vector<StPinyin> &jointPinyin, int level = 0);

public:
	QTreeWidget *m_pSearchTreeWgt;

	QTreeView *m_pSearchTreeView;
	QStandardItemModel *m_pModel;

	Qt::CaseSensitivity m_cs;		// 大小写是否敏感
	bool m_bAutoExpand;				// 是否自动展开

	ESearchMode		m_searchMode;

	Hanz2Piny m_hanzi2Py;
	std::vector<int> m_vecIndexs;
};

CTreeSearchToolPrivate::CTreeSearchToolPrivate()
	: m_pSearchTreeWgt(nullptr)
	, m_pSearchTreeView(nullptr)
	, m_pModel(nullptr)
	, m_cs(Qt::CaseInsensitive)
	, m_bAutoExpand(false)
	, m_searchMode(WIDGET)
{

}

CTreeSearchToolPrivate::~CTreeSearchToolPrivate()
{

}

void CTreeSearchToolPrivate::searchByText(const QString &strKeyword)
{
	if (m_searchMode == WIDGET)
	{
		int iTopLevelItemCount = m_pSearchTreeWgt->topLevelItemCount();
		if (strKeyword.isEmpty())
		{
			for (int i = 0; i < iTopLevelItemCount; ++i)
			{
				QTreeWidgetItem *pCurItem = m_pSearchTreeWgt->topLevelItem(i);
				pCurItem->setHidden(false);
				showTotalChildItems(pCurItem);
			}
			return;
		}
		for (int i = 0; i < iTopLevelItemCount; ++i)
		{
			QTreeWidgetItem *pCurItem = m_pSearchTreeWgt->topLevelItem(i);
			if (pCurItem->text(0).contains(strKeyword, m_cs))
			{
				pCurItem->setHidden(false);
				showTotalChildItems(pCurItem);
			}
			else
			{
				pCurItem->setHidden(true);
				searchChildByText(pCurItem, strKeyword);
			}
		}
	}
	else if (m_searchMode == MODELVIEW)
	{
		int iRootCount = m_pModel->rowCount();
		if (strKeyword.isEmpty())
		{
			for (int i = 0; i < iRootCount; ++i)
			{
				QStandardItem *pCurItem = m_pModel->item(i);
				m_pSearchTreeView->setRowHidden(pCurItem->row(), m_pModel->indexFromItem(m_pModel->invisibleRootItem()), false);
				showTotalChildItems(pCurItem);
			}
			return;
		}
		for (int i = 0; i < iRootCount; ++i)
		{
			QStandardItem *pCurItem = m_pModel->item(i);
			if (pCurItem->text().contains(strKeyword, m_cs))
			{
				QStandardItem *pCurItem = m_pModel->item(i);
				m_pSearchTreeView->setRowHidden(pCurItem->row(), m_pModel->indexFromItem(m_pModel->invisibleRootItem()), false);
				showTotalChildItems(pCurItem);
			}
			else
			{
				m_pSearchTreeView->setRowHidden(pCurItem->row(), m_pModel->indexFromItem(m_pModel->invisibleRootItem()), true);
				searchChildByText(pCurItem, strKeyword);
			}
		}
	}
	
}

void CTreeSearchToolPrivate::searchByPinyin(const QString &strKeyword)
{
	if (m_searchMode == WIDGET)
	{
		// 将每个节点从中文转为拼音，然后进行判断
		int iTopLevelItemCount = m_pSearchTreeWgt->topLevelItemCount();
		if (strKeyword.isEmpty())
		{
			for (int i = 0; i < iTopLevelItemCount; ++i)
			{
				QTreeWidgetItem *pCurItem = m_pSearchTreeWgt->topLevelItem(i);
				pCurItem->setHidden(false);
				showTotalChildItems(pCurItem);
			}
			return;
		}
		for (int i = 0; i < iTopLevelItemCount; ++i)
		{
			QTreeWidgetItem *pCurItem = m_pSearchTreeWgt->topLevelItem(i);

			std::vector<std::vector<std::string>> vecEachWordPinyins = m_hanzi2Py.toPinyinFromUtf8(pCurItem->text(0).toUtf8().data());
			m_vecIndexs.clear();
			m_vecIndexs.resize(vecEachWordPinyins.size());
			std::vector<StPinyin> vecJointPinyin;
			jointPinyin(vecEachWordPinyins, vecJointPinyin);
			bool bFind = false;
			for (int j = 0; j < vecJointPinyin.size(); ++j)
			{
				if (QString(vecJointPinyin[j].quanPin.c_str()).contains(strKeyword, m_cs)
					|| QString(vecJointPinyin[j].firstLetter.c_str()).contains(strKeyword, m_cs))
				{
					bFind = true;
					pCurItem->setHidden(false);
					showTotalChildItems(pCurItem);
					break;
				}
			}

			if (!bFind)
			{
				pCurItem->setHidden(true);
				searchChildByPinyin(pCurItem, strKeyword);
			}
		}
	}
	else if (m_searchMode == MODELVIEW)
	{
		// 将每个节点从中文转为拼音，然后进行判断
		int iRootCount = m_pModel->rowCount();
		if (strKeyword.isEmpty())
		{
			for (int i = 0; i < iRootCount; ++i)
			{
				QStandardItem *pCurItem = m_pModel->item(i);
				m_pSearchTreeView->setRowHidden(pCurItem->row(), m_pModel->indexFromItem(m_pModel->invisibleRootItem()), false);
				showTotalChildItems(pCurItem);
			}
			return;
		}
		for (int i = 0; i < iRootCount; ++i)
		{
			QStandardItem *pCurItem = m_pModel->item(i);

			std::vector<std::vector<std::string>> vecEachWordPinyins = m_hanzi2Py.toPinyinFromUtf8(pCurItem->text().toUtf8().data());
			m_vecIndexs.clear();
			m_vecIndexs.resize(vecEachWordPinyins.size());
			std::vector<StPinyin> vecJointPinyin;
			jointPinyin(vecEachWordPinyins, vecJointPinyin);
			bool bFind = false;
			for (int j = 0; j < vecJointPinyin.size(); ++j)
			{
				if (QString(vecJointPinyin[j].quanPin.c_str()).contains(strKeyword, m_cs)
					|| QString(vecJointPinyin[j].firstLetter.c_str()).contains(strKeyword, m_cs))
				{
					bFind = true;
					m_pSearchTreeView->setRowHidden(pCurItem->row(), m_pModel->indexFromItem(m_pModel->invisibleRootItem()), false);
					showTotalChildItems(pCurItem);
					break;
				}
			}

			if (!bFind)
			{
				m_pSearchTreeView->setRowHidden(pCurItem->row(), m_pModel->indexFromItem(m_pModel->invisibleRootItem()), true);
				searchChildByPinyin(pCurItem, strKeyword);
			}
		}
	}
	
}

void CTreeSearchToolPrivate::showTotalParentItems(QTreeWidgetItem *pCurItem)
{
	QTreeWidgetItem *pParentItem = pCurItem->parent();
	if (pParentItem != nullptr)
	{
		pParentItem->setHidden(false);
		showTotalParentItems(pParentItem);
		if (m_bAutoExpand)
		{
			m_pSearchTreeWgt->expandItem(pParentItem);
		}
	}
}

void CTreeSearchToolPrivate::showTotalParentItems(QStandardItem *pCurItem)
{
	QStandardItem *pParentItem = pCurItem->parent();
	if (pParentItem != nullptr)
	{
		if (pParentItem->parent())
		{
			m_pSearchTreeView->setRowHidden(pParentItem->row(), m_pModel->indexFromItem(pParentItem->parent()), false);
		}
		else
		{
			m_pSearchTreeView->setRowHidden(pParentItem->row(), m_pModel->indexFromItem(m_pModel->invisibleRootItem()), false);
		}
		showTotalParentItems(pParentItem);
		if (m_bAutoExpand)
		{
			m_pSearchTreeView->expand(pParentItem->index());
		}
	}
}

void CTreeSearchToolPrivate::showTotalChildItems(QTreeWidgetItem *pCurItem)
{
	int iChildCount = pCurItem->childCount();
	for (int i = 0; i < iChildCount; ++i)
	{
		QTreeWidgetItem *pChildItem = pCurItem->child(i);
		pChildItem->setHidden(false);
		showTotalChildItems(pChildItem);
	}
}

void CTreeSearchToolPrivate::showTotalChildItems(QStandardItem *pCurItem)
{
	int iChildCount = pCurItem->rowCount();
	for (int i = 0; i < iChildCount; ++i)
	{
		QStandardItem *pChildItem = pCurItem->child(i);
		QString strText = pChildItem->text();
		m_pSearchTreeView->setRowHidden(pChildItem->row(), m_pModel->indexFromItem(pCurItem), false);
		showTotalChildItems(pChildItem);
	}
}

void CTreeSearchToolPrivate::searchChildByText(QTreeWidgetItem *pCurItem, const QString &strKeyword)
{
	int iRootCount = pCurItem->childCount();
	for (int i = 0; i < iRootCount; ++i)
	{
		QTreeWidgetItem *pCurChildItem = pCurItem->child(i);
		QString strCurChildItem = pCurChildItem->text(0);
		//如果输入的关键字匹配当前节点，则显示当前节点及其所有子节点
		if (strCurChildItem.contains(strKeyword, m_cs))
		{
			pCurChildItem->setHidden(false);
			showTotalParentItems(pCurChildItem);
			showTotalChildItems(pCurChildItem);
			continue;
		}
		//否则继续在其子节点中搜索
		else
		{
			pCurChildItem->setHidden(true);
			searchChildByText(pCurChildItem, strKeyword);
		}
	}
}

void CTreeSearchToolPrivate::searchChildByText(QStandardItem *pCurItem, const QString &strKeyword)
{
	int iRootCount = pCurItem->rowCount();
	for (int i = 0; i < iRootCount; ++i)
	{
		QStandardItem *pCurChildItem = pCurItem->child(i);
		QString strCurChildItem = pCurChildItem->text();
		//如果输入的关键字匹配当前节点，则显示当前节点及其所有子节点
		if (strCurChildItem.contains(strKeyword, m_cs))
		{
			m_pSearchTreeView->setRowHidden(pCurChildItem->row(), m_pModel->indexFromItem(pCurItem), false);
			showTotalParentItems(pCurChildItem);
			showTotalChildItems(pCurChildItem);
			continue;
		}
		//否则继续在其子节点中搜索
		else
		{
			m_pSearchTreeView->setRowHidden(pCurChildItem->row(), m_pModel->indexFromItem(pCurItem), true);
			searchChildByText(pCurChildItem, strKeyword);
		}
	}
}

void CTreeSearchToolPrivate::searchChildByPinyin(QTreeWidgetItem *pCurItem, const QString &strKeyword)
{
	int iRootCount = pCurItem->childCount();
	for (int i = 0; i < iRootCount; ++i)
	{
		QTreeWidgetItem *pCurChildItem = pCurItem->child(i);
		QString strCurChildItem = pCurChildItem->text(0);

		std::vector<std::vector<std::string>> vec = m_hanzi2Py.toPinyinFromUtf8(strCurChildItem.toUtf8().data());
		m_vecIndexs.clear();
		m_vecIndexs.resize(vec.size());
		std::vector<StPinyin> vecJointPinyin;
		jointPinyin(vec, vecJointPinyin);
		bool bFind = false;
		for (int j = 0; j < vecJointPinyin.size(); ++j)
		{
			if (QString(vecJointPinyin[j].quanPin.c_str()).contains(strKeyword, m_cs)
				|| QString(vecJointPinyin[j].firstLetter.c_str()).contains(strKeyword, m_cs))
			{
				bFind = true;
				pCurChildItem->setHidden(false);
				showTotalParentItems(pCurChildItem);
				showTotalChildItems(pCurChildItem);
				break;
			}
		}

		if (!bFind)
		{
			pCurChildItem->setHidden(true);
			searchChildByPinyin(pCurChildItem, strKeyword);
		}
	}
}

void CTreeSearchToolPrivate::searchChildByPinyin(QStandardItem *pCurItem, const QString &strKeyword)
{
	int iRootCount = pCurItem->rowCount();
	for (int i = 0; i < iRootCount; ++i)
	{
		QStandardItem *pCurChildItem = pCurItem->child(i);
		QString strCurChildItem = pCurChildItem->text();

		std::vector<std::vector<std::string>> vec = m_hanzi2Py.toPinyinFromUtf8(strCurChildItem.toUtf8().data());
		m_vecIndexs.clear();
		m_vecIndexs.resize(vec.size());
		std::vector<StPinyin> vecJointPinyin;
		jointPinyin(vec, vecJointPinyin);
		bool bFind = false;
		for (int j = 0; j < vecJointPinyin.size(); ++j)
		{
			if (QString(vecJointPinyin[j].quanPin.c_str()).contains(strKeyword, m_cs)
				|| QString(vecJointPinyin[j].firstLetter.c_str()).contains(strKeyword, m_cs))
			{
				bFind = true;
				m_pSearchTreeView->setRowHidden(pCurChildItem->row(), m_pModel->indexFromItem(pCurItem), false);
				showTotalParentItems(pCurChildItem);
				showTotalChildItems(pCurChildItem);
				break;
			}
		}

		if (!bFind)
		{
			m_pSearchTreeView->setRowHidden(pCurChildItem->row(), m_pModel->indexFromItem(pCurItem), true);
			searchChildByPinyin(pCurChildItem, strKeyword);
		}
	}
}

void CTreeSearchToolPrivate::jointPinyin(const std::vector<std::vector<std::string>> &vecPinyinsOfEachCharacter, std::vector<StPinyin> &vecJointPinyin, int iLevel)
{
	if (iLevel < vecPinyinsOfEachCharacter.size())
	{
		for (int i = 0; i < vecPinyinsOfEachCharacter[iLevel].size(); ++i)
		{
			m_vecIndexs[iLevel] = i;
			jointPinyin(vecPinyinsOfEachCharacter, vecJointPinyin, iLevel + 1);
		}
	}
	else
	{
		StPinyin stPinyin;
		std::string strTotalCharacterPinyin;
		for (int i = 0; i < vecPinyinsOfEachCharacter.size(); ++i)
		{
			stPinyin.quanPin.append(vecPinyinsOfEachCharacter[i][m_vecIndexs[i]]);
			stPinyin.firstLetter += vecPinyinsOfEachCharacter[i][m_vecIndexs[i]][0];
		}
		vecJointPinyin.push_back(stPinyin);
	}

}

QScopedPointer<CTreeSearchToolPrivate> CTreeSearchTool::d_ptr(new CTreeSearchToolPrivate);

CTreeSearchTool::CTreeSearchTool(QObject *parent)
	: QObject(parent)
{

}

CTreeSearchTool::~CTreeSearchTool()
{
}

void CTreeSearchTool::setMode()
{

}

void CTreeSearchTool::setSearchRole()
{

}

void CTreeSearchTool::setSearchCaseSensitivity(Qt::CaseSensitivity cs)
{
	d_ptr->m_cs = cs;
}

void CTreeSearchTool::setAutoExpand(bool bExpand)
{
	d_ptr->m_bAutoExpand = bExpand;
}

void CTreeSearchTool::search(const QString &strKeyword, QTreeWidget *pTreeWidget)
{
	if (d_ptr->m_pSearchTreeWgt != pTreeWidget)
	{
		d_ptr->m_pSearchTreeWgt = pTreeWidget;
		if (d_ptr->m_pSearchTreeWgt == nullptr)
		{
			return;
		}
	}
	d_ptr->m_searchMode = WIDGET;
	QRegExp rx("([a-z]?[0-9]?)*");
	QRegExpValidator v(rx);
	int pos = 0;
	// 如果关键字全部是字母或数字，则按照拼音搜索
    QString tempKeyword =strKeyword.trimmed();
    if (v.validate(tempKeyword, pos) == QValidator::Acceptable)
	{
		d_ptr->searchByPinyin(strKeyword);
	}
	// 非拼音，直接搜文字
	else
	{
		d_ptr->searchByText(strKeyword);
	}
}

void CTreeSearchTool::search(const QString &strKeyword /*= QString()*/, QTreeView *treeView /*= nullptr*/)
{
	if (d_ptr->m_pSearchTreeView != treeView)
	{
		d_ptr->m_pSearchTreeView = treeView;
		d_ptr->m_pModel = qobject_cast<QStandardItemModel*>(treeView->model());
		if (d_ptr->m_pSearchTreeView == nullptr || d_ptr->m_pModel == nullptr)
		{
			return;
		}
	}
	d_ptr->m_searchMode = MODELVIEW;
	QRegExp rx("([a-z]?[0-9]?)*");
	QRegExpValidator v(rx);
	int pos = 0;
	// 如果关键字全部是字母或数字，则按照拼音搜索
    QString tempKeyword = strKeyword.trimmed();
    if (v.validate(tempKeyword, pos) == QValidator::Acceptable)
	{
		d_ptr->searchByPinyin(strKeyword);
	}
	// 非拼音，直接搜文字
	else
	{
		d_ptr->searchByText(strKeyword);
	}
}
