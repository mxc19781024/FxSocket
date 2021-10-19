#ifndef GXServiceProWgt_H
#define GXServiceProWgt_H

#include <QWidget>
//#include <FreeXServiceUtilUi/Common.h>
//#include <FreeXServiceUtilUi/CServerProUtils.h>
#include <FxBaseWindow/CFxBaseWindow.h>

#include "Common.h"
#include "GxServerProUtils.h"

class CGxOGCLayerProWindow;	
class CGxOGCLayerSetProWindow;
class CGxDataSetLayerProWindow;
class CGxTileLayerProWindow;

namespace Ui {
	class CGxServiceProWgt;
}

class CGxServiceProWgt : public QWidget
{
	Q_OBJECT

public:

	CGxServiceProWgt(EWindowType eType, QWidget *parent = 0);
	~CGxServiceProWgt();

	void setServerType(EWindowType type);

	void setLayerProPerty(EWindowType eType, const SLayerPropertyStruct &stLayerProPerty);

	void setServerProPerty(const SServerPropertyStruct &stServerProPerty);

	void setServerName(EWindowType eType, const QString &strName);
signals:
	// 通过属性编辑更改服务名称
	void reSetByProEditSignal(const SServerPropertyStruct &stServerPropertyStruct);
	// 通过属性编辑更改图层信息
	void reLayerSetByProEditSignal(const SLayerPropertyStruct &stLayerPropertyStruct);

private:
	void init();

	void initSignalsAndSlots();

private:
	// OGC图层属性窗口
	CGxOGCLayerProWindow*					m_pOGCLayerProWindow;

	// OGC图层集属性窗口
	CGxOGCLayerSetProWindow*				m_pOGCLayerSetProWindow;

	// 数据集图层属性窗口
	CGxDataSetLayerProWindow*				m_pDataSetLayerProWindow;

	// Tile图层属性窗口
	CGxTileLayerProWindow*				m_pTileLayerProWindow;

	// 未知属性窗口
	QWidget*							m_pUnknowWgt;

	Ui::CGxServiceProWgt					*ui;
};

#endif // GXServiceProWgt_H
