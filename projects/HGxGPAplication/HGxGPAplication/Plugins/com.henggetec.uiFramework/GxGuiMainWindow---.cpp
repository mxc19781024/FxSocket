#include "GxGuiMainWindow.h"
#include <QLabel>
#include <QScreen>
#include <QApplication>

#include "GxGuiMainWindowPrivate.h"
#include "CXGisTopWidget.h"

HGxGUI_USE_NAMESPACE

CGxGuiMainWindow::CGxGuiMainWindow(QWidget* parent)
    : DockMainWindow(parent, Qt::Window | Qt::FramelessWindowHint)
    , d_ptr(new CGxGuiMainWindowPrivate(this))
{
    //dockPanelManager()->setDockPanelTransparentWhileDragging(true);
    //dockPanelManager()->setArrowMarkersShown(true);
    dockPanelManager()->setDockPanelFullContentsWhileDraggingShown(true);
    dockPanelManager()->centralLayoutPanel();
    //createStyleActions();   //创建样式菜单项
    //createMainMenu();   //创建主菜单
    //createToolBars();       //床单工具栏
    //createDockPanels();     //创建停靠窗
    //createStatusBar();      //创建状态栏

    //默认半屏居中
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    const QRect availableGeometry = screen()->availableGeometry();  //当前屏幕
#else
    const QRect availableGeometry = QGuiApplication::primaryScreen()->availableGeometry();  //主屏
#endif
    resize(availableGeometry.width() / 2, availableGeometry.height() / 2);
    move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);

    setTitleBarVisible(true);
    //titleBar()->setWidget(new QLabel(windowTitle()), WindowTitleBar::AlignClient);
    //titleBar()->show();

	m_pTopWgt = new AppUI::CXGisTopWidget(this);
	m_pTopWgt->setAppType();


	connect(m_pTopWgt, SIGNAL(closeAppSignal()), this, SLOT(closeAppSlot()));
	m_pTopWgt->move(0, 0);
	m_pTopWgt->setMinimumWidth(1920);
	m_pTopWgt->show();

}

CGxGuiMainWindow::~CGxGuiMainWindow()
{

}

/**
* @brief 窗口基础功能
*/
void CGxGuiMainWindow::show()
{
    DockMainWindow::show();
}
void CGxGuiMainWindow::hide()
{
    DockMainWindow::hide();
}
void CGxGuiMainWindow::setVisible(bool visible)
{
    DockMainWindow::setVisible(visible);
}


QPoint CGxGuiMainWindow::pos() const
{
    return DockMainWindow::pos();
}
void CGxGuiMainWindow::move(int x, int y)
{
    DockMainWindow::move(x, y);
}
void CGxGuiMainWindow::move(const QPoint& pos)
{
    DockMainWindow::move(pos);
}

const QRect& CGxGuiMainWindow::geometry() const
{
    return DockMainWindow::geometry();
}
void CGxGuiMainWindow::setGeometry(int x, int y, int w, int h)
{
    DockMainWindow::setGeometry(x, y, w, h);
}
void CGxGuiMainWindow::setGeometry(const QRect &geo)
{
    DockMainWindow::setGeometry(geo);
}

int CGxGuiMainWindow::x() const
{
    return DockMainWindow::x();
}
int CGxGuiMainWindow::y() const
{
    return DockMainWindow::y();
}
int CGxGuiMainWindow::width() const
{
    return DockMainWindow::width();
}
int CGxGuiMainWindow::height() const
{
    return DockMainWindow::height();
}

QSize CGxGuiMainWindow::size() const
{
    return DockMainWindow::size();
}
void CGxGuiMainWindow::resize(int w, int h)
{
    DockMainWindow::resize(w, h);
}
void CGxGuiMainWindow::resize(const QSize &size)
{
    DockMainWindow::resize(size);
}
QRect CGxGuiMainWindow::rect() const
{
    return DockMainWindow::rect();
}

QWidget* CGxGuiMainWindow::titleBarWidget() const
{
	return DockMainWindow::titleBarWidget();
}

void CGxGuiMainWindow::setTitleBarWidget(QWidget* widget)
{
	DockMainWindow::setTitleBarWidget(widget);
}

QMenuBar* CGxGuiMainWindow::menuBar() const
{
	return DockMainWindow::menuBar();
}
void CGxGuiMainWindow::setMenuBar(QMenuBar* menubar)
{
	DockMainWindow::setMenuBar(menubar);
}

