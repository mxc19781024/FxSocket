//
// 
//

#ifndef HG_SRC_WARNINGMESSAGEITEM_H_
#define HG_SRC_WARNINGMESSAGEITEM_H_

#include <QWidget>
#include <QTime>
#include "common\UnitEventCommon.h"

namespace Ui
{
	class AlarmInfoItem;
}


/**
 * 告警信息子项
 */
class CAlarmInfoItem : public QWidget {
	Q_OBJECT;
	Q_ENUMS(ELevel);
public:

	explicit CAlarmInfoItem(QWidget *parent);

	~CAlarmInfoItem();

	void setTime(QTime);

	void setTitle(QString);

	void setInfo(QString);

	void setData(QDate);

	void setLevel(ELevel);

	ELevel getLevel() const;

protected:

private:
	Ui::AlarmInfoItem *ui;
	ELevel m_level;
};

#endif //HG_SRC_WARNINGMESSAGEITEM_H_
