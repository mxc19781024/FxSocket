#ifndef GxServerDataWindow_H
#define GxServerDataWindow_H

#include <QWidget>
#include <QStandardItemModel>
#include <QKeyEvent>

//#include <FreeXFramework/FreeXFramework.h>
//#include <FreeXCommonUtils/EnvironmentVariableReader.h>
//#include <FreeXCommonUtils/PathRegistry.h>

//#include <scene/IFxSceneService.h>
//#include <earth_layer/IFxEarthLayer.h>
//#include <map_layer/IFxMapLayer.h>

//#include <service/IFxDataService.h>
#include "layer_ui/CLayerManager.h"

//#include "FreeXServiceUtilUi/Common.h"
//#include "FxBaseWindow/CFxBaseWindow.h"

#include "Common.h"
#include "FxBaseWindow/CFxBaseWindow.h"


#ifdef GXDATARESOURCESWINDOW_LIB
# define GXDATARESOURCESWINDOW_EXPORT Q_DECL_EXPORT
#else
# define GXDATARESOURCESWINDOW_EXPORT Q_DECL_IMPORT
#endif

namespace Ui {
	class CGxServerDataWindow;
}

class CGxLinkFileWindow;

class GXDATARESOURCESWINDOW_EXPORT CGxServerDataWindow : public CFxBaseWindow
{
	Q_OBJECT

public:
	CGxServerDataWindow(QWidget *parent = 0);
	~CGxServerDataWindow();

	void show();

	/**
     * @brief 过滤本地文件
	*/
	void filterAllFiles();
	
	/**  
	  * @brief 设置文件过滤
	  * @param filters [in] 文件类型
	*/
	void setFileFilter(QStringList filters) {m_listFilefilters = filters;}

	/**  
	  * @brief 设置当前要处理的数据图层类型
	  * @note 之前根据数据判断图层类型，但某些数据类型可以作为多种图层类型添加（如:tif）
	  * @param eLayerType [in] 图层类型枚举
	  * @auther g00034
	*/
	void setCurLayerType(CLayerConfig::LayerType eLayerType) { m_eCurLayerType = eLayerType; }

protected:
	void keyPressEvent(QKeyEvent *);

private:
	/**  
	  * @brief 初始化窗口
	*/
	void init();

	/**  
	  * @brief 初始化信号和槽
	*/
	void initSignalsAndSlots();

	/**  
	  * @brief 获取当前节点
	*/
	QStandardItem* getCurrentItem();

	/**
     * @brief 过滤文件
	*/
	void filterFile(QStandardItem* pItem);

signals:
	void addLayerToSceneSignal(const QString &strName, const QString &strPath, const QString &strDriveName, CLayerConfig::LayerType layerType );

	void addServerLayerToSceneSignal(const QString &strName, const SLayerPropertyStruct &stLayerStruct, CLayerConfig::LayerType layerType );

private slots:
	/**  
	  * @brief 切换文件视图
	*/
	void switchFileViewSlot(int index);


	/**  
	  * @brief 点击文件视图触发的槽函数
	  * @param index [in] 点击选项
	*/
	void viewClickedSlot(const QModelIndex &index);
	/**  
	  * @brief 确认槽函数
	*/
	void confirmSlot();

	/**  
	  * @brief 取消槽函数
	*/
	void cancelSlot();

	/**  
	  * @brief 右键菜单响应的槽函数
	*/
	void contextMenuSlot(const QPoint &pos);

	// 添加服务信号槽
	void addServerSlot();

	/**  
	  * @brief 链接到本地文件或者本地文件夹
	*/
	void linkToSlot();

	/**  
	  * @brief 重置系统文件信号槽
	*/
	void on_pResetTBtn_clicked();

	/**
     * @brief 展开树节点
   */
	void expandItemsSlot( const QModelIndex & index);

private:
	// 服务树model
	QStandardItemModel*									m_pItemModel;

	QScopedPointer<Ui::CGxServerDataWindow>				ui;	
	
	QStringList											m_listFilefilters;
	
	bool												m_bLinkFiles;

	CLayerConfig::LayerType                             m_eCurLayerType;
};

#endif // GxServerDataWindow_H
