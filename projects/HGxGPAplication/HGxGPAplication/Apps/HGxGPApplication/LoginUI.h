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

/**
 * @class CLogin
 * @brief ��¼������
 * @author L00341
*/
class CLogin : public QWidget
{
	Q_OBJECT
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
	CLogin(QWidget * parent = 0, Qt::WindowFlags flag = 0);

	/**  
	 * @note ��������
	*/
	~CLogin();

	/**  
	 * @note ��ʼ������
	*/
	void setupUi();

	/**  
	 * @note ��ʼ������
	 * @parm SLoginInfo[out] �û���¼��Ϣ�ṹ��
	*/
	SLoginInfo getLoginInfo();

	/**  
	 * @note ����¼��Ϣ�Ƿ���Ч;�ɹ�����true,���򷵻�false
	 * @parm bool[out] ��¼��Ϣ��ʽ�Ƿ���ȷ
	*/
	bool checkLoginData();

	/**  
	 * @note ���������λ�ü���С
	 * @parm int[in]x���꣬y���꣬����
	*/
	void setInputPosition(int x=330, int y=60);
	void setInputAligenCenter();
	
	/**  
	 * @note ���õ�¼ҳ�汳��ͼƬ
	 * @parm QString[in] ��¼����ͼƬ·��
	*/
	void setBackGroundImage(QString strPath);

	/**  
	 * @note �����û�ͷ��ͼƬ
	 * @parm QString[in] �û�ͷ��ͼƬ·��
	*/
	void setUsrImage(QString strPath);

	/**  
	 * @note ��ȡԲ��ͷ��ͼƬ
	 * @parm image[in] ������ͼƬ
	 * @parm rect[in] ͼƬ�ߴ�
	 * @parm radius[in]Բ�Ǵ�С
	*/
	QPixmap getRoundImage(const QPixmap& image, QRect rect, int radius);

	void InitStartPlugin();

	public slots:
		/**  
		* @note ��¼��ť����¼���
		*/
		void slotLoginButtonClicked();

		/**  
		* @note �˳���ť����¼���
		*/
		void slotExitButtonClicked();
	private slots:
		/**  
		* @note ȥ���������󣬴����޷���ק����д�����Ӧ�ۺ���
		*/
		void mousePressEvent(QMouseEvent *e);//����ͷ�
		void mouseMoveEvent(QMouseEvent *e);//����ƶ�
		void mouseReleaseEvent(QMouseEvent *e);//��갴��
private:
	QPoint last;//�����϶�����

protected:
private:
	QWidget * m_pInput;//��¼��
	QWidget * m_pLoginContext;//���ݿ�
	QLabel * m_pTitle;//����
	QLineEdit * m_pLE_User;//�û���
	QLineEdit * m_pLE_Passwd;//����
	QPushButton * m_pButton_Login;//��¼��ť
	QPushButton * m_pButton_Exit;//�˳���ť
	QPushButton * m_pButton_UsrImage;//�û�ͷ���
};
#endif