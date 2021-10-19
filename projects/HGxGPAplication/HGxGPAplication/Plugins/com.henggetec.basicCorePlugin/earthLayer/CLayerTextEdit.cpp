#include "CLayerTextEdit.h"




CLayerTextEdit::CLayerTextEdit(QWidget *parent, int iMaxLength)
	: QTextEdit(parent)
	, m_iMaxLength(iMaxLength)
{
	connect(this, SIGNAL(textChanged()), this, SLOT(textChangedSlot()));
}

CLayerTextEdit::~CLayerTextEdit()
{
	
}

void CLayerTextEdit::textChangedSlot()
{
	if (this->toPlainText().size() > m_iMaxLength)
	{
		textCursor().deletePreviousChar();
	}
}
