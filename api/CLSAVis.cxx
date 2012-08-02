/*=========================================================================

  Program:  CLSAVis (Canadian Longitudinal Study on Aging Visualizer)
  Module:   CLSAVis.cxx
  Language: C++

  Author: Patrick Emond <emondpd@mcmaster.ca>

=========================================================================*/
//
// .SECTION Description
// The main function which launches the application.
//

#include "CVApplication.h"
#include "CVUtilities.h"
#include "QCVMainWindow.h"
#include "vtkSmartPointer.h"
#include <QApplication>

// main function
int main( int argc, char** argv )
{
  // create the qt application object and set some defaults
  QApplication app( argc, argv );
  
  // set main widget for the application to the main window
  QCVMainWindow mainWindow;
  app.setMainWidget( &mainWindow );  
  mainWindow.show();

  // execute the application, then delete the application
  int status = app.exec();
  CVApplication::DeleteInstance();

  // return the result of the executed application
  return status;
}
