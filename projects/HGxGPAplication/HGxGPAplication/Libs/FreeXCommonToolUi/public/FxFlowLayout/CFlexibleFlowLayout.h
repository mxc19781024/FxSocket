#ifndef FLOWLAYOUT_H
#define FLOWLAYOUT_H

#include <QLayout>
#include <QRect>
#include <QStyle>
//#include <FreeXSysConfig.h>

#include "freexcommontoolui_export.h"

/**
* @class	: CFlexibleFlowLayout
* @brief	: 流布局
* @note		: 
* @author	: k00026
**/

#if __unix__
#define FREEXCOMMONTOOLUI_EXPORT
#endif

class FREEXCOMMONTOOLUI_EXPORT CFlexibleFlowLayout : public QLayout
{
public:
	/**
	* @brief	: 构造函数
	* @note		: 可伸缩的流布局
	* @params	: hSpacing [in] 横向间距
	* @params	: vSpacing [in] 竖向间距
	*/
    explicit CFlexibleFlowLayout(QWidget *parent, int hSpacing = -1, int vSpacing = -1);
    explicit CFlexibleFlowLayout(int hSpacing = -1, int vSpacing = -1);
    ~CFlexibleFlowLayout();

	// 设置元素最小宽度
	void setMinimumItemWidth(int width);

	// 设置元素最大宽度
	void setMaximumItemWidth(int width);

	void setHiddenEnabled(bool);

    void addItem(QLayoutItem *item) override;
    int horizontalSpacing() const;
    int verticalSpacing() const;
    Qt::Orientations expandingDirections() const override;
    bool hasHeightForWidth() const override;
    int heightForWidth(int) const override;
    int count() const override;
    QLayoutItem *itemAt(int index) const override;
    QSize minimumSize() const override;
    void setGeometry(const QRect &rect) override;
    QSize sizeHint() const override;
    QLayoutItem *takeAt(int index) override;

private:
    int doLayout(const QRect &rect, bool testOnly) const;
    int smartSpacing(QStyle::PixelMetric pm) const;

    QList<QLayoutItem *> m_itemList;
    int m_iHSpace;
    int m_iVSpace;
	int m_iMinimumItemWidth;
	bool m_bHiddenEnabled;
};
//! [0]

#endif // FLOWLAYOUT_H
