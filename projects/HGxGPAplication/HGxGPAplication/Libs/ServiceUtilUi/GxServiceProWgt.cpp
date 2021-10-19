//#include "FreeXServiceUtilUi/CGxServiceProWgt.h"
#include "GxDataSetLayerProWindow.h"
#include "GxOGCLayerProWindow.h"
#include "GxOGCLayerSetProWindow.h"
#include "GxTileLayerProWindow.h"
#include "ui_GxServiceProWgt.h"
//#include <FreeXSysConfig.h>

#include "public/GxServiceProWgt.h"


CGxServiceProWgt::CGxServiceProWgt(EWindowType eType, QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::CGxServiceProWgt)
	, m_pOGCLayerProWindow(NULL)
	, m_pOGCLayerSetProWindow(NULL)
	, m_pDataSetLayerProWindow(NULL)
	, m_pTileLayerProWindow(NULL)
	, m_pUnknowWgt(NULL)
{
	ui->setupUi(this);
	setWindowTitle(tr("property"));
	init();

	setServerType(eType);
	setStyleSheet(styleSheet() + "#scrollAreaWidgetContents{background-color: #333333;}QLabel{color:#ffffff;font-family:Microsoft YaHei;font-size:12px;}");
}

CGxServiceProWgt::~CGxServiceProWgt()
{

}

void CGxServiceProWgt::setServerType(EWindowType type)
{
	int index = -1;
	switch (type)
	{
	case OGC:
		{
			index = ui->stackedWidget->indexOf(m_pOGCLayerProWindow);
		}
		break;
	case OGCSET:
		{
			index = ui->stackedWidget->indexOf(m_pOGCLayerSetProWindow);
		}
		break;
	case DATASET:
		{
			index = ui->stackedWidget->indexOf(m_pDataSetLayerProWindow);
		}
		break;
	case TITLE_XYZ:
	case TITLE_TMS:
		{
			index = ui->stackedWidget->indexOf(m_pTileLayerProWindow);
		}
		break;
	case UNKNOW:
		{
			index = ui->stackedWidget->indexOf(m_pUnknowWgt);
		}
	default:
		break;
	}
	if ( index != -1)
	{
		ui->stackedWidget->setCurrentIndex(index);
	}
}

void CGxServiceProWgt::setLayerProPerty(EWindowType eType, const SLayerPropertyStruct &stLayerProPerty)
{
	switch (eType)
	{
	case OGC:
		{
			if (m_pOGCLayerProWindow)
			{
				m_pOGCLayerProWindow->setLayerProPerty(stLayerProPerty);
			}
		}
		break;
	case DATASET:
		{
			if (m_pDataSetLayerProWindow)
			{
				m_pDataSetLayerProWindow->setLayerProPerty(stLayerProPerty);
			}
		}
		break;
	case TITLE_XYZ:
	case TITLE_TMS:
		{
			if (m_pTileLayerProWindow)
			{
				m_pTileLayerProWindow->setLayerProPerty(stLayerProPerty);
			}
		}
		break;
	default:
		break;
	}
}

void CGxServiceProWgt::setServerProPerty(const SServerPropertyStruct &stServerProPerty)
{
	if (m_pOGCLayerSetProWindow)
	{
		m_pOGCLayerSetProWindow->setServerProPerty(stServerProPerty);
	}
}

void CGxServiceProWgt::setServerName(EWindowType eType, const QString &strName )
{
	switch (eType)
	{
	case OGCSET:
		{
			if (m_pOGCLayerSetProWindow)
			{
				m_pOGCLayerSetProWindow->setServerName(strName);
			}
		}
	case DATASET:
		{
			if (m_pDataSetLayerProWindow)
			{
				m_pDataSetLayerProWindow->setServerName(strName);
			}
		}
		break;
	case TITLE_XYZ:
	case TITLE_TMS:
		{
			if (m_pTileLayerProWindow)
			{
				m_pTileLayerProWindow->setServerName(strName);
			}
		}
		break;
	default:
		break;
	}
}

void CGxServiceProWgt::init()
{
	m_pOGCLayerProWindow = new CGxOGCLayerProWindow(this);
	connect(m_pOGCLayerProWindow, SIGNAL(reLayerSetByProEditSignal(const LayerPropertyStruct & )), this, SIGNAL(reLayerSetByProEditSignal(const LayerPropertyStruct & )));
	m_pOGCLayerSetProWindow = new CGxOGCLayerSetProWindow(this);
	connect(m_pOGCLayerSetProWindow, SIGNAL(reSetByProEditSignal(const ServerPropertyStruct & )), this, SIGNAL(reSetByProEditSignal(const ServerPropertyStruct & )));
	m_pDataSetLayerProWindow = new CGxDataSetLayerProWindow(this);
	connect(m_pDataSetLayerProWindow, SIGNAL(reLayerSetByProEditSignal(const LayerPropertyStruct & )), this, SIGNAL(reLayerSetByProEditSignal(const LayerPropertyStruct & )));
	m_pTileLayerProWindow = new CGxTileLayerProWindow(this);
	connect(m_pTileLayerProWindow, SIGNAL(reLayerSetByProEditSignal(const LayerPropertyStruct & )), this, SIGNAL(reLayerSetByProEditSignal(const LayerPropertyStruct & )));
	
	m_pUnknowWgt = new QWidget(this);
	
	ui->stackedWidget->addWidget(m_pOGCLayerProWindow);
	ui->stackedWidget->addWidget(m_pOGCLayerSetProWindow);
	ui->stackedWidget->addWidget(m_pDataSetLayerProWindow);
	ui->stackedWidget->addWidget(m_pTileLayerProWindow);
	ui->stackedWidget->addWidget(m_pUnknowWgt);

	initSignalsAndSlots();
}

void CGxServiceProWgt::initSignalsAndSlots()
{
}
