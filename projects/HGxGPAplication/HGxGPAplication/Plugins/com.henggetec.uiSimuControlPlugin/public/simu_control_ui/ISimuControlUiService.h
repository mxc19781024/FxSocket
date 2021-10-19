/**************************************************************************************************
* @file ISimuControlUiService.h
* @note 【UI】仿真控制
* @author c00037
* @date 2019-3-14
**************************************************************************************************/
#ifndef ISIMU_CONTROL_UI_SERVICE_H
#define ISIMU_CONTROL_UI_SERVICE_H

#include <QObject>
/**
  * @class ISimuControlUiService
  * @brief 【UI】仿真控制
  * @author c00037
*/
class ISimuControlUiService
{
public:
	virtual QWidget* getWgt() = 0;

};

Q_DECLARE_INTERFACE(ISimuControlUiService, "freex_simu_control_ui_service")

#endif
