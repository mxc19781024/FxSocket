#ifndef PYRAMID_PROCESS_LISTENER_H
#define PYRAMID_PROCESS_LISTENER_H

#include <QObject>
//#include <FreeXGeoUtil/PyramidBuilder.h>

#include "layer_ui/CLayerManager.h"

#include "gdpAlgor/GxPyramidBuilder.h"

/**
* @class	: CPyramidProcessListener
* @brief	: 金字塔缓存处理器
* @note		: 
* @author	: k00026
**/

class CPyramidProcessListener : public QObject, public HGxAlgor::CGxPyramidProcess
{
	Q_OBJECT

public:
	CPyramidProcessListener(QObject *parent = nullptr);

 	// 处理中
 	bool onProcessing(double dPercentComplate) override;

 	// 停止
 	void onFinished() override;

	// 初始化处理
	void initProcess(const QString &name, const QString &path, const QString &driverType, CLayerConfig::LayerType layerType);

	// 是否正在处理
	bool isProcessing() const;

	// 停止处理
	void stopProcessing();

signals:
	void onPyramidProcessSignal(double);
	void onPyramidProcessFinishedSignal(const QString &name, const QString &path, const QString &driverType, int layerType);

private:
	bool m_bContinue;		// 用于控制处理停止/继续
	bool m_bIsProcessing;	// 当前是否正在处理
	QString m_strLayerName;	// 图层名称
	QString m_strPath;		// 数据路径
	QString m_strDriver;	// 驱动名称
	int m_iLayerType;		// 图层类型
};

#endif // !PYRAMID_PROCESS_LISTENER_H
