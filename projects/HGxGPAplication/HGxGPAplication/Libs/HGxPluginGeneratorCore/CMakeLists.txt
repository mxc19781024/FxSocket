project(HGxPluginGeneratorCore)

set(KIT_export_directive "HGX_PLUGINGENERATOR_CORE_EXPORT")

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

if(CTK_QT_VERSION VERSION_GREATER "4")
  list(APPEND KIT_target_libraries Qt5::Core)
else()
  list(APPEND KIT_target_libraries ${QT_LIBRARIES})
endif()


HGxMacroBuildLib(
  NAME ${PROJECT_NAME}
  EXPORT_DIRECTIVE ${KIT_export_directive}
  SRCS ${KIT_SRCS}
  MOC_SRCS ${KIT_MOC_SRCS}
  UI_FORMS ${KIT_UI_FORMS}
  TARGET_LIBRARIES ${KIT_target_libraries}
  RESOURCES ${KIT_resources}
  LIBRARY_TYPE ${CTK_LIBRARY_MODE}
)
