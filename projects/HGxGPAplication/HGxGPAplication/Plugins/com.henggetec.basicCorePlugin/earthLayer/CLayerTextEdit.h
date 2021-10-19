#ifndef CLAYER_TEXT_EDIT_H
#define CLAYER_TEXT_EDIT_H

#include <QTextEdit>

class CLayerTextEdit : public QTextEdit
{
	Q_OBJECT

public:
	CLayerTextEdit(QWidget *parent = nullptr, int maxLength = 100);
	~CLayerTextEdit();

private slots:
	void textChangedSlot();

private:
	int m_iMaxLength;
};

#endif
