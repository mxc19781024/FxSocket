/**************************************************************************************************
* @file CTiltPropertyWgt.h
* @note 倾斜摄影模型属性窗口，设置、获取倾斜摄影模型属性
* @author w00040
* @data 2019-5-10
**************************************************************************************************/
#ifndef TILT_PROPERTY_WGT_H
#define TILT_PROPERTY_WGT_H

#include <QWidget>
#include "ui_CTiltPropertyWgt.h"

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
  * @class CTiltPropertyWgt
  * @brief 倾斜摄影模型属性窗口
  * @author w00040
*/
class CTiltPropertyWgt : public QWidget
{
    Q_OBJECT

public:

    CTiltPropertyWgt( HGxScene::CGxGlobalLayer* pLayer = 0, QWidget *parent = 0);
    ~CTiltPropertyWgt();

	/**
	* @brief 获取实景图层
	*/
	HGxScene::CGxGlobalLayer* getCurrentLayer();

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
	  * @brief 设置图层 l180
	*/
	void setFeLiveLayer(HGxScene::CGxGlobalLayer* pLayer);
private:
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
signals:
	/**  
	  * @brief 图层名称更改信号
	*/
	void layerNameChangedSignal(QString strLayerName);

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
	void scaleChangedSlot(double nScale);

	/**  
	  * @brief 旋转角度改变槽函数
	*/
	void rotateChangedSlot(double dRotate);

	/**  
	  * @brief 世界坐标高度
	*/
	void highChangedSlot(double dHigh);

private:
    Ui::CTiltPropertyWgt						ui;

 	// 三维实景图层
 	HGxScene::CGxLayerLive*		m_pLiveLayer;

	// 实景图层显隐状态标识
	bool									m_bStatus;
	/// 是否是地理坐标系（否为世界坐标系）
	bool                        m_bIsLLH;
	///是否是像素模式
	bool                        m_bIsPixs;
};


#endif
