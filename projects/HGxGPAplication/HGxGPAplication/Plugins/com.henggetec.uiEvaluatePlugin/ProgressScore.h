//
// Created by HG on 2020/3/27.
//

#ifndef SITUATION_SCORE_SRC_UI_PROGRESSBALL_H
#define SITUATION_SCORE_SRC_UI_PROGRESSBALL_H

#include <QWidget>
#include <QTimer>

class ProgressScore : public QWidget
{
	Q_OBJECT

public:
	explicit ProgressScore(QWidget *parent);

protected:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
private:
	QStringList m_listTopk;
};

#endif //SITUATION_DETAIL_SRC_UI_PROGRESSBALL_H
