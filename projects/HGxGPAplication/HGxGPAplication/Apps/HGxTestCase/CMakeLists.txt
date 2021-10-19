project(HGxTestCase)


include_directories(
	${CMAKE_SOURCE_DIR}/Libs/HGxPFWCore
)


set(KIT_SRCS
	HGxTestCaseMain.cpp
	GxMainWindow.cpp
	GxMainWindow.h
)

# Headers that should run through moc
set(KIT_MOC_SRCS
	GxMainWindow.h
)

# UI files
set(KIT_UI_FORMS
	GxMainWindow.ui
)

# Resources
set(KIT_resources
	GxMainWindow.qrc
)

# Target libraries - See CMake/ctkMacroTargetLibraries.cmake
# The following macro will read the target libraries from the file 'target_libraries.cmake'
ctkFunctionGetTargetLibraries(KIT_target_libraries)

if(CTK_QT_VERSION VERSION_GREATER "4")
  list(APPEND KIT_target_libraries Qt5::Widgets)
else()
  list(APPEND KIT_target_libraries ${QT_LIBRARIES})
endif()

ctkMacroBuildApp(
  NAME ${PROJECT_NAME}
  SRCS ${KIT_SRCS}
  MOC_SRCS ${KIT_MOC_SRCS}
  UI_FORMS ${KIT_UI_FORMS}
  TARGET_LIBRARIES ${KIT_target_libraries} 
  RESOURCES ${KIT_resources}
  )