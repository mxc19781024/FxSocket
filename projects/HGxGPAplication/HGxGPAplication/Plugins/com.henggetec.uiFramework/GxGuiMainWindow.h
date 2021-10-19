#ifndef XGIS_MAINWINDOW_H
#define XGIS_MAINWINDOW_H

#include <QHash>
#include <QWidget>
#include <QScopedPointer>

#include "GxUiFrameworkServiceInterface.h"


namespace Ui
{
	class CXGisMainWindow;
}
namespace HGxGui {

	class CXGisTopWidget;

	/**
	* @class	: CXGisMainWindow
	* @brief	: 主窗口
	* @note
	* @author	: k00026
	**/
	class CGxGuiMainWindow : public QWidget, public CGxUiFrameworkServiceInterface
	{
		Q_OBJECT
		Q_INTERFACES(CGxUiFrameworkServiceInterface)
	public:
		CGxGuiMainWindow(QString qName = "FreeXGisStudio", QString qFlag = "", QWidget *parent = nullptr);
		~CGxGuiMainWindow();

		/**
		* @brief 窗口基础功能
		*/
		void show() override;
		void hide() override;
		void setVisible(bool visible) override;


		QPoint pos() const override;
		void move(int x, int y) override;
		void move(const QPoint& pos) override;

		const QRect& geometry() const override;
		void setGeometry(int x, int y, int w, int h) override;
		void setGeometry(const QRect &geo) override;

		int x() const override;
		int y() const override;
		int width() const override;
		int height() const override;

		QSize size() const override;
		void resize(int w, int h) override;
		void resize(const QSize &size) override;
		QRect rect() const override;


		/**
		* @brief 获取窗口框架的主窗口
		* @return   QWidget* 主窗口指针
		*/
		QWidget *mainWindow();

		/**
		* @brief	 添加场景窗
		* @param	 id [in] 窗口的唯一id
		* @param	 title [in] 工具窗口名称
		* @param	 widget [in] 工具窗口widget
		* @param	 userData [in] 用户自定义字段
		* @param     role [in]  用户自定义字段类型
		* @return	 DockDocumentPanel* 场景停靠面板指针
		* @note      场景窗只能在中央核心区域
		*/
		QWidget* addSceneWidget(const QString &id, const QString &title, QWidget *widget);
	protected:
		/**
		* @brief	: paint事件
		* @params	:
		*/
		void paintEvent(QPaintEvent *);

		/**
		* @brief	: 事件过滤器
		* @params	:
		*/
		bool eventFilter(QObject *watched, QEvent *event);

	private:

		/**
		* @brief	: 初始化场景
		* @params	:
		*/
		void InitApp();
		/**
		* @brief	: 初始化小屏界面
		* @params	:
		*/
		void InitNormal();
		/**
		* @brief	: 初始化大屏界面
		* @params	:
		*/
		void InitBigScreen();

	private slots:
		/**
		* @brief 关闭主程序
		* @details
		* @param  [in]
		* @return
		* @note
		*/
		void closeAppSlot();

	private:
		Ui::CXGisMainWindow *ui;					// ui
		HGxGui::CXGisTopWidget  *m_pTopWgt;
		QString m_qWindowsName;
		QString m_qWindowsFlag;
	};
}
#endif // !XGIS_MAINWINDOW_H
