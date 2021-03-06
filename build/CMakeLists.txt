CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
PROJECT( CLSAVis )

# Set the version
SET( CLSAVIS_VERSION_MAJOR 0 )
SET( CLSAVIS_VERSION_MINOR 1 )
SET( CLSAVIS_VERSION_PATCH 0 )

# Set archive name
SET( CLSAVIS_ARCHIVE_NAME CLSAVis-${CLSAVIS_VERSION_MAJOR}.${CLSAVIS_VERSION_MINOR}.${CLSAVIS_VERSION_PATCH} )

# Icons sizes to build
SET( CLSAVIS_ICON_SIZES 16x16 22x22 24x24 32x32 48x48 64x64 128x128 )

# We need VTK
FIND_PACKAGE( VTK REQUIRED )
INCLUDE( ${VTK_USE_FILE} )

# We need QT4
SET( QT_MIN_VERSION 4.0.0 )
FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE} )

# We need libxml2
FIND_PACKAGE( LibXml2 REQUIRED )

# We need convert
IF( UNIX AND NOT APPLE )
  FIND_PACKAGE( ImageMagick COMPONENTS convert REQUIRED )
ENDIF( UNIX AND NOT APPLE )

SET( CLSAVIS_ROOT_DIR ${PROJECT_SOURCE_DIR}/.. )
SET( CLSAVIS_AUX_DIR ${CLSAVIS_ROOT_DIR}/aux )
SET( CLSAVIS_API_DIR ${CLSAVIS_ROOT_DIR}/api )
SET( CLSAVIS_APP_DIR ${CLSAVIS_API_DIR}/app )
SET( CLSAVIS_QT_DIR ${CLSAVIS_API_DIR}/ui/qt )
SET( CLSAVIS_VTK_DIR ${CLSAVIS_API_DIR}/vtk )

# Configure the utitlities header
CONFIGURE_FILE( ${CLSAVIS_APP_DIR}/CVUtilities.h.in
                ${PROJECT_BINARY_DIR}/CVUtilities.h @ONLY IMMEDIATE )

# Copy the readme file to build
CONFIGURE_FILE( ${CLSAVIS_ROOT_DIR}/readme
                ${PROJECT_BINARY_DIR}/readme COPYONLY )

# Set up include directories
SET( CLSAVIS_INCLUDE_DIR
  ${CLSAVIS_APP_DIR}
  ${CLSAVIS_QT_DIR}
  ${CLSAVIS_VTK_DIR}
)

# Set up headers and source
SET( CLSAVIS_SOURCE
  ${CLSAVIS_API_DIR}/CLSAVis.cxx
  ${CLSAVIS_APP_DIR}/CVApplication.cxx
  ${CLSAVIS_APP_DIR}/CVSession.cxx
  ${CLSAVIS_VTK_DIR}/vtkXMLFileReader.cxx
  ${CLSAVIS_VTK_DIR}/vtkXMLFileWriter.cxx
  ${CLSAVIS_VTK_DIR}/vtkCVSessionReader.cxx
  ${CLSAVIS_VTK_DIR}/vtkCVSessionWriter.cxx
  ${CLSAVIS_VTK_DIR}/vtkCVView.cxx
  ${CLSAVIS_QT_DIR}/QCVAboutDialog.cxx
  ${CLSAVIS_QT_DIR}/QCVMainWindow.cxx
)

SET( CLSAVIS_UIS
  ${CLSAVIS_QT_DIR}/QCVAboutDialog.ui
  ${CLSAVIS_QT_DIR}/QCVMainWindow.ui
)

SET( CLSAVIS_HEADERS
  ${CLSAVIS_QT_DIR}/QCVAboutDialog.h
  ${CLSAVIS_QT_DIR}/QCVMainWindow.h
)

# Wrap QT files
QT4_WRAP_UI( UIHeaders ${CLSAVIS_UIS} )
QT4_WRAP_CPP( MOCSrcs ${CLSAVIS_HEADERS} )
ADD_DEFINITIONS( -DQT_GUI_LIBS -DQT_CORE_LIB -DQT3_SUPPORT )
SET_SOURCE_FILES_PROPERTIES( ${CLSAVIS_SOURCE} PROPERTIES OBJECT_DEPENDS "${UIHeaders}" )

# Use the include path and library for Qt
INCLUDE_DIRECTORIES(
  ${CLSAVIS_INCLUDE_DIR}
  ${QT_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# Targets
ADD_EXECUTABLE( CLSAVis ${CLSAVIS_SOURCE} ${CLSAVIS_UISrcs} ${MOCSrcs} )

TARGET_LINK_LIBRARIES( CLSAVis
  QVTK
  ${QT_LIBRARIES}
  ${LIBXML2_LIBRARIES}
  vtkRendering
  vtkGraphics
  vtkIO
  vtkCommon
)
INSTALL( TARGETS CLSAVis RUNTIME DESTINATION bin )

ADD_CUSTOM_TARGET( dist
  COMMAND git archive --prefix=${CLSAVIS_ARCHIVE_NAME}/ HEAD
    | bzip2 > ${CMAKE_BINARY_DIR}/${CLSAVIS_ARCHIVE_NAME}.tar.bz2
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Desktop entries and icons
IF( UNIX AND NOT APPLE )
  INSTALL( FILES desktop/CLSAVis.desktop DESTINATION share/applications )

  INSTALL( FILES desktop/logo_clsa.png DESTINATION share/icons/hicolor/scalable/apps )
  FOREACH( SIZE ${CLSAVIS_ICON_SIZES} )
    ADD_CUSTOM_COMMAND( OUTPUT logo_clsa_${SIZE}.png
      COMMAND convert -background none -resize ${SIZE} ${CLSAVIS_AUX_DIR}/logo_clsa.png logo_clsa_${SIZE}.png
      COMMENT "Generating ${SIZE} icon"
    )
    SET( CLSAVIS_ICON_FILES ${CLSAVIS_ICON_FILES} logo_clsa_${SIZE}.png )
    INSTALL( FILES ${CMAKE_BINARY_DIR}/logo_clsa_${SIZE}.png
      DESTINATION share/icons/hicolor/${SIZE}/apps
      RENAME logo_clsa.png
    )
  ENDFOREACH( SIZE )
  ADD_CUSTOM_TARGET( icons ALL DEPENDS ${CLSAVIS_ICON_FILES} )
ENDIF( UNIX AND NOT APPLE )
