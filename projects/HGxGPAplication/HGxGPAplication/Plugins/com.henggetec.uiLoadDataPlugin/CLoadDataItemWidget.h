#ifndef LOADDATAITEMWIDGET_H
#define LOADDATAITEMWIDGET_H

#include <QWidget>
#include <QHash>
#include "qlistwidget.h"
#include "FxBaseWindow\CFxBaseWindow.h"

enum Script_Type
{
	SCRIPT_AIRCRAFT = 0, //航天
	SCRIPT_DEBRIS = 1,	//星历碎片
	SCRIPT_ELEC = 2,	//电磁特效
	SCRIPT_ROCKET = 3, 	//火箭发射
	SCRIPT_SATELLITE	//卫星推演
};

struct SItemInfo
{
	std::string strType;
	std::string strPath;
	std::string strImage;
	std::string strName;
	std::string strStartTime;
	std::string strEndTime;
	std::string strDescribe;
	SItemInfo()
	{
		strType = "";
		strPath = "";
		strImage = "";
		strName = "";
		strStartTime = "";
		strEndTime = "";
		strDescribe = "";
	}
};
namespace Ui {
class CLoadDataItemWidget;
}


class CLoadDataItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CLoadDataItemWidget( QWidget *parent = nullptr);
    ~CLoadDataItemWidget();

	void setItemInfo(SItemInfo sItemInfo);
protected:

private Q_SLOTS:
private:
    Ui::CLoadDataItemWidget *ui;
	std::vector<SItemInfo>			m_vecChartInfo;
	std::map<std::string , QListWidgetItem*>    m_mapPath2Item;
	std::map<std::string , QListWidgetItem*>    m_mapType2Item;
};

#endif // NODEWIDGET_H
