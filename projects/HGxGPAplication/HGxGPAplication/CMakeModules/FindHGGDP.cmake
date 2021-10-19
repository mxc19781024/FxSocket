SET(HGGDP_DIR "" CACHE PATH "Set to HGGDP install path")

set(FIND_HGGDP_ROOT_PATH
	${HGGDP_DIR}
    $ENV{HGGDP_BUILD_DIR}
    $ENV{HGGDP_DIR}
    $ENV{HGGDPDIR}
    $ENV{HGGDP_ROOT}
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt
)

FIND_PATH(HGGDP_INCLUDE_DIR gdpAlgor/GxConfig.h
	PATHS 
		${FIND_HGGDP_ROOT_PATH}
	PATH_SUFFIXES
		/include/
	)

MACRO(FIND_HGGDP_LIBRARY MYLIBRARY MYLIBRARYNAME)

FIND_LIBRARY(${MYLIBRARY}
    NAMES
        ${MYLIBRARYNAME}
    PATHS
        ${FIND_HGGDP_ROOT_PATH}
    PATH_SUFFIXES
        /lib/
        /lib64/
        /build/lib/
        /build/lib64/
        /Build/lib/
        /Build/lib64/
     )
	 
ENDMACRO(FIND_HGGDP_LIBRARY MYLIBRARY LIBRARYNAME)


FIND_HGGDP_LIBRARY( HGGDP_ALGOR_LIBRARY gdpAlgor )
FIND_HGGDP_LIBRARY( HGGDP_ALGOR_LIBRARY_DEBUG gdpAlgord)

FIND_HGGDP_LIBRARY( HGGDP_UTIL_LIBRARY gdpUtil )
FIND_HGGDP_LIBRARY( HGGDP_UTIL_LIBRARY_DEBUG gdpUtild)

FIND_HGGDP_LIBRARY( HGGDP_SCENE_LIBRARY gdpScene )
FIND_HGGDP_LIBRARY( HGGDP_SCENE_LIBRARY_DEBUG gdpScened)

FIND_HGGDP_LIBRARY( HGGDP_OBJECT_LIBRARY gdpObject )
FIND_HGGDP_LIBRARY( HGGDP_OBJECT_LIBRARY_DEBUG gdpObjectd)

FIND_HGGDP_LIBRARY( HGGDP_TOOL_LIBRARY gdpTool )
FIND_HGGDP_LIBRARY( HGGDP_TOOL_LIBRARY_DEBUG gdpToold)

IF((HGGDP_ALGOR_LIBRARY OR HGGDP_ALGOR_LIBRARY_DEBUG) AND HGGDP_INCLUDE_DIR)
    SET(HGGDP_FOUND "YES")
	include_directories("${HGGDP_INCLUDE_DIR}")
	SET(HGGDP_LIBRARIES
		debug ${HGGDP_ALGOR_LIBRARY_DEBUG} optimized ${HGGDP_ALGOR_LIBRARY}
		debug ${HGGDP_UTIL_LIBRARY_DEBUG} optimized ${HGGDP_UTIL_LIBRARY}
		debug ${HGGDP_SCENE_LIBRARY_DEBUG} optimized ${HGGDP_SCENE_LIBRARY}
		debug ${HGGDP_OBJECT_LIBRARY_DEBUG} optimized ${HGGDP_OBJECT_LIBRARY}
		debug ${HGGDP_TOOL_LIBRARY_DEBUG} optimized ${HGGDP_TOOL_LIBRARY}
		)
ENDIF((HGGDP_ALGOR_LIBRARY OR HGGDP_ALGOR_LIBRARY_DEBUG) AND HGBDP_INCLUDE_DIR)

