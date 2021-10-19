/**************************************************************************************************
* @file CTiltModelWgt.h
* @note 倾斜摄影模型属性窗口，设置、获取倾斜摄影模型属性
* @author w00040
* @data 2019-5-10
**************************************************************************************************/
#ifndef TILT_MODEL_WGT_H
#define TILT_MODEL_WGT_H

#include <QWidget>
//#include <earth_layer/IFxEarthLayer.h>
#include "ui_CTiltModelWgt.h"

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
  * @class CTiltModelWgt
  * @brief 倾斜摄影模型属性窗口
  * @author w00040
*/
class CTiltModelWgt : public CFxBaseWindow
{
    Q_OBJECT

public:
    CTiltModelWgt(bool bIsAddTree = false, HGxScene::CGxGlobalLayer* pLayer = NULL, QWidget *parent = 0);
    ~CTiltModelWgt();

	void setCurrentLayer(HGxScene::CGxGlobalLayer* pLayer);

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
	  * @brief 数据路径
	*/
	void BrowseDataPath();

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

	/**  
	  * @brief 更新属性信息
	*/
	void updateInfo();

signals:
	/**  
	  * @brief 图层名称更改信号
	*/
	void layerNameChangedSignal(const QString &strLayerName);

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
	  * @brief 路径编辑槽函数
	*/
	void pathChangedSlot(QString strPath);

	/**  
	  * @brief 经度改变槽函数
	*/
	void lonChangedSlot(double dLon);

	/**  
	  * @brief 纬度改变槽函数
	*/
	void latChangedSlot(double dLat);

	/**  
	  * @brief 高度改变槽函数
	*/
	void heightChangedSlot(int nHeight);

	/**  
	  * @brief 缩放改变槽函数
	*/
	void scaleChangedSlot(int nScale);

	/**  
	  * @brief 旋转角度改变槽函数
	*/
	void rotateChangedSlot(double dRotate);

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
	  * @brief 路径单选按钮改变槽函数
	*/
	void radioBtnChangedSlot();

private:
    Ui::CTiltModelWgt						ui;

 	// 三维实景图层
 	HGxScene::CGxLayerLive*                 m_pLiveLayer;

	// 实景图层显隐状态标识
	bool									m_bStatus;

	bool                                    m_bAddTreeStatus;
};


#endif