QStatusBar* CGxGuiMainWindow::statusBar() const
{
	return DockMainWindow::statusBar();
}

void CGxGuiMainWindow::setStatusBar(QStatusBar* statusbar)
{
	DockMainWindow::setStatusBar(statusbar);
}

void CGxGuiMainWindow::setTitleBarVisible(bool visible)
{
    titleBar()->setVisible(visible);
}

void CGxGuiMainWindow::setCustomStyle(CGxGuiStyle appStyle)
{
    Q_D(CGxGuiMainWindow);
    d->setCustomStyle(appStyle);
}

QMenu* CGxGuiMainWindow::styleMenu() const
{
    Q_D(const CGxGuiMainWindow);
    return d->m_pMenuStyles;
}

QMenu* CGxGuiMainWindow::dockMenu() const
{
	Q_D(const CGxGuiMainWindow);
	return d->m_pMenuDocks;
}

void CGxGuiMainWindow::showMessageBox()
{

}


void CGxGuiMainWindow::showToast()
{

}

void CGxGuiMainWindow::framelessWindowHint()
{
#if WIN32
	setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
#else if __linux__
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
#endif

	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_Hover);
}

void CGxGuiMainWindow::closeAppSlot()
{
	close();
}

/**
* @brief 获取窗口框架的主窗口
* @return   QWidget* 主窗口指针
*/
QWidget* CGxGuiMainWindow::mainWindow()
{
    return this;
}

/**
* @brief	 添加停靠窗
* @param	 id [in] 窗口的唯一id
* @param	 title [in] 工具窗口名称
* @param	 widget [in] 工具窗口widget
* @param	 userData [in] 用户自定义字段
* @param     role [in]  用户自定义字段类型
* @return	 DockDocumentPanel* 场景停靠面板指针，为空表示异常
*/
DockWidgetPanel* CGxGuiMainWindow::addDockWidget(const QString &id, const QString &title, QWidget *widget, DockPanelArea area, DockPanelBase* targetPanel, const QVariant& userData, const Qt::ItemDataRole role)
{
    Q_D(CGxGuiMainWindow);
    //判断id是否重复
    if (d->contains(id))
    {
        //报错，返回
        qCritical("id has been existed");
        return Q_NULL;
    }
    DockWidgetPanel* pDockPanel = new DockWidgetPanel(title, dockPanelManager());
    if (pDockPanel)
    {
        pDockPanel->setWidget(widget);
        dockPanelManager()->insertDockPanel(pDockPanel, area, targetPanel);
        d->insertPanel(id, pDockPanel);
        d->insertUserData(id, userData, role);
    }

    return pDockPanel;
}




/**
* @brief	: 添加停靠窗
* @param	 id [in] 窗口的唯一id
* @param	 title [in] 窗口名称
* @param	 widget [in] 窗口widget
* @param	 userData [in] 用户自定义字段
* @param     role [in]  用户自定义字段类型
* @return	 DockDocumentPanel* 场景停靠面板指针
*/
DockWidgetPanel* CGxGuiMainWindow::addFloatingDockWidget(const QString &id, const QString &title, QWidget *widget, DockPanelArea area, DockPanelBase* targetPanel, const QVariant& userData, const Qt::ItemDataRole role)
{
#if 0
    DockWidgetPanel* pDockPanel = new DockWidgetPanel(title, dockPanelManager());
    if (pDockPanel)
    {
        pDockPanel->setWidget(widget);
        pDockPanel->setFloat(true);
        dockPanelManager()->insertDockPanel(pDockPanel, Qtitan::AllDockPanelAreas);
    }
#endif
    DockWidgetPanel* pDockPanel = addDockWidget(id, title, widget, Qtitan::AllDockPanelAreas, targetPanel, userData, role);
    if (pDockPanel)
    {
        pDockPanel->setFloat(true);
    }
    return pDockPanel;
}

