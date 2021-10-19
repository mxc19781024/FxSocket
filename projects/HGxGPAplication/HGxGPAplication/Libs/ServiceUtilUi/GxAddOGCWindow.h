#ifndef GXADDOGCWINDOW_H
#define GXADDOGCWINDOW_H

#include <QWidget>
#include <QKeyEvent>

#include "ui_GxAddOGCWindow.h"
//#include "FreeXServiceUtilUi/Common.h"

//#include <FreeXFramework/FreeXFramework.h>
//#include <FreeXCommonUtils/EnvironmentVariableReader.h>
//#include <FreeXCommonUtils/PathRegistry.h>
#include <FxBaseWindow/CFxBaseWindow.h>
//#include <scene/IFxSceneService.h>
//#include <earth_layer/IFxEarthLayer.h>
//#include <map_layer/IFxMapLayer.h>
//#include <service/IFxDataService.h>
//#include <service/IFxOGCService.h>

#include "public/Common.h"

#include <gdpUtil/service/GxOGCService.h>
#include <gdpUtil/service/GxDataService.h>
#include <gdpUtil/service/GxServiceLayer.h>

#include <osg/ref_ptr>

/*
* class CGxAddOGCWindow
* 展示需要添加的OGC服务，
*包含了服务类型(可选择) WMS,WMTS,WFS,WCS
*服务名称，默认未命名
*服务地址
*服务版本，根据服务类型更改
*获取图层，通过外部接口获取到图层
*确定后，将图层信息发送给树界面展示
*/

class CGxAddOGCWindow : public CFxBaseWindow
{
	Q_OBJECT

public:
	CGxAddOGCWindow(QWidget *parent = 0);
	~CGxAddOGCWindow();

private:
	// 初始化界面
	void initUi();
public:
	// 设置当前服务器
	void setCurrentServer(QString strServerType);

protected:
	void showEvent(QShowEvent *event);

	void keyPressEvent(QKeyEvent *);

private slots:
	// 当服务类型更改时更改版本号信号槽
	void on_pServerTypeCombo_currentIndexChanged(const QString &text);
	// 获取图层按钮信号槽
	void on_pGetLayerBtn_clicked();
	// 确定按钮信号槽
	void on_pOkBtn_clicked();
	// 取消按钮信号槽
	void on_pCancelBtn_clicked();

private:
	Ui::CGxAddOGCWindow ui;

	// 服务的连接状态
	bool	m_bDisconnect;

	HGxUtil::CGxDataService* m_pDataService;

	//  指定类型的服务指针
	osg::ref_ptr<HGxUtil::CGxOGCService> m_pService;

	// 存储图层信息
	std::vector<HGxUtil::CGxServiceLayer*> m_vecLayers;

	//存储支持数据格式
	std::vector<QString>							m_serviceFormats;
};

#endif // GXADDOGCWINDOW_H
