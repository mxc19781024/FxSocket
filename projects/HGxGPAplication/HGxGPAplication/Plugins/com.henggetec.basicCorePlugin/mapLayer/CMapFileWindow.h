#ifndef CFILE_SELECT_WINDOW__H
#define CFILE_SELECT_WINDOW__H

#include <QWidget>
#include <QFileSystemModel>
#include <QStandardItemModel>
#include <QKeyEvent>
//#include "FreeXServiceUtilUi/Common.h"
#include "FxBaseWindow/CFxBaseWindow.h"

#include "Common.h"

namespace Ui {
	class CMapFileWindow;
}

class  CMapFileSelectWindow : public CFxBaseWindow
{
	Q_OBJECT
	
public:
	CMapFileSelectWindow(QWidget *parent=0);
	~CMapFileSelectWindow();
	/**  
	  * @enum 链接类型
	*/
	enum ELinkType
	{
		FOLDER = 0,
		FILE = 1
	};

	void show();

	/// 获取选择的文件路径
	void getSelectFilePaths(QStringList &strListPath);
	

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

	/// 
	void getFilePaths(const QModelIndex &index, QStringList &);

signals:

	/// 
	void confirmSignals();

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

	void getPathByDir( const QString &strDir,  QList<QString> &strPathSet);
private:
	// 文件系统
	QFileSystemModel *								m_systemModel;

	// 文件系统过滤项
	QStringList										m_filters;

	QScopedPointer<Ui::CMapFileWindow>				ui;	

};

#endif // CFILE_WINDOW___H
