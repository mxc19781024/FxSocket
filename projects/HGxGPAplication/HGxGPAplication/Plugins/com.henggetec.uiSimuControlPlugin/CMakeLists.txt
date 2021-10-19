PROJECT(com_henggetec_uiSimuControlPlugin)

SET(PLUGIN_export_directive "com_henggetec_uiSimuControlPlugin_EXPORT")


set(PUBLIC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/public)
INCLUDE_DIRECTORIES(${PUBLIC_PATH})

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Libs/ServiceUtilUi)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Libs/ServiceModelUtilUi)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Libs/DataResUtilUi)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Plugins/com.henggetec.uiFramework)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Plugins/com.henggetec.basicCorePlugin)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Libs/PluginFrameworkCore)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Libs/SceneDndManager)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Libs/HGxPFWCore)

SET(PROJ_DIR_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/public/simu_control_ui)
SET(PROJ_DIR_SRC ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB PROJ_SRCS ${PROJ_DIR_SRC}/*.cpp ${PROJ_DIR_SRC}/*.h ${PROJ_DIR_SRC}/*.hpp ${PROJ_DIR_SRC}/*.tpp)
file(GLOB PROJ_HEADERS ${PROJ_DIR_INCLUDE}/*.h ${PROJ_DIR_INCLUDE}/*.hpp ${PROJ_DIR_INCLUDE}/*.tpp)
file(GLOB PROJ_UI ${PROJ_DIR_SRC}/*.ui)
file(GLOB PROJ_QRC ${PROJ_DIR_SRC}/*.qrc)
SOURCE_GROUP("Interface Files" FILES ${PROJ_HEADERS})
include_directories(${PROJ_DIR_SRC})

include_directories(${CMAKE_SOURCE_DIR}/Plugins/com.henggetec.uiFramework)

# 对外接口文件的路径
set(PUBLIC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/public)
INCLUDE_DIRECTORIES(${PUBLIC_PATH})

SET(PLUGIN_SRCS
  ${PROJ_SRCS}
  ${PROJ_HEADERS}
)


# Files which should be processed by Qts moc
SET(PLUGIN_MOC_SRCS
  UiSimuControlPluginPlugin_p.h
    CSimularControlUiPlugin.h
	CSimularControlWidget.h
)

# Qt Designer files which should be processed by Qts uic
SET(PLUGIN_UI_FORMS
	CSimularControlWidget.ui
)

# QRC Files which should be compiled into the plugin
SET(PLUGIN_resources
	freex_biz_simu_control_ui.qrc
)

#Compute the plugin dependencies
ctkFunctionGetTargetLibraries(PLUGIN_target_libraries)

ctkMacroBuildPlugin(
  NAME ${PROJECT_NAME}
  EXPORT_DIRECTIVE ${PLUGIN_export_directive}
  SRCS ${PLUGIN_SRCS}
  MOC_SRCS ${PLUGIN_MOC_SRCS}
  UI_FORMS ${PLUGIN_UI_FORMS}
  RESOURCES ${PLUGIN_resources}
  TARGET_LIBRARIES ${PLUGIN_target_libraries}
      ${OPENSCENEGRAPH_LIBRARIES} 
  ${HGBDP_LIBRARIES}
  ${HGGDP_LIBRARIES}
  debug ${OSG_LIBRARY_DEBUG} optimized ${OSG_LIBRARY}
  debug ${OSGDB_LIBRARY_DEBUG} optimized ${OSGDB_LIBRARY}
  debug ${osgEarth_DEBUG_LIBRARIES} optimized ${osgEarth_RELEASE_LIBRARIES}
  debug ${osgEarthSymbology_DEBUG_LIBRARIES} optimized ${osgEarthSymbology_RELEASE_LIBRARIES}
  debug ${osgEarthFeatures_DEBUG_LIBRARIES} optimized ${osgEarthFeatures_RELEASE_LIBRARIES}
  debug ${osgEarthAnnotation_DEBUG_LIBRARIES} optimized ${osgEarthAnnotation_RELEASE_LIBRARIES}
  debug ${HGBDP_DB_LIBRARY_DEBUG} optimized ${HGBDP_DB_LIBRARY} 
  debug ${OSGVIEWER_LIBRARY_DEBUG} optimized ${OSGVIEWER_LIBRARY}
  debug ${HGBDP_TOOL_LIBRARY_DEBUG} optimized ${HGBDP_TOOL_LIBRARY}
  debug ${HGBDP_SCENE_LIBRARY_DEBUG} optimized ${HGBDP_SCENE_LIBRARY}
  debug ${HGGDP_TOOL_LIBRARY_DEBUG} optimized ${HGGDP_TOOL_LIBRARY}
  debug ${HGGDP_SCENE_LIBRARY_DEBUG} optimized ${HGGDP_SCENE_LIBRARY}
  FreeXCommonToolUi
  DataResUtilUi
  ServiceUtilUi
  SceneDndManager
  Qt5::Widgets 
  Qt5::Gui 
  Qt5::Core
  Qt5::Concurrent
)
