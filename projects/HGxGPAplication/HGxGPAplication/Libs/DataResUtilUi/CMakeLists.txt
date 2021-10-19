project(DataResUtilUi)

include_directories(${CMAKE_SOURCE_DIR}/Libs/ServiceModelUtilUi/public)
include_directories(${CMAKE_SOURCE_DIR}/Plugins/com.henggetec.basicCorePlugin/public)

ADD_DEFINITIONS(-DGXDATARESOURCESWINDOW_LIB)

file(GLOB DATA_RES_WND_PUBLIC_FILES ./public/*.h)
file(GLOB DATA_RES_WND_HEADER_FILES *.h)
file(GLOB DATA_RES_WND_SOURCE_FILES *.cpp)
file(GLOB DATA_RES_WND_FORM_FILES *.ui)
set(DATA_RES_WND_INCLUDE_FILES
	${DATA_RES_WND_PUBLIC_FILES}
	${DATA_RES_WND_HEADER_FILES}
)
file(GLOB DATA_RES_WND_QRC_FILES *.qrc)

SOURCE_GROUP(HeaderFiles FILES ${DATA_RES_WND_INCLUDE_FILES})
SOURCE_GROUP(SrcFiles FILES ${DATA_RES_WND_SOURCE_FILES})
SOURCE_GROUP(FormFiles FILES ${DATA_RES_WND_FORM_FILES})
SOURCE_GROUP(ResFiles FILES ${DATA_RES_WND_QRC_FILES})

set(LIB_MOC_HDRS
  public/GxDataResourcesUtils.h
  public/GxLinkFileWindow.h
  public/GxServerDataWindow.h
)

IF(Qt5Widgets_FOUND)
	#moc
    SET(LIB_MOC_SRCS)
    FOREACH( LIB_MOC_HDR ${LIB_MOC_HDRS} )
        GET_FILENAME_COMPONENT( LIB_MOC_HDR_ABS ${LIB_MOC_HDR} ABSOLUTE )
        QT5_WRAP_CPP( LIB_MOC_SRC ${LIB_MOC_HDR} OPTIONS "-f${LIB_MOC_HDR_ABS}" )
        LIST( APPEND LIB_MOC_SRCS ${LIB_MOC_SRC} )
    ENDFOREACH()
	#ui
	SET(LIB_UI_SRCS)
	FOREACH( LIB_UI ${DATA_RES_WND_FORM_FILES} )
		 QT5_WRAP_UI( LIB_UI_SRC ${LIB_UI} )
		 LIST( APPEND LIB_UI_SRCS ${LIB_UI_SRC} )
	ENDFOREACH()
	#qrc
	QT5_ADD_RESOURCES(LIB_QRC_SRCS ${DATA_RES_WND_QRC_FILES})
ELSE()
	FIND_PACKAGE(Qt4)
    INCLUDE( ${QT_USE_FILE} )
	#moc
    QT4_WRAP_CPP( LIB_MOC_SRCS ${LIB_MOC_HDRS} OPTIONS "-f" )
	#ui
	QT4_WRAP_UI( LIB_UI_SRCS ${DATA_RES_WND_FORM_FILES} )
	#qrc
	QT4_ADD_RESOURCES(LIB_QRC_SRCS ${DATA_RES_WND_QRC_FILES})
ENDIF()

ctkFunctionGetTargetLibraries(PLUGIN_target_libraries)


ADD_LIBRARY(
	${PROJECT_NAME} SHARED
	${DATA_RES_WND_INCLUDE_FILES}
	${DATA_RES_WND_SOURCE_FILES}
	${DATA_RES_WND_FORM_FILES}
#	${DATA_RES_WND_QRC_FILES}
	${LIB_QRC_SRCS}
	${LIB_MOC_HDRS}
	${LIB_MOC_SRCS}
	${LIB_UI_SRCS}
)

if (Qt5Widgets_FOUND)
	target_link_libraries(
		${PROJECT_NAME}
		${PLUGIN_target_libraries}
        Qt5::OpenGL
        Qt5::Widgets
		Qt5::Network
		Qt5::Xml
		ServiceUtilUi
		FreeXCommonToolUi
		ServiceModelUtilUi
	) 
else()
    target_link_libraries(
        ${PROJECT_NAME}
        ${QT_LIBRARIES}
		ServiceUtilUi
		FreeXCommonToolUi
		ServiceModelUtilUi
        )
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${DATA_RES_WND_PUBLIC_PATH}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)