#include "GxAddOGCWindow.h"
#include "GxAddTileWindow.h"
#include "GxAddDataSetWindow.h"
#include "GxAddServiceMgr.h"
//#include "ICore.h"
//#include "CMainWindowManager.h"

#include <GxPluginFrameworkCore.h>
#include <GxPluginFwLauncherInterface.h>
#include "GxUiFrameworkServiceInterface.h"

//using namespace HGxPFWCore;

CGxAddServiceMgr::CGxAddServiceMgr(QWidget *parent)
	: QWidget(parent)
	, m_pAddOGCWindow(NULL)
	, m_pAddDataSetWindow(NULL)
	, m_pAddTileWindow(NULL)
{
	setWindowFlags(Qt::Dialog);
	setWindowModality(Qt::ApplicationModal);

	m_pGuiFrameworkService = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(HGxFWLauncherInstance.getPluginContext());
}

CGxAddServiceMgr::~CGxAddServiceMgr()
{

}


QWidget* CGxAddServiceMgr::creatAddServiceWindow(EWindowType type,  const QString& strType)
{
	switch (type)
	{
	case OGC:
		{
			if (!m_pAddOGCWindow)
			{
 				if (m_pGuiFrameworkService)
 				{
 					// 设置ICore::appWindow为父窗口，继承了其样式表设置
 					m_pAddOGCWindow = new CGxAddOGCWindow(m_pGuiFrameworkService->mainWindow());
 				}
			}
			m_pAddOGCWindow->setCurrentServer(strType);
			return m_pAddOGCWindow;
		}
		break;
	case DATASET:
		{
			if (!m_pAddDataSetWindow)
			{
				if (m_pGuiFrameworkService)
				{
					m_pAddDataSetWindow = new CGxAddDataSetWindow(m_pGuiFrameworkService->mainWindow());
				}
			}
			return m_pAddDataSetWindow;
		}
		break;
	case TITLE_XYZ:
		{
			if (!m_pAddTileWindow)
			{
				if (m_pGuiFrameworkService)
				{
					m_pAddTileWindow = new CGxAddTileWindow("XYZ", m_pGuiFrameworkService->mainWindow());
				}
			}
			m_pAddTileWindow->setCurrentServer("XYZ");
			return m_pAddTileWindow;
		}
		break;
	case TITLE_TMS:
		{
			if (!m_pAddTileWindow)
			{
				if (m_pGuiFrameworkService)
				{
					m_pAddTileWindow = new CGxAddTileWindow("TMS", m_pGuiFrameworkService->mainWindow());
				}
			}
			m_pAddTileWindow->setCurrentServer("TMS");
			return m_pAddTileWindow;
		}
		break;
	default:
		return NULL;
		break;
	}
	return NULL;
}

