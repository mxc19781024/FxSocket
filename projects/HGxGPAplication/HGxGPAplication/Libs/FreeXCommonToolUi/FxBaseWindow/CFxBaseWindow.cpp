#include "FxBaseWindow/CFxBaseWindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QEvent>
#include <QHoverEvent>
#include <QFile>
#include <QStyleOption>
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>


#ifdef WIN32
#include <Windows.h>
#include <WinUser.h>
#endif
#include <qscrollbar.h>
#define HEADER_MIN_HEIGHT 28



CFxBaseWindow::CFxBaseWindow(QWidget *parent)
    : QWidget(parent)
    , m_bResizable(true)
    , m_bMousePressed(false)
    , m_iDragMargin(6)
    , m_mouseStyle(NORMAL)
    , m_iDragXPadding(0)
    , m_iDragYPadding(0)
    , m_bBlink(true)
{
#if WIN32
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window );
#else if __linux__
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog );
#endif

    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_Hover);
    installEventFilter(this);


    QMetaObject::connectSlotsByName(this);

    QFile file(":/Resources/qss/FxWidgetSystem.qss");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        setStyleSheet(file.readAll());
    }

    m_pHeaderWgt = new QWidget;
    m_pHeaderWgt->setObjectName("fx_tool_window_header");
    m_pHeaderWgt->setMinimumHeight(HEADER_MIN_HEIGHT);
    m_pHeaderWgt->setMaximumHeight(HEADER_MIN_HEIGHT);
    m_pTitleLbl = new QLabel;
    m_pTitleLbl->setMinimumHeight(HEADER_MIN_HEIGHT);
    m_pCloseBtn = new QPushButton;
    m_pTitleLbl->setObjectName("fx_dock_tool_title_label");
    m_pCloseBtn->setObjectName("fx_dock_tool_operate_btn");
    QHBoxLayout *pHLayout = new QHBoxLayout(m_pHeaderWgt);
    pHLayout->setContentsMargins(6, 2, 17, 0);
    pHLayout->setSpacing(0);
    pHLayout->addWidget(m_pTitleLbl);
    pHLayout->addStretch();
    pHLayout->addWidget(m_pCloseBtn);

    QVBoxLayout *pVBLayout = new QVBoxLayout(this);
    pVBLayout->setMargin(0);
    pVBLayout->setSpacing(0);
    m_pCentralWgt = new QWidget();
    m_pCentralWgt->setObjectName("fx_tool_window_center");
    pVBLayout->addWidget(m_pHeaderWgt);
    pVBLayout->addWidget(m_pCentralWgt);
    pVBLayout->setStretch(1, 1);

    connect(m_pCloseBtn, SIGNAL(clicked()), this, SLOT(onCloseWindowSlot()));
    connect(this, SIGNAL(ncActiveSignal(bool)), this, SLOT(ncActiveSlot(bool)));
}

CFxBaseWindow::~CFxBaseWindow()
{

}

void CFxBaseWindow::setResizable(bool bResizable)
{
    m_bResizable = bResizable;
}

void CFxBaseWindow::setWindowTitle(const QString &strTitle)
{
    m_pTitleLbl->setText(strTitle);
    QWidget::setWindowTitle(strTitle);
}

