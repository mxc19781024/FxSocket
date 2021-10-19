#include "FxEditingDelegate/CEditingDelegate.h"

#include <QLineEdit>


CEditingDelegate::CEditingDelegate(QObject *parent, int iMaxLength)
	: QStyledItemDelegate(parent)
	, m_iMaxLength(iMaxLength)
{

}

CEditingDelegate::~CEditingDelegate()
{

}

QWidget *CEditingDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const 
{
	QLineEdit *pLineEdit = new QLineEdit(parent);
	pLineEdit->setContextMenuPolicy(Qt::PreventContextMenu);
	// 限制不能输入空格和字数
	QRegExp rx(QString("\\S{0,").append(QString::number(m_iMaxLength)).append("}"));
	pLineEdit->setValidator(new QRegExpValidator(rx, pLineEdit));
	return pLineEdit;
}

void CEditingDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QStyledItemDelegate::setModelData(editor, model, index);
	if (index.column() == 0)
	{
		emit editingFinishedSignal(index);
	}
}

void CEditingDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const 
{
	editor->setGeometry(option.rect);
}
