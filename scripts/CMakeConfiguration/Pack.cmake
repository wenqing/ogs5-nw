#### Packaging setup for OGS-5 ####

#INCLUDE(CMakeConfiguration/InstallRequiredSystemLibraries.cmake)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "OGS-5")
SET(CPACK_PACKAGE_VENDOR "OpenGeoSys Development Team")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "OGS-${OGS_VERSION_MAJOR}.${OGS_VERSION_MINOR}.${OGS_VERSION_PATCH}")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.txt")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR "${OGS_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${OGS_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${OGS_VERSION_PATCH}")
SET(CPACK_PACKAGE_FILE_NAME "ogs-${OGS_VERSION_MAJOR}.${OGS_VERSION_MINOR}.${OGS_VERSION_PATCH}-${CMAKE_SYSTEM}-x${BITS}")

IF (OGS_PACKAGING_ZIP)
  SET (CPACK_GENERATOR ZIP)
  SET(CPACK_PACKAGE_FILE_NAME "ogs-5")
  MESSAGE (STATUS "Packaging set to ZIP")
ELSE (OGS_PACKAGING_ZIP)
  IF(WIN32)
    SET(CPACK_GENERATOR NSIS)
    # There is a bug in NSI that does not handle full unix paths properly. Make
    # sure there is at least one set of four (4) backlasshes.
    SET(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/CMakeConfiguration\\\\OGS_Logo_Installer.bmp")
    #SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\MyExecutable.exe")
    SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}")
    SET(CPACK_NSIS_HELP_LINK "https:\\\\\\\\geosys.ufz.de")
    SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\geosys.ufz.de")
    SET(CPACK_NSIS_CONTACT "lars.bilke@ufz.de")
    SET(CPACK_NSIS_MODIFY_PATH ON)
    #SET(CPACK_NSIS_MENU_LINKS "https://geosys.ufz.de/trac" "OGS Wiki")
    MESSAGE (STATUS "Packaging set to NSIS")
  ELSE(WIN32)
    IF(UNIX)
      SET(CPACK_GENERATOR TGZ)
      MESSAGE (STATUS "Packaging set to TGZ")
    ELSE(UNIX)
      IF(APPLE)
		SET(CPACK_GENERATOR TGZ)
	  	MESSAGE (STATUS "Packaging set to TGZ")
      ENDIF(APPLE)
    ENDIF(UNIX)
    #SET(CPACK_STRIP_FILES "bin/ogs")
    #SET(CPACK_SOURCE_STRIP_FILES "")
  ENDIF(WIN32)
ENDIF (OGS_PACKAGING_ZIP)

# INCLUDE (FindMSVCRedist)
# IF (MSVC_REDIST)
# 	SET(CPACK_COMPONENTS_ALL msvc_redist)
# ENDIF () # MSVC_REDIST

SET(CPACK_COMPONENT_OGS_DISPLAY_NAME "Executable")
SET(CPACK_COMPONENT_OGS_DESCRIPTION "The command line executable (former rf4)")
SET(CPACK_COMPONENT_MSVC_REDIST_DISPLAY_NAME "C++ Redistributable")
SET(CPACK_COMPONENT_MSVC_REDIST_DESCRIPTION "Microsoft Visual C++ Redistributable")

SET(CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE 1)
IF (OGS_USE_QT)
	SET(CPACK_PACKAGE_EXECUTABLES "ogs" "OGS Command Line" "ogs-gui" "OGS User Interface")
	SET(CPACK_COMPONENTS_ALL ${CPACK_COMPONENTS_ALL} ogs ogs_gui Unspecified)
	SET(CPACK_COMPONENT_OGS_GUI_DISPLAY_NAME "Graphical User Interface")
	SET(CPACK_COMPONENT_OGS_GUI_DESCRIPTION "The graphical user interface")
	SET(CPACK_COMPONENT_OGS_GUI_DEPENDS ogs)
ELSE ()
	SET(CPACK_PACKAGE_EXECUTABLES "ogs" "OGS Command Line")
	SET(CPACK_COMPONENTS_ALL ${CPACK_COMPONENTS_ALL} ogs Unspecified)
ENDIF ()

INCLUDE (CPack)
