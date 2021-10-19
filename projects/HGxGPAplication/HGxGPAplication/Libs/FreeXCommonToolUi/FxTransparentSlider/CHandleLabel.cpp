#include "CHandleLabel.h"
#include <QRegExpValidator>



CHandleLabel::CHandleLabel(QWidget *parent)
	: QLabel(parent),
	m_pLineEdit(NULL)
{
	m_pLineEdit  = new QLineEdit(this);
	QRegExp  rx("[0-9][0-9]{0,1}|100");
	QRegExpValidator *pReg = new QRegExpValidator(rx, this);
	setFocusPolicy(Qt::NoFocus);
	m_pLineEdit->setValidator(pReg);
	m_pLineEdit->move(0,0);
	m_pLineEdit->hide();
	//m_pLineEdit->setStyleSheet("color:#3b7cff; border:1px ; background:red; font-size:10px; border-radius:4px;");

	connect(m_pLineEdit, SIGNAL(editingFinished()), this, SLOT(editingFinishedSlot()));
}

CHandleLabel::~CHandleLabel()
{

}


void CHandleLabel::mouseDoubleClickEvent( QMouseEvent *event )
{
   if (m_pLineEdit != NULL)
   {
	   QString strText = text();
	   strText.chop(1);
	   m_pLineEdit->setText(strText);
	   m_pLineEdit->setFixedSize(rect().width(), rect().height());
	   m_pLineEdit->show();
	   m_pLineEdit->setEnabled(true);
	   m_pLineEdit->setFocus();
   }

    QLabel::mouseDoubleClickEvent(event);
}
  
void CHandleLabel::hideEvent( QHideEvent * )
{
	if (!m_pLineEdit->isHidden())
	{
		m_pLineEdit->hide();
	}
}

void CHandleLabel::editingFinishedSlot()
{
	m_pLineEdit->hide();
	this->setText(m_pLineEdit->text() + "%");
	emit editingFinished();    
}
