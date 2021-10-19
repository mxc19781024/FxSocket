project(HGxPluginGenerator)

set(KIT_SRCS
  GxPluginGenerator.cpp
  HGxPluginGeneratorMain.cpp
  GxPluginGeneratorOptionsDialog.cpp
)

# Headers that should run through moc
set(KIT_MOC_SRCS
  GxPluginGenerator_p.h
  GxPluginGeneratorOptionsDialog_p.h
)

# UI files
set(KIT_UI_FORMS
  GxPluginGeneratorMainWindow.ui
  GxPluginGeneratorOptionsDialog.ui
)

# Resources
set(KIT_resources
  GxPluginGenerator.qrc
)

# Target libraries - See CMake/ctkMacroTargetLibraries.cmake
# The following macro will read the target libraries from the file 'target_libraries.cmake'
ctkFunctionGetTargetLibraries(KIT_target_libraries)

ctkMacroBuildApp(
  NAME ${PROJECT_NAME}
  SRCS ${KIT_SRCS}
  MOC_SRCS ${KIT_MOC_SRCS}
  UI_FORMS ${KIT_UI_FORMS}
  TARGET_LIBRARIES ${KIT_target_libraries} debug ${HGBDP_DB_LIBRARY_DEBUG} optimized ${HGBDP_DB_LIBRARY}
  RESOURCES ${KIT_resources}
  )
