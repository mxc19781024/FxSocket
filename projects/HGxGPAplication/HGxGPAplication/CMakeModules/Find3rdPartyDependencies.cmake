################################################################################################
# this Macro find a generic dependency, handling debug suffix
# all the paramenter are required, in case of lists, use "" in calling
################################################################################################

################################################################################################
#FIND_DEPENDENCY��Ĳ���˵��
#DEPNAME 				������������TIFF��CURL��
#INCLUDEFILE 			INCLUDE�а�����ͷ�ļ�����tiff.h��curl.h��
#LIBRARY_NAMES_BASE		��̬��������"libcurl;curllib;libcurl_imp"��
#SEARCHPATHLIST			����·��
#DEBUGSUFFIX			DEBUG��ĺ�׺
#EXSUFFIX				������׺
################################################################################################
MACRO(FIND_DEPENDENCY DEPNAME INCLUDEFILE LIBRARY_NAMES_BASE SEARCHPATHLIST DEBUGSUFFIX EXSUFFIX)

    MESSAGE(STATUS "searching ${DEPNAME} -->${INCLUDEFILE}<-->${LIBRARY_NAMES_BASE}<-->${SEARCHPATHLIST}<--")

    SET(MY_PATH_INCLUDE )
    SET(MY_PATH_LIB )
    
	#forѭ����������SEARCHPATHLIST�в���INCLUDE·����LIB·�����ֱ�ֵ��MY_PATH_INCLUDE��MY_PATH_LIB
    FOREACH( MYPATH ${SEARCHPATHLIST} )
        SET(MY_PATH_INCLUDE ${MY_PATH_INCLUDE} ${MYPATH}/include ${MYPATH}/include/${DEPNAME})
        SET(MY_PATH_LIB ${MY_PATH_LIB} ${MYPATH}/lib ${MYPATH}/lib/${DEPNAME})
    ENDFOREACH( MYPATH ${SEARCHPATHLIST} )
    
	
	#��MY_PATH_INCLUDE��NO_DEFAULT_PATH·���в���INCLUDEFILE�ļ�������·����ֵ��${DEPNAME}_INCLUDE_DIR
    FIND_PATH("${DEPNAME}_INCLUDE_DIR" ${INCLUDEFILE}
      ${MY_PATH_INCLUDE}
      NO_DEFAULT_PATH
    )
    MARK_AS_ADVANCED("${DEPNAME}_INCLUDE_DIR")
    #MESSAGE( " ${DEPNAME}_INCLUDE_DIR --> ${${DEPNAME}_INCLUDE_DIR}<--")
    
	#����Release��
    SET(LIBRARY_NAMES "")
    FOREACH(LIBNAME ${LIBRARY_NAMES_BASE})
        LIST(APPEND LIBRARY_NAMES "${LIBNAME}")
        LIST(APPEND LIBRARY_NAMES "${LIBNAME}${EXSUFFIX}")
    ENDFOREACH(LIBNAME)
    FIND_LIBRARY("${DEPNAME}_LIBRARY"
        NAMES ${LIBRARY_NAMES}
      PATHS ${MY_PATH_LIB}
      NO_DEFAULT_PATH
    )
	
	#����Debug��
    SET(LIBRARY_NAMES_DEBUG "")
    FOREACH(LIBNAME ${LIBRARY_NAMES_BASE})
        LIST(APPEND LIBRARY_NAMES_DEBUG "${LIBNAME}${DEBUGSUFFIX}")
        LIST(APPEND LIBRARY_NAMES_DEBUG "${LIBNAME}${EXSUFFIX}${DEBUGSUFFIX}")
        LIST(APPEND LIBRARY_NAMES_DEBUG "${LIBNAME}${DEBUGSUFFIX}${EXSUFFIX}")
    ENDFOREACH(LIBNAME)
    FIND_LIBRARY("${DEPNAME}_LIBRARY_DEBUG" 
        NAMES ${LIBRARY_NAMES_DEBUG}
      PATHS ${MY_PATH_LIB}
      NO_DEFAULT_PATH
    )
    MARK_AS_ADVANCED("${DEPNAME}_LIBRARY")
    #MESSAGE( " ${DEPNAME}_LIBRARY --> ${${DEPNAME}_LIBRARY}<--")
	
	#���ñ�ǿ����ҵ�����CURL_FOUND��,���DEBUG��δ�ҵ�����ʹ��Release��
    SET( ${DEPNAME}_FOUND "NO" )
    IF(${DEPNAME}_INCLUDE_DIR AND ${DEPNAME}_LIBRARY)
      SET( ${DEPNAME}_FOUND "YES" )
      IF(NOT ${DEPNAME}_LIBRARY_DEBUG)
          MESSAGE("-- Warning Debug ${DEPNAME} not found, using: ${${DEPNAME}_LIBRARY}")
          SET(${DEPNAME}_LIBRARY_DEBUG "${${DEPNAME}_LIBRARY}")
      ENDIF(NOT ${DEPNAME}_LIBRARY_DEBUG)
    ENDIF(${DEPNAME}_INCLUDE_DIR AND ${DEPNAME}_LIBRARY)
