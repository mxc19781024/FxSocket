set(LIBS
  HGxPFWCore
  HGxPluginGeneratorCore
  HGxPluginGeneratorUI
  DataResUtilUi
  FreeXCommonToolUi
  ServiceModelUtilUi
  ServiceUtilUi
  SceneDndManager
)

#### 定义路径变量
# public目录
SET(FREEX_UI_PUBLIC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

# 插件的头文件目录
SET(FX_LIB_HEADER_DIR "${FREEX_UI_PUBLIC_DIR}/FreeXCommonToolUi/public")
include_directories("${FX_LIB_HEADER_DIR}")
include_directories("${FREEX_UI_PUBLIC_DIR}/FreeXCommonToolUi")

foreach(lib ${LIBS})
  option(BUILD_LIBS_${lib} "Build the ${lib} lib" ON)
  add_subdirectory(${lib})
  set_target_properties(${lib} PROPERTIES FOLDER "Libs")
endforeach()
