#ifndef GXDATARESOURCESWINDOW_H
#define GXDATARESOURCESWINDOW_H

#include <QObject>
#include <QtCore/qglobal.h>
#include <QAbstractItemModel>
//#include <FreeXSysConfig.h>
//#include "FreeXServiceUtilUi/Common.h"

#include "Common.h"

#ifdef GXDATARESOURCESWINDOW_LIB
# define GXDATARESOURCESWINDOW_EXPORT Q_DECL_EXPORT
#else
# define GXDATARESOURCESWINDOW_EXPORT Q_DECL_IMPORT
#endif

class CGxLinkFileWindow;
class CGxServerDataWindow;

class GXDATARESOURCESWINDOW_EXPORT CGxDataResourcesUtils: public QObject
{
	Q_OBJECT
public:
	
	/**  
	  * @brief 
	  * @param
	  * @param
	*/
	static CGxLinkFileWindow* getDataResourceWindow(QWidget *parent = 0);

	/**  
	  * @brief 
	  * @param
	  * @param
	*/
	static CGxServerDataWindow* getServerDataWindow2D(QWidget *parent = 0);//将二三维窗口分开 l180

	/**  
	  * @brief 
	  * @param
	  * @param
	*/
	static CGxServerDataWindow* getServerDataWindow3D(QWidget *parent = 0);//将二三维窗口分开 l180


private:
	CGxDataResourcesUtils(QAbstractItemModel *pModel, QWidget *parent = 0);
	~CGxDataResourcesUtils();

public:
	static CGxLinkFileWindow*						s_pLinkFileWindow;

	static CGxServerDataWindow*					s_pServerDataWindow2D;
	static CGxServerDataWindow*					s_pServerDataWindow3D;

	/*static CGxDataResourcesUtils*					s_pDataResourcesUtils;*/
};

#endif // GXDATARESOURCESWINDOW_H
