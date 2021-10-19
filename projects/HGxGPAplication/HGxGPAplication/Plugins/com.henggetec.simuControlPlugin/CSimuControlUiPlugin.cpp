#include <CSimuControlUiPlugin.h>
#include <CSimuControlWidget.h>



CSimuControlUiPlugin::CSimuControlUiPlugin()
	:m_pSimuControlWgt(nullptr)
{
	if (!m_pSimuControlWgt)
	{
		m_pSimuControlWgt = new CSimuControlWidget;
	}
}

CSimuControlUiPlugin::~CSimuControlUiPlugin()
{
	delete m_pSimuControlWgt;
}

QWidget* CSimuControlUiPlugin::getWgt()
{
	return m_pSimuControlWgt;
}
