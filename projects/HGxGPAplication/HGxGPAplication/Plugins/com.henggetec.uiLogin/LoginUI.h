/**************************************************************************************************
* @file LoginUI.h
* @note 登录窗口
* @author L00341
* @data 2020-5-11
**************************************************************************************************/
#ifndef LOGIN_UI_H
#define LOGIN_UI_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QPixmap>
#include <QMouseEvent>

#include "ctkPluginContext.h"
#include "service/event/ctkEventHandler.h"
#include "QProgressBar"
#include <QtNetwork\QHostAddress>
#include <QtNetwork\QUdpSocket>


struct SLoginData
{
	int		        iValue; //实体ID
	std::string		strMessage;//父节点ID
};

/**
 * @class CLogin
 * @brief 登录界面类
 * @author L00341
*/
class CLogin : public QWidget, public ctkEventHandler
{
	Q_OBJECT
	Q_INTERFACES(ctkEventHandler)
public:
	//用户登录信息结构体
	struct SLoginInfo
	{
		std::string strUsrName;
		std::string strPasswd;
	};

	/**  
	 * @note 构造函数
	*/
	CLogin(ctkPluginContext *context, QWidget * parent = 0, Qt::WindowFlags flag = 0);

	/**  
	 * @note 析构函数
	*/
	~CLogin();

protected:
	// 事件处理
	void handleEvent(const ctkEvent &event) override;

signals:
	void uiFrameworkShowSignal();
	void pluginStartSignal();
	void loadProgressSignal(int ,QString);
public slots:
		/**
		* @note 主窗口显示槽
		*/
		void slotuiFrameworkShow();
		/**
		* @note 主窗口显示槽
		*/
		void slotLoadProgress(int, QString);
		void readFortune();
private:
	QUdpSocket *m_pUdpSocket;

};
#endif