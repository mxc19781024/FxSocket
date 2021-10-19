project(HGxPFWCore)


set(KIT_export_directive "HGX_PFCORE_EXPORT")

set(KIT_SRCS
	GxPluginFrameworkCore.h
	GxPluginFrameworkCore.cpp
	GxPluginFwLauncherInterface.h
	GxPluginFwLauncherImpl.h
	GxPluginFwLauncherImpl.cpp
	GxEventHelperInterface.h
	GxEventHelperImpl.cpp
	GxEventHelperImpl.h
	GxServiceQuerier.h
)


# Headers that should run through moc
set(KIT_MOC_SRCS
	GxPluginFwLauncherImpl.h
	GxEventHelperImpl.h

)

# UI files
set(KIT_UI_FORMS
)

# Resources
set(KIT_resources
)

#include(${CMAKE_SOURCE_DIR}/CMake/HGxMacroBuildLib.cmake)

# Target libraries - See CMake/ctkMacroTargetLibraries.cmake
# The following macro will read the target libraries from the file 'target_libraries.cmake'
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