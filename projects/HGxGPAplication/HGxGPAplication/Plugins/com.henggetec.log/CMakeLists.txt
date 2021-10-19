project(com_henggetec_log)

set(PLUGIN_export_directive "com_henggetec_log_EXPORT")

SET(PLUGIN_SRCS
    GxLog.h
    GxLog.cpp
    GxLogPlugin.h
    GxLogPlugin.cpp
    GxLogService.h
)

SET(PLUGIN_MOC_SRCS
    GxLog.h
    GxLogPlugin.h

)


#Compute the plugin dependencies
ctkFunctionGetTargetLibraries(PLUGIN_target_libraries)

HGxMacroBuildPlugin(
	NAME ${PROJECT_NAME}
	EXPORT_DIRECTIVE ${PLUGIN_export_directive}
	SRCS ${PLUGIN_SRCS}
  MOC_SRCS ${PLUGIN_MOC_SRCS}
	TARGET_LIBRARIES ${PLUGIN_target_libraries}
)