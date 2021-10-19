#include "FxColorPickerTool/FxColorSelector.h"

#include "CColorSelectWidget.h"
#include "FxColorSelectorPrivate.h"
#include <QPainter>
#include <QEvent>


FxColorSelectorPrivate::FxColorSelectorPrivate(FxColorSelector *q)
	: q_ptr(q)
{
	m_pColorSelcWgt = new CColorSelectWidget(q);
	m_pColorSelcWgt->setObjectName("fx_color_picker_bg");
	connect(m_pColorSelcWgt, SIGNAL(colorChangedSignal(const QColor &)), this, SLOT(colorChangedSlot(const QColor &)));
	m_pColorSelcWgt->hide();

	q->installEventFilter(this);
}

FxColorSelectorPrivate::~FxColorSelectorPrivate()
{

}

bool FxColorSelectorPrivate::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == q_ptr)
	{
		switch (event->type())
		{
		case QEvent::MouseButtonPress:
			{
				m_pColorSelcWgt->move(q_ptr->mapToGlobal(QPoint(-2, q_ptr->height() - 1)));
				m_pColorSelcWgt->show();
			}
			break;
		case QEvent::Paint:
			{
				QPainter painter(q_ptr);

				painter.setRenderHints(QPainter::Antialiasing);
				painter.setPen(Qt::NoPen);
				painter.setBrush(m_color);
				painter.drawRoundedRect(q_ptr->rect(), 4 , 4);
			}
			break;
		default:
			break;
		}
	}
	return QObject::eventFilter(obj, event);
}

void FxColorSelectorPrivate::colorChangedSlot(const QColor &color)
{
	m_color = color;
	q_ptr->update();
	q_ptr->colorChangedSignal(color);
}

FxColorSelector::FxColorSelector(QWidget *parent, const QColor &color)
	: QWidget(parent)
	, d_ptr(new FxColorSelectorPrivate(this))
{
	setStyleSheet("CColorSelectWidget{background:transparent;border-image:url(:/images_colorpicker/Resources/bg.png) 1 7 8 5; border-width:1 7 8 5;}\
				  QLabel{background:transparent;}");
	setCurrentColor(color);
	
}

FxColorSelector::~FxColorSelector()
{

}

void FxColorSelector::setCurrentColor(const QColor &color)
{
	d_ptr->m_color = color;
	update();
	d_ptr->m_pColorSelcWgt->setCurrentColor(color);
}

QColor FxColorSelector::getCurrentColor() const
{
	return d_ptr->m_color;
}
