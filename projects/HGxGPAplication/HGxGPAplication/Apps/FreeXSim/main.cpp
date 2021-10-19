#include "FreeXSim.h"

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
#include <QtGui/QApplication>
#else
#include <QApplication>
#endif

#include <QTextCodec>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
	QTextCodec *pTextCodec = QTextCodec::codecForName("utf-8");
	QTextCodec::setCodecForCStrings(pTextCodec);
	QTextCodec::setCodecForTr(pTextCodec);
	pTextCodec = QTextCodec::codecForName("system");
	QTextCodec::setCodecForLocale(pTextCodec);
#endif


	FreeXSim w;
	w.show();
	return a.exec();
}