DockDocumentPanel* CGxGuiMainWindow::addSceneWidget(const QString &id, const QString &title, QWidget *widget, const QVariant& userData, const Qt::ItemDataRole role)
{
    Q_D(CGxGuiMainWindow);
    //判断id是否重复
    if (d->contains(id))
    {
        //报错，返回
        qCritical("id has been existed");
        return Q_NULL;
    }
    DockDocumentPanel* pScenePanel = new DockDocumentPanel(dockPanelManager());
    if (pScenePanel)
    {
        //二三维窗口最小不大不能是0,0，否则显示会有问题
        widget->setMinimumSize(QSize(1, 1));
        //手动激活窗口，防止二三维窗口因为从来没有显示而获取不到上下文
        widget->show();
        pScenePanel->setWidget(widget);
        pScenePanel->setCaption(title);
        dockPanelManager()->insertDocumentPanel(pScenePanel);
        d->insertPanel(id, pScenePanel);
        d->insertUserData(id, userData, role);
        //手动激活窗口，防止二三维窗口因为从来没有显示而获取不到上下文
        pScenePanel->activate();
    }
    return pScenePanel;
}


/**
* @brief	 停靠窗是否浮动
* @param	 id [in] 工具窗口id
* @return	 浮动返回true，否则返回false
*/
bool CGxGuiMainWindow::isDockFloat(const QString& id)
{
    Q_D(CGxGuiMainWindow);
    return d->isDockFloat(id);
}

/**
* @brief	 设置停靠窗是否浮动
* @param	 id [in] 工具窗口id
*/
void CGxGuiMainWindow::setDockFloat(const QString& id, bool bFloat)
{
    Q_D(CGxGuiMainWindow);
    d->setDockFloat(id, bFloat);
}

/**
* @brief	 停靠窗是否浮动
* @param	 id [in] 工具窗口id
* @param     role [in]  用户自定义字段类型
* @return	 浮动返回true，否则返回false
*/
QVariant CGxGuiMainWindow::dockUserData(const QString& id, const Qt::ItemDataRole role) const
{
    Q_D(const CGxGuiMainWindow);
    return d->m_idToUserDatas.value(id).value(role);
}

/**
* @brief	 停靠窗是否浮动
* @param	 id [in] 工具窗口id
* @param     role [in]  用户自定义字段类型
* @return	 浮动返回true，否则返回false
*/
void CGxGuiMainWindow::setDockUserData(const QString& id, const QVariant& userData, const Qt::ItemDataRole role)
{
    Q_D(CGxGuiMainWindow);
    d->insertUserData(id, userData, role);
}


/**
* @brief	 关闭工具窗口
* @param	 id [in] 工具窗口id
* @return	 移除成功返回true，否则返回false
* @noe       对应showDockPanel
*/
void CGxGuiMainWindow::closeDockWidget(const QString &id)
{
    Q_D(CGxGuiMainWindow);
	if (!dockPanelManager()->isDockPanelClosed(d->getPanel(id)))
	{
		dockPanelManager()->closeDockPanel(d->getPanel(id));
	}
   
}

/**
* @brief	 显示工具窗口
* @param	 id [in] 工具窗口id
* @param     setFocus [in] 焦点
* @return	 对应closeDockWidget
*/
void CGxGuiMainWindow::showDockPanel(const QString& id, bool setFocus)
{
    Q_D(CGxGuiMainWindow);
	if (dockPanelManager()->isDockPanelClosed(d->getPanel(id)))
	{
		dockPanelManager()->showDockPanel(d->getPanel(id), setFocus);
	}
    
}

/**
* @brief	 设置工具窗口的显隐
* @param	 id [in] 工具窗口id
* @param	 visible [in] 是否显示
* @return	 成功返回true，否则返回false
*/
void CGxGuiMainWindow::setDockVisible(const QString &id, bool visible)
{
    if (visible)
    {
        showDockPanel(id);
    }
    else
    {
        closeDockWidget(id);
    }
}

/**
* @brief	 设置全部工具窗口的显隐
* @params	 visible [in] 是否显示
*/
void CGxGuiMainWindow::setAllDocksVisible(bool visible)
{
    Q_D(CGxGuiMainWindow);
    QHash<QString, DockWidgetPanel*>::iterator it = d->m_idToDock.begin();
    while (it != d->m_idToDock.end())
    {
        setDockVisible(it.key(), visible);
        ++it;
    }
}

/**
* @brief	 获取工具窗口是否存在
* @param	 id [in] 工具窗口id
* @return	 存在返回true，否则返回false
*/
bool CGxGuiMainWindow::isDockExists(const QString &id)
{
    Q_D(CGxGuiMainWindow);
    return d->contains(id);
}
