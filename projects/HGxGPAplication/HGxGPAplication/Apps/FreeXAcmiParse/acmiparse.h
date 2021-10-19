#ifndef ACMIPARSE_H
#define ACMIPARSE_H

#include <QtWidgets/QMainWindow>

#include "ACMIDataParse.h"

class AcmiParse : public QMainWindow
{
	Q_OBJECT

public:
	AcmiParse(QWidget *parent = 0);
	~AcmiParse();

private:
//	Ui::AcmiParseClass ui;

	ACMIDataParse m_acmiParse;
};

#endif // ACMIPARSE_H
