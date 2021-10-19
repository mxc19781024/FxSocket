/*************************************
 *
 * @file GxGuiMainWindowPrivate.h
 * @brief 主窗口私有类
 * @author H00015
 * @date 2021-04-06 08:42:40
 * @note
 *
 *************************************/

#ifndef GX_UI_FRAMEWORK_SERVICE_INTERFACE_H
#define GX_UI_FRAMEWORK_SERVICE_INTERFACE_H

class CGxUiFrameworkServiceInterface
{

public:
    /**
    * @brief 窗口基础功能
    */
    virtual void show() = 0;
    virtual void hide() = 0;
    virtual void setVisible(bool visible) = 0;


    virtual QPoint pos() const = 0;
    virtual void move(int x, int y) = 0;
    virtual void move(const QPoint& pos) = 0;

    virtual const QRect& geometry() const = 0;
    virtual void setGeometry(int x, int y, int w, int h) = 0;
    virtual void setGeometry(const QRect &geo) = 0;

    virtual int x() const = 0;
    virtual int y() const = 0;
    virtual int width() const = 0;
    virtual int height() const = 0;

    virtual QSize size() const = 0;
    virtual void resize(int w, int h) = 0;
    virtual void resize(const QSize &size) = 0;
    virtual QRect rect() const = 0;

    /**
    * @brief 获取窗口框架的主窗口
    * @return   QWidget* 主窗口指针
    */
    virtual QWidget *mainWindow() = 0;

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
    virtual QWidget* addSceneWidget(const QString &id, const QString &title, QWidget *widget) = 0;

};

//HGxGUI_END_NAMESPACE

Q_DECLARE_INTERFACE(CGxUiFrameworkServiceInterface, "com.henggetec.service.uiFramework")

#endif  //GX_UI_FRAMEWORK_SERVICE_INTERFACE_H