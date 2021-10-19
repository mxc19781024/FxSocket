#include "acmiparse.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	AcmiParse w;
	w.show();
	return a.exec();
}
