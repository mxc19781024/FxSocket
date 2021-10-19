project(ServiceUtilUi)

include_directories(${CMAKE_SOURCE_DIR}/Libs/ServiceModelUtilUi/public)
#include_directories(${CMAKE_SOURCE_DIR}/Plugins/com.henggetec.uiFramework)
#include_directories(${CMAKE_SOURCE_DIR}/Libs/PluginFrameworkCore)

ADD_DEFINITIONS(-DCSERVERPROUTILS_LIB)

#µÚÈý·½ÒÀÀµ¿â
find_package(QtitanBase)
find_package(QtitanDocking)

set(SERVER_PRO_UTILS_PUBLIC_PATH ./public)
file(GLOB SERVER_PRO_UTILS_PUBLIC_FILES
	${SERVER_PRO_UTILS_PUBLIC_PATH}/*.h
)
file(GLOB SERVER_PRO_UTILS_HEADER_FILES *.h)
file(GLOB SERVER_PRO_UTILS_SOURCE_FILES *.cpp)
file(GLOB  SERVER_PRO_UTILS_QRC_FILES *.qrc)
file(GLOB SERVER_PRO_UTILS_FORM_FILES *.ui)
set(SERVER_PRO_UTILS_INCLUDE_FILES
	${SERVER_PRO_UTILS_PUBLIC_FILES}
	${SERVER_PRO_UTILS_HEADER_FILES}
)

SOURCE_GROUP(HeaderFiles FILES ${SERVER_PRO_UTILS_INCLUDE_FILES})
SOURCE_GROUP(SrcFiles FILES ${SERVER_PRO_UTILS_SOURCE_FILES})
SOURCE_GROUP(FormFiles FILES ${SERVER_PRO_UTILS_FORM_FILES})

# Files which should be processed by Qts moc
set(LIB_MOC_HDRS
  ${CMAKE_CURRENT_SOURCE_DIR}/public/GxServerProUtils.h
  ${CMAKE_CURRENT_SOURCE_DIR}/public/GxServiceProWgt.h
  GxAddDataSetWindow.h
  GxAddOGCWindow.h
  GxAddServiceMgr.h
  GxAddTileWindow.h
  GxDataSetLayerProWindow.h
  GxLimitLineEdit.h
  GxOGCLayerProWindow.h
  GxOGCLayerSetProWindow.h
  GxTileLayerProWindow.h
)

set (GUI_QRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/GxServerProUtils.qrc
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
	FOREACH( LIB_UI ${SERVER_PRO_UTILS_FORM_FILES} )
		 QT5_WRAP_UI( LIB_UI_SRC ${LIB_UI} )
		 LIST( APPEND LIB_UI_SRCS ${LIB_UI_SRC} )
	ENDFOREACH()
	#qrc
	QT5_ADD_RESOURCES(LIB_QRC_SRCS ${GUI_QRC_FILES})
ELSE()
	FIND_PACKAGE(Qt4)
    INCLUDE( ${QT_USE_FILE} )
	#moc
    QT4_WRAP_CPP( LIB_MOC_SRCS ${LIB_MOC_HDRS} OPTIONS "-f" )
	#ui
	QT4_WRAP_UI( LIB_UI_SRCS ${SERVER_PRO_UTILS_FORM_FILES} )
	#qrc
	QT4_ADD_RESOURCES(LIB_QRC_SRCS ${GUI_QRC_FILES})
ENDIF()

ctkFunctionGetTargetLibraries(PLUGIN_target_libraries)

ADD_LIBRARY(
	${PROJECT_NAME} SHARED
	${SERVER_PRO_UTILS_INCLUDE_FILES}
	${SERVER_PRO_UTILS_SOURCE_FILES}
	${SERVER_PRO_UTILS_FORM_FILES}
	${SERVER_PRO_UTILS_QRC_FILES}
	${LIB_MOC_HDRS}
	${LIB_MOC_SRCS}
	${LIB_UI_SRCS}
	${LIB_QRC_SRCS}
)

if (Qt5Widgets_FOUND)
	target_link_libraries(
		${PROJECT_NAME}
		${PLUGIN_target_libraries}
        Qt5::OpenGL
        Qt5::Widgets
		Qt5::Network
		Qt5::Xml
		FreeXCommonToolUi
		ServiceModelUtilUi
#		QtitanBase
#		QtitanDocking
		debug ${HGGDP_UTIL_LIBRARY_DEBUG} optimized ${HGGDP_UTIL_LIBRARY}
		debug ${OSG_LIBRARY_DEBUG} optimized ${OSG_LIBRARY}
		debug ${OPENTHREADS_LIBRARY_DEBUG} optimized ${OPENTHREADS_LIBRARY}
	) 
else()
    target_link_libraries(
        ${PROJECT_NAME}
        ${QT_LIBRARIES}
		${PLUGIN_target_libraries}
		FreeXCommonToolUi
		ServiceModelUtilUi
#		QtitanBase
#		QtitanDocking
		debug ${HGGDP_UTIL_LIBRARY_DEBUG} optimized ${HGGDP_UTIL_LIBRARY}
		debug ${OSG_LIBRARY_DEBUG} optimized ${OSG_LIBRARY}
		debug ${OPENTHREADS_LIBRARY_DEBUG} optimized ${OPENTHREADS_LIBRARY}
	)
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${SERVER_PRO_UTILS_PUBLIC_PATH}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)