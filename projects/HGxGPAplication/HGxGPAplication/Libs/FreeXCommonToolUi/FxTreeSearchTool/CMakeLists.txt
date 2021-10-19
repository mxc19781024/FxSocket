include_directories(${CMAKE_SOURCE_DIR}/FreeXUI/FxCommonTools/public)

file(GLOB FXTOOL_HEADER_FILES *.h)
aux_source_directory(./ FXTOOL_SOURCE_FILES)
set(FXTOOL_INCLUDE_FILES
	${FXTOOL_HEADER_FILES}
	${CMAKE_SOURCE_DIR}/FreeXUI/FxCommonTools/public/FxTreeSearchTool/CTreeSearchTool.h
)

#设置自动moc、uic、rcc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

SOURCE_GROUP(HeaderFiles FILES ${FXTOOL_INCLUDE_FILES})
SOURCE_GROUP(SrcFiles FILES ${FXTOOL_SOURCE_FILES})

ADD_LIBRARY(
	${OUT_NAME} SHARED
	${FXTOOL_INCLUDE_FILES}
	${FXTOOL_SOURCE_FILES}
)

if (Qt5Widgets_FOUND)
	target_link_libraries(
		${OUT_NAME}
        Qt5::OpenGL
        Qt5::Widgets
		Qt5::Network
		Qt5::Test
		Qt5::Xml
	) 
else()
    target_link_libraries(
        ${OUT_NAME}
        ${QT_LIBRARIES}
    )
endif()