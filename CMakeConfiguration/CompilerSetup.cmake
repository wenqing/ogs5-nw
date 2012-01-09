INCLUDE(ResetConfigurations)        # To Debug, Release, RelWithDbgInfo
INCLUDE(SetDefaultBuildType)
SET_DEFAULT_BUILD_TYPE(Debug)
INCLUDE(MSVCMultipleProcessCompile) # /MP Switch for VS

IF (WIN32)
	## For Visual Studio compiler
	IF (MSVC)
		ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_WARNINGS
			-D_CRT_XNONSTDC_NO_WARNINGS)
		# Sets warning level 3 and ignores some warnings
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /wd4290 /wd4267")
		SET(GCC OFF)
	ELSE (MSVC)
#FOR CYGWIN.  25.02.2010. WW
		MESSAGE (STATUS "Might be GCC under cygwin.")
		SET(GCC ON)
#		MESSAGE (FATAL_ERROR "Aborting: On Windows only the Visual Studio compiler is supported!")
	ENDIF (MSVC)
ENDIF (WIN32)

### For GNU C/CXX. WW
IF(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
	SET(GCC ON)
	IF( NOT CMAKE_BUILD_TYPE STREQUAL "Debug" )
		MESSAGE(STATUS "Set GCC release flags")
		SET(CMAKE_CXX_FLAGS "-O3 -DNDEBUG")
	ENDIF()
	# -g
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -Wall -Wextra -Woverloaded-virtual -fno-nonansi-builtins")
	# would be cool: -Woverloaded-virtual, would be overkill: -Weffc++
        ADD_DEFINITIONS(-DGCC)
	IF (OGS_PROFILE)
		IF( NOT CMAKE_BUILD_TYPE STREQUAL "Release" )
			MESSAGE(STATUS "When using profiling you should set CMAKE_BUILD_TYPE to Release.")
		ENDIF()
		SET(PROFILE_FLAGS "-pg -fno-omit-frame-pointer -O2 -DNDEBUG -fno-inline-functions -fno-inline-functions-called-once -fno-optimize-sibling-calls")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PROFILE_FLAGS}")
	ENDIF (OGS_PROFILE)
ENDIF(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)