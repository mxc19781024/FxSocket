#ifndef FX_SIMPLE_TOAST_H
#define FX_SIMPLE_TOAST_H

#include <QWidget>
//#include <FreeXSysConfig.h>
#include "freexcommontoolui_export.h"

class CSimpleToastPrivate;

/**
* @class	: CSimpleToast
* @brief	: 另一个消息提示Toast工具
* @note	
* @author	: k00026
**/

#if __unix__
#define FREEXCOMMONTOOLUI_EXPORT
#endif

class FREEXCOMMONTOOLUI_EXPORT CSimpleToast : public QWidget
{
	Q_OBJECT

public:
	/**
	* @brief	: 提示消息
	* @params	: text [in] 内容
	* @params	: parent [in] 父窗口对象
	*/
	static void information(const QString &text, QWidget *parent);

	/**
	* @brief	: 错误消息
	* @params	: text [in] 内容
	* @params	: parent [in] 父窗口对象
	*/
	static void error(const QString &text, QWidget *parent);

private:
	CSimpleToast(QWidget *parent);
	~CSimpleToast();

private:
	Q_DECLARE_PRIVATE(CSimpleToast)
	Q_DISABLE_COPY(CSimpleToast)
	const QScopedPointer<CSimpleToastPrivate> d_ptr;	// d指针
};

#endif // FX_SIMPLE_TOAST_H
