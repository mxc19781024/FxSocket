/*************************************
 *
 * @file GxGuiDef.h
 * @brief 公共定义头文件
 * @author H00015
 * @date 2021-03-30 11:29:22
 * @note
 *
 *************************************/

#ifndef GX_GUI_DEF_H
#define GX_GUI_DEF_H

#include <qglobal.h>

#include <QString>

#define HGxGUI_QUOTE_(x) #x
#define HGxGUI_QUOTE(x) HGxGUI_QUOTE_(x)


 //定义空指针
#ifdef Q_COMPILER_NULLPTR
#define Q_NULL    nullptr
#else   //Q_COMPILER_NULLPTR
#define Q_NULL    NULL
#endif  //Q_COMPILER_NULLPTR
// #define QWIDGET_NULL static_cast<QWidget *>(Q_NULL)
// #define QSTYLEOPTION_NULL static_cast<QStyleOption *>(Q_NULL)


//定义最大整型数
#define Q_MAXINT INT_MAX
//释放并置空指针
// #define Q_DELETE_AND_NULL(x) { delete x; x = Q_NULL; }


#if defined(HGX_GUI_NAMESPACE)
#define HGxGUI_NAMESPACE HGxGui
#endif  //defined(HGX_GUI_NAMESPACE)


#if !defined(HGxGUI_NAMESPACE)
#define HGxGUI_BEGIN_NAMESPACE
#define HGxGUI_END_NAMESPACE
#define HGxGUI_USE_NAMESPACE
#define HGxGui_PREPEND_NAMESPACE(name) name
#define HGxGui_CLASSNAME(classname) classname
#define HGxGui_META_CLASSNAME(classname) classname
#else   //!defined(HGxGUI_NAMESPACE)
#define HGxGUI_BEGIN_NAMESPACE namespace HGxGUI_NAMESPACE {
#define HGxGUI_END_NAMESPACE }
#define HGxGUI_USE_NAMESPACE using namespace HGxGUI_NAMESPACE;
#define HGxGui_PREPEND_NAMESPACE(name) HGxGUI_NAMESPACE::name
#define HGxGui_CLASSNAME(classname) HGxGUI_QUOTE(HGxGUI_NAMESPACE::)classname

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#define HGxGui_META_CLASSNAME(classname) HGxGUI_QUOTE(HGxGUI_NAMESPACE::)classname
#else   //(QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#define HGxGui_META_CLASSNAME(classname) classname
#endif  //(QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#endif  ////!defined(HGxGUI_NAMESPACE)


#ifndef HGxGUI_LIBRARY_STATIC
#ifdef HGxGUI_LIBRARY
#define HGxGUI_EXPORT Q_DECL_EXPORT
#else   //HGxGUI_LIBRARY
#define HGxGUI_EXPORT Q_DECL_IMPORT
#endif  //HGxGUI_LIBRARY

#ifdef HGxGUI_BASE_LIBRARY
#define HGxGUI_BASE_EXPORT Q_DECL_EXPORT
#else   //HGxGUI_BASE_LIBRARY
#define HGxGUI_BASE_EXPORT Q_DECL_IMPORT
#endif  //HGxGUI_BASE_LIBRARY
#else   //HGxGUI_LIBRARY_STATIC
#define HGxGUI_EXPORT
#define HGxGUI_BASE_EXPORT
#endif  //HGxGUI_LIBRARY_STATIC


#ifndef Q_DECL_NOTHROW
#define Q_DECL_NOTHROW throw()
#endif  //Q_DECL_NOTHROW


#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
#if !defined(QT_PRINTSUPPORT_LIB) && !defined(Q_QDOC)
#define HGxGUI_NO_PRINTSUPPORT
#endif

#if !QT_CONFIG(process) && !defined(Q_QDOC)
#define HGxGUI_NO_PROCESS
#endif  //!QT_CONFIG(process) && !defined(Q_QDOC)
#endif  //(QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))


#if !defined(QStringLiteral)
#define QStringLiteral(str) QString(QLatin1String(str))
#endif  //!defined(QStringLiteral)

namespace HGxGui
{
    enum CGxGuiStyle
    {
        Default = 0,
        Fusion = 1,
        Blue = 2,
        Dark = 3,
        Light = 4,
    };

}   //HGxGui

#endif //GX_GUI_DEF_H

