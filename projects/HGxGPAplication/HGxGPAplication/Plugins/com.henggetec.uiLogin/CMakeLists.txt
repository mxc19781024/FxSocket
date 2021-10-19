PROJECT(com_henggetec_uiLogin)

SET(PLUGIN_export_directive "com_henggetec_uiLogin_EXPORT")

set(PUBLIC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/public/common)
INCLUDE_DIRECTORIES(${PUBLIC_PATH})

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Libs/ServiceUtilUi)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Libs/ServiceModelUtilUi)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Libs/DataResUtilUi)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Plugins/com.henggetec.uiFramework)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Plugins/com.henggetec.simuControlPlugin/public)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Plugins/com.henggetec.earthManipulatorPlugin/public)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Plugins/com.henggetec.mapManipulatorPlugin/public)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Plugins/com.henggetec.basicCorePlugin/public)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Plugins/com.henggetec.servicePlugin/public)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Libs/PluginFrameworkCore)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Libs/SceneDndManager)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Libs/HGxPFWCore)
SET(PLUGIN_SRCS
  henggetecUiLoginPlugin.cpp
  LoginUI.cpp
  LoginUI.h
  GxProcessor.cpp
  GxProcessor.h
)

# Files which should be processed by Qts moc
SET(PLUGIN_MOC_SRCS
  henggetecUiLoginPlugin_p.h
  LoginUI.h
  GxProcessor.h
)

# Qt Designer files which should be processed by Qts uic
SET(PLUGIN_UI_FORMS

)

# QRC Files which should be compiled into the plugin
SET(PLUGIN_resources
    Resource/image.qrc
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
