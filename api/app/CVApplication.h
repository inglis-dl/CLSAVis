/*=========================================================================

  Program:  CLSAVis (Canadian Longitudinal Study on Aging Visualizer)
  Module:   CVApplication.h
  Language: C++

  Author: Patrick Emond <emondpd@mcmaster.ca>

=========================================================================*/
//
// .NAME CVApplication - Application or "central" object
//
// .SECTION Description
//
// .SECTION See Also
// CVApplicationReader CVApplicationWriter
// 

#ifndef __CVApplication_h
#define __CVApplication_h

#include "vtkObject.h"

#include "CVUtilities.h"

class CVSession;
class vtkCVView;

class CVApplication : public vtkObject
{
public:
  static CVApplication *GetInstance();
  static void DeleteInstance();

  vtkTypeRevisionMacro( CVApplication, vtkObject );
  void PrintSelf( ostream& os, vtkIndent indent );

  vtkGetObjectMacro( Session, CVSession );
  vtkGetObjectMacro( View, vtkCVView );
  
protected:
  CVApplication();
  ~CVApplication();

  static CVApplication *New();
  static CVApplication *Instance;

  CVSession *Session;
  vtkCVView *View;
  
private:
  CVApplication( const CVApplication& );  // Not implemented.
  void operator=( const CVApplication& );  // Not implemented.
};

#endif
