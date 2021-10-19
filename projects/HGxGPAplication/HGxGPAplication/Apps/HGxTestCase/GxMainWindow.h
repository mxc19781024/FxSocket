#pragma once

#include <QWidget>
#include "ui_GxMainWindow.h"

#include <service/event/ctkEventHandler.h>

class CGxMainWindow : public QWidget, public ctkEventHandler
{
    Q_OBJECT
        Q_INTERFACES(ctkEventHandler)
public:
    CGxMainWindow(QWidget *parent = nullptr);



protected:
    void handleEvent(const ctkEvent &event);

private:
    void initFramework();

    void initTestEvent();

private:
    Ui::GxMainWindowClass ui;
};
