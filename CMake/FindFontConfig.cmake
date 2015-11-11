# Find the FontConfig library
#
# Defines the following variables:
# FONTCONFIG_FOUND
# FONTCONFIG_INCLUDE_DIR
# FONTCONFIG_LIBRARIES

if ( FONTCONFIG_LIBRARIES AND FONTCONFIG_INCLUDE_DIR )
	set( FONTCONFIG_FOUND true )
else()
	if ( NOT WIN32 )
		find_package( PkgConfig ) # Use pkg-config to help discover it
		pkg_check_modules( PC_FONTCONFIG fontconfig )
	endif()

	find_path( FONTCONFIG_INCLUDE_DIR fontconfig/fontconfig.h
		PATHS
		${PC_FONTCONFIG_INCLUDEDIR}
        ${PC_FONTCONFIG_INCLUDE_DIRS}
        /usr/X11/include
        /usr/X11R6/include
        /usr/include
	)
	find_library( FONTCONFIG_LIBRARIES NAMES fontconfig
		PATHS
        ${PC_FONTCONFIG_LIBDIR}
        ${PC_FONTCONFIG_LIBRARY_DIRS}
	)

	include( FindPackageHandleStandardArgs )
	FIND_PACKAGE_HANDLE_STANDARD_ARGS( FontConfig DEFAULT_MSG FONTCONFIG_LIBRARIES FONTCONFIG_INCLUDE_DIR )

	mark_as_advanced( FONTCONFIG_LIBRARIES FONTCONFIG_INCLUDE_DIR )
endif()
