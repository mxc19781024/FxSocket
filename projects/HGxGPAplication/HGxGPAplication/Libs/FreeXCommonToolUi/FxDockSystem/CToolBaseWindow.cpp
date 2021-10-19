#include "FxDockSystem/CToolBaseWindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QEvent>
#include <QHoverEvent>
#include <QFile>
#include <QStyleOption>
#include <QPainter>

#define HEADER_MIN_HEIGHT 28

CToolBaseWindow::CToolBaseWindow(QWidget *parent)
	: QWidget(parent)
	, m_bResizable(true)
	, m_bMousePressed(false)
	, m_iDragMargin(4)
	, m_mouseStyle(NORMAL)
	, m_bCanGrabMouse(true)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
	setAttribute(Qt::WA_Hover);

	installEventFilter(this);

	QFile file(":/qss_docksystem/Resources/qss_docksystem/docksystem.css");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		setStyleSheet(file.readAll());
	}

	// TODO 目前这么做，后面做成dock窗口需要修改
	m_pHeaderWgt = new QWidget;
	m_pHeaderWgt->setObjectName("fx_tool_window_header");
	m_pHeaderWgt->setMinimumHeight(HEADER_MIN_HEIGHT);
	m_pTitleLbl = new QLabel;
	m_pTitleLbl->setMinimumHeight(HEADER_MIN_HEIGHT);
	QPushButton *pOperateBtn = new QPushButton;
	m_pTitleLbl->setObjectName("fx_dock_tool_title_label");
	pOperateBtn->setObjectName("fx_dock_tool_operate_btn");
	QHBoxLayout *pHLayout = new QHBoxLayout(m_pHeaderWgt);
	pHLayout->setContentsMargins(0, 0, 5, 0);
	pHLayout->setSpacing(0);
	pHLayout->addWidget(m_pTitleLbl);
	pHLayout->addStretch();
	pHLayout->addWidget(pOperateBtn);

	QVBoxLayout *pVBLayout = new QVBoxLayout(this);
	pVBLayout->setMargin(0);
	pVBLayout->setSpacing(0);
	m_pCentralWgt = new QWidget();
	pVBLayout->addWidget(m_pHeaderWgt);
	pVBLayout->addWidget(m_pCentralWgt);

	// TODO
	connect(pOperateBtn, SIGNAL(clicked()), this, SLOT(onCloseWindowSlot()));
}

CToolBaseWindow::~CToolBaseWindow()
{

}

void CToolBaseWindow::setWindowTitle(const QString &strTitle)
{
	m_pTitleLbl->setText(strTitle);
}

