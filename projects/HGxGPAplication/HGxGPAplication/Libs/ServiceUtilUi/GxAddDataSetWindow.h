#ifndef GxAddDataSetWindow_H
#define GxAddDataSetWindow_H

#include <QWidget>
#include <QKeyEvent>

#include "ui_GxAddDataSetWindow.h"
//#include "FreeXServiceUtilUi/Common.h"

//#include <FreeXFramework/FreeXFramework.h>
//#include <FreeXCommonUtils/EnvironmentVariableReader.h>
//#include <FreeXCommonUtils/PathRegistry.h>


//#include <scene/IFxSceneService.h>
//#include <earth_layer/IFxEarthLayer.h>
//#include <map_layer/IFxMapLayer.h>

//#include <service/IFxDataService.h>

#include "public/Common.h"
#include <FxBaseWindow/CFxBaseWindow.h>

#include "gdpUtil/service/GxTileService.h"
#include "gdpUtil/service/GxDataService.h"

/*
* class CGxAddDataSetWindow
* 展示需要添加的Tile服务，
*包含了服务类型(可选择) , flatgis，tdb，mbtiles 
*图层名称，默认未命名
*服务地址
*确定后，将图层信息发送给树界面展示
*/


class CGxAddDataSetWindow : public CFxBaseWindow
{
	Q_OBJECT

public:
	CGxAddDataSetWindow(QWidget *parent = 0);
	~CGxAddDataSetWindow();

protected:
	void showEvent(QShowEvent *event);

	void keyPressEvent(QKeyEvent *);

private:
	// 初始化界面
	void initUi();

private slots:
	// 确定按钮信号槽
	void on_pOkBtn_clicked();
	// 取消按钮信号槽
	void on_pCancelBtn_clicked();

private:
	Ui::CGxAddDataSetWindow ui;

	HGxUtil::CGxDataService* m_pDataService;

	HGxUtil::CGxTileservice* m_pService;
};

#endif // GxAddDataSetWindow_H
