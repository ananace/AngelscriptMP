include(FindPackageHandleStandardArgs)

FIND_PATH(Angelscript_INCLUDE_DIR angelscript.h)

IF (WIN32 AND "${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
  SET(Angelscript_NAMES ${Angelscript_NAMES} angelscript64)
ELSE()
  SET(Angelscript_NAMES ${Angelscript_NAMES} angelscript)
ENDIF()

FIND_LIBRARY(Angelscript_LIBRARY_DEBUG
  NAMES ${Angelscript_NAMES}d
  )

FIND_LIBRARY(Angelscript_LIBRARY_RELEASE
  NAMES ${Angelscript_NAMES}
  )

IF (Angelscript_LIBRARY_DEBUG AND Angelscript_LIBRARY_RELEASE)
   SET(Angelscript_LIBRARIES debug ${Angelscript_LIBRARY_DEBUG} optimized ${Angelscript_LIBRARY_RELEASE})
ELSEIF (Angelscript_LIBRARY_DEBUG AND NOT Angelscript_LIBRARY_RELEASE)
   SET(Angelscript_LIBRARIES ${Angelscript_LIBRARY_DEBUG})
ELSEIF (NOT Angelscript_LIBRARY_DEBUG AND Angelscript_LIBRARY_RELEASE)
   SET(Angelscript_LIBRARIES ${Angelscript_LIBRARY_RELEASE})
ELSE()
   SET(Angelscript_LIBRARIES "")
ENDIF()

find_package_handle_standard_args(Angelscript DEFAULT_MSG
                                  Angelscript_LIBRARIES Angelscript_INCLUDE_DIR)

MARK_AS_ADVANCED(
  Angelscript_LIBRARY_DEBUG
  Angelscript_LIBRARY_RELEASE
)
