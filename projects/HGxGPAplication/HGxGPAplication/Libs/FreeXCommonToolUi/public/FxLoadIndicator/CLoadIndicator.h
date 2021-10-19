#ifndef CLOADINDICATOR_H
#define CLOADINDICATOR_H

#include <QWidget>
#include <QLabel>
#include <QMutex>

#include "freexcommontoolui_export.h"
#include "bdpCommon/GxStrUtil.h"

/**
* @class	: CLoadIndicator
* @brief	: 加载提示工具
* @note	
* @author	: k00026
**/
#if __unix__
#define FREEXCOMMONTOOLUI_EXPORT
#endif
class FREEXCOMMONTOOLUI_EXPORT CLoadIndicator : public QWidget
{
	Q_OBJECT

public:
	static void addUseCount(QWidget *parent, QString text = tr("loading"));
	static void removeUseCount();

	CLoadIndicator(QWidget *parent, QString text = tr("loading"));
	~CLoadIndicator();
	void setIndicateText(const QString &);

protected:
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

private:
	int useCountIncrease();
	int useCountDecrease();

private:
	static CLoadIndicator *s_pLoadIndicator;
	int m_iUseCount;
	QWidget *m_pOverlayWgt;
	QWidget *m_pBgWgt;
	QLabel *m_pLoadLbl;
	QLabel *m_pTextLbl;
	bool m_bMousePressed;			// 标记鼠标是否按下
	QPoint m_mousePressPt;			// 标记鼠标按下的位置
	static QMutex *m_pMutex;
};

#endif // CLOADINDICATOR_H
