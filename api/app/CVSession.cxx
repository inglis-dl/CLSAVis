/*=========================================================================

  Program:  CLSAVis (Canadian Longitudinal Study on Aging Visualizer)
  Module:   CVSession.cxx
  Language: C++

  Author: Patrick Emond <emondpd@mcmaster.ca>

=========================================================================*/
#include "CVSession.h"

#include "CVUtilities.h"
#include "vtkCamera.h"
#include "vtkCVSessionReader.h"
#include "vtkCVSessionWriter.h"
#include "vtkObjectFactory.h"
#include "vtkSmartPointer.h"

#include <vtkstd/algorithm>

vtkCxxRevisionMacro( CVSession, "$Revision: $" );
vtkStandardNewMacro( CVSession );
vtkCxxSetObjectMacro( CVSession, Camera, vtkCamera );

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
CVSession::CVSession()
{
  this->Camera = vtkCamera::New();
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
CVSession::~CVSession()
{
  this->SetCamera( NULL );
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
bool CVSession::operator == ( const CVSession &rhs ) const
{
  if( this->Camera->GetClippingRange()[0] == rhs.Camera->GetClippingRange()[0] &&
      this->Camera->GetClippingRange()[1] == rhs.Camera->GetClippingRange()[1] &&
      this->Camera->GetDistance() == rhs.Camera->GetDistance() &&
      this->Camera->GetFocalDisk() == rhs.Camera->GetFocalDisk() &&
      this->Camera->GetFocalPoint()[0] == rhs.Camera->GetFocalPoint()[0] &&
      this->Camera->GetFocalPoint()[1] == rhs.Camera->GetFocalPoint()[1] &&
      this->Camera->GetFocalPoint()[2] == rhs.Camera->GetFocalPoint()[2] &&
      this->Camera->GetViewShear()[0] == rhs.Camera->GetViewShear()[0] &&
      this->Camera->GetViewShear()[1] == rhs.Camera->GetViewShear()[1] &&
      this->Camera->GetViewShear()[2] == rhs.Camera->GetViewShear()[2] &&
      this->Camera->GetParallelProjection() == rhs.Camera->GetParallelProjection() &&
      this->Camera->GetParallelScale() == rhs.Camera->GetParallelScale() &&
      this->Camera->GetPosition()[0] == rhs.Camera->GetPosition()[0] &&
      this->Camera->GetPosition()[1] == rhs.Camera->GetPosition()[1] &&
      this->Camera->GetPosition()[2] == rhs.Camera->GetPosition()[2] &&
      this->Camera->GetLeftEye() == rhs.Camera->GetLeftEye() &&
      this->Camera->GetThickness() == rhs.Camera->GetThickness() &&
      this->Camera->GetViewAngle() == rhs.Camera->GetViewAngle() &&
      this->Camera->GetUseHorizontalViewAngle() == rhs.Camera->GetUseHorizontalViewAngle() &&
      this->Camera->GetViewUp()[0] == rhs.Camera->GetViewUp()[0] &&
      this->Camera->GetViewUp()[1] == rhs.Camera->GetViewUp()[1] &&
      this->Camera->GetViewUp()[2] == rhs.Camera->GetViewUp()[2] &&
      this->Camera->GetWindowCenter()[0] == rhs.Camera->GetWindowCenter()[0] &&
      this->Camera->GetWindowCenter()[1] == rhs.Camera->GetWindowCenter()[1] &&
      this->Camera->GetScreenBottomLeft()[0] == rhs.Camera->GetScreenBottomLeft()[0] &&
      this->Camera->GetScreenBottomLeft()[1] == rhs.Camera->GetScreenBottomLeft()[1] &&
      this->Camera->GetScreenBottomLeft()[2] == rhs.Camera->GetScreenBottomLeft()[2] &&
      this->Camera->GetScreenBottomRight()[0] == rhs.Camera->GetScreenBottomRight()[0] &&
      this->Camera->GetScreenBottomRight()[1] == rhs.Camera->GetScreenBottomRight()[1] &&
      this->Camera->GetScreenBottomRight()[2] == rhs.Camera->GetScreenBottomRight()[2] &&
      this->Camera->GetScreenTopRight()[0] == rhs.Camera->GetScreenTopRight()[0] &&
      this->Camera->GetScreenTopRight()[1] == rhs.Camera->GetScreenTopRight()[1] &&
      this->Camera->GetScreenTopRight()[2] == rhs.Camera->GetScreenTopRight()[2] ) return true;

  return false;
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void CVSession::DeepCopy( CVSession *copy )
{
  this->Camera->DeepCopy( copy->Camera );
}


//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void CVSession::Save( const char* fileName )
{
  vtkSmartPointer< vtkCVSessionWriter > writer = vtkSmartPointer< vtkCVSessionWriter >::New();
  writer->SetFileName( fileName );
  writer->SetInput( this );
  writer->Update();
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void CVSession::Load( const char* fileName )
{
  vtkSmartPointer< vtkCVSessionReader > reader = vtkSmartPointer< vtkCVSessionReader >::New();
  reader->SetFileName( fileName );
  reader->Update();
  this->DeepCopy( reader->GetOutput() );
}

//-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-+#+-
void CVSession::PrintSelf( ostream &os, vtkIndent indent )
{
  Superclass::PrintSelf( os, indent );

  os << indent << "Camera: ";
  if( NULL == this->Camera )
  {
    os << "NULL" << endl;
  }
  else
  {
    os << this->Camera << endl;
    this->Camera->PrintSelf( os, indent.GetNextIndent() );
  }
}
