set(APPS
  HGxPluginGenerator
  #HGxTestCase
  HGxGPApplication
  FreeXSim
  FrameLogin
  FreeXAcmiParse
)

foreach(app ${APPS})
  option(BUILD_APP_${app} "Build the ${app} application" ON)
  add_subdirectory(${app})
  set_target_properties(${app} PROPERTIES FOLDER "Apps")
endforeach()
