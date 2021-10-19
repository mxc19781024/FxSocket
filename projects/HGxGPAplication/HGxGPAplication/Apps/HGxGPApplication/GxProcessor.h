#ifndef GX_PROCESSOR_H
#define GX_PROCESSOR_H

#include <QObject>
#include <service/event/ctkEventHandler.h>

class QStackedWidget;

class CGxProcessor : public QObject, public ctkEventHandler
{
    Q_OBJECT
        Q_INTERFACES(ctkEventHandler)

public:
    CGxProcessor(QObject *parent = nullptr);

protected:
    void handleEvent(const ctkEvent &event);

private slots:

private:
	QStackedWidget* m_pLayerTreeStackedWgt;
	QStackedWidget* m_pSceneToolStackedWgt;
};

#endif	//GX_PROCESSOR_H