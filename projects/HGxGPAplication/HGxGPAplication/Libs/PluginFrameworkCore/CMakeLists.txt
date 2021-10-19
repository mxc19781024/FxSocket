project(HGxPFWCore)


set(KIT_export_directive "HGX_PFCORE_EXPORT")

set(CMAKE_DEBUG_POSTFIX d)

set(KIT_SRCS
	GxPluginFrameworkCore.h
	GxPluginFrameworkCore.cpp
	GxPluginFwLauncherInterface.h
	GxPluginFwLauncherImpl.h
	GxPluginFwLauncherImpl.cpp
	GxEventHelperInterface.h
	GxEventHelperImpl.cpp
	GxEventHelperImpl.h
	GxServiceQuerier.h
)


# Headers that should run through moc
set(KIT_MOC_SRCS
	GxPluginFwLauncherImpl.h
	GxEventHelperImpl.h

)

# UI files
set(KIT_UI_FORMS
)

# Resources
set(KIT_resources
)

#include(${CMAKE_SOURCE_DIR}/CMake/HGxMacroBuildLib.cmake)

# Target libraries - See CMake/ctkMacroTargetLibraries.cmake
# The following macro will read the target libraries from the file 'target_libraries.cmake'
set(KIT_target_libraries)

ctkFunctionGetTargetLibraries(KIT_target_libraries)


IF(Qt5Widgets_FOUND)
	#moc
    SET(LIB_MOC_SRCS)
    FOREACH( LIB_MOC_HDR ${KIT_MOC_SRCS} )
        GET_FILENAME_COMPONENT( LIB_MOC_HDR_ABS ${LIB_MOC_HDR} ABSOLUTE )
        QT5_WRAP_CPP( LIB_MOC_SRC ${LIB_MOC_HDR} OPTIONS "-f${LIB_MOC_HDR_ABS}" )
        #LIST( APPEND LIB_MOC_SRCS ${LIB_MOC_SRC} )
		LIST( APPEND KIT_MOC_SRCS ${LIB_MOC_SRC} )
    ENDFOREACH()
	#ui
	SET(LIB_UI_SRCS)
	FOREACH( LIB_UI ${KIT_UI_FORMS} )
		 QT5_WRAP_UI( LIB_UI_SRC ${LIB_UI} )
		 LIST( APPEND LIB_UI_SRCS ${LIB_UI_SRC} )
	ENDFOREACH()
	#qrc
	QT5_ADD_RESOURCES(LIB_QRC_SRCS ${KIT_resources})
ELSE()
	FIND_PACKAGE(Qt4)
    INCLUDE( ${QT_USE_FILE} )
	#moc
    #QT4_WRAP_CPP( LIB_MOC_SRCS ${KIT_MOC_SRCS} OPTIONS "-f" )
	QT4_WRAP_CPP( KIT_MOC_SRCS ${KIT_MOC_SRCS} OPTIONS "-f" )
	#ui
	QT4_WRAP_UI( LIB_UI_SRCS ${KIT_UI_FORMS} )
	#qrc
	QT4_ADD_RESOURCES(LIB_QRC_SRCS ${KIT_resources})
ENDIF()


add_library(
	${PROJECT_NAME} SHARED
	${KIT_SRCS}
	${KIT_MOC_SRCS}
	${KIT_UI_FORMS}
	${KIT_resources}
)

target_link_libraries(
	${PROJECT_NAME}
	${KIT_target_libraries}
)



# 生成导出宏文件
include(GenerateExportHeader)
generate_export_header(
	${PROJECT_NAME}
	EXPORT_FILE_NAME HGX_PFCORE_EXPORT.h
)


target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}	# 生成的导出宏文件在该路径下
)

#HGxMacroBuildLib(
#  NAME ${PROJECT_NAME}
#  EXPORT_DIRECTIVE ${KIT_export_directive}
#  SRCS ${KIT_SRCS}
#  MOC_SRCS ${KIT_MOC_SRCS}
#  UI_FORMS ${KIT_UI_FORMS}
#  TARGET_LIBRARIES ${KIT_target_libraries} 
#  RESOURCES ${KIT_resources}
#  LIBRARY_TYPE ${CTK_LIBRARY_MODE}
#  )