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
#include <QUdpSocket>
#include <QTimer>
#include "QProgressBar"

/**
 * @class CLogin
 * @brief 登录界面类
 * @author L00341
*/
class CLogin : public QWidget
{
	Q_OBJECT
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
	CLogin(QWidget * parent = 0, Qt::WindowFlags flag = 0);

	/**  
	 * @note 析构函数
	*/
	~CLogin();

	/**  
	 * @note 初始化窗口
	*/
	void setupUi();

	/**  
	 * @note 初始化窗口
	 * @parm SLoginInfo[out] 用户登录信息结构体
	*/
	SLoginInfo getLoginInfo();

	/**  
	 * @note 检测登录信息是否有效;成功返回true,否则返回false
	 * @parm bool[out] 登录信息格式是否正确
	*/
	bool checkLoginData();

	/**  
	 * @note 设置输入框位置及大小
	 * @parm int[in]x坐标，y坐标，宽，高
	*/
	void setInputPosition(int x=330, int y=60);
	void setInputAligenCenter();
	
	/**  
	 * @note 设置登录页面背景图片
	 * @parm QString[in] 登录背景图片路径
	*/
	void setBackGroundImage(QString strPath);

	/**  
	 * @note 设置用户头像图片
	 * @parm QString[in] 用户头像图片路径
	*/
	void setUsrImage(QString strPath);

	/**  
	 * @note 获取圆角头像图片
	 * @parm image[in] 待处理图片
	 * @parm rect[in] 图片尺寸
	 * @parm radius[in]圆角大小
	*/
	QPixmap getRoundImage(const QPixmap& image, QRect rect, int radius);

	/**  
	 * @note 设置进度条的值
	 * @parm iValue[in] 进度条的值
	*/
	void setProgerssBarValue(int iValue);

	/**  
	 * @note 设置进度条上方提示信息
	 * @parm strMessage[in] 提示信息
	*/
	void  setPromptMessage( const QString strMessage);
protected:
	/**  
	 * @note QWidget重写PaintEvent函数
	*/
	void paintEvent(QPaintEvent* event);

public slots:
		/**  
		* @note 登录按钮点击事件槽
		*/
		void slotLoginButtonClicked();

		/**  
		* @note 退出按钮点击事件槽
		*/
		void slotExitButtonClicked();

		void socketReayReadHandler();

		// 检测主进程状态
		void checkMainProcessSlot();
private slots:
		/**  
		* @note 去掉标题栏后，窗口无法拖拽，重写鼠标响应槽函数
		*/
		void mousePressEvent(QMouseEvent *e);//鼠标释放
		void mouseMoveEvent(QMouseEvent *e);//鼠标移动
		void mouseReleaseEvent(QMouseEvent *e);//鼠标按下
private:
	QPoint last;//窗口拖动变量

protected:
private:
	QWidget * m_pInput;//登录框
	QWidget * m_pLoginContext;//内容框
	QLabel * m_pLogo;//公司Logo
	QLabel * m_pTitle;//标题
	QLabel * m_pHalvingLine;//分割线
	QLabel * m_pProgerssText;//进度条提示信息
	QLineEdit * m_pLE_User;//用户名
	QLineEdit * m_pLE_Passwd;//密码
	QPushButton * m_pButton_Login;//登录按钮
	QPushButton * m_pButton_Exit;//退出按钮
	QPushButton * m_pButton_UsrImage;//用户头像框
	QProgressBar * m_pProgressBar;//进度条
								  // 定时器
	QTimer					*m_pTimer;
	// UDP套接字
	QUdpSocket*				m_pUdpSocket;
};
#endif