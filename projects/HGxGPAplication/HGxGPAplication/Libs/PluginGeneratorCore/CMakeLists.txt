project(HGxPluginGeneratorCore)

set(KIT_export_directive "HGX_PLUGINGENERATOR_CORE_EXPORT")

set(CMAKE_DEBUG_POSTFIX d)

set(KIT_SRCS
  GxPluginGeneratorAbstractExtension.cpp
  GxPluginGeneratorAbstractTemplate.cpp
  GxPluginGeneratorConstants.cpp
  GxPluginGeneratorCorePlugin.cpp
  GxPluginGeneratorCMakeLists.cpp
  GxPluginGeneratorCodeModel.cpp
  GxPluginGeneratorCppPluginActivator
  GxPluginGeneratorCppTemplate.cpp
  GxPluginGeneratorHeaderTemplate.cpp
  GxPluginGeneratorTargetLibraries.cpp
)

set(KIT_MOC_SRCS
  GxPluginGeneratorAbstractExtension.h
  GxPluginGeneratorAbstractTemplate.h
  GxPluginGeneratorCodeModel.h
  GxPluginGeneratorCorePlugin_p.h
)

set(KIT_UI_FORMS

)

set(KIT_resources

)

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

if(CTK_QT_VERSION VERSION_LESS "5") #qt4
target_link_libraries(
	${PROJECT_NAME}
	${KIT_target_libraries}
	${QT_LIBRARIES}
)
else() #qt5
	target_link_libraries( 
	${PROJECT_NAME}
	${KIT_target_libraries}
	Qt5::Core
	)
endif()



# 生成导出宏文件
include(GenerateExportHeader)
generate_export_header(
	${PROJECT_NAME}
	EXPORT_FILE_NAME HGX_PLUGINGENERATOR_CORE_EXPORT.h
)


target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}	# 生成的导出宏文件在该路径下
)


#ctkMacroBuildLib(
#  NAME ${PROJECT_NAME}
#  EXPORT_DIRECTIVE ${PLUGIN_export_directive}
#  SRCS ${PLUGIN_SRCS}
#  MOC_SRCS ${PLUGIN_MOC_SRCS}
#  UI_FORMS ${PLUGIN_UI_FORMS}
#  RESOURCES ${PLUGIN_resources}
#  TARGET_LIBRARIES ${PLUGIN_target_libraries}
#)
