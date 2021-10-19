/*************************************
 *
 * @file GxGuiMainWindow.h
 * @brief 框架主窗口类
 * @author H00015
 * @date 2021-03-31 15:05:33
 * @note
 *
 *************************************/

#ifndef GX_GUI_MAINWINDOW_H
#define GX_GUI_MAINWINDOW_H

#include "GxGuiDef.h"
#include "GxUiFrameworkServiceInterface.h"
#include <QtitanDocking.h>
#include "CXGisTopWidget.h"


HGxGUI_BEGIN_NAMESPACE
using namespace Qtitan;
using namespace AppUI;

class CGxGuiMainWindowPrivate;
class CXGisTopWidget;
/**
 * @class CGxGuiMainWindow
 * @brief 框架主窗口
 * @details
 * @author H00015
 * @note
 */
class CGxGuiMainWindow : public DockMainWindow, public CGxUiFrameworkServiceInterface
{
    Q_OBJECT
    Q_INTERFACES(CGxUiFrameworkServiceInterface)
public:
    explicit CGxGuiMainWindow(QWidget* parent = Q_NULL);
    virtual ~CGxGuiMainWindow();

    /**
    * @brief 窗口基础功能
    */
    void show() override;
    void hide() override;
    void setVisible(bool visible) override;


    QPoint pos() const override;
    void move(int x, int y) override;
    void move(const QPoint& pos) override;

    const QRect& geometry() const override;
    void setGeometry(int x, int y, int w, int h) override;
    void setGeometry(const QRect &geo) override;

    int x() const override;
    int y() const override;
    int width() const override;
    int height() const override;

    QSize size() const override;
    void resize(int w, int h) override;
    void resize(const QSize &size) override;
    QRect rect() const override;

	/**
	* @brief 获取标题栏窗口
	* @return QWidget* 标题栏窗口
	*/
	QWidget* titleBarWidget() const override;
	/**
	* @brief 设置标题栏窗口
	* @param  [in] widget 标题栏窗口指针
	*/
	void setTitleBarWidget(QWidget* widget) override;

	/**
	* @brief 获取菜单栏窗口
	* @return QWidget* 菜单栏窗口
	*/
	QMenuBar* menuBar() const override;
	/**
	* @brief 设置菜单栏窗口
	* @param  [in] widget 菜单栏窗口指针
	*/
	void setMenuBar(QMenuBar* menubar) override;

	/**
	* @brief 获取状态栏窗口
	* @return QWidget* 状态栏窗口
	*/
	QStatusBar* statusBar() const override;
	/**
	* @brief 设置状态栏窗口
	* @param  [in] widget 状态栏窗口指针
	*/
	void setStatusBar(QStatusBar* statusbar) override;

    /**
     * @brief 设置标题栏是否显示
     * @param  [in] visible 是否可见
     * @note 默认可见
     */
    void setTitleBarVisible(bool visible) override;

    /**
     * @brief 设置自定义样式
     * @details 可快速切换预定义的5种风格样式
     * @param  [in] appStyle 样式对应的枚举值
     */
    void setCustomStyle(CGxGuiStyle appStyle) override;

    /**
     * @brief 获取样式选项菜单
     * @return QMenu* 样式选项菜单
     */
    QMenu* styleMenu() const override;

	/**
	* @brief 获取被管理窗口操作菜单
	* @return QMenu* 被管理窗口菜单
	*/
	QMenu* dockMenu() const override;

    /**
    * @brief 获取窗口框架的主窗口
    * @return   QWidget* 主窗口指针
    */
    virtual QWidget *mainWindow() override;

    /**
    * @brief	 添加停靠窗
    * @param	 id [in] 窗口的唯一id
    * @param	 title [in] 工具窗口名称
    * @param	 widget [in] 工具窗口widget
    * @param	 userData [in] 用户自定义字段
    * @param     role [in]  用户自定义字段类型
    * @return	 DockDocumentPanel* 场景停靠面板指针，为空表示异常
    */
    virtual DockWidgetPanel* addDockWidget(const QString &id, const QString &title, QWidget *widget, DockPanelArea area, DockPanelBase* targetPanel = Q_NULL, const QVariant& userData = "", const Qt::ItemDataRole role = Qt::UserRole) override;

