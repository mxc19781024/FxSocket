/**************************************************************************************************
* @file CTiltAddModelWgt.h
* @note 倾斜摄影模型属性窗口，设置、获取倾斜摄影模型属性
* @author w00040
* @data 2019-5-10
**************************************************************************************************/
#ifndef TILT_ADDMODEL_WGT_H
#define TILT_ADDMODEL_WGT_H

#include <QWidget>
#include "ui_CTiltAddModelWgt.h"

#include <layer_ui/CLayerManager.h>
//#include "earth_layer/IFxEarthLayer.h"
//#include <earth_layer/IFxLayer.h>
#include "FxBaseWindow/CFxBaseWindow.h"

namespace HGxScene
{
	class CGxGlobalLayer;
	class CGxLayerLive;
}


/**
  * @class CTiltAddModelWgt
  * @brief 倾斜摄影模型属性窗口
  * @author w00040
*/
class CTiltAddModelWgt : public CFxBaseWindow
{
    Q_OBJECT

public:
    CTiltAddModelWgt(bool bIsAddTree, HGxScene::CGxGlobalLayer* pLayer, QWidget *parent = 0);
    ~CTiltAddModelWgt();

	/**  
	  * @brief 设置界面信息
	*/
	void setWidgetInfo();

	/**  
	  * @brief 初始化信号和槽
	*/
	void initSignalAndSlot();

	/**  
	  * @brief 配置文件路径
	*/
	void BrowseConfigPath();

	

	/**  
	  * @brief 设置显隐状态，true是显示，false是不显示
	*/
	void setTileModelShow(bool bStatus);

	/**  
	  * @brief 获取显隐状态
	*/
	bool getTileModelShow();

	/**  
	  * @brief 设置名称
	*/
	void setTiltModelName(QString strName);

signals:
	/**  
	  * @brief 图层名称更改信号
	*/
	void layerNameChanged(QString strLayerName);

	void linkTiltToolSignal();

	void closeSignals();

protected slots:
	/**  
	  * @brief 名称改变槽函数
	*/
	void nameChangedSlot(QString strName);

	/**  
	  * @brief 描述改变槽函数
	*/
	void descChangedSlot();
	/**  
	  * @brief 路径按钮槽函数
	*/
	void pathBtnChangedSlot();

	/**  
	  * @brief 保存按钮槽函数
	*/
	void saveBtnClickedSlot();

	/**  
	  * @brief 取消按钮槽函数
	*/
	void cancleBtnClickedSlot();

	/**  
	  * @brief 连接到转换工具
	*/
	void linkTiltToolSlot();

	void dataFilePushBtn(bool);
	void httpDirPushBtn(bool);
	void httpBtnChangedSlot();
	void httpChangedSlot(QString);

private:
    Ui::CTiltAddModelWgt				    ui;
	    

 	// 三维实景图层
 	HGxScene::CGxLayerLive*					m_pLiveLayer;

	// 实景图层显隐状态标识
	bool									m_bStatus;
	
	bool                                    m_bAddTreeStatus;

	bool                                    m_bIsDataFileType;

	//标记是否加载过当前模式的数据
	bool                                    m_bIsLoadedFile;
};


#endif
