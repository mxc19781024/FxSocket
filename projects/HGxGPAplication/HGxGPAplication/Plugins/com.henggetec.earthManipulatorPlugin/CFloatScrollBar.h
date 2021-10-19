#ifndef FLOAT_SCROLLBAR_H
#define FLOAT_SCROLLBAR_H

#include <QScrollBar>

class CFloatScrollBar : public QScrollBar
{
    Q_OBJECT
public:
    explicit CFloatScrollBar(QWidget *parent = 0);
    ~CFloatScrollBar();
    QSize sizeHint() const override;

	// 设置透明度
	void setScrollBarOpacity(double opacity);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
	double m_dfOpacity;		// 透明度
};

#endif // FLOAT_SCROLLBAR_H
