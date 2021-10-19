project(HGxGPApplication)

#
# See CTK/CMake/ctkMacroBuildApp.cmake for details
#

add_definitions(-DHGX_GUI_NAMESPACE -DQTN_NAMESPACE)

include_directories(
	${CMAKE_SOURCE_DIR}/Libs/PluginFrameworkCore
)

include_directories(${CMAKE_SOURCE_DIR}/Plugins/com.henggetec.uiFramework)
include_directories(${CMAKE_SOURCE_DIR}/Plugins/com.henggetec.simuControlPlugin/public)
include_directories(${CMAKE_SOURCE_DIR}/Plugins/com.henggetec.earthManipulatorPlugin/public)
include_directories(${CMAKE_SOURCE_DIR}/Plugins/com.henggetec.mapManipulatorPlugin/public)
include_directories(${CMAKE_SOURCE_DIR}/Plugins/com.henggetec.basicCorePlugin/public)
include_directories(${CMAKE_SOURCE_DIR}/Plugins/com.henggetec.servicePlugin/public)

set(KIT_SRCS
	HGxGPApplicationMain.cpp
	GxProcessor.h
	GxProcessor.cpp
	LoginUI.cpp
    LoginUI.h
)

# Headers that should run through moc
set(KIT_MOC_SRCS
	GxProcessor.h
	LoginUI.h
)

# UI files
set(KIT_UI_FORMS
)

# Resources
set(KIT_resources
   Resource/image.qrc
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

  debug ${HGBDP_DB_LIBRARY_DEBUG} optimized ${HGBDP_DB_LIBRARY} 
  debug ${OSGVIEWER_LIBRARY_DEBUG} optimized ${OSGVIEWER_LIBRARY}
  debug ${HGBDP_TOOL_LIBRARY_DEBUG} optimized ${HGBDP_TOOL_LIBRARY}
  debug ${HGBDP_UTIL_LIBRARY_DEBUG} optimized ${HGBDP_UTIL_LIBRARY}
  debug ${HGBDP_SCENE_LIBRARY_DEBUG} optimized ${HGBDP_SCENE_LIBRARY}
  debug ${HGGDP_TOOL_LIBRARY_DEBUG} optimized ${HGGDP_TOOL_LIBRARY}
  debug ${HGGDP_SCENE_LIBRARY_DEBUG} optimized ${HGGDP_SCENE_LIBRARY}
  FreeXCommonToolUi
  RESOURCES ${KIT_resources}
  )
  
IF(WIN32)
    # 去除命令提示框
    SET_PROPERTY(TARGET HGxGPApplication PROPERTY LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
ENDIF()