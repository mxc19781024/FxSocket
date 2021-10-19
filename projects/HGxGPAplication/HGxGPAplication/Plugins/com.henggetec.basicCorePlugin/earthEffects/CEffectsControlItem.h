#ifndef CEFFECTSCONTROLITEM_H
#define CEFFECTSCONTROLITEM_H

#include <QWidget>
#include "ui_CEffectsControlItem.h"

#define IMAGE_PATH ":/images/Resources/images/"

//特效控制项
class CEffectsControlItem : public QWidget
{
	Q_OBJECT

public:
	CEffectsControlItem(QWidget *parent = 0);
	~CEffectsControlItem();

	/**  
	* @brief 设置开关状态
	* @param bChecked [in] 开启为true，关闭为false
	*/
	void setSwitchOn(bool bChecked);
	
	/**  
	* @brief 获取开关状态
	* @return 返回开关的状态
	*/
	bool getSwitchState();

	/**  
	* @brief 设置按钮tooltip
	* @param strTip [in] 按钮的tooltip内容
	*/
	void setBtnToolTip(QString &strTip);
	
	/**  
	* @brief 设置项名称
	* @param bChecked [in] 开启为true，关闭为false
	*/
	void setItemName(const QString &strName, const QString &strIconName);
	
	/**  
	* @brief 获取项名称
	* @return 返回项名称
	*/
	const QString & getItemName(){return m_strName;}

private slots:
	/**  
	* @brief 点击开关响应的槽函数
	* @param bChecked [in] 点击后开关的状态，true为开启，false为关闭
	* @return 返回项名称
	*/
	void btnSwitchedSlot(bool bChecked);

signals:
	/**  
	* @brief 发送按钮点击后的信号
	* @param strName [in] 按钮名称
	* @param bChecked [in] 按钮的开启和关闭状态
	* @return 返回项名称
	*/
	void ItemChecked(const QString &strName, bool bChecked);
private:
	Ui::CEffectsControlItem ui;

	QString m_strName;
};

#endif // CEFFECTSCONTROLITEM_H
