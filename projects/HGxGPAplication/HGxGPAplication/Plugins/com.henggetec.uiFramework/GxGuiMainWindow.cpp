#include <QDesktopWidget>
#include <QStackedLayout>
#include <QShortcut>
#include <QFile>
#include <QLabel>
#include <QTimer>
#include <QStyleOption>
#include <QPainter>
#include <QKeySequence>
#include <QSettings>
#include <QCloseEvent>

#include "ui_GxGuiMainWindow.h"
#include "CXGisTopWidget.h"
#include "GxGuiMainWindow.h"

namespace HGxGui {
	CGxGuiMainWindow::CGxGuiMainWindow(QString qName /*= "FreeXGisStudio"*/, QString qFlag /*= ""*/, QWidget *parent)
		: QWidget(parent, Qt::Window | Qt::FramelessWindowHint)
		, m_qWindowsName(qName)
		, m_qWindowsFlag(qFlag)
	{
		// 初始化界面 begin
		ui = new Ui::CXGisMainWindow;
		ui->setupUi(this);

		setWindowTitle(m_qWindowsName);
		InitApp();
	}

	void CGxGuiMainWindow::InitApp()
	{
		if (m_qWindowsFlag == "1")
		{
			InitNormal();
		}
		else
		{
			InitBigScreen();
		}
	}

	void CGxGuiMainWindow::InitNormal()
	{
		QWidget *pScreen = QApplication::desktop()->screen();
		int Screen_Width = pScreen->width();
		int Screen_Height = pScreen->height();
		setMaximumSize(Screen_Width, Screen_Height);

		ui->mainframe->setFixedSize(Screen_Width, Screen_Height);

		m_pTopWgt = new CXGisTopWidget(this);
		m_pTopWgt->setAppType(HGxGui::EAppType::E_Normal);
		connect(m_pTopWgt, SIGNAL(closeAppSignal()), this, SLOT(closeAppSlot()));
		m_pTopWgt->move(0, 0);
		m_pTopWgt->setMinimumWidth(Screen_Width);
		m_pTopWgt->hide();

		installEventFilter(this);
	}

	void CGxGuiMainWindow::InitBigScreen()
	{
		QWidget *pScreen = QApplication::desktop()->screen();
		int Screen_Width = pScreen->width() * 2;
		int Screen_Height = pScreen->height();
		setFixedSize(Screen_Width, Screen_Height);

		m_pTopWgt = new CXGisTopWidget(this);
		m_pTopWgt->setAppType(HGxGui::EAppType::E_BigScreen);
		connect(m_pTopWgt, SIGNAL(closeAppSignal()), this, SLOT(closeAppSlot()));
		m_pTopWgt->move(0, 0);
		m_pTopWgt->setMinimumWidth(Screen_Width);
		m_pTopWgt->hide();

		installEventFilter(this);
	}

	CGxGuiMainWindow::~CGxGuiMainWindow()
	{
		delete ui;
	}
	/**
	* @brief 窗口基础功能
	*/
	void CGxGuiMainWindow::show()
	{
		m_pTopWgt->show();
		QWidget::show();
	}
	void CGxGuiMainWindow::hide()
	{
		m_pTopWgt->hide();
		QWidget::hide();
	}
	void CGxGuiMainWindow::setVisible(bool visible)
	{
		m_pTopWgt->setVisible(visible);
		QWidget::setVisible(visible);
	}


	QPoint CGxGuiMainWindow::pos() const
	{
		return QWidget::pos();
	}
	void CGxGuiMainWindow::move(int x, int y)
	{
		m_pTopWgt->move(x, y);
		QWidget::move(x, y);
	}
	void CGxGuiMainWindow::move(const QPoint& pos)
	{
		m_pTopWgt->move(pos);
		QWidget::move(pos);
	}

	const QRect& CGxGuiMainWindow::geometry() const
	{
		return QWidget::geometry();
	}
	void CGxGuiMainWindow::setGeometry(int x, int y, int w, int h)
	{
		QWidget::setGeometry(x, y, w, h);
	}
	void CGxGuiMainWindow::setGeometry(const QRect &geo)
	{
		QWidget::setGeometry(geo);
	}

	int CGxGuiMainWindow::x() const
	{
		return QWidget::x();
	}
	int CGxGuiMainWindow::y() const
	{
		return QWidget::y();
	}
	int CGxGuiMainWindow::width() const
	{
		return QWidget::width();
	}
	int CGxGuiMainWindow::height() const
	{
		return QWidget::height();
	}

	QSize CGxGuiMainWindow::size() const
	{
		return QWidget::size();
	}
	void CGxGuiMainWindow::resize(int w, int h)
	{
		m_pTopWgt->resize(w, m_pTopWgt->height());
		QWidget::resize(w, h);
	}
	void CGxGuiMainWindow::resize(const QSize &size)
	{
		m_pTopWgt->resize(QSize(size.width(), m_pTopWgt->height()));
		QWidget::resize(size);
	}
	QRect CGxGuiMainWindow::rect() const
	{
		return QWidget::rect();
	}

	void CGxGuiMainWindow::closeAppSlot()
	{
		m_pTopWgt->close();
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

	QWidget* CGxGuiMainWindow::addSceneWidget(const QString &id, const QString &title, QWidget *widget)
	{
		QHBoxLayout *pHLayout = static_cast<QHBoxLayout *>(ui->mainframe->layout());
		pHLayout->addWidget(widget);
		return widget;
	}

	void CGxGuiMainWindow::paintEvent(QPaintEvent *event)
	{
		QWidget::paintEvent(event);

		QStyleOption opt;
		opt.init(this);
		QPainter painter(this);
		style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
	}

	bool CGxGuiMainWindow::eventFilter(QObject *watched, QEvent *event)
	{
		return QWidget::eventFilter(watched, event);
	}

}

