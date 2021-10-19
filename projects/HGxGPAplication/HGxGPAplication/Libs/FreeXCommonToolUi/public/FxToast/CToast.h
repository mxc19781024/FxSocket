#ifndef FX_TOAST_H
#define FX_TOAST_H

#include <QWidget>
//#include <FreeXSysConfig.h>
#include "freexcommontoolui_export.h"

class CToastPrivate;

/**
* @class	: CToast
* @brief	: 消息提示Toast工具
* @note	
* @author	: k00026
**/
#if __unix__
#define FREEXCOMMONTOOLUI_EXPORT
#endif
class FREEXCOMMONTOOLUI_EXPORT CToast : public QWidget
{
	Q_OBJECT

public:
	/**
	* @brief	：提示消息
	* @params	：title [in] 标题
	* @params	: text [in] 内容
	* @params	: parent [in] 父窗口对象
	*/
	static void information(const QString &title, const QString &text, QWidget *parent);

private:
	static CToast*    m_pInstance;
	CToast(QWidget *parent);
	~CToast();

private:
	Q_DECLARE_PRIVATE(CToast)
	Q_DISABLE_COPY(CToast)
	const QScopedPointer<CToastPrivate> d_ptr;	// d指针

};

#endif // FX_TOAST_H