bool CToolBaseWindow::eventFilter(QObject *watched, QEvent *event)
{
	switch (event->type())
	{
		// [1]鼠标Hover移动 begin
	case QEvent::HoverMove:
		{
			QHoverEvent *pHovEvt = dynamic_cast<QHoverEvent *>(event);
			// 如果鼠标没有按下，则根据当前鼠标位置设置鼠标样式
			if (!m_bMousePressed)
			{
				changeMouseStyle(pHovEvt->pos());
			}
		}
		break;
		// [1]鼠标Hover移动 end

		// [2]鼠标按下 begin
	case QEvent::MouseButtonPress:
		{
			QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent *>(event);
			if (mouseEvent->button() == Qt::LeftButton)
			{
				QPoint pressPt = mouseEvent->pos();
				if (m_pHeaderWgt->rect().contains(pressPt) || m_mouseStyle != NORMAL)
				{
					m_bMousePressed = true;
					m_mousePos = mouseEvent->globalPos() - this->frameGeometry().topLeft();
				}
			}
		}
		break;
		// [2]鼠标按下 end

		// [3]鼠标松开 begin
	case QEvent::MouseButtonRelease:
		{
			m_bCanGrabMouse = true;
			m_bMousePressed = false;
			QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent *>(event);
			changeMouseStyle(mouseEvent->pos());
		}
		break;
		// [3]鼠标松开 end

		// [5]鼠标移动 begin
	case QEvent::MouseMove:
		{
			if (m_bMousePressed)
			{
				QMouseEvent *pMouseEvt = dynamic_cast<QMouseEvent *>(event);
				// 鼠标是正常样式时，拖动界面
				if (m_mouseStyle == NORMAL)
				{
					if (windowState().testFlag(Qt::WindowMaximized) || windowState().testFlag(Qt::WindowFullScreen))
					{
						return QWidget::eventFilter(watched, event);
					}
					move(this->mapToGlobal(pMouseEvt->pos()) - m_mousePos);
					return QWidget::eventFilter(watched, event);
				}
				// 否则，进行缩放
				QPoint globalMousePos = mapToGlobal(pMouseEvt->pos());
				QPoint topleftPos = frameGeometry().topLeft();
				QPoint bottomrightPos = frameGeometry().bottomRight();
				switch (m_mouseStyle)
				{
				case BOTTOM:
					if (globalMousePos.y() - topleftPos.y() > minimumHeight())
					{
						bottomrightPos.setY(globalMousePos.y());
					}
					else
					{
						bottomrightPos.setY(topleftPos.y() + minimumHeight());
					}
					break;
				case RIGHT:
					if (globalMousePos.x() - topleftPos.x() > this->minimumWidth())
					{
						bottomrightPos.setX(globalMousePos.x());
					}
					else
					{
						bottomrightPos.setX(topleftPos.x() + minimumWidth());
					}
					break;
				case BOTTOMRIGHT:
					if (globalMousePos.x() - topleftPos.x() > minimumWidth())
					{
						bottomrightPos.setX(globalMousePos.x());
					}
					else
					{
						bottomrightPos.setX(topleftPos.x() + minimumWidth());
					}
					if (globalMousePos.y() - topleftPos.y() > minimumHeight())
					{
						bottomrightPos.setY(globalMousePos.y());
					}
					else
					{
						bottomrightPos.setY(topleftPos.y() + minimumHeight());
					}
					break;
				case TOP:
					if (bottomrightPos.y() - globalMousePos.y() > minimumHeight())
					{
						topleftPos.setY(globalMousePos.y());
					}
					else
					{
						topleftPos.setY(bottomrightPos.y() - minimumHeight());
					}
					break;
				case LEFT:
					if (bottomrightPos.x() - globalMousePos.x() > minimumWidth())
					{
						topleftPos.setX(globalMousePos.x());
					}
					else
					{
						topleftPos.setX(bottomrightPos.x() - minimumWidth());
					}
					break;
				case TOPLEFT:
					if (bottomrightPos.y() - globalMousePos.y() > minimumHeight())
					{
						topleftPos.setY(globalMousePos.y());
					}
					else
					{
						topleftPos.setY(bottomrightPos.y() - minimumHeight());
					}
					if (bottomrightPos.x() - globalMousePos.x() > minimumWidth())
					{
						topleftPos.setX(globalMousePos.x());
					}
					else
					{
						topleftPos.setX(bottomrightPos.x() - minimumWidth());
					}
					break;
				case TOPRIGHT:
					if (bottomrightPos.y() - globalMousePos.y() > minimumHeight())
					{
						topleftPos.setY(globalMousePos.y());
					}
					else
					{
						topleftPos.setY(bottomrightPos.y() - minimumHeight());
					}
					if (globalMousePos.x() - topleftPos.x() > this->minimumWidth())
					{
						bottomrightPos.setX(globalMousePos.x());
					}
					else
					{
						bottomrightPos.setX(topleftPos.x() + minimumWidth());
					}
					break;
				case BOTTOMLEFT:
					if (bottomrightPos.x() - globalMousePos.x() > minimumWidth())
					{
						topleftPos.setX(globalMousePos.x());
					}
					else
					{
						topleftPos.setX(bottomrightPos.x() - minimumWidth());
					}
					if (globalMousePos.y() - topleftPos.y() > minimumHeight())
					{
						bottomrightPos.setY(globalMousePos.y());
					}
					else
					{
						bottomrightPos.setY(topleftPos.y() + minimumHeight());
					}
					break;
				default:
					break;
				}
				setGeometry(QRect(topleftPos, bottomrightPos));
			}
		}
		break;
		// [5]鼠标移动 end

		// [6]窗口大小改变 begin
	case QEvent::Resize:
		{
			m_bCanGrabMouse = false;
		}
		break;
		// [6]窗口大小改变 end

		// [7]鼠标改变 begin
	case QEvent::CursorChange:
		{
			m_bCanGrabMouse = true;
		}
		break;
		// [7]鼠标改变 end

		default:
			break;
	}
	return QWidget::eventFilter(watched, event);
}

