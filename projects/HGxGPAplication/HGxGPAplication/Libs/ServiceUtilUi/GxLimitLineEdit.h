#ifndef GXLIMITLINEEDIT_H
#define GXLIMITLINEEDIT_H

#include <QLineEdit>

class CGxLimitLineEdit : public QLineEdit
{
	Q_OBJECT
	Q_PROPERTY(QString errorMsg READ errorMsg WRITE setErrorMsg USER true)

public:
	CGxLimitLineEdit(QWidget *parent = 0);
	~CGxLimitLineEdit();

	void setErrorMsg(const QString &strErrorMsg);
	QString errorMsg(){ return m_strErrorMsg; }

private slots:
	void editingFinshedSlot();

private:
	QString				m_strErrorMsg;

};

#endif // GXLIMITLINEEDIT_H