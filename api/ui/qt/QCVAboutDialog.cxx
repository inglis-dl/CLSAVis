/*=========================================================================

  Program:  CLSAVis (Canadian Longitudinal Study on Aging Visualizer)
  Module:   QCVAboutDialog.cxx
  Language: C++

  Author: Patrick Emond <emondpd@mcmaster.ca>

=========================================================================*/
#include "QCVAboutDialog.h"
#include "ui_QCVAboutDialog.h"

#include <QFile>
#include <QTextStream>

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
QCVAboutDialog::QCVAboutDialog( QWidget* parent )
  : QDialog( parent )
{
  this->ui = new Ui_QCVAboutDialog;
  this->ui->setupUi( this );
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
QCVAboutDialog::~QCVAboutDialog()
{
}
