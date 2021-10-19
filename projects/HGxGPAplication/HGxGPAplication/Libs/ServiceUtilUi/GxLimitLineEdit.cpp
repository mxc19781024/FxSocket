#include <QFile>
#include <QStyle>
#include "GxLimitLineEdit.h"




CGxLimitLineEdit::CGxLimitLineEdit( QWidget *parent)
      :QLineEdit(parent)
{
	//this->setMaxLength(20);
	setProperty("errorMsg",  false);
	connect(this, SIGNAL(editingFinished()), this , SLOT(editingFinshedSlot()));

	setObjectName("limitLineEdit");

	//读样式表文件
// 	QFile file(":/css/Resources/qss/ServerProUtils.css");
// 	if (file.open(QIODevice::ReadOnly| QIODevice::Text))
// 	{
// 		setStyleSheet(styleSheet().append(file.readAll()));
// 		file.close();
// 	}
}

CGxLimitLineEdit::~CGxLimitLineEdit()
{
      
}

void CGxLimitLineEdit::setErrorMsg( const QString &strErrorMsg )
{
	m_strErrorMsg = strErrorMsg; 
	style()->unpolish(this);
	style()->polish(this);
}

void CGxLimitLineEdit::editingFinshedSlot()
{
	if (this->text() == "")
	{
		setErrorMsg("red");
// 		this->setStyleSheet("QLineEdit:hover{border-image:url(:/images/Resources/images/input_RED.png)2 2 2 2;border-width:2;}\
//           QLineEdit:focus{border-image:url(:/images/Resources/images/input_RED.png)2 2 2 2;border-width:2;}\
// 		 QLineEdit{border-image:url(:/images/Resources/images/input_RED.png)2 2 2 2;border-width:2;}");
	}
	else
	{
		setErrorMsg("normal");
// 		this->setStyleSheet("QLineEdit:hover{border-image:url(:/images/Resources/images/input box hover.png)2 2 2 2;border-width:2;}\
// 		QLineEdit:focus{border-image:url(:/images/Resources/images/input box click.png)2 2 2 2;border-width:2;}\
// 		QLineEdit{border-image:url(:/images/Resources/images/input box.png)2 2 2 2;border-width:2;}");
	}
}
