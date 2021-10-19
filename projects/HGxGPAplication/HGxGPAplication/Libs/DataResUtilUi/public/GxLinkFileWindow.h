#ifndef GxLinkFileWindow_H
#define GxLinkFileWindow_H

#include <QWidget>
#include <QFileSystemModel>
#include <QStandardItemModel>
#include <QKeyEvent>
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
	class CGxLinkFileWindow;
}

class GXDATARESOURCESWINDOW_EXPORT CGxLinkFileWindow : public CFxBaseWindow
{
	Q_OBJECT
	
public:
	CGxLinkFileWindow(QWidget *parent = 0);
	~CGxLinkFileWindow();
	/**  
	  * @enum 链接类型
	*/
	enum ELinkType
	{
		FOLDER = 0,
		FILE = 1
	};

	void show();

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

private slots:
	
	/**  
	  * @brief 确认槽函数
	*/
	void confirmSlot();

	/**  
	  * @brief 取消槽函数
	*/
	void cancelSlot();

	/**  
	  * @brief 点击文件视图触发的槽函数
	  * @param index [in] 点击选项
	*/
	void viewClickedSlot(const QModelIndex &index);

	/**  
	  * @brief 文件过滤槽函数
	  * @param index [in] 过滤下标
	*/
	void fileFilterSlot(int index);

	/**  
	  * @brief 选择文件信号槽
	*/
	void on_pFileSeletedCBBox_currentIndexChanged(int index);
	
	/**  
	  * @brief 重置系统文件信号槽
	*/
	void on_pResetTBtn_clicked();

private:
	// 文件系统
	QFileSystemModel								m_systemModel;

	// 文件系统过滤项
	QStringList										m_filters;

	// 链接信息
	QString											m_strLinkInfo;

	QScopedPointer<Ui::CGxLinkFileWindow>				ui;	
};

#endif // GxLinkFileWindow_H