ENDMACRO(FIND_DEPENDENCY DEPNAME INCLUDEFILE LIBRARY_NAMES_BASE SEARCHPATHLIST DEBUGSUFFIX)


################################################################################################
# this Macro is tailored to Mike and Torbens dependencies
################################################################################################

MACRO(SEARCH_3RDPARTY ACTUAL_3RD_PARTY_DIR)
	#����CURL��
    FIND_DEPENDENCY(CURL curl/curl.h "libcurl;curllib" ${ACTUAL_3RD_PARTY_DIR} "D" "_i")
	#����JPEG��
    FIND_DEPENDENCY(JPEG jpeglib.h "libjpeg;jpeg" ${ACTUAL_3RD_PARTY_DIR} "D" "")
	#����LOG4CPP��
    FIND_DEPENDENCY(LOG4CPP Configurator.hh "log4cpp" ${ACTUAL_3RD_PARTY_DIR} "D" "")
	#����ZLIB��
    FIND_DEPENDENCY(ZLIB zlib.h "z;zlib;zlib1" ${ACTUAL_3RD_PARTY_DIR} "D" "")
    #����GDAL��
    FIND_DEPENDENCY(GDAL gdal/gdal.h "gdal_i" ${ACTUAL_3RD_PARTY_DIR} "D" "")
    IF(ZLIB_FOUND)
        FIND_DEPENDENCY(PNG png.h "libpng;libpng13;libpng14;libpng15;libpng16" ${ACTUAL_3RD_PARTY_DIR} "D" "")
        IF(PNG_FOUND)
            #forcing subsequent FindPNG stuff to not search for other variables.... kind of a hack 
            #SET(PNG_PNG_INCLUDE_DIR ${PNG_INCLUDE_DIR} CACHE FILEPATH "")
            MARK_AS_ADVANCED(PNG_PNG_INCLUDE_DIR)
        ENDIF(PNG_FOUND)
    ENDIF(ZLIB_FOUND)
ENDMACRO(SEARCH_3RDPARTY 3RD_PARTY_DIR)




################################################################################################
# this is code for handling optional 3RDPARTY usage
################################################################################################

OPTION(USE_3RDPARTY_BIN "Set to ON to use Mike prebuilt dependencies situated side of OpenSceneGraph source.  Use OFF for avoiding." ON)
IF(USE_3RDPARTY_BIN)

    # Check Architecture
    IF( CMAKE_SIZEOF_VOID_P EQUAL 4 )
        MESSAGE( STATUS "32 bit architecture detected" )
        SET(DESTINATION_ARCH "x86")
    ENDIF()
    IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
        MESSAGE( STATUS "64 bit architecture detected" )
        SET(DESTINATION_ARCH "x64")
    ENDIF()

    GET_FILENAME_COMPONENT(PARENT_DIR ${PROJECT_SOURCE_DIR} PATH)
    SET(TEST_3RDPARTY_DIR "${PARENT_DIR}/3rdparty")
    IF(NOT EXISTS ${TEST_3RDPARTY_DIR})
        SET(3RDPARTY_DIR_BY_ENV $ENV{OSG_3RDPARTY_DIR})
        IF(3RDPARTY_DIR_BY_ENV)
            MESSAGE( STATUS "3rdParty-Package ENV variable found:${3RDPARTY_DIR_BY_ENV}/${DESTINATION_ARCH}" )
            SET(TEST_3RDPARTY_DIR "${3RDPARTY_DIR_BY_ENV}/${DESTINATION_ARCH}")
        ELSEIF(MSVC71)
            SET(TEST_3RDPARTY_DIR "${PARENT_DIR}/3rdParty_win32binaries_vs71")
        ELSEIF(MSVC80)
            SET(TEST_3RDPARTY_DIR "${PARENT_DIR}/3rdParty_win32binaries_vs80sp1")
        ELSEIF(MSVC90)
            SET(TEST_3RDPARTY_DIR "${PARENT_DIR}/3rdParty_win32binaries_vs90sp1")
        ENDIF()
    ENDIF(NOT EXISTS ${TEST_3RDPARTY_DIR})
    
    IF(EXISTS ${3RD_PARTY_DIR})
        SET (3rdPartyRoot ${3RD_PARTY_DIR})
        SEARCH_3RDPARTY(${3RD_PARTY_DIR})
    ENDIF(EXISTS ${3RD_PARTY_DIR})
ENDIF(USE_3RDPARTY_BIN)
