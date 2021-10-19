//
// Created by HG on 2020/4/8.
//

#ifndef INFORMATION_FLOW_UI_SRC_UI_CHART_PROGRESSCIRCLE00_H
#define INFORMATION_FLOW_UI_SRC_UI_CHART_PROGRESSCIRCLE00_H

#include <QWidget>
#include <QTime>
#include <QTimer>
class ProgressCircle00 : public QWidget
{
public:
    ProgressCircle00(QWidget *parent = nullptr);

    void setValue(size_t value) { m_value = value; }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    size_t m_value;
    float m_frameTime;
    QTimer m_timer;
};


#endif //INFORMATION_FLOW_UI_SRC_UI_CHART_PROGRESSCIRCLE00_H
