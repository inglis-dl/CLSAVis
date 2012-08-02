/*=========================================================================

  Program:  CLSAVis (Canadian Longitudinal Study on Aging Visualizer)
  Module:   QCVMainWindow.h
  Language: C++

  Author: Patrick Emond <emondpd@mcmaster.ca>

=========================================================================*/

#ifndef __QCVMainWindow_h
#define __QCVMainWindow_h

#include <QMainWindow>

#include "CVUtilities.h"
#include "vtkSmartPointer.h"

class Ui_QCVMainWindow;

class QCVMainWindowProgressCommand;

class QCVMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  QCVMainWindow( QWidget* parent = 0 );
  ~QCVMainWindow();
  
public slots:
  // file event functions
  virtual void slotTakeScreenshot();
  virtual void slotOpenSession();
  virtual void slotSaveSession();
  virtual void slotSaveSessionAs();
  virtual void slotReloadSession();

  // view event functions
  virtual void slotReCenterGraph();

  // help event functions
  virtual void slotAbout();

protected:
  // called whenever the main window is closed
  virtual void closeEvent( QCloseEvent *event );

  // read/write application GUI settings
  virtual void ReadSettings();
  virtual void WriteSettings();

  // asks the user if they would like to save (if any changes have been made)
  virtual bool MaybeSave();

  virtual void Render( bool resetCamera );

  vtkSmartPointer< QCVMainWindowProgressCommand > ProgressObserver;
  QString CurrentSessionFileName;
protected slots:

private:
  // Designer form
  Ui_QCVMainWindow *ui;
};

#endif
