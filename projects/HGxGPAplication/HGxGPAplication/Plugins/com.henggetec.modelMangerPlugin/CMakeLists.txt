PROJECT(com_henggetec_modelMangerPlugin)

SET(PLUGIN_export_directive "com_henggetec_modelMangerPlugin_EXPORT")

SET(PLUGIN_SRCS
  ModelMangerPluginPlugin.cpp
  ModelMgr.cpp
  ModelMgr.h
  FxModelConfigReader.cpp
  FxModelConfigReader.h
  IModelSTMgrInterface.h 
)

# Files which should be processed by Qts moc
SET(PLUGIN_MOC_SRCS
  ModelMangerPluginPlugin_p.h
  ModelMgr.h
  FxModelConfigReader.h
  IModelSTMgrInterface.h 
)

# Qt Designer files which should be processed by Qts uic
SET(PLUGIN_UI_FORMS
)

# QRC Files which should be compiled into the plugin
SET(PLUGIN_resources
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
  debug ${HGBDP_DB_LIBRARY_DEBUG} optimized ${HGBDP_DB_LIBRARY} 
  debug ${HGBDP_TOOL_LIBRARY_DEBUG} optimized ${HGBDP_TOOL_LIBRARY}
  debug ${HGBDP_COMMON_LIBRARY_DEBUG} optimized ${HGBDP_COMMON_LIBRARY}
  debug ${HGGDP_TOOL_LIBRARY_DEBUG} optimized ${HGGDP_TOOL_LIBRARY}
  FreeXCommonToolUi
)
