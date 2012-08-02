/*=========================================================================

  Program:  CLSAVis (Canadian Longitudinal Study on Aging Visualizer)
  Module:   QCVMainWindow.cxx
  Language: C++

  Author: Patrick Emond <emondpd@mcmaster.ca>

=========================================================================*/
#include "QCVMainWindow.h"
#include "ui_QCVMainWindow.h"

#include "CVApplication.h"
#include "CVSession.h"
#include "QCVAboutDialog.h"
#include "vtkCamera.h"
#include "vtkCommand.h"
#include "vtkCVView.h"
#include "vtkMath.h"
#include "vtkPNGWriter.h"
#include "vtkSmartPointer.h"
#include "vtkWindowToImageFilter.h"

#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

class QCVMainWindowProgressCommand : public vtkCommand
{
public:
  static QCVMainWindowProgressCommand *New() { return new QCVMainWindowProgressCommand; }
  void Execute( vtkObject *caller, unsigned long eventId, void *callData );
  Ui_QCVMainWindow *ui;

protected:
  QCVMainWindowProgressCommand() { this->ui = NULL; }
};


//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QCVMainWindowProgressCommand::Execute(
  vtkObject *caller, unsigned long eventId, void *callData )
{
  if( this->ui )
  {
    // display the progress
    double progress = *( static_cast<double*>( callData ) );
    int value = vtkMath::Floor( 100 * progress ) + 1;
    if( 100 < value ) value = 100;
    if( this->ui->progressBar->value() != value ) this->ui->progressBar->setValue( value );

    // show what's happening in the status bar
    if( 100 == value )
    {
      this->ui->statusbar->clearMessage();
    }
    else
    {
      // Display data update strings here
      QString message = QString( "" );
      if( message.length() ) this->ui->statusbar->showMessage( message );

      // if we want the status bar to look smooth then we can call repaint on it here
      // however, let's not do that since it substantially slows down processing
      //this->ui->statusbar->repaint();
    }
  }
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
QCVMainWindow::QCVMainWindow( QWidget* parent )
  : QMainWindow( parent )
{
  
  QMenu *menu;
  this->CurrentSessionFileName = QString( "" );
  
  this->ui = new Ui_QCVMainWindow;
  this->ui->setupUi( this );
  
  // connect the file menu items
  QObject::connect(
    this->ui->actionTakeScreenshot, SIGNAL( triggered() ),
    this, SLOT( slotTakeScreenshot() ) );
  QObject::connect(
    this->ui->actionOpenSession, SIGNAL( triggered() ),
    this, SLOT( slotOpenSession() ) );
  QObject::connect(
    this->ui->actionSaveSession, SIGNAL( triggered() ),
    this, SLOT( slotSaveSession() ) );
  QObject::connect(
    this->ui->actionSaveSessionAs, SIGNAL( triggered() ),
    this, SLOT( slotSaveSessionAs() ) );
  QObject::connect(
    this->ui->actionReloadSession, SIGNAL( triggered() ),
    this, SLOT( slotReloadSession() ) );
  QObject::connect( this->ui->actionExit, SIGNAL( triggered() ),
    qApp, SLOT( closeAllWindows() ) );
  
  // connect the view menu items
  QObject::connect(
    this->ui->actionReCenterGraph, SIGNAL( triggered() ),
    this, SLOT( slotReCenterGraph() ) );

  // connect the help menu items
  QObject::connect(
    this->ui->actionAbout, SIGNAL( triggered() ),
    this, SLOT( slotAbout() ) );

  // set up the observer to update the progress bar
  this->ProgressObserver = vtkSmartPointer< QCVMainWindowProgressCommand >::New();
  this->ProgressObserver->ui = this->ui;

  // link the view and the qt render widget
  CVApplication *cvapp = CVApplication::GetInstance();
  cvapp->GetView()->SetInteractor( this->ui->renderWidget->GetInteractor() );
  this->ui->renderWidget->SetRenderWindow( cvapp->GetView()->GetRenderWindow() );

  this->ReadSettings();
};

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
QCVMainWindow::~QCVMainWindow()
{
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QCVMainWindow::closeEvent( QCloseEvent *event )
{
  if( this->MaybeSave() )
  {
    WriteSettings();
    event->accept();
  }
  else
  {
    event->ignore();
  }
}

bool QCVMainWindow::MaybeSave()
{
  if( false /* TODO: check if session has changed */ )
  {
    QMessageBox::StandardButton messageBox;
    messageBox = QMessageBox::warning(
      this,
      tr( "Application" ),
      tr( "Your session has been modified.\n"
          "Do you want to save your changes?" ),
      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel );

    if( messageBox == QMessageBox::Save )
    {
      // TODO: save session here
      return true;
    }
    else if( messageBox == QMessageBox::Cancel )
    {
      return false;
    }
  }
  return true;
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QCVMainWindow::ReadSettings()
{
  QSettings settings( "CLSA", "CLSAVis" );
  
  settings.beginGroup( "MainWindow" );
  if( settings.contains( "size" ) ) this->resize( settings.value( "size" ).toSize() );
  if( settings.contains( "pos" ) ) this->move( settings.value( "pos" ).toPoint() );
  if( settings.contains( "maximized" ) && settings.value( "maximized" ).toBool() )
    this->showMaximized();
  settings.endGroup();
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QCVMainWindow::WriteSettings()
{
  QSettings settings( "CLSA", "CLSAVis" );
  
  settings.beginGroup( "MainWindow" );
  settings.setValue( "size", this->size() );
  settings.setValue( "pos", this->pos() );
  settings.setValue( "maximized", this->isMaximized() );
  settings.endGroup();
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QCVMainWindow::slotTakeScreenshot()
{
  QString fileName = QFileDialog::getSaveFileName(
    this, tr( "Save Snapshot" ), ".", tr( "PNG Files (*.png)" ) );
  
  if( "" != fileName )
  {
    // add .png if it isn't already there
    if( !fileName.endsWith( ".png" ) ) fileName.append( ".png" );
    
    CVApplication *cvapp = CVApplication::GetInstance();
    cvapp->GetView()->SaveScreenshot( fileName.toStdString().c_str() );
  }
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QCVMainWindow::slotReCenterGraph()
{
  CVApplication *cvapp = CVApplication::GetInstance();
  cvapp->GetView()->ResetCamera();
  cvapp->GetView()->Render();
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QCVMainWindow::slotOpenSession()
{
  QString fileName = QFileDialog::getOpenFileName(
    this, tr( "Open CLSAVis Session" ), ".", tr( "CLSAVis Session Files (*.cvx)" ) );
  
  if( "" != fileName )
  {
    try
    {
      CVApplication *cvapp = CVApplication::GetInstance();
      cvapp->GetSession()->Load( fileName.toStdString().c_str() );
      this->CurrentSessionFileName = fileName;
      cvapp->GetView()->Render();
    }
    catch( vtkstd::exception &e )
    {
      QMessageBox errorMessage( this );
      errorMessage.setWindowModality( Qt::WindowModal );
      errorMessage.setIcon( QMessageBox::Warning );
      errorMessage.setText( "There was an error while attempting to open the session." );
      errorMessage.exec();
    }
  }
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QCVMainWindow::slotSaveSession()
{
  // make sure we have a session file name
  if( "" == this->CurrentSessionFileName )
  {
    this->slotSaveSessionAs();
  }
  else
  {
    try
    {
      CVApplication *cvapp = CVApplication::GetInstance();
      cvapp->GetSession()->Save( this->CurrentSessionFileName.toStdString().c_str() );
    }
    catch( vtkstd::exception &e )
    {
      QMessageBox errorMessage( this );
      errorMessage.setWindowModality( Qt::WindowModal );
      errorMessage.setIcon( QMessageBox::Warning );
      errorMessage.setText( "There was an error while attempting to save the session." );
      errorMessage.exec();
    }
  }
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QCVMainWindow::slotSaveSessionAs()
{
  QString fileName = QFileDialog::getSaveFileName(
    this, tr( "Save CLSAVis Session" ), ".", tr( "CLSAVis Session Files (*.cvx)" ) );
  
  if( "" != fileName )
  {
    // add .cvx if it isn't already there
    if( !fileName.endsWith( ".cvx" ) ) fileName.append( ".cvx" );
    this->CurrentSessionFileName = fileName.toStdString().c_str();
    this->slotSaveSession();
  }
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QCVMainWindow::slotReloadSession()
{
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QCVMainWindow::slotAbout()
{
  QCVAboutDialog dialog( this );
  dialog.setModal( true );
  dialog.setWindowTitle( tr( "About CLSAVis" ) );
  dialog.exec();
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void QCVMainWindow::Render( bool resetCamera )
{
  // we're about to do an operation that might take a while, so update the GUI and cursor
  this->repaint();
  this->setCursor( Qt::WaitCursor );
  this->ui->renderWidget->setCursor( Qt::WaitCursor );

  clock_t start = clock();
  if( resetCamera )
  {
    // TODO ResetCamera
  }

  // TODO Render
  clock_t end = clock();

  // update the cursor and report how many vertices and edges are currently visible
  this->setCursor( Qt::ArrowCursor );
  this->ui->renderWidget->setCursor( Qt::CrossCursor );
  char buffer[128];
  sprintf( buffer, "Processing time: %0.2fs",
    static_cast< double >( end - start ) / CLOCKS_PER_SEC );
  this->ui->statusbar->showMessage( buffer );
}
