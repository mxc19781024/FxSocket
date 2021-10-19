project(SceneDndManager)

file(GLOB SERVICE_MODEL_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/public/*.h)
file(GLOB SERVICE_MODEL_SOURCE_FILES *.cpp)

SOURCE_GROUP(HeaderFiles FILES ${SERVICE_MODEL_HEADER_FILES})
SOURCE_GROUP(SrcFiles FILES ${SERVICE_MODEL_SOURCE_FILES})
SOURCE_GROUP(ResFiles FILES ${SERVICE_MODEL_QRC_FILES})

set(LIB_MOC_HDRS
  ${CMAKE_CURRENT_SOURCE_DIR}/public/CSceneDndManager.h
  ${CMAKE_CURRENT_SOURCE_DIR}/CSceneDndManager_p.h
)

IF(Qt5Widgets_FOUND)
	#moc
    SET(LIB_MOC_SRCS)
    FOREACH( LIB_MOC_HDR ${LIB_MOC_HDRS} )
        GET_FILENAME_COMPONENT( LIB_MOC_HDR_ABS ${LIB_MOC_HDR} ABSOLUTE )
        QT5_WRAP_CPP( LIB_MOC_SRC ${LIB_MOC_HDR} OPTIONS "-f${LIB_MOC_HDR_ABS}" )
        LIST( APPEND LIB_MOC_SRCS ${LIB_MOC_SRC} )
    ENDFOREACH()
ELSE()
	FIND_PACKAGE(Qt4)
    INCLUDE( ${QT_USE_FILE} )
	#moc
    QT4_WRAP_CPP( LIB_MOC_SRCS ${LIB_MOC_HDRS} OPTIONS "-f" )
ENDIF()

ADD_LIBRARY(
	${PROJECT_NAME} SHARED
	${SERVICE_MODEL_HEADER_FILES}
	${SERVICE_MODEL_SOURCE_FILES}
	${LIB_MOC_HDRS}
	${LIB_MOC_SRCS}
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
    target_link_libraries(
        ${PROJECT_NAME}
        ${QT_LIBRARIES}
        )
endif()

ADD_DEFINITIONS(-DSCENEDNDMANAGER_EXPORTS)

# 生成导出宏文件
include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${SERVICE_MODEL_HEADER_FILES}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_CURRENT_DIR}
)