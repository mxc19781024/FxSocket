#include "CColorSelectWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include <QRegExpValidator>
#include <QStyleOption>
#include <QPainter>
#include "CBasicColorAreaWidget.h"


CColorSelectWidget::CColorSelectWidget(QWidget *parent)
	: QWidget(parent)
{
	QLabel *pLbl = new QLabel;
	pLbl->setText("#");
	m_pColorEdit = new QLineEdit;
	QRegExp rx("(\\d?[a-f]?){0,6}");
	m_pColorEdit->setValidator(new QRegExpValidator(rx, this));
	connect(m_pColorEdit, SIGNAL(textEdited(const QString &)), this, SLOT(colorEditChangedSlot(const QString &)));
	connect(m_pColorEdit, SIGNAL(editingFinished()), this, SLOT(editFinishedSlot()));

	QHBoxLayout *pHLayout = new QHBoxLayout;
	pHLayout->addWidget(pLbl);
	pHLayout->addWidget(m_pColorEdit);
	pHLayout->addStretch();
	pHLayout->setSpacing(3);

	CBasicColorAreaWidget *pBasicColorArea = new CBasicColorAreaWidget;
	connect(pBasicColorArea, SIGNAL(ColorItemSelcSignal(const QColor &)), this, SLOT(colorItemSelcSlot(const QColor &)));

	QVBoxLayout *pVLayout = new QVBoxLayout(this);
	pVLayout->addLayout(pHLayout);
	pVLayout->addWidget(pBasicColorArea);
	pVLayout->setContentsMargins(8, 7, 11, 11);

	setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
}

CColorSelectWidget::~CColorSelectWidget()
{

}

void CColorSelectWidget::setCurrentColor(const QColor &color)
{
	QString strR = QString::number(color.red(), 16);
	QString strG = QString::number(color.green(), 16);
	QString strB = QString::number(color.blue(), 16);
	QString strRgb = QString("%1%2%3").arg(QString("%1").arg(strR.size() == 1 ? strR.prepend("0") : strR),
		QString("%1").arg(strG.size() == 1 ? strG.prepend("0") : strG),
		QString("%1").arg(strB.size() == 1 ? strB.prepend("0") : strB));
	m_pColorEdit->setText(strRgb);
}

void CColorSelectWidget::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);

	// 避免样式表不生效
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void CColorSelectWidget::colorEditChangedSlot(const QString &strColor)
{
	int r, g, b;
	switch (strColor.length())
	{
	case 0:
		{
			r = g = b = 0;
		}
		break;
	case 1:
	case 2:
		{
			r = g = 0;
			bool ok;
			b = strColor.toInt(&ok, 16);
		}
		break;
	case 3:
		{
			QString strR = QString("%1%2").arg(strColor.left(1), strColor.left(1));
			QString strG = QString("%1%2").arg(strColor.mid(1, 1), strColor.mid(1, 1));
			QString strB = QString("%1%2").arg(strColor.right(1), strColor.right(1));
			bool ok;
			r = strR.toInt(&ok, 16);
			g = strG.toInt(&ok, 16);
			b = strB.toInt(&ok, 16);
		}
		break;
	case 4:
		{
			r = 0;
			QString strG = QString("%1").arg(strColor.left(2));
			QString strB = QString("%1").arg(strColor.right(2));
			bool ok;
			g = strG.toInt(&ok, 16);
			b = strB.toInt(&ok, 16);
		}
		break;
	case 5:
		{
			QString strR = QString("%1").arg(strColor.left(1));
			QString strG = QString("%1").arg(strColor.mid(1, 2));
			QString strB = QString("%1").arg(strColor.right(2));
			bool ok;
			r = strR.toInt(&ok, 16);
			g = strG.toInt(&ok, 16);
			b = strB.toInt(&ok, 16);
		}
		break;
	case 6:
		{
			QString strR = QString("%1").arg(strColor.left(2));
			QString strG = QString("%1").arg(strColor.mid(2, 2));
			QString strB = QString("%1").arg(strColor.right(2));
			bool ok;
			r = strR.toInt(&ok, 16);
			g = strG.toInt(&ok, 16);
			b = strB.toInt(&ok, 16);
		}
		break;
	default:
		break;
	}

	QColor color;
	color.setRgb(r, g, b);
	emit colorChangedSignal(color);
}

void CColorSelectWidget::editFinishedSlot()
{
	QString strColor = m_pColorEdit->text();
	switch (strColor.length())
	{
	case 0:
		{
			m_pColorEdit->setText("000000");
		}
		break;
	case 1:
		{
			m_pColorEdit->setText(strColor.prepend("00000"));
		}
		break;
	case 2:
		{
			m_pColorEdit->setText(strColor.prepend("0000"));
		}
		break;
	case 3:
		{
			QString strR = QString("%1%2").arg(strColor.left(1), strColor.left(1));
			QString strG = QString("%1%2").arg(strColor.mid(1, 1), strColor.mid(1, 1));
			QString strB = QString("%1%2").arg(strColor.right(1), strColor.right(1));
			m_pColorEdit->setText(strR.append(strG).append(strB));
		}
		break;
	case 4:
		{
			QString strG = QString("%1").arg(strColor.left(2));
			QString strB = QString("%1").arg(strColor.right(2));
			m_pColorEdit->setText(strG.append(strB).prepend("00"));
		}
		break;
	case 5:
		{
			QString strR = QString("%1").arg(strColor.left(1));
			QString strG = QString("%1").arg(strColor.mid(1, 2));
			QString strB = QString("%1").arg(strColor.right(2));
			m_pColorEdit->setText(strR.append(strG).append(strB).prepend("0"));
		}
		break;
	default:
		break;
	}
}

void CColorSelectWidget::colorItemSelcSlot(const QColor &color)
{
	setCurrentColor(color);

	emit colorChangedSignal(color);
}
