#ifndef CDRAGTREEVIEW_H
#define CDRAGTREEVIEW_H

#include <QObject>
#include <QTreeView>
#include <QMouseEvent>
#include <QMimeData>

#include "public/ISceneDropInterface.h"
#include "public/Common.h"

class CDragTreeView : public QTreeView, public ISceneDropInterface
{
	Q_OBJECT

public:
	CDragTreeView(QWidget *parent = 0);
	~CDragTreeView();

 	void handleDrop(std::string, const QPoint &) override;
 
 	QString mimeType() const override;

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

	QPointF m_startPos;
	QDataStream m_dataStream;

	QString m_strLayerName;
	QString m_strDriveType;
	QString m_strUrl;
	QString m_strFormat;
	int m_iLayerType;
	bool m_bIsLocalFile;
};

#endif    // CDRAGTREEVIEW_H