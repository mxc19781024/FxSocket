#ifndef GxAddTileWindow_H
#define GxAddTileWindow_H

#include <QWidget>
#include <QKeyEvent>

#include "ui_GxAddTileWindow.h"
//#include "FreeXServiceUtilUi/Common.h"

//#include <FreeXFramework/FreeXFramework.h>
//#include <FreeXCommonUtils/EnvironmentVariableReader.h>
//#include <FreeXCommonUtils/PathRegistry.h>
#include <FxBaseWindow/CFxBaseWindow.h>

//#include <scene/IFxSceneService.h>
//#include <earth_layer/IFxEarthLayer.h>
//#include <map_layer/IFxMapLayer.h>

//#include <service/IFxDataService.h>

#include "public/Common.h"

#include "gdpUtil/service/GxTileService.h"
#include "gdpUtil/service/GxServiceLayer.h"
#include "gdpUtil/service/GxDataService.h"

class CGxLinkFileWindow;

/*
* class CGxAddTileWindow
* 展示需要添加的Tile服务，
*包含了服务类型(可选择) TMS, XYZ
*图层名称，默认未命名
*服务地址
*确定后，将图层信息发送给树界面展示
*/

class CGxAddTileWindow : public CFxBaseWindow
{
	Q_OBJECT

public:
	CGxAddTileWindow(const QString& strServerType, QWidget *parent = 0);
	~CGxAddTileWindow();

	// 设置当前服务器
	void setCurrentServer(const QString& strServerType);

private:
	// 初始化界面
	void initUi();

protected:
	void showEvent(QShowEvent *event);

	void keyPressEvent(QKeyEvent *);

private slots:
	// 确定按钮信号槽
	void on_pOkBtn_clicked();
	// 取消按钮信号槽
	void on_pCancelBtn_clicked();
	// XYZ本地服务链接按钮
	void on_pXYZLocalTBtn_clicked();
	// 当服务类型更改时更改版本号信号槽
	void on_pServerTypeCombo_currentIndexChanged(const QString &text);
	// 文件链接信号槽
	void folderLinkSlot(const QString & strPath, int linkType);
private:
	Ui::CGxAddTileWindow ui;

	HGxUtil::CGxDataService* m_pDataService;

	//  指定类型的服务指针
	HGxUtil::CGxTileservice* m_pService;

	// 存储图层信息
	std::vector<HGxUtil::CGxServiceLayer*>  m_vecLayers;
};

#endif // GxAddTileWindow_H
