PROJECT(com_henggetec_uiFramework)

SET(PLUGIN_export_directive "com_henggetec_uiFramework_EXPORT")

#添加预处理器
add_definitions(-DHGX_GUI_NAMESPACE -DQTN_NAMESPACE)
#第三方依赖库
find_package(QtitanBase)
find_package(QtitanDocking)

include_directories(
	${CMAKE_SOURCE_DIR}/Libs/PluginFrameworkCore
)

SET(PLUGIN_SRCS
  GxUiFrameworkPlugin.cpp
  GxGuiDef.h
  GxUiFrameworkServiceInterface.h
  GxGuiMainWindow.h
  CXGisTopWidget.h
  GxGuiMainWindow.cpp
  CXGisTopWidget.cpp
)

# Files which should be processed by Qts moc
SET(PLUGIN_MOC_SRCS
  GxUiFrameworkPlugin_p.h
  GxUiFrameworkServiceInterface.h
  GxGuiMainWindow.h
  CXGisTopWidget.h
)

# Qt Designer files which should be processed by Qts uic
SET(PLUGIN_UI_FORMS
  CXGisTopWidget.ui
  GxGuiMainWindow.ui
)

# QRC Files which should be compiled into the plugin
SET(PLUGIN_resources
  CXGisTopWidget.qrc
)

#Compute the plugin dependencies
ctkFunctionGetTargetLibraries(PLUGIN_target_libraries)

#链接第三方依赖库
if(CTK_QT_VERSION VERSION_GREATER "4")
  list(APPEND PLUGIN_target_libraries Qt5::Widgets Qt5::Gui Qt5::Core QtitanBase QtitanDocking)
else()
  list(APPEND PLUGIN_target_libraries ${QT_LIBRARIES} QtitanBase QtitanDocking)
endif()

HGxMacroBuildPlugin(
  NAME ${PROJECT_NAME}
  EXPORT_DIRECTIVE ${PLUGIN_export_directive}
  SRCS ${PLUGIN_SRCS}
  MOC_SRCS ${PLUGIN_MOC_SRCS}
  UI_FORMS ${PLUGIN_UI_FORMS}
  RESOURCES ${PLUGIN_resources}
  TARGET_LIBRARIES ${PLUGIN_target_libraries}
  ${OPENSCENEGRAPH_LIBRARIES} 
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
)
