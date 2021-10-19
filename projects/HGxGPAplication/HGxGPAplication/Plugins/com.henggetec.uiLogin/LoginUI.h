/**************************************************************************************************
* @file LoginUI.h
* @note ��¼����
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
	int		        iValue; //ʵ��ID
	std::string		strMessage;//���ڵ�ID
};

/**
 * @class CLogin
 * @brief ��¼������
 * @author L00341
*/
class CLogin : public QWidget, public ctkEventHandler
{
	Q_OBJECT
	Q_INTERFACES(ctkEventHandler)
public:
	//�û���¼��Ϣ�ṹ��
	struct SLoginInfo
	{
		std::string strUsrName;
		std::string strPasswd;
	};

	/**  
	 * @note ���캯��
	*/
	CLogin(ctkPluginContext *context, QWidget * parent = 0, Qt::WindowFlags flag = 0);

	/**  
	 * @note ��������
	*/
	~CLogin();

protected:
	// �¼�����
	void handleEvent(const ctkEvent &event) override;

signals:
	void uiFrameworkShowSignal();
	void pluginStartSignal();
	void loadProgressSignal(int ,QString);
public slots:
		/**
		* @note ��������ʾ��
		*/
		void slotuiFrameworkShow();
		/**
		* @note ��������ʾ��
		*/
		void slotLoadProgress(int, QString);
		void readFortune();
private:
	QUdpSocket *m_pUdpSocket;

};
#endif