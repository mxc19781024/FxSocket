macro(HGxMacroBuildLib)
  ctkMacroParseArguments(MY
    "NAME;EXPORT_DIRECTIVE;SRCS;MOC_SRCS;GENERATE_MOC_SRCS;UI_FORMS;INCLUDE_DIRECTORIES;TARGET_LIBRARIES;RESOURCES;LIBRARY_TYPE"
    "ENABLE_QTTESTING"
    ${ARGN}
    )

  set(CMAKE_DEBUG_POSTFIX d)
  
  # Keep parameter 'INCLUDE_DIRECTORIES' for backward compatiblity

  # Sanity checks
  if(NOT DEFINED MY_NAME)
    message(FATAL_ERROR "NAME is mandatory")
  endif()
  string(REGEX MATCH "^HGx.+" valid_library_name ${MY_NAME})
  if(NOT valid_library_name)
    message(FATAL_ERROR "HGx library name [${MY_NAME}] should start with 'HGx'!")
  endif()
  if(NOT DEFINED MY_EXPORT_DIRECTIVE)
    message(FATAL_ERROR "EXPORT_DIRECTIVE is mandatory")
  endif()
  if(NOT DEFINED MY_LIBRARY_TYPE)
    set(MY_LIBRARY_TYPE "SHARED")
  endif()

  # Define library name
  set(lib_name ${MY_NAME})

  # Library target names must not contain a '_' (reserved for plug-in target names)
  if(lib_name MATCHES _)
    message(FATAL_ERROR "The library name ${lib_name} must not contain a '_' character.")
  endif()

  # --------------------------------------------------------------------------
  # Include dirs
  set(my_includes
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    # with CMake >2.9, use QT4_MAKE_OUTPUT_FILE instead ?
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/UI
    )

  # Add the include directories from the library dependencies
  ctkFunctionGetIncludeDirs(my_includes ${lib_name})

  include_directories(
    ${my_includes}
    )

  if(CTK_QT_VERSION VERSION_LESS "5")
    # Add Qt include dirs and defines
    include(${QT_USE_FILE})
  endif()

  # Add the library directories from the external project
  ctkFunctionGetLibraryDirs(my_library_dirs ${lib_name})

  link_directories(
    ${my_library_dirs}
    )

  set(MY_LIBRARY_EXPORT_DIRECTIVE ${MY_EXPORT_DIRECTIVE})
  set(MY_EXPORT_HEADER_PREFIX ${MY_NAME})
  string(REGEX REPLACE "^HGx" "Gx" MY_EXPORT_HEADER_PREFIX ${MY_EXPORT_HEADER_PREFIX})
  set(MY_LIBNAME ${lib_name})

  configure_file(
    ${CMAKE_SOURCE_DIR}/Libs/HGxExport.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/${MY_EXPORT_HEADER_PREFIX}Export.h
    )
  set(dynamicHeaders
    "${dynamicHeaders};${CMAKE_CURRENT_BINARY_DIR}/${MY_EXPORT_HEADER_PREFIX}Export.h")

  # Make sure variable are cleared
  set(MY_MOC_CPP)
  set(MY_UI_CPP)
  set(MY_QRC_SRCS)

  # Wrap
  if(MY_MOC_SRCS)
    # this is a workaround for Visual Studio. The relative include paths in the generated
    # moc files can get very long and can't be resolved by the MSVC compiler.
    if(CTK_QT_VERSION VERSION_GREATER "4")
      foreach(moc_src ${MY_MOC_SRCS})
        qt5_wrap_cpp(MY_MOC_CPP ${moc_src} OPTIONS -f${moc_src} OPTIONS -DHAVE_QT5)
      endforeach()
    else()
      foreach(moc_src ${MY_MOC_SRCS})
        QT4_WRAP_CPP(MY_MOC_CPP ${moc_src} OPTIONS -f${moc_src})
      endforeach()
    endif()
  endif()
  if(MY_GENERATE_MOC_SRCS)
    QT4_GENERATE_MOCS(${MY_GENERATE_MOC_SRCS})
  endif()
  if(CTK_QT_VERSION VERSION_GREATER "4")
    if(Qt5Widgets_FOUND)
      qt5_wrap_ui(MY_UI_CPP ${MY_UI_FORMS})
    elseif(MY_UI_FORMS)
      message(WARNING "Argument UI_FORMS ignored because Qt5Widgets module was not specified")
    endif()
  else()
    QT4_WRAP_UI(MY_UI_CPP ${MY_UI_FORMS})
  endif()
  if(DEFINED MY_RESOURCES AND NOT MY_RESOURCES STREQUAL "")
    if(CTK_QT_VERSION VERSION_GREATER "4")
      qt5_add_resources(MY_QRC_SRCS ${MY_RESOURCES})
    else()
      QT4_ADD_RESOURCES(MY_QRC_SRCS ${MY_RESOURCES})
    endif()
  endif()

  source_group("Resources" FILES
    ${MY_RESOURCES}
    ${MY_UI_FORMS}
    )

  source_group("Generated" FILES
    ${MY_QRC_SRCS}
    ${MY_MOC_CPP}
    ${MY_UI_CPP}
    ${MOC_CPP_DECORATOR}
    )

  add_library(${lib_name} ${MY_LIBRARY_TYPE}
    ${MY_SRCS}
    ${MY_MOC_CPP}
    ${MY_UI_CPP}
    ${MY_QRC_SRCS}
    )
	
	
#	message("${my_includes}")
	
    target_include_directories(${lib_name}
                           INTERFACE
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
							$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
                            $<INSTALL_INTERFACE:include/HGxGPA/${lib_name}>
                           )
	
	
	
#  target_include_directories( ${lib_name}
#    PUBLIC
#    ${my_includes}
#   )


  # Set labels associated with the target.
  set_target_properties(${lib_name} PROPERTIES LABELS ${lib_name})

  # Apply user-defined properties to the library target.
  if(CTK_LIBRARY_PROPERTIES AND MY_LIBRARY_TYPE STREQUAL "SHARED")
    set_target_properties(${lib_name} PROPERTIES ${CTK_LIBRARY_PROPERTIES})
  endif()
  set_target_properties(${lib_name} PROPERTIES CTK_LIB_TARGET_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

  # Library properties specific to STATIC build
  if(MY_LIBRARY_TYPE STREQUAL "STATIC")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64-bit
      set_target_properties(${lib_name} PROPERTIES COMPILE_FLAGS "-fPIC")
    endif()
  endif()

  # 安装库
  install(TARGETS ${lib_name}
	EXPORT HGxGPAExport
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

  install(EXPORT HGxGPAExport
	FILE HGxGPAConfig.cmake
	DESTINATION lib/HGxGPA/cmake)

  # 安装头文件
  file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.tpp")
  install(FILES
    ${headers}
    ${dynamicHeaders}
    DESTINATION include/HGxGPA/${lib_name} COMPONENT Development
    )
	
 
  set(my_libs
    ${MY_TARGET_LIBRARIES}
    )

  if(MINGW)
    list(APPEND my_libs ssp) # add stack smash protection lib
  endif()
  target_link_libraries(${lib_name} ${my_libs})

endmacro()