bool CFxBaseWindow::eventFilter(QObject *watched, QEvent *event)
{
    switch (event->type())
    {
    // [1]鼠标Hover移动 begin
    case QEvent::HoverMove:
    {
        // 如果鼠标没有按下，则根据当前鼠标位置设置鼠标样式 ， 添加m_bResizable判断（不能缩放则鼠标不变形） l180
        if (!m_bMousePressed && m_bResizable)
        {
            QHoverEvent *pHovEvt = dynamic_cast<QHoverEvent *>(event);
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
                QPoint mouseMovePos = pMouseEvt->globalPos();
                QRect availGeo = QApplication::desktop()->availableGeometry(QApplication::desktop()->screenNumber(mouseMovePos));
                if (windowState().testFlag(Qt::WindowMaximized) || windowState().testFlag(Qt::WindowFullScreen))
                {
                    int iNormalWidth = normalGeometry().width();
                    showNormal();
                    m_mousePos.setX(m_mousePos.x() * 1.0 * iNormalWidth / availGeo.width());
                    return QWidget::eventFilter(watched, event);
                }
                mouseMovePos.setX(qBound(availGeo.x(), mouseMovePos.x(), availGeo.x() + availGeo.width()));
                mouseMovePos.setY(qBound(availGeo.y(), mouseMovePos.y(), availGeo.y() + availGeo.height()));
                move(mouseMovePos - m_mousePos);
                return true;
            }
            // 否则，进行缩放
            if (!m_bResizable)
            {
                return QWidget::eventFilter(watched, event);
            }
            QPoint globalMousePos = pMouseEvt->globalPos();
            QPoint topleftPos = frameGeometry().topLeft();
            QPoint bottomrightPos = frameGeometry().bottomRight();

            switch (m_mouseStyle)
            {
            case BOTTOM:
                if (globalMousePos.y() - topleftPos.y() > minimumHeight())
                {
                    bottomrightPos.setY(globalMousePos.y() + m_iDragYPadding);
                }
                else
                {
                    bottomrightPos.setY(topleftPos.y() + minimumHeight());
                }
                break;
            case RIGHT:
                if (globalMousePos.x() - topleftPos.x() > this->minimumWidth())
                {
                    bottomrightPos.setX(globalMousePos.x() + m_iDragXPadding);
                }
                else
                {
                    bottomrightPos.setX(topleftPos.x() + minimumWidth());
                }
                break;
            case BOTTOMRIGHT:
                if (globalMousePos.x() - topleftPos.x() > minimumWidth())
                {
                    bottomrightPos.setX(globalMousePos.x() + m_iDragXPadding);
                }
                else
                {
                    bottomrightPos.setX(topleftPos.x() + minimumWidth());
                }
                if (globalMousePos.y() - topleftPos.y() > minimumHeight())
                {
                    bottomrightPos.setY(globalMousePos.y() + m_iDragYPadding);
                }
                else
                {
                    bottomrightPos.setY(topleftPos.y() + minimumHeight());
                }
                break;
            case TOP:
            {
                bottomrightPos.setY(m_ptBottomRight.y());
                if (bottomrightPos.y() - globalMousePos.y() > minimumHeight())
                {
                    topleftPos.setY(globalMousePos.y() - m_iDragYPadding);
                }
                else
                {
                    topleftPos.setY(bottomrightPos.y() - minimumHeight());
                }
            }
                break;
            case LEFT:
            {
                bottomrightPos.setX(m_ptBottomRight.x());
                if (bottomrightPos.x() - globalMousePos.x() > minimumWidth())
                {
                    topleftPos.setX(globalMousePos.x() - m_iDragXPadding);
                }
                else
                {
                    topleftPos.setX(bottomrightPos.x() - minimumWidth());
                }
            }
                break;
            case TOPLEFT:
            {
                bottomrightPos = m_ptBottomRight;
                if (bottomrightPos.y() - globalMousePos.y() > minimumHeight())
                {
                    topleftPos.setY(globalMousePos.y() - m_iDragYPadding);
                }
                else
                {
                    topleftPos.setY(bottomrightPos.y() - minimumHeight());
                }
                if (bottomrightPos.x() - globalMousePos.x() > minimumWidth())
                {
                    topleftPos.setX(globalMousePos.x() - m_iDragXPadding);
                }
                else
                {
                    topleftPos.setX(bottomrightPos.x() - minimumWidth());
                }

            }
                break;
            case TOPRIGHT:
            {
                bottomrightPos.setY(m_ptBottomRight.y());
                if (bottomrightPos.y() - globalMousePos.y() > minimumHeight())
                {
                    topleftPos.setY(globalMousePos.y() - m_iDragYPadding);
                }
                else
                {
                    topleftPos.setY(bottomrightPos.y() - minimumHeight());
                }

                if (globalMousePos.x() - topleftPos.x() > this->minimumWidth())
                {
                    bottomrightPos.setX(globalMousePos.x() + m_iDragXPadding);
                }
                else
                {
                    bottomrightPos.setX(topleftPos.x() + minimumWidth());
                }
            }
                break;
            case BOTTOMLEFT:
            {
                bottomrightPos.setX(m_ptBottomRight.x());
                if (bottomrightPos.x() - globalMousePos.x() > minimumWidth())
                {
                    topleftPos.setX(globalMousePos.x() - m_iDragXPadding);
                }
                else
                {
                    topleftPos.setX(bottomrightPos.x() - minimumWidth());
                }

                if (globalMousePos.y() - topleftPos.y() > minimumHeight())
                {
                    bottomrightPos.setY(globalMousePos.y() + m_iDragYPadding);
                }
                else
                {
                    bottomrightPos.setY(topleftPos.y() + minimumHeight());
                }
                break;
            }
            default:
                break;
            }
            setGeometry(QRect(topleftPos, bottomrightPos));

        }
        break;
        // [5]鼠标移动 end

    default:
            break;
        }
    }

    return QWidget::eventFilter(watched, event);
}

void CFxBaseWindow::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void CFxBaseWindow::setBlink(bool bBlink)
{
    m_bBlink = bBlink;
}

#ifdef WIN32
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
bool CFxBaseWindow::nativeEvent( const QByteArray &eventType, void *message, long *result )
{
    MSG* msg = (MSG*)message;
    Q_UNUSED(result)
    if (msg->message == WM_NCACTIVATE)
    {
        bool bActive = (bool)(msg->wParam);
        if (m_bBlink)
        {
            emit ncActiveSignal(bActive);
        }
    }

    return false;
}

#else

bool CFxBaseWindow::winEvent( MSG *msg, long *result )
{
    Q_UNUSED(result)
    if(msg->message == WM_NCACTIVATE)
    {
        bool bActive = (bool)(msg->wParam);
        if (m_bBlink)
        {
            emit ncActiveSignal(bActive);
        }
    }

    return false;
}
#endif
#endif

void CFxBaseWindow::changeMouseStyle(const QPoint &mousePos)
{
    if (!m_bResizable)
    {
        setCursor(Qt::ArrowCursor);
        m_mouseStyle = NORMAL;
        return;
    }

    bool bResizable = !windowState().testFlag(Qt::WindowMaximized) && !windowState().testFlag(Qt::WindowFullScreen);
    bool bWidthResizable = minimumWidth() != maximumWidth() && bResizable;
    bool bHeightResizable = minimumHeight() != maximumHeight() && bResizable;

    int iPosX = mousePos.x();
    int iPosY = mousePos.y();

    int iWidth = width();
    //int iHeight = height();
    int iHeight = height() - 8;//取消阴影对鼠标变形的影响 l180
    m_ptBottomRight = this->frameGeometry().bottomRight();
    if (bWidthResizable && bHeightResizable)
    {
        // 左下
        if (iPosX >= 0 && iPosX < m_iDragMargin &&
                iPosY <= iHeight && iPosY >= iHeight - m_iDragMargin)
        {
            m_iDragXPadding = iPosX;
            m_iDragYPadding = iHeight - iPosY;
            setCursor(Qt::SizeBDiagCursor);
            m_mouseStyle = BOTTOMLEFT;
            return;
        }
        // 右下
        if (iPosX <= iWidth && iPosX >= iWidth - m_iDragMargin &&
                iPosY <= iHeight && iPosY >= iHeight - m_iDragMargin)
        {
            m_iDragXPadding = iWidth - iPosX;
            m_iDragYPadding = iHeight - iPosY;
            setCursor(Qt::SizeFDiagCursor);
            m_mouseStyle = BOTTOMRIGHT;
            return;
        }
        // 左上
        if (iPosX >= 0 && iPosX < m_iDragMargin &&
                iPosY >= 0 && iPosY < m_iDragMargin)
        {
            m_iDragXPadding = iPosX;
            m_iDragYPadding = iPosY;
            setCursor(Qt::SizeFDiagCursor);
            m_mouseStyle = TOPLEFT;
            return;
        }
        // 右上
        if (iPosX < iWidth && iPosX >= iWidth - m_iDragMargin &&
                iPosY >= 0 && iPosY < m_iDragMargin)
        {
            m_iDragXPadding = iWidth - iPosX;
            m_iDragYPadding = iPosY;
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
            m_iDragXPadding = iPosX;
            setCursor(Qt::SizeHorCursor);
            m_mouseStyle = LEFT;
            return;
        }
        // 右
        if (iPosX <= iWidth + 5 && iPosX > iWidth - m_iDragMargin)
        {
            m_iDragXPadding = iWidth - iPosX;
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
            m_iDragYPadding = iHeight - iPosY;
            setCursor(Qt::SizeVerCursor);
            m_mouseStyle = BOTTOM;
            return;
        }
        // 上
        if (iPosY >= 0 && iPosY < 8)
        {
            m_iDragYPadding = iPosY;
            setCursor(Qt::SizeVerCursor);
            m_mouseStyle = TOP;
            return;
        }
    }
    setCursor(Qt::ArrowCursor);
    m_mouseStyle = NORMAL;
}

void CFxBaseWindow::onCloseWindowSlot()
{
    close();
}

void CFxBaseWindow::ncActiveSlot( bool bActive )
{
    // 判断是否为模态窗口
    if(isModal())
    {
        // 标题栏设置显隐闪烁
        if (bActive)
        {
            m_pTitleLbl->setVisible(true);
            m_pCloseBtn->setVisible(true);
        }
        else
        {
            m_pTitleLbl->setVisible(false);
            m_pCloseBtn->setVisible(false);
        }
    }
}
