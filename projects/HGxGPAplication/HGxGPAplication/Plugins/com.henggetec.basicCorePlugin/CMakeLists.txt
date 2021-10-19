PROJECT(com_henggetec_basicCorePlugin)

SET(PLUGIN_export_directive "com_henggetec_basicCorePlugin_EXPORT")

# 对外接口文件的路径
set(PUBLIC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/public)
INCLUDE_DIRECTORIES(${PUBLIC_PATH})

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Libs/ServiceUtilUi)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Libs/ServiceModelUtilUi)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Libs/DataResUtilUi)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Plugins/com.henggetec.uiFramework)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Plugins/com.henggetec.basicCorePlugin)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Libs/PluginFrameworkCore)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Libs/SceneDndManager)


SET(PROJ_DIR_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/public)
SET(PROJ_DIR_SRC ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB PROJ_SRCS ${PROJ_DIR_SRC}/*.cpp ${PROJ_DIR_SRC}/*.h ${PROJ_DIR_SRC}/*.hpp ${PROJ_DIR_SRC}/*.tpp
					${PROJ_DIR_SRC}/scene/*.cpp ${PROJ_DIR_SRC}/scene/*.h ${PROJ_DIR_SRC}/scene/*.hpp ${PROJ_DIR_SRC}/scene/*.tpp
					${PROJ_DIR_SRC}/mapLayer/*.cpp ${PROJ_DIR_SRC}/mapLayer/*.h ${PROJ_DIR_SRC}/mapLayer/*.hpp ${PROJ_DIR_SRC}/mapLayer/*.tpp
					${PROJ_DIR_SRC}/earthLayer/*.cpp ${PROJ_DIR_SRC}/earthLayer/*.h ${PROJ_DIR_SRC}/earthLayer/*.hpp ${PROJ_DIR_SRC}/earthLayer/*.tpp
					${PROJ_DIR_SRC}/earthEffects/*.cpp ${PROJ_DIR_SRC}/earthEffects/*.h ${PROJ_DIR_SRC}/earthEffects/*.hpp ${PROJ_DIR_SRC}/earthEffects/*.tpp)

file(GLOB PROJ_LAYER_SRCS ${PROJ_DIR_SRC}/earthLayer/LayerProperty/*.cpp ${PROJ_DIR_SRC}/earthLayer/LayerProperty/*.h ${PROJ_DIR_SRC}/earthLayer/LayerProperty/*.hpp ${PROJ_DIR_SRC}/earthLayer/LayerProperty/*.tpp
						  ${PROJ_DIR_SRC}/mapLayer/LayerProperty/*.cpp ${PROJ_DIR_SRC}/mapLayer/LayerProperty/*.h ${PROJ_DIR_SRC}/mapLayer/LayerProperty/*.hpp ${PROJ_DIR_SRC}/mapLayer/LayerProperty/*.tpp)
						  
file(GLOB PROJ_HEADERS ${PROJ_DIR_INCLUDE}/earth_effects_ui/*.h ${PROJ_DIR_INCLUDE}/earth_effects_ui/*.hpp ${PROJ_DIR_INCLUDE}/earth_effects_ui/*.tpp
					   ${PROJ_DIR_INCLUDE}/earth_layer_ui/*.h ${PROJ_DIR_INCLUDE}/earth_layer_ui/*.hpp ${PROJ_DIR_INCLUDE}/earth_layer_ui/*.tpp
					   ${PROJ_DIR_INCLUDE}/layer_ui/*.h ${PROJ_DIR_INCLUDE}/layer_ui/*.hpp ${PROJ_DIR_INCLUDE}/layer_ui/*.tpp
					   ${PROJ_DIR_INCLUDE}/map_layer_ui/*.h ${PROJ_DIR_INCLUDE}/map_layer_ui/*.hpp ${PROJ_DIR_INCLUDE}/map_layer_ui/*.tpp
					   ${PROJ_DIR_INCLUDE}/scene_ui/*.h ${PROJ_DIR_INCLUDE}/scene_ui/*.hpp ${PROJ_DIR_INCLUDE}/scene_ui/*.tpp)

file(GLOB PROJ_UI ${PROJ_DIR_SRC}/*.ui 
				  ${PROJ_DIR_SRC}/mapLayer/LayerProperty/*.ui
				  ${PROJ_DIR_SRC}/earthLayer/LayerProperty/*.ui
				  ${PROJ_DIR_SRC}/earthEffects/*.ui
				  ${PROJ_DIR_SRC}/scene/*.ui
				  ${PROJ_DIR_SRC}/earthLayer/*.ui
				  ${PROJ_DIR_SRC}/mapLayer/*.ui)

file(GLOB PROJ_QRC ${PROJ_DIR_SRC}/*.qrc 				  
				   ${PROJ_DIR_SRC}/mapLayer/LayerProperty/*.qrc 
				   ${PROJ_DIR_SRC}/earthLayer/LayerProperty/*.qrc 
				   ${PROJ_DIR_SRC}/earthEffects/*.qrc 
				   ${PROJ_DIR_SRC}/scene/*.qrc 
				   ${PROJ_DIR_SRC}/earthLayer/*.qrc 
				   ${PROJ_DIR_SRC}/mapLayer/*.qrc)

SOURCE_GROUP("Interface Files" FILES ${PROJ_HEADERS})
SOURCE_GROUP("LayerProperty" FILES ${PROJ_LAYER_SRCS})
include_directories(${PROJ_DIR_SRC})
include_directories(${PROJ_DIR_INCLUDE})

SET(PLUGIN_SRCS
  ${PROJ_SRCS}
  ${PROJ_LAYER_SRCS}
  ${PROJ_HEADERS}
 )

# Files which should be processed by Qts moc
SET(PLUGIN_MOC_SRCS
  GxBasicCorePlugin_p.h
  CCheckableTreeItem.h
  CCommonItem.h
  CPyramidProcessListener.h
  
  earthLayer/C3DRealSceneItem.h
  earthLayer/C3DRealScene.h
  earthLayer/C3DRealSceneSelectWidget.h
  earthLayer/C3DRealSceneUiPlugin.h
  earthLayer/CEarthLayerItem.h
  earthLayer/CEarthLayerTreeWidget.h
  earthLayer/CEarthLayerWidget.h
  earthLayer/CEarthFileWindow.h
  earthLayer/CLayerTextEdit.h
  earthLayer/CEarthPyramidCache.h
  earthLayer/CEarthPyramidCacheWgt.h
  earthLayer/CTiltAddModelWgt.h
  earthLayer/CTiltToolWgt.h
  earthLayer/LayerProperty/CEarthDotLayerPropertyWgt.h
  earthLayer/LayerProperty/CEarthLayerPropertyMgr.h
  earthLayer/LayerProperty/CEleLayerPropertyWgt.h
  earthLayer/LayerProperty/CEarthFacetLayerPropertyWgt.h
  earthLayer/LayerProperty/CEarthImageLayerPropertyWgt.h
  earthLayer/LayerProperty/CEarthLineLayerPropertyWgt.h
  earthLayer/LayerProperty/CTiltModelWgt.h
  earthLayer/LayerProperty/CTiltPropertyWgt.h
  
  mapLayer/CMapFileWindow.h
  mapLayer/CMapLayerItem.h
  mapLayer/CMapLayerTreeWidget.h
  mapLayer/CMapLayerWidget.h
  mapLayer/CMapPyramidCache.h
  mapLayer/CMapPyramidCacheWgt.h
  mapLayer/LayerProperty/CMapDotLayerPropertyWgt.h
  mapLayer/LayerProperty/CMapFacetLayerPropertyWgt.h
  mapLayer/LayerProperty/CMapImageLayerPropertyWgt.h
  mapLayer/LayerProperty/CMapLayerPropertyMgr.h
  mapLayer/LayerProperty/CMapLineLayerPropertyWgt.h
  
  earthEffects/CEffectsControlItem.h
  earthEffects/CEffectsUiPlugin.h
  earthEffects/CEnvirAttrSetting.h
  earthEffects/CSpecialEffectsControl.h
  
  scene/CSceneObj.h
  scene/CSceneUiPlugin.h
)

# Qt Designer files which should be processed by Qts uic
SET(PLUGIN_UI_FORMS
	${PROJ_UI}
)

# QRC Files which should be compiled into the plugin
SET(PLUGIN_resources
	${PROJ_QRC}
)

message(${GDAL_LIBRARIES})

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
  ${HGBDP_LIBRARIES}
  ${HGGDP_LIBRARIES}
  debug ${OSG_LIBRARY_DEBUG} optimized ${OSG_LIBRARY}
  debug ${OSGDB_LIBRARY_DEBUG} optimized ${OSGDB_LIBRARY}
  debug ${osgEarth_DEBUG_LIBRARIES} optimized ${osgEarth_RELEASE_LIBRARIES}
  debug ${OPENTHREADS_LIBRARY_DEBUG} optimized ${OPENTHREADS_LIBRARY}
  debug ${osgEarthSymbology_DEBUG_LIBRARIES} optimized ${osgEarthSymbology_RELEASE_LIBRARIES}
  debug ${HGGDP_SCENE_LIBRARY_DEBUG} optimized ${HGGDP_SCENE_LIBRARY}
  debug ${HGGDP_ALGOR_LIBRARY_DEBUG} optimized ${HGGDP_ALGOR_LIBRARY}
  ${GDAL_LIBRARIES}
  FreeXCommonToolUi
  DataResUtilUi
  ServiceUtilUi
  SceneDndManager
  Qt5::Widgets 
  Qt5::Gui 
  Qt5::Core
  Qt5::Concurrent
)