void CToolBaseWindow::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);

	QStyleOption opt;
	opt.init(this);
	QPainter painter(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void CToolBaseWindow::changeMouseStyle(const QPoint &mousePos)
{
	bool bResizable = !windowState().testFlag(Qt::WindowMaximized) && !windowState().testFlag(Qt::WindowFullScreen);
	bool bWidthResizable = minimumWidth() != maximumWidth() && bResizable;
	bool bHeightResizable = minimumHeight() != maximumHeight() && bResizable;

	int iPosX = mousePos.x();
	int iPosY = mousePos.y();

	int iWidth = width();
	int iHeight = height();

	if (bWidthResizable && bHeightResizable)
	{
		// 左下
		if (iPosX >= 0 && iPosX < m_iDragMargin &&
			iPosY <= iHeight && iPosY >= iHeight - m_iDragMargin)
		{
			if (m_bCanGrabMouse)
			{
				grabMouse();
			}
			setCursor(Qt::SizeBDiagCursor);
			m_mouseStyle = BOTTOMLEFT;
			return;
		}
		// 右下
		if (iPosX <= iWidth && iPosX >= iWidth - m_iDragMargin &&
			iPosY <= iHeight && iPosY >= iHeight - m_iDragMargin)
		{
			if (m_bCanGrabMouse)
			{
				grabMouse();
			}
			setCursor(Qt::SizeFDiagCursor);
			m_mouseStyle = BOTTOMRIGHT;
			return;
		}
		// 左上
		if (iPosX >= 0 && iPosX < m_iDragMargin &&
			iPosY >= 0 && iPosY < m_iDragMargin)
		{
			if (m_bCanGrabMouse)
			{
				grabMouse();
			}
			setCursor(Qt::SizeFDiagCursor);
			m_mouseStyle = TOPLEFT;
			return;
		}
		// 右上
		if (iPosX < iWidth && iPosX >= iWidth - m_iDragMargin &&
			iPosY >= 0 && iPosY < m_iDragMargin)
		{
			if (m_bCanGrabMouse)
			{
				if (QWidget::mouseGrabber() != nullptr)
				{
					grabMouse();
				}
			}
			setCursor(Qt::SizeBDiagCursor);
			m_mouseStyle = TOPRIGHT;
			return;
		}
	}
	if (bWidthResizable)
	{
		// 左
		if (iPosX >= 0 && iPosX < m_iDragMargin)
		{
			if (m_bCanGrabMouse)
			{
				grabMouse();
			}
			setCursor(Qt::SizeHorCursor);
			m_mouseStyle = LEFT;
			return;
		}
		// 右
		if (iPosX <= iWidth + 5 && iPosX > iWidth - m_iDragMargin)
		{
			if (m_bCanGrabMouse)
			{
				grabMouse();
			}
			setCursor(Qt::SizeHorCursor);
			m_mouseStyle = RIGHT;
			return;
		}
	}
	if (bHeightResizable)
	{
		// 下
		if (iPosY < iHeight && iPosY >= iHeight - m_iDragMargin)
		{
			if (m_bCanGrabMouse)
			{
				grabMouse();
			}
			setCursor(Qt::SizeVerCursor);
			m_mouseStyle = BOTTOM;
			return;
		}
		// 上
		if (iPosY >= 0 && iPosY < m_iDragMargin)
		{
			if (m_bCanGrabMouse)
			{
				grabMouse();
			}
			setCursor(Qt::SizeVerCursor);
			m_mouseStyle = TOP;
			return;
		}
	}
	if (testAttribute(Qt::WA_SetCursor))
	{
		releaseMouse();
		unsetCursor();
	}
	m_mouseStyle = NORMAL;
}

void CToolBaseWindow::onCloseWindowSlot()
{
	close();
}
