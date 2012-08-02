/*=========================================================================

  Program:  CLSAVis (Canadian Longitudinal Study on Aging Visualizer)
  Module:   CVApplication.cxx
  Language: C++

  Author: Patrick Emond <emondpd@mcmaster.ca>

=========================================================================*/
#include "CVApplication.h"

#include "CVUtilities.h"
#include "CVSession.h"
#include "vtkCamera.h"
#include "vtkCVView.h"
#include "vtkObjectFactory.h"
#include "vtkRenderer.h"

#include <vtkstd/algorithm>

vtkCxxRevisionMacro( CVApplication, "$Revision: $" );

CVApplication* CVApplication::Instance = NULL; // set the initial application

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
CVApplication::CVApplication()
{
  this->Session = CVSession::New();
  this->View = vtkCVView::New();

  // link the session and view's camera
  this->Session->SetCamera( this->View->GetRenderer()->GetActiveCamera() );
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
CVApplication::~CVApplication()
{
  if( NULL != this->Session )
  {
    this->Session->Delete();
    this->Session = NULL;
  }

  if( NULL != this->View )
  {
    this->View->Delete();
    this->View = NULL;
  }
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
// this must be implemented instead of the standard new macro since the New()
// method is protected (ie: we do not want an instantiator new function)
CVApplication* CVApplication::New()
{
  vtkObject* ret = vtkObjectFactory::CreateInstance( "CVApplication" );
  return ret ? static_cast< CVApplication* >( ret ) : new CVApplication;
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
CVApplication* CVApplication::GetInstance()
{
  if( NULL == CVApplication::Instance ) CVApplication::Instance = CVApplication::New();
  return CVApplication::Instance;
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void CVApplication::DeleteInstance()
{
  if( NULL != CVApplication::Instance )
  {
    CVApplication::Instance->Delete();
    CVApplication::Instance = NULL;
  }
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void CVApplication::PrintSelf( ostream &os, vtkIndent indent )
{
  Superclass::PrintSelf( os, indent );
}
