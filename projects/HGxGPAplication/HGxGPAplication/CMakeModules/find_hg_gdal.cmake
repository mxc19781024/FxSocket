
SET(GDAL_DIR "" CACHE PATH "Set to base GDAL install path")
MACRO( FIND_GDAL_INCLUDE THIS_GDAL_INCLUDE_DIR THIS_GDAL_INCLUDE_FILE )

FIND_PATH( ${THIS_GDAL_INCLUDE_DIR} ${THIS_GDAL_INCLUDE_FILE}
    PATHS
        ${GDAL_DIR}
        $ENV{GDAL_SOURCE_DIR}
        $ENV{GDALDIR}
        $ENV{GDAL_DIR}
        /usr/local/
        /usr/
        /sw/ # Fink
        /opt/local/ # DarwinPorts
        /opt/csw/ # Blastwave
        /opt/
        [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;GDAL_ROOT]/
        ~/Library/Frameworks
        /Library/Frameworks
    PATH_SUFFIXES
        /include/
)

ENDMACRO( FIND_GDAL_INCLUDE THIS_GDAL_INCLUDE_DIR THIS_GDAL_INCLUDE_FILE )

FIND_GDAL_INCLUDE( GDAL_INCLUDE_DIR       gdal.h)

###### libraries ######

MACRO( FIND_GDAL_LIBRARY MYLIBRARY MYLIBRARYNAME )

FIND_LIBRARY(${MYLIBRARY}
    NAMES
        ${MYLIBRARYNAME}
    PATHS
        ${GDAL_DIR}
        $ENV{GDAL_BUILD_DIR}
        $ENV{GDAL_DIR}
        $ENV{GDALDIR}
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local
        /usr
        /sw
        /opt/local
        /opt/csw
        /opt
        [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;GDAL_ROOT]/lib
        /usr/freeware
    PATH_SUFFIXES
        /lib/
        /lib64/
        /build/lib/
        /build/lib64/
        /Build/lib/
        /Build/lib64/
     )

ENDMACRO(FIND_GDAL_LIBRARY LIBRARY LIBRARYNAME)

FIND_GDAL_LIBRARY( GDAL_DEBUG_LIBRARIES gdal_i)



FIND_GDAL_LIBRARY( GDAL_RELEASE_LIBRARIES gdal_i)



SET( GDAL_FOUND "NO" )
IF( GDAL_LIBRARY AND GDAL_INCLUDE_DIR )
    SET( GDAL_FOUND "YES" )
    SET( GDAL_INCLUDE_DIRS ${GDAL_INCLUDE_DIR})
    GET_FILENAME_COMPONENT( GDAL_LIBRARIES_DIR ${GDAL_LIBRARY} PATH )
ENDIF( GDAL_LIBRARY AND GDAL_INCLUDE_DIR )


