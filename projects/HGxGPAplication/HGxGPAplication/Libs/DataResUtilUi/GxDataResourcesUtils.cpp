#include "public/GxDataResourcesUtils.h"
#include "public/GxLinkFileWindow.h"
#include "public/GxServerDataWindow.h"



/*CGxDataResourcesUtils* CGxDataResourcesUtils::s_pDataResourcesUtils = NULL;*/
CGxServerDataWindow* CGxDataResourcesUtils::s_pServerDataWindow2D = NULL;//将二三维窗口分开 l180
CGxLinkFileWindow* CGxDataResourcesUtils::s_pLinkFileWindow = NULL;

CGxServerDataWindow* CGxDataResourcesUtils::s_pServerDataWindow3D = NULL;//将二三维窗口分开 l180

CGxLinkFileWindow* CGxDataResourcesUtils::getDataResourceWindow(QWidget *parent)
{
	if (!s_pLinkFileWindow)
	{
		s_pLinkFileWindow = new CGxLinkFileWindow(parent);
	}
	return s_pLinkFileWindow;
}

CGxServerDataWindow* CGxDataResourcesUtils::getServerDataWindow2D(QWidget *parent)
{

	if (!s_pServerDataWindow2D)
	{
		s_pServerDataWindow2D = new CGxServerDataWindow(parent);;
	}
	return s_pServerDataWindow2D;
}
CGxServerDataWindow* CGxDataResourcesUtils::getServerDataWindow3D(QWidget *parent)
{

	if (!s_pServerDataWindow3D)
	{
		s_pServerDataWindow3D = new CGxServerDataWindow(parent);
	}
	return s_pServerDataWindow3D;
}
CGxDataResourcesUtils::CGxDataResourcesUtils(QAbstractItemModel *pModel, QWidget *parent )
{

}

CGxDataResourcesUtils::~CGxDataResourcesUtils()
{
	
}