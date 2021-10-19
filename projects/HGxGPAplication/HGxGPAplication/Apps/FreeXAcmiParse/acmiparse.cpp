#include "acmiparse.h"
//#include <FlyEventParse.h>
#include <QTableWidget>
AcmiParse::AcmiParse(QWidget *parent)
	: QMainWindow(parent)
{
	//ui.setupUi(this);

	m_acmiParse.SetHDelt(3000);
	m_acmiParse.SetLatDelt(-1.5);
	m_acmiParse.SetLonDelt(2);

	//设置文件读取路径
	m_acmiParse.ParseAcmiFile("E:/Product/tacviewdata/Tacview-Demo-Ironhand-Su-27-M01.txt.acmi");

	//保存路径
	m_acmiParse.SaveFile("E:/Project/TestProject/AcmiParse/testData/");

	//QTableWidget *pTable = new QTableWidget;

	//pTable->setColumnCount(2);

	//QStringList strList ;
	//strList << "Txtx" << "dfgdfg";

	//pTable->setHorizontalHeaderLabels(strList);

	//pTable->show();

	//FlyEventParse flyParse;
	//flyParse.parseFlyRecord("E:/Product/态势推演/同行软件/TrackView/Tacview-Demo-Ironhand-Su-27-M01.xml");
	//flyParse.saveFlyRecord("F:/Tacview-Demo-Ironhand-Su-27-M01_my.txt");

	//flyParse.saveObjRealtion("F:/Tacview-Demo-Ironhand-Su-27-M01_Real.txt");

}

AcmiParse::~AcmiParse()
{

}
