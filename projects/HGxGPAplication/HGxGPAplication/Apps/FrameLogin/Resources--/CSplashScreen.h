/**
* @file		: CSplashScreen.h
* @brief	: 启动界面
* @note 
* @author	: c00169
*/
#ifndef CSPLASHSCREEN_H
#define CSPLASHSCREEN_H

#include <QWidget>
#include "ui_CSplashScreen.h"

#include <FreeXSysConfig.h>
#include <QMovie>
#include <QPropertyAnimation>
#include <QUdpSocket>
#include <QTimer>


/**
* @class	: CSplashScreen
* @brief	: 启动界面
* @note 
* @author	: c00169
*/
class CSplashScreen : public QWidget
{
	Q_OBJECT

public:
	CSplashScreen(QWidget *parent = 0);
	~CSplashScreen();

protected:
	/**
	* @brief	: 鼠标点击事件
	*/
	void mousePressEvent(QMouseEvent *);

	/**
	* @brief	: 鼠标移动事件
	*/
	void mouseMoveEvent(QMouseEvent *);

	/**
	* @brief	: 鼠标释放事件
	*/
	void mouseReleaseEvent(QMouseEvent *);

private slots:
	/**
	* @brief	: 动画结束
	*/
	void animationFinishedSlot();

	void socketReayReadHandler();

	// 检测主进程状态
	void checkMainProcessSlot();

private:
	Ui::CSplashScreenClass ui;

	// Gif
	QMovie*					m_pMovie;

	// 动画
	QPropertyAnimation*		m_pAnimation; 

	// UDP套接字
	QUdpSocket*				m_pUdpSocket;

	// 鼠标是否点击
	bool					m_bMousePressed;

	// 鼠标点击位置
	QPoint					m_mousePressPt;

	// 定时器
	QTimer					*m_pTimer;
};

#endif // CSPLASHSCREEN_H
