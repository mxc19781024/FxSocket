#include "LoginUI.h"
#include <QApplication>
#include <QTextCodec>
#include <QProcess>



int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
	QTextCodec *pCodec = QTextCodec::codecForName("system");
	QTextCodec::setCodecForTr(pCodec);
	QTextCodec::setCodecForLocale(pCodec);
	QTextCodec::setCodecForCStrings(pCodec);
#endif
	CLogin w;
	w.show();
	a.exec();


	return 0;

}
