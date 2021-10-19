//
// Created by HG on 2020/3/26.
//

#ifndef EQUIPMENT_SETTING_H
#define EQUIPMENT_SETTING_H

#include <QWidget>
namespace Ui
{
class EquipmentSetting;
} // namespace Ui


class CEquipmentSetting : public QWidget
{
	Q_OBJECT
public:
	explicit CEquipmentSetting(QWidget *parent = nullptr);

	~CEquipmentSetting() ;

	void setType(QString type);
	/**
	* @brief 设置设备温度值
	* @param 参数 dTM  温度值
	* @return 返回值  无
	*/
	void setEquipmentTemperatureData(double dTM);
	/**
	* @brief 设置设备电量
	* @param 参数 dTM  温度值
	* @return 返回值  无
	*/
	void setEquipmentPowerData(double dTM);
	/**
	* @brief 设置设备压力
	* @param 参数 dTM  温度值
	* @return 返回值  无
	*/
	void setEquipmentPressData(double dTM);
private Q_SLOTS:
	
protected:
	void paintEvent(QPaintEvent *event) ;
private:
	Ui::EquipmentSetting *ui;
};

#endif //SITUATION_DETAIL_SRC_UI_SITUATIONCLARITY_H
