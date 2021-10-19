
SET(OSGEARTH_DIR "" CACHE PATH "Set to base osgEarth install path")
MACRO( FIND_OSGEARTH_INCLUDE THIS_OSGEARTH_INCLUDE_DIR THIS_OSGEARTH_INCLUDE_FILE )

FIND_PATH( ${THIS_OSGEARTH_INCLUDE_DIR} ${THIS_OSGEARTH_INCLUDE_FILE}
    PATHS
        ${OSGEARTH_DIR}
        $ENV{OSGEARTH_SOURCE_DIR}
        $ENV{OSGEARTHDIR}
        $ENV{OSGEARTH_DIR}
        /usr/local/
        /usr/
        /sw/ # Fink
        /opt/local/ # DarwinPorts
        /opt/csw/ # Blastwave
        /opt/
        [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;OSGEARTH_ROOT]/
        ~/Library/Frameworks
        /Library/Frameworks
    PATH_SUFFIXES
        /include/
)

ENDMACRO( FIND_OSGEARTH_INCLUDE THIS_OSGEARTH_INCLUDE_DIR THIS_OSGEARTH_INCLUDE_FILE )

FIND_OSGEARTH_INCLUDE( OSGEARTH_INCLUDE_DIR       osgEarth/Version )

###### libraries ######

MACRO( FIND_OSGEARTH_LIBRARY MYLIBRARY MYLIBRARYNAME )

FIND_LIBRARY(${MYLIBRARY}
    NAMES
        ${MYLIBRARYNAME}
    PATHS
        ${OSGEARTH_DIR}
        $ENV{OSGEARTH_BUILD_DIR}
        $ENV{OSGEARTH_DIR}
        $ENV{OSGEARTHDIR}
        $ENV{OSG_ROOT}
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local
        /usr
        /sw
        /opt/local
        /opt/csw
        /opt
        [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;OSGEARTH_ROOT]/lib
        /usr/freeware
    PATH_SUFFIXES
        /lib/
        /lib64/
        /build/lib/
        /build/lib64/
        /Build/lib/
        /Build/lib64/
     )

ENDMACRO(FIND_OSGEARTH_LIBRARY LIBRARY LIBRARYNAME)

FIND_OSGEARTH_LIBRARY( osgEarth_DEBUG_LIBRARIES           osgEarthd)
FIND_OSGEARTH_LIBRARY( osgEarthFeatures_DEBUG_LIBRARIES   osgEarthFeaturesd)
FIND_OSGEARTH_LIBRARY( osgEarthUtil_DEBUG_LIBRARIES       osgEarthUtild)
FIND_OSGEARTH_LIBRARY( osgEarthQt_DEBUG_LIBRARIES         osgEarthQtd)
FIND_OSGEARTH_LIBRARY( osgEarthSymbology_DEBUG_LIBRARIES  osgEarthSymbologyd)
FIND_OSGEARTH_LIBRARY( osgEarthAnnotation_DEBUG_LIBRARIES osgEarthAnnotationd)


FIND_OSGEARTH_LIBRARY( osgEarth_RELEASE_LIBRARIES           osgEarth)
FIND_OSGEARTH_LIBRARY( osgEarthFeatures_RELEASE_LIBRARIES   osgEarthFeatures)
FIND_OSGEARTH_LIBRARY( osgEarthUtil_RELEASE_LIBRARIES       osgEarthUtil)
FIND_OSGEARTH_LIBRARY( osgEarthQt_RELEASE_LIBRARIES         osgEarthQt)
FIND_OSGEARTH_LIBRARY( osgEarthSymbology_RELEASE_LIBRARIES   osgEarthSymbology)
FIND_OSGEARTH_LIBRARY( osgEarthAnnotation_RELEASE_LIBRARIES osgEarthAnnotation)


SET( OSGEARTH_FOUND "NO" )
IF( OSGEARTH_LIBRARY AND OSGEARTH_INCLUDE_DIR )
    SET( OSGEARTH_FOUND "YES" )
    SET( OSGEARTH_INCLUDE_DIRS ${OSGEARTH_INCLUDE_DIR})
    GET_FILENAME_COMPONENT( OSGEARTH_LIBRARIES_DIR ${OSGEARTH_LIBRARY} PATH )
ENDIF( OSGEARTH_LIBRARY AND OSGEARTH_INCLUDE_DIR )


