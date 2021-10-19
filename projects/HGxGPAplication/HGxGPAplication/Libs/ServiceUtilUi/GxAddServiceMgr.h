#ifndef GxAddServiceMgr_H
#define GxAddServiceMgr_H

#include <QWidget>
//#include "FreeXServiceUtilUi/Common.h"
#include "public/Common.h"

class CGxAddOGCWindow;			
class CGxAddDataSetWindow;	
class CGxAddTileWindow;
class CGxUiFrameworkServiceInterface;

class CGxAddServiceMgr : public QWidget
{
	Q_OBJECT

public:
	CGxAddServiceMgr(QWidget *parent = 0);
	~CGxAddServiceMgr();

	QWidget* creatAddServiceWindow(EWindowType type,  const QString& strType);

private:
	void init();

private:

	CGxAddOGCWindow*						m_pAddOGCWindow;

	CGxAddDataSetWindow*					m_pAddDataSetWindow;

	CGxAddTileWindow*						m_pAddTileWindow;

	CGxUiFrameworkServiceInterface*         m_pGuiFrameworkService;
};

#endif // GxAddServiceMgr_H
