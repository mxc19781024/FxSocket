#ifndef XGIS_TOP_WIDGET_H
#define XGIS_TOP_WIDGET_H

#include <QWidget>

#include "ui_CXGisTopWidget.h"
#include <QPushButton>
#include "QTimer"
#include "GxGuiDef.h"
#include "QString"
#include "qevent.h"

namespace HGxGui {

	// 应用类型
	enum EAppType
	{
		E_Normal,  //正常屏幕
		E_BigScreen    //大屏
	};

	Q_DECLARE_FLAGS(EAppTypes, EAppType)

		/**
		* @class	: CXGisTopWidget
		* @brief	: 主窗口顶部widget
		* @note		:
		* @author	: w00085
		**/
		class CXGisTopWidget : public QWidget
	{
		Q_OBJECT


	public:
		/// 构造函数
		CXGisTopWidget(QWidget *parent = nullptr);

		/// 析构函数
		~CXGisTopWidget();

		/**
		* @brief设置最大化按钮的提示消息	: l180
		*/
		void setMaxBtnText(QString text);

		/**
		* @brief设置最大化按钮的提示消息	: l180
		*/
		void setAppType(HGxGui::EAppType eAppType);

		/**
		* @brief初始化正常
		*/
		bool InitNormal();

		/**
		* @brief初始化大屏
		*/
		bool InitBigScreen();

	private:
		// 初始化信号槽
		void initSignalsAndSlots();

	signals:
		void closeAppSignal();
		private Q_SLOTS:
		// 关闭按钮单击槽函数
		void closeBtnClickedSlot();
		// 3D按钮单击槽函数
		void Btn3DClickedSlot();
		// 2D按钮单击槽函数
		void Btn2DClickedSlot();
		// 2D3D按钮单击槽函数
		void Btn2D3DClickedSlot();
		/**
		* @brie系统时间更新
		*/
		void SlotUpdataDataTime();

	private:
		// paint事件
		void paintEvent(QPaintEvent *);
	private:
		Ui::CXGisTopWidget ui;
		QTimer            * m_pTimer;
	};

}

#endif // !XGIS_TOP_WIDGET_H
