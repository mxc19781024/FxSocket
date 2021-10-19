PROJECT(com_henggetec_earthManipulatorPlugin)

SET(PLUGIN_export_directive "com_henggetec_earthManipulatorPlugin_EXPORT")

SET(PROJ_DIR_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/public/earth_manipulator_ui)
SET(PROJ_DIR_SRC ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB PROJ_SRCS ${PROJ_DIR_SRC}/*.cpp ${PROJ_DIR_SRC}/*.h ${PROJ_DIR_SRC}/*.hpp ${PROJ_DIR_SRC}/*.tpp)
file(GLOB PROJ_HEADERS ${PROJ_DIR_INCLUDE}/*.h ${PROJ_DIR_INCLUDE}/*.hpp ${PROJ_DIR_INCLUDE}/*.tpp)
file(GLOB PROJ_UI ${PROJ_DIR_SRC}/*.ui)
file(GLOB PROJ_QRC ${PROJ_DIR_SRC}/*.qrc)
SOURCE_GROUP("Interface Files" FILES ${PROJ_HEADERS})

include_directories(${PROJ_DIR_SRC})
include_directories(${CMAKE_SOURCE_DIR}/Plugins/com.henggetec.basicCorePlugin/public)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Plugins/com.henggetec.uiFramework)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Libs/PluginFrameworkCore)

# 对外接口文件的路径
set(PUBLIC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/public)
INCLUDE_DIRECTORIES(${PUBLIC_PATH})

SET(PLUGIN_SRCS
  ${PROJ_SRCS}
  ${PROJ_HEADERS}
)

# Files which should be processed by Qts moc
SET(PLUGIN_MOC_SRCS
  CFloatScrollBar.h
  CSceneTool.h
  CSceneToolItem.h
  CSceneToolSelectWidget.h
  CSceneToolUiPlugin.h
  FlowLayout.h
  GxEarthManipulatorPlugin_p.h
)

# Qt Designer files which should be processed by Qts uic
SET(PLUGIN_UI_FORMS
	${PROJ_UI}
)

# QRC Files which should be compiled into the plugin
SET(PLUGIN_resources
	${PROJ_QRC}
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
  debug ${osgEarth_DEBUG_LIBRARIES} optimized ${osgEarth_RELEASE_LIBRARIES}
  FreeXCommonToolUi
  debug ${HGBDP_TOOL_LIBRARY_DEBUG} optimized ${HGBDP_TOOL_LIBRARY}
  debug ${HGBDP_SCENE_LIBRARY_DEBUG} optimized ${HGBDP_SCENE_LIBRARY}
  debug ${HGGDP_TOOL_LIBRARY_DEBUG} optimized ${HGGDP_TOOL_LIBRARY}
  debug ${HGGDP_SCENE_LIBRARY_DEBUG} optimized ${HGGDP_SCENE_LIBRARY}
  Qt5::Widgets 
  Qt5::Gui 
  Qt5::Core
  Qt5::Concurrent
)
