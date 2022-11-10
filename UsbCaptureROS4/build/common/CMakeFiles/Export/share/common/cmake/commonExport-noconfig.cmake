#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "common::common" for configuration ""
set_property(TARGET common::common APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(common::common PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libcommon.so"
  IMPORTED_SONAME_NOCONFIG "libcommon.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS common::common )
list(APPEND _IMPORT_CHECK_FILES_FOR_common::common "${_IMPORT_PREFIX}/lib/libcommon.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
