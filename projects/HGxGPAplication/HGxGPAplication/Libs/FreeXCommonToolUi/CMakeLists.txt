# 输出名称
project(FreeXCommonToolUi)

# 对外接口文件的路径
set(PUBLIC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/public)

# 颜色选取器（下拉形式）
file(GLOB COLORPICKER_HEADER_FILES FxColorPickerTool/*.h)
file(GLOB COLORPICKER_SOURCE_FILES FxColorPickerTool/*.cpp)
file(GLOB COLORPICKER_FORM_FILES FxColorPickerTool/*.ui)
file(GLOB COLORPICKER_QRC_FILES FxColorPickerTool/*.qrc)
set(COLORPICKER_INCLUDE_FILES
	${COLORPICKER_HEADER_FILES}
	${PUBLIC_PATH}/FxColorPickerTool/FxColorSelector.h
)

# 树的搜索工具
file(GLOB TREESEARCH_HEADER_FILES FxTreeSearchTool/*.h)
file(GLOB TREESEARCH_SOURCE_FILES FxTreeSearchTool/*.cpp)
set(TREESEARCH_INCLUDE_FILES
	${TREESEARCH_HEADER_FILES}
	${PUBLIC_PATH}/FxTreeSearchTool/CTreeSearchTool.h
)

# 提示对话框
file(GLOB MESSAGEBOX_HEADER_FILES FxMessageBox/*.h)
file(GLOB MESSAGEBOX_SOURCE_FILES FxMessageBox/*.cpp)
file(GLOB MESSAGEBOX_QRC_FILES FxMessageBox/*.qrc)
set(MESSAGEBOX_INCLUDE_FILES
	${MESSAGEBOX_HEADER_FILES}
	${PUBLIC_PATH}/FxMessageBox/CMessageBox.h
)

# 搜索框
file(GLOB SEARCHEDIT_HEADER_FILES FxSearchEdit/*.h)
file(GLOB SEARCHEDIT_SOURCE_FILES FxSearchEdit/*.cpp)
file(GLOB SEARCHEDIT_QRC_FILES FxSearchEdit/*.qrc)
set(SEARCHEDIT_INCLUDE_FILES
	${SEARCHEDIT_HEADER_FILES}
	${PUBLIC_PATH}/FxSearchEdit/CSearchEdit.h
)

# Toast提示
file(GLOB TOAST_HEADER_FILES FxToast/*.h)
file(GLOB TOAST_SOURCE_FILES FxToast/*.cpp)
file(GLOB TOAST_QRC_FILES FxToast/*.qrc)
set(TOAST_INCLUDE_FILES
	${TOAST_HEADER_FILES}
	${PUBLIC_PATH}/FxToast/CToast.h
)

# Toast提示2
file(GLOB SIMPLE_TOAST_HEADER_FILES FxSimpleToast/*.h)
file(GLOB SIMPLE_TOAST_SOURCE_FILES FxSimpleToast/*.cpp)
file(GLOB SIMPLE_TOAST_QRC_FILES FxSimpleToast/*.qrc)
set(SIMPLE_TOAST_INCLUDE_FILES
	${SIMPLE_TOAST_HEADER_FILES}
	${PUBLIC_PATH}/FxSimpleToast/CSimpleToast.h
)

# 窗口基类
file(GLOB BASEWINDOW_HEADER_FILES FxBaseWindow/*.h)
file(GLOB BASEWINDOW_SOURCE_FILES FxBaseWindow/*.cpp)
file(GLOB BASEWINDOW_QRC_FILES FxBaseWindow/*.qrc)
set(BASEWINDOW_INCLUDE_FILES
	${BASEWINDOW_HEADER_FILES}
	${PUBLIC_PATH}/FxBaseWindow/CFxBaseWindow.h
)

# 可伸缩流布局
file(GLOB FLOWLAYOUT_HEADER_FILES FxFlowLayout/*.h)
file(GLOB FLOWLAYOUT_SOURCE_FILES FxFlowLayout/*.cpp)
set(FLOWLAYOUT_INCLUDE_FILES
	${FLOWLAYOUT_HEADER_FILES}
	${PUBLIC_PATH}/FxFlowLayout/CFlexibleFlowLayout.h
)

# 树节点编辑代理
file(GLOB TREEEDIT_HEADER_FILES FxEditingDelegate/*.h)
file(GLOB TREEEDIT_SOURCE_FILES FxEditingDelegate/*.cpp)
set(TREEEDIT_INCLUDE_FILES
	${TREEEDIT_HEADER_FILES}
	${PUBLIC_PATH}/FxEditingDelegate/CEditingDelegate.h
)

# 透明度设置控件
file(GLOB TRANS_SLIDER_HEADER_FILES FxTransparentSlider/*.h)
file(GLOB TRANS_SLIDER_SOURCE_FILES FxTransparentSlider/*.cpp)
set(TRANS_SLIDER_INCLUDE_FILES
	${TRANS_SLIDER_HEADER_FILES}
	${PUBLIC_PATH}/FxTransparentSlider/CPercentSlider.h
)

# 线型选择控件
file(GLOB PEN_STYLE_HEADER_FILES FxPenStyleSelector/*.h)
file(GLOB PEN_STYLE_SOURCE_FILES FxPenStyleSelector/*.cpp)
set(PEN_STYLE_INCLUDE_FILES
	${PEN_STYLE_HEADER_FILES}
	${PUBLIC_PATH}/FxPenStyleSelector/CPenStyleSelector.h
)

# 颜色选取窗口
file(GLOB COLORDIALOG_HEADER_FILES FxColorDialog/*.h)
file(GLOB COLORDIALOG_SOURCE_FILES FxColorDialog/*.cpp)
file(GLOB COLORDIALOG_FORM_FILES FxColorDialog/*.ui)
file(GLOB COLORDIALOG_QRC_FILES FxColorDialog/*.qrc)
set(COLORDIALOG_INCLUDE_FILES
	${COLORDIALOG_HEADER_FILES}
	${PUBLIC_PATH}/FxColorDialog/FxColorPickerWidget.h
)

# 程序唯一实例控制
file(GLOB SINGLE_INSTANCE_HEADER_FILES FxSingleInstanceGuard/*.h)
file(GLOB SINGLE_INSTANCE_SOURCE_FILES FxSingleInstanceGuard/*.cpp)
set(SINGLE_INSTANCE_INCLUDE_FILES
	${SINGLE_INSTANCE_HEADER_FILES}
	${PUBLIC_PATH}/FxSingleInstanceGuard/CSingleInstanceGuard.h
)

# 加载提示
file(GLOB LOAD_INDICATOR_HEADER_FILES FxLoadIndicator/*.h)
file(GLOB LOAD_INDICATOR_SOURCE_FILES FxLoadIndicator/*.cpp)
file(GLOB LOAD_INDICATOR_QRC_FILES FxLoadIndicator/*.qrc)
set(LOAD_INDICATOR_INCLUDE_FILES
	${LOAD_INDICATOR_HEADER_FILES}
	${PUBLIC_PATH}/FxLoadIndicator/CLoadIndicator.h
)

# 将不同工具放在VS中不同的筛选器下，注意格式：2个反斜杠
SOURCE_GROUP(FxColorPickerTool\\HeaderFiles FILES ${COLORPICKER_INCLUDE_FILES})
SOURCE_GROUP(FxColorPickerTool\\SrcFiles FILES ${COLORPICKER_SOURCE_FILES})
SOURCE_GROUP(FxColorPickerTool\\FormFiles FILES ${COLORPICKER_FORM_FILES})
SOURCE_GROUP(FxColorPickerTool\\ResFiles FILES ${COLORPICKER_QRC_FILES})

SOURCE_GROUP(FxTreeSearchTool\\HeaderFiles FILES ${TREESEARCH_INCLUDE_FILES})
SOURCE_GROUP(FxTreeSearchTool\\SrcFiles FILES ${TREESEARCH_SOURCE_FILES})

SOURCE_GROUP(FxMessageBox\\HeaderFiles FILES ${MESSAGEBOX_INCLUDE_FILES})
SOURCE_GROUP(FxMessageBox\\SrcFiles FILES ${MESSAGEBOX_SOURCE_FILES})
SOURCE_GROUP(FxMessageBox\\ResFiles FILES ${MESSAGEBOX_QRC_FILES})

SOURCE_GROUP(FxSearchEdit\\HeaderFiles FILES ${SEARCHEDIT_INCLUDE_FILES})
SOURCE_GROUP(FxSearchEdit\\SrcFiles FILES ${SEARCHEDIT_SOURCE_FILES})
SOURCE_GROUP(FxSearchEdit\\ResFiles FILES ${SEARCHEDIT_QRC_FILES})

SOURCE_GROUP(FxToast\\HeaderFiles FILES ${TOAST_INCLUDE_FILES})
SOURCE_GROUP(FxToast\\SrcFiles FILES ${TOAST_SOURCE_FILES})
SOURCE_GROUP(FxToast\\ResFiles FILES ${TOAST_QRC_FILES})

SOURCE_GROUP(FxSimpleToast\\HeaderFiles FILES ${SIMPLE_TOAST_INCLUDE_FILES})
SOURCE_GROUP(FxSimpleToast\\SrcFiles FILES ${SIMPLE_TOAST_SOURCE_FILES})
SOURCE_GROUP(FxSimpleToast\\ResFiles FILES ${SIMPLE_TOAST_QRC_FILES})

SOURCE_GROUP(FxBaseWindow\\HeaderFiles FILES ${BASEWINDOW_INCLUDE_FILES})
SOURCE_GROUP(FxBaseWindow\\SrcFiles FILES ${BASEWINDOW_SOURCE_FILES})
SOURCE_GROUP(FxBaseWindow\\ResFiles FILES ${BASEWINDOW_QRC_FILES})

SOURCE_GROUP(FxFlowLayout\\HeaderFiles FILES ${FLOWLAYOUT_INCLUDE_FILES})
SOURCE_GROUP(FxFlowLayout\\SrcFiles FILES ${FLOWLAYOUT_SOURCE_FILES})

SOURCE_GROUP(FxEditingDelegate\\HeaderFiles FILES ${TREEEDIT_INCLUDE_FILES})
SOURCE_GROUP(FxEditingDelegate\\SrcFiles FILES ${TREEEDIT_SOURCE_FILES})

SOURCE_GROUP(FxTransparentSlider\\HeaderFiles FILES ${TRANS_SLIDER_INCLUDE_FILES})
SOURCE_GROUP(FxTransparentSlider\\SrcFiles FILES ${TRANS_SLIDER_SOURCE_FILES})

SOURCE_GROUP(FxPenStyleSelector\\HeaderFiles FILES ${PEN_STYLE_INCLUDE_FILES})
SOURCE_GROUP(FxPenStyleSelector\\SrcFiles FILES ${PEN_STYLE_SOURCE_FILES})

SOURCE_GROUP(FxColorDialog\\HeaderFiles FILES ${COLORDIALOG_INCLUDE_FILES})
SOURCE_GROUP(FxColorDialog\\SrcFiles FILES ${COLORDIALOG_SOURCE_FILES})
SOURCE_GROUP(FxColorDialog\\FormFiles FILES ${COLORDIALOG_FORM_FILES})
SOURCE_GROUP(FxColorDialog\\ResFiles FILES ${COLORDIALOG_QRC_FILES})

SOURCE_GROUP(FxSingleInstanceGuard\\HeaderFiles FILES ${SINGLE_INSTANCE_INCLUDE_FILES})
SOURCE_GROUP(FxSingleInstanceGuard\\SrcFiles FILES ${SINGLE_INSTANCE_SOURCE_FILES})

SOURCE_GROUP(FxLoadIndicator\\HeaderFiles FILES ${LOAD_INDICATOR_INCLUDE_FILES})
SOURCE_GROUP(FxLoadIndicator\\SrcFiles FILES ${LOAD_INDICATOR_SOURCE_FILES})
SOURCE_GROUP(FxLoadIndicator\\ResFiles FILES ${LOAD_INDICATOR_QRC_FILES})

# Files which should be processed by Qts moc
set(LIB_MOC_HDRS
	${FX_LIB_HEADER_DIR}/FxBaseWindow/CFxBaseWindow.h
	${FX_LIB_HEADER_DIR}/FxColorDialog/FxColorPickerWidget.h
	${FX_LIB_HEADER_DIR}/FxColorPickerTool/FxColorSelector.h
	${FX_LIB_HEADER_DIR}/FxEditingDelegate/CEditingDelegate.h
	${FX_LIB_HEADER_DIR}/FxFlowLayout/CFlexibleFlowLayout.h
	${FX_LIB_HEADER_DIR}/FxLoadIndicator/CLoadIndicator.h
	${FX_LIB_HEADER_DIR}/FxMessageBox/CMessageBox.h
	${FX_LIB_HEADER_DIR}/FxPenStyleSelector/CPenStyleSelector.h
	${FX_LIB_HEADER_DIR}/FxPointingHandButton/FxPointingHandButton.h
	${FX_LIB_HEADER_DIR}/FxSearchEdit/CSearchEdit.h
	${FX_LIB_HEADER_DIR}/FxSimpleToast/CSimpleToast.h
	${FX_LIB_HEADER_DIR}/FxSingleInstanceGuard/CSingleInstanceGuard.h
	${FX_LIB_HEADER_DIR}/FxToast/CToast.h
	${FX_LIB_HEADER_DIR}/FxTransparentSlider/CPercentSlider.h
	${FX_LIB_HEADER_DIR}/FxTreeSearchTool/CTreeSearchTool.h
	${FX_LIB_HEADER_DIR}/FxTreeSearchTool/FuzzyCompare.h
	FxColorDialog/CBasicColorArea.h
	FxColorDialog/CCustomColorArea.h
	FxColorDialog/CCustomColorItem.h
	FxColorDialog/CHandPushButton.h
	FxColorDialog/CHColorArea.h
	FxColorDialog/CPreviewColorArea.h
	FxColorDialog/CSingleColorItem.h
	FxColorDialog/CSVColorArea.h
	FxColorDialog/FxColorPickerWidgetPrivate.h
	FxColorPickerTool/CBasicColorAreaWidget.h
	FxColorPickerTool/CColorSelectWidget.h
	FxColorPickerTool/CSingleColorPickerItem.h
	FxColorPickerTool/FxColorSelectorPrivate.h
	FxSimpleToast/CSimpleToastPrivate.h
	FxToast/CToastPrivate.h
	FxTransparentSlider/CHandleLabel.h
)

set(LIB_QRCS
	FxBaseWindow/CFxBaseWindow.qrc
	FxColorDialog/FxColorPickerWidget.qrc
	FxColorPickerTool/colorpicker.qrc
	FxDockSystem/FxDockSystem.qrc
	FxLoadIndicator/loadindicator.qrc
	FxMessageBox/CMessageBox.qrc
	FxSearchEdit/CSearchEdit.qrc
	FxSimpleToast/simpletoast.qrc
	FxToast/toast.qrc
)

IF(Qt5Widgets_FOUND)
	#moc
    SET(LIB_MOC_SRCS)
    FOREACH( LIB_MOC_HDR ${LIB_MOC_HDRS} )
        GET_FILENAME_COMPONENT( LIB_MOC_HDR_ABS ${LIB_MOC_HDR} ABSOLUTE )
        QT5_WRAP_CPP( LIB_MOC_SRC ${LIB_MOC_HDR} OPTIONS "-f${LIB_MOC_HDR_ABS}" )
        LIST( APPEND LIB_MOC_SRCS ${LIB_MOC_SRC} )
    ENDFOREACH()
	#qrc
	QT5_ADD_RESOURCES(LIB_QRC_SRCS ${LIB_QRCS})
ELSE()
	FIND_PACKAGE(Qt4)
    INCLUDE( ${QT_USE_FILE} )
	#moc
    QT4_WRAP_CPP( LIB_MOC_SRCS ${LIB_MOC_HDRS} OPTIONS "-f" )
	#qrc
	QT4_ADD_RESOURCES(LIB_QRC_SRCS ${LIB_QRCS})
ENDIF()

ADD_LIBRARY(
	${PROJECT_NAME} SHARED
	${COLORPICKER_INCLUDE_FILES}
	${COLORPICKER_SOURCE_FILES}
	${COLORPICKER_FORM_FILES}
	${COLORPICKER_QRC_FILES}
	${TREESEARCH_INCLUDE_FILES}
	${TREESEARCH_SOURCE_FILES}
	${MESSAGEBOX_INCLUDE_FILES}
	${MESSAGEBOX_SOURCE_FILES}
	${MESSAGEBOX_QRC_FILES}
	${SEARCHEDIT_INCLUDE_FILES}
	${SEARCHEDIT_SOURCE_FILES}
	${SEARCHEDIT_QRC_FILES}
	${TOAST_INCLUDE_FILES}
	${TOAST_SOURCE_FILES}
	${TOAST_QRC_FILES}
	${SIMPLE_TOAST_INCLUDE_FILES}
	${SIMPLE_TOAST_SOURCE_FILES}
	${SIMPLE_TOAST_QRC_FILES}
	${BASEWINDOW_INCLUDE_FILES}
	${BASEWINDOW_SOURCE_FILES}
	${BASEWINDOW_QRC_FILES}
	${FLOWLAYOUT_INCLUDE_FILES}
	${FLOWLAYOUT_SOURCE_FILES}
	${TREEEDIT_INCLUDE_FILES}
	${TREEEDIT_SOURCE_FILES}
	${TRANS_SLIDER_INCLUDE_FILES}
	${TRANS_SLIDER_SOURCE_FILES}
	${PEN_STYLE_INCLUDE_FILES}
	${PEN_STYLE_SOURCE_FILES}
	${COLORDIALOG_INCLUDE_FILES}
	${COLORDIALOG_SOURCE_FILES}
	${COLORDIALOG_FORM_FILES}
	${COLORDIALOG_QRC_FILES}
	${SINGLE_INSTANCE_INCLUDE_FILES}
	${SINGLE_INSTANCE_SOURCE_FILES}
	${LOAD_INDICATOR_INCLUDE_FILES}
	${LOAD_INDICATOR_SOURCE_FILES}
	${LOAD_INDICATOR_QRC_FILES}
	${LIB_MOC_HDRS}
	${LIB_MOC_SRCS}
	${LIB_QRC_SRCS}
)

if (Qt5Widgets_FOUND)
	target_link_libraries(
		${PROJECT_NAME}
        Qt5::OpenGL
        Qt5::Widgets
		Qt5::Network
		Qt5::Xml
	) 
else()
INCLUDE( ${QT_USE_FILE} )
    target_link_libraries(
        ${PROJECT_NAME}
        ${QT_LIBRARIES}
    )
endif()

# 生成导出宏文件
include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
	${PUBLIC_PATH}
    ${CMAKE_CURRENT_BINARY_DIR}	# 生成的导出宏文件在该路径下
)

# 安装文件
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY
	${CMAKE_CURRENT_SOURCE_DIR}/public/
	DESTINATION include/FreeXUI/CommonTools)
	
install(FILES
	${PROJECT_BINARY_DIR}/${PROJECT_NAME}_export.h
	DESTINATION include/FreeXUI/CommonTools)
