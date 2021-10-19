#ifndef CHANDLELABEL_H
#define CHANDLELABEL_H

#include <QLineEdit>
#include<QLabel>
#include <QMouseEvent>
//#include <FreeXSysConfig.h>
#include <freexcommontoolui_export.h>


class FREEXCOMMONTOOLUI_EXPORT CHandleLabel : public QLabel
{
	Q_OBJECT

public:
	CHandleLabel(QWidget *parent);
	~CHandleLabel();

signals:
	void editingFinished();

protected:
	void mouseDoubleClickEvent(QMouseEvent *event);

	void hideEvent(QHideEvent *);

private slots:
	void editingFinishedSlot();

private:
	QLineEdit *m_pLineEdit;
};

#endif // CHANDLELABEL_H
