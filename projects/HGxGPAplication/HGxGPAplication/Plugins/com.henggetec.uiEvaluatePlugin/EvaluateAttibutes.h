//
// Created by HG on 2020/3/26.
//

#ifndef EQUIPMENTATTIBUTES_H
#define EQUIPMENTATTIBUTES_H

#include <QWidget>
namespace Ui
{
class EvaluateAttibutes;
} // namespace Ui


class CEvaluateAttibutes : public QWidget
{
Q_OBJECT
public:
    explicit CEvaluateAttibutes(QWidget *parent = nullptr);

    ~CEvaluateAttibutes() ;

    void setType(QString type);
private Q_SLOTS:

	void TimerUpdateSlot();
protected:
    void paintEvent(QPaintEvent *event) ;
private:

    Ui::EvaluateAttibutes *ui;
};


#endif //SITUATION_DETAIL_SRC_UI_SITUATIONCLARITY_H
