#include "CPyramidProcessListener.h"




CPyramidProcessListener::CPyramidProcessListener(QObject *parent)
	: QObject(parent)
	, m_bContinue(true)
	, m_bIsProcessing(false)
{

}

bool CPyramidProcessListener::onProcessing(double dPercentComplate)
{
	emit onPyramidProcessSignal(dPercentComplate);
	return m_bContinue;
}

void CPyramidProcessListener::onFinished()
{
	m_bIsProcessing = false;
	emit onPyramidProcessFinishedSignal(m_strLayerName, m_strPath, m_strDriver, m_iLayerType);
}

void CPyramidProcessListener::initProcess(const QString &strName, const QString &strPath, const QString &strDriverType, CLayerConfig::LayerType layerType)
{
	m_bContinue = true;
	m_bIsProcessing = true;
	m_strLayerName = strName;
	m_strPath = strPath;
	m_strDriver = strDriverType;
	m_iLayerType = layerType;
}

bool CPyramidProcessListener::isProcessing() const
{
	return m_bIsProcessing;
}

void CPyramidProcessListener::stopProcessing()
{
	m_bContinue = false;
	m_bIsProcessing = false;
}
