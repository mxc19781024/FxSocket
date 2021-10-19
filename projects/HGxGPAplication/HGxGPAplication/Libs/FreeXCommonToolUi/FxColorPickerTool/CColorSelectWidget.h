#ifndef CCOLORSELECTWIDGET_H
#define CCOLORSELECTWIDGET_H

#include <QWidget>
#include <QLineEdit>
////#include <FreeXSysConfig.h>


class CColorSelectWidget : public QWidget
{
	Q_OBJECT

public:
	explicit CColorSelectWidget(QWidget *parent);
	~CColorSelectWidget();
	void setCurrentColor(const QColor &);

protected:
	void paintEvent(QPaintEvent *event) override;

private slots:
	void colorEditChangedSlot(const QString &);
	void editFinishedSlot();
	void colorItemSelcSlot(const QColor &);

signals:
	// 颜色改变信号
	void colorChangedSignal(const QColor &);

private:
	QLineEdit *m_pColorEdit;
};

#endif // CCOLORSELECTWIDGET_H
