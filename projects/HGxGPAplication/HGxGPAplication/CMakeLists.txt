cmake_minimum_required(VERSION 3.0)

foreach(p
  CMP0054 # CMake 3.1
  CMP0020 # CMake 2.8.11
  )
  if(POLICY ${p})
    cmake_policy(SET ${p} NEW)
  endif()
endforeach()

project(HGxGPApp)

#生成过滤器里的文件夹
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

IF(WIN32 AND NOT ANDROID)
    IF(MSVC)
		OPTION(WIN32_USE_MP "Set to ON to build OpenSceneGraph with the /MP option (Visual Studio 2005 and above)." OFF)
        MARK_AS_ADVANCED(WIN32_USE_MP)
        IF(WIN32_USE_MP)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
        ENDIF(WIN32_USE_MP)
	ENDIF()
ENDIF()

#-----------------------------------------------------------------------------
# CTK
#
find_package(CTK REQUIRED)
#-----------------------------------------------------------------------------


#-----------------------------------------------------------------------------
# QT
#
ctkMacroSetupQt()
#-----------------------------------------------------------------------------

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

include(find_hg_osgEarth)
IF(WIN32 AND NOT ANDROID)
	SET(OSG_DIR "" CACHE PATH "Set to base OpenSceneGraph install path")
	include_directories("${OSG_DIR}/include")
	link_directories("${OSG_DIR}/lib")
	SET(OSGEARTH_DIR "" CACHE PATH "Set to OSGEARTH   install path")
	include_directories("${OSGEARTH_DIR}/include")
	link_directories("${OSGEARTH_DIR}/lib")
ENDIF()

#添加三方库
	SET(3RD_PARTY_DIR "" CACHE PATH "Set to base lib install path")
	include_directories("${3RD_PARTY_DIR}/include")
	link_directories("${3RD_PARTY_DIR}/lib")
find_package(GDAL)

find_package(OpenSceneGraph 3.2.1 REQUIRED 
	osg 
	osgDB
	osgViewer
)
if (WIN32)
    if (OPENSCENEGRAPH_FOUND)
        include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})
    else()
        MESSAGE("=========== Need Set OSG_DIR environment=============")
        MESSAGE("=========== Need Set OSG_DIR environment=============")
        MESSAGE("=========== Need Set OSG_DIR environment=============")
    endif()
endif()

include(FindHGBDP)
IF(WIN32 AND NOT ANDROID)
	SET(HGBDP_DIR "" CACHE PATH "Set to HGBDP install path")
	include_directories("${HGBDP_DIR}/include")
	link_directories("${HGBDP_DIR}/lib")
ENDIF()

include(FindHGGDP)
IF(WIN32 AND NOT ANDROID)
	SET(HGGDP_DIR "" CACHE PATH "Set to HGGDP install path")
	include_directories("${HGGDP_DIR}/include")
	link_directories("${HGGDP_DIR}/lib")
ENDIF()



include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CTK_INCLUDE_DIRS} 
)

include(${CMAKE_SOURCE_DIR}/CMake/HGxMacroBuildLib.cmake)
include(${CMAKE_SOURCE_DIR}/CMake/HGxMacroBuildPlugin.cmake)

FIND_PACKAGE(Qt5PrintSupport)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
        LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

#> Extract all library names starting with com_henggetec_
macro(GetMyTargetLibraries all_target_libraries varname)
  set(re_ctkplugin "^com_henggetec_[a-zA-Z0-9_]+$")
  set(_tmp_list)
  list(APPEND _tmp_list ${all_target_libraries})
  ctkMacroListFilter(_tmp_list re_ctkplugin OUTPUT_VARIABLE ${varname})
endmacro()
#>


#> Create a list of available plug-ins and make them known to the build system
set(plugins
	Plugins/com.henggetec.uiFramework:ON
	Plugins/com.henggetec.log:ON
	Plugins/com.henggetec.simuControlPlugin:ON
	Plugins/com.henggetec.mapManipulatorPlugin:ON
	Plugins/com.henggetec.basicCorePlugin:ON
	Plugins/com.henggetec.earthManipulatorPlugin:ON
	Plugins/com.henggetec.servicePlugin:ON
	Plugins/com.henggetec.uiEquipmentInfoPlugin:ON
	Plugins/com.henggetec.uiEequipEnergyInfoPlugin:ON
	Plugins/com.henggetec.uiGanateChartPlugin:ON	
	Plugins/com.henggetec.uiEvaluatePlugin:ON	
	Plugins/com.henggetec.uiSimuControlPlugin:ON	
	Plugins/com.henggetec.uiLoadDataPlugin:ON	
	Plugins/com.henggetec.dataMgrPlugin:ON	
	Plugins/com.henggetec.driveMgrInterfacePlugin:ON	
	Plugins/com.henggetec.uiLogin:ON
	Plugins/com.henggetec.networkRecv:ON
	Plugins/com.henggetec.eventadmin:ON
	Plugins/com.henggetec.modelMangerPlugin:ON	
)

ctkMacroSetupPlugins(${plugins})
#>

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Apps)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Libs)

set(HGX_PLUGIN_LIBRARIES_DIR_CONFIG "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins")
set(HGX_PLUGINS_SRC_DIR_CONFIG "${CMAKE_CURRENT_SOURCE_DIR}/Plugins")
configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/CMake/GxGPAppConfig.h.in"
            "${CMAKE_CURRENT_BINARY_DIR}/GxGPAppConfig.h" )