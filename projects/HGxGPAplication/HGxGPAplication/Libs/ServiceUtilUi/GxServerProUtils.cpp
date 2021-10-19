#include "GxAddServiceMgr.h"

#include "public/GxServerProUtils.h"
#include "public/GxServiceProWgt.h"

#include <QApplication>
#include <QDesktopWidget>

#include <GxPluginFrameworkCore.h>
#include <GxPluginFwLauncherInterface.h>

#include <GxUiFrameworkServiceInterface.h>


CGxServerProUtils* CGxServerProUtils::s_pServerProUtils = NULL;


void CGxServerProUtils::showAddServiceWindow(const QPoint &point, EWindowType type, QString strType )
{
	QWidget *pAddServiceWgt = createAddServiceWindow(type, strType);
	if (pAddServiceWgt)
	{
		pAddServiceWgt->show();
		pAddServiceWgt->move(point);
	}
}


void CGxServerProUtils::showAddServiceWindow( EWindowType type, QString strType )
{
	QWidget *pAddServiceWgt = createAddServiceWindow(type, strType);
	if (pAddServiceWgt)
	{
		pAddServiceWgt->show();
		QPoint point = QApplication::desktop()->rect().center();
		pAddServiceWgt->move(point.x() - pAddServiceWgt->width() / 2, point.y() - pAddServiceWgt->height() / 2);
	}
}

void CGxServerProUtils::showServiceProWindow(EWindowType type)
{
	CGxServiceProWgt *pServiceProWgt = createServiceProWindow(type);
	if (pServiceProWgt)
	{
		pServiceProWgt->show();
		QPoint point = QApplication::desktop()->rect().center();
		pServiceProWgt->move(point.x() - pServiceProWgt->width() / 2, point.y() - pServiceProWgt->height() / 2);
	}
}

CGxServiceProWgt* CGxServerProUtils::getServiceProWindow( EWindowType type )
{
	CGxServiceProWgt *pServiceProWgt = createServiceProWindow(type);
	return pServiceProWgt;
}

void CGxServerProUtils::setLayerProperty(EWindowType eType, const SLayerPropertyStruct &stLayerProPerty)
{
	m_pServiceProWgt = createServiceProWindow(eType);
	m_pServiceProWgt->setLayerProPerty(eType, stLayerProPerty);
}

void CGxServerProUtils::setServerProPerty(const SServerPropertyStruct &stServerProPerty)
{

	m_pServiceProWgt = createServiceProWindow(OGCSET);
	m_pServiceProWgt->setServerProPerty(stServerProPerty);
}



CGxServerProUtils::CGxServerProUtils(QWidget *parent)
	: m_pParent(NULL)
{
	init();
	m_pParent = parent;
}

CGxServerProUtils::~CGxServerProUtils()
{

}

void CGxServerProUtils::init()
{
	m_pServiceProWgt = NULL;
	m_pAddServiceMgr = new CGxAddServiceMgr(m_pParent);
}


void CGxServerProUtils::setServerName(EWindowType eType, const QString &strServerName)
{
	if (!m_pServiceProWgt)
	{
		createServiceProWindow(eType);
	}
	
	m_pServiceProWgt->setServerName(eType, strServerName);
}

CGxServiceProWgt* CGxServerProUtils::createServiceProWindow(EWindowType type)
{
	if (!m_pServiceProWgt)
	{
		CGxUiFrameworkServiceInterface* pUiInterface = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(HGxFWLauncherInstance.getPluginContext());

		if (NULL == pUiInterface)
		{
			return NULL;
		}

		m_pServiceProWgt = new CGxServiceProWgt(type, pUiInterface->mainWindow());
		connect(m_pServiceProWgt, SIGNAL(reLayerSetByProEditSignal(const LayerPropertyStruct & )), this, SIGNAL(reLayerSetByProEditSignal(const LayerPropertyStruct & )));
		connect(m_pServiceProWgt, SIGNAL(reSetByProEditSignal(const ServerPropertyStruct & )), this, SIGNAL(reSetByProEditSignal(const ServerPropertyStruct & )));
	}

	m_pServiceProWgt->setServerType(type);
	return m_pServiceProWgt;
}

QWidget* CGxServerProUtils::createAddServiceWindow(EWindowType type,QString strType)
{
	QWidget *pBaseWgt = NULL;
	if (m_pAddServiceMgr)
	{
		CGxAddServiceMgr *pMgr = static_cast<CGxAddServiceMgr *>(m_pAddServiceMgr);
		pBaseWgt = pMgr->creatAddServiceWindow(type,strType);
	}
	return pBaseWgt;
}

bool CGxServerProUtils::bProWgtVisible()
{
	if (!m_pServiceProWgt)
	{
		return false;
	}
	else
	{
		return m_pServiceProWgt->isVisible();
	}
	return false;
}