    /**
    * @brief	: 添加停靠窗
    * @param	 id [in] 窗口的唯一id
    * @param	 title [in] 窗口名称
    * @param	 widget [in] 窗口widget
    * @param	 userData [in] 用户自定义字段
    * @param     role [in]  用户自定义字段类型
    * @return	 DockDocumentPanel* 场景停靠面板指针，为空表示异常
    */
    virtual DockWidgetPanel* addFloatingDockWidget(const QString &id, const QString &title, QWidget *widget, DockPanelArea area, DockPanelBase* targetPanel = Q_NULL, const QVariant& userData = "", const Qt::ItemDataRole role = Qt::UserRole) override;

    /**
    * @brief	 添加场景窗
    * @param	 id [in] 窗口的唯一id
    * @param	 title [in] 工具窗口名称
    * @param	 widget [in] 工具窗口widget
    * @param	 userData [in] 用户自定义字段
    * @param     role [in]  用户自定义字段类型
    * @return	 DockDocumentPanel* 场景停靠面板指针
    * @note      场景窗只能在中央核心区域
    */
    virtual DockDocumentPanel* addSceneWidget(const QString &id, const QString &title, QWidget *widget, const QVariant& userData = "", const Qt::ItemDataRole role = Qt::UserRole) override;

    /**
    * @brief	 关闭工具窗口
    * @param	 id [in] 工具窗口id
    * @return	 移除成功返回true，否则返回false
    * @noe       对应showDockPanel
    */
    virtual void closeDockWidget(const QString &id) override;

    /**
    * @brief	 显示工具窗口
    * @param	 id [in] 工具窗口id
    * @param     setFocus [in] 焦点
    * @return	 对应closeDockWidget
    */
    virtual void showDockPanel(const QString& id, bool setFocus = false) override;

    /**
    * @brief	 设置工具窗口的显隐
    * @param	 id [in] 工具窗口id
    * @param	 visible [in] 是否显示
    * @return	 成功返回true，否则返回false
    */
    virtual void setDockVisible(const QString &id, bool visible) override;


    /**
    * @brief	 设置全部工具窗口的显隐
    * @params	 visible [in] 是否显示
    */
    virtual void setAllDocksVisible(bool visible) override;

    /**
    * @brief	 获取工具窗口是否存在
    * @param	 id [in] 工具窗口id
    * @return	 存在返回true，否则返回false
    */
    virtual bool isDockExists(const QString &id) override;

    /**
    * @brief	 停靠窗是否浮动
    * @param	 id [in] 工具窗口id
    * @return	 浮动返回true，否则返回false
    */
    virtual bool isDockFloat(const QString& id) override;

    /**
    * @brief	 设置停靠窗是否浮动
    * @param	 id [in] 工具窗口id
    */
    virtual void setDockFloat(const QString& id, bool bFloat) override;

    /**
    * @brief	 停靠窗是否浮动
    * @param	 id [in] 工具窗口id
    * @param     role [in]  用户自定义字段类型
    * @return	 QVariant 自定义数据
    */
    virtual QVariant dockUserData(const QString& id, const Qt::ItemDataRole role = Qt::UserRole) const override;

    /**
    * @brief	 停靠窗是否浮动
    * @param	 id [in] 工具窗口id
    * @param     role [in]  用户自定义字段类型
    * @return	 浮动返回true，否则返回false
    */
    virtual void setDockUserData(const QString& id, const QVariant& userData, const Qt::ItemDataRole role = Qt::UserRole) override;


public Q_SLOTS:

    /**
     * @brief 显示QMessageBox提醒窗口
     * @details
     * @param  [in]
     * @return
     * @note
     */
    void showMessageBox();

    /**
     * @brief 显示Toast弱提醒窗
     * @details
     * @param  [in]
     * @return
     * @note
     */
    void showToast();

	/**
	* @brief 设置主窗口无边框
	* @details
	* @param  [in]
	* @return
	* @note
	*/
	void framelessWindowHint();

	void closeAppSlot();

private:
    Q_DISABLE_COPY(CGxGuiMainWindow)
    Q_DECLARE_PRIVATE(CGxGuiMainWindow)
    QScopedPointer<CGxGuiMainWindowPrivate> d_ptr;	/// d指针
	AppUI::CXGisTopWidget  *m_pTopWgt;
};

HGxGUI_END_NAMESPACE

#endif  //GX_GUI_MAINWINDOW_H