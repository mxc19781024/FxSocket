/**************************************************************************************************
* @file CTiltToolWgt.h
* @note 倾斜摄影格式转化工具界面
* @author l180
* @data 2019-7-5
**************************************************************************************************/
#ifndef TILT_TOOL_WGT_H
#define TILT_TOOL_WGT_H

#include <QWidget>
#include <layer_ui/CLayerManager.h>
//#include <earth_layer/IFxEarthLayer.h>
//#include <earth_layer/IFxLayer.h>
#include <FxBaseWindow/CFxBaseWindow.h>


#include "ui_CTiltToolWgt.h"

/**
  * @class CTiltToolWgt
  * @brief 倾斜摄影模型属性窗口
  * @author l180
*/
class CTiltToolWgt : public CFxBaseWindow
{
    Q_OBJECT

public:
    CTiltToolWgt(QWidget *parent = 0);
    ~CTiltToolWgt();
signals:
	void closeSignals();


protected slots:
	/**  
	  * @brief 路径按钮槽函数
	*/
	void pathBtnChangedSlot();
	/**  
	  * @brief 路径输入框改变槽函数
	*/
	void pathChangedSlot(QString path);
	/**  
	  * @brief 保存按钮槽函数
	*/
	void saveBtnClickedSlot();

	/**  
	  * @brief 高级选项按钮槽函数
	*/
	void advancedPushBtnChangedSlot();
	/**  
	  * @brief 切换坐标系槽函数
	*/
	void changeAxisBtnChangedSlot();
	/**  
	  * @brief 像素按钮切槽函数
	*/
	void pixelPushBtnSlot(bool);
	/**  
	  * @brief 视距按钮切换槽函数
	*/
	void distancePushBtnSlot(bool);

private:
	/**  
	  * @brief 设置界面信息
	*/
	void initWidgetInfo();

	/**  
	  * @brief 初始化信号和槽
	*/
	void initSignalAndSlot();

	/**  
	  * @brief 数据路径
	*/
	void BrowseDataPath();
	/**  
	  * @brief 解析路径
	*/
	void analysisFilePath(const QString filePath) ;
	/**  
	  * @brief 解析Xml文件
	*/
	void analysisXmlFile(const char* strXmlName);
	/**  
	  * @brief 解析Scp文件
	*/
	void analysisScpFile(const char* strScpName);
	/**  
	  * @brief 解析Lfp文件
	*/
	void analysisLfpFile(const char* strLfpName);
	/**  
	  * @brief 通过路径获取其中的osbg文件名列表
	*/
	QList<std::string> getOsgbFileNames();
private:
    Ui::CTiltToolWgt			ui;
	/// 是否打开高级选项
	bool                        m_bIsOpenAdvance;
	/// 是否是地理坐标系（否为世界坐标系）
	bool                        m_bIsLLH;
	///是否是像素模式
	bool                        m_bIsPixs;
	QString                     m_strPath;


};


#endif
