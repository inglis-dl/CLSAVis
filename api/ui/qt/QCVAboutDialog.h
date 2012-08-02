/*=========================================================================

  Program:  CLSAVis (Canadian Longitudinal Study on Aging Visualizer)
  Module:   QCVAboutDialog.h
  Language: C++

  Author: Patrick Emond <emondpd@mcmaster.ca>

=========================================================================*/

#ifndef __QCVAboutDialog_h
#define __QCVAboutDialog_h

#include <QDialog>

#include "CVUtilities.h"

class Ui_QCVAboutDialog;

class QCVAboutDialog : public QDialog
{
  Q_OBJECT

public:
  //constructor
  QCVAboutDialog( QWidget* parent = 0 );
  //destructor
  ~QCVAboutDialog();
  
public slots:

protected:

protected slots:

private:
  // Designer form
  Ui_QCVAboutDialog *ui;
};

#endif
