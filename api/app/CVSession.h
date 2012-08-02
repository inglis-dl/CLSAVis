/*=========================================================================

  Program:  CLSAVis (Canadian Longitudinal Study on Aging Visualizer)
  Module:   CVSession.h
  Language: C++

  Author: Patrick Emond <emondpd@mcmaster.ca>

=========================================================================*/
//
// .NAME CVSession - Session information object
//
// .SECTION Description
//
// .SECTION See Also
// CVSessionReader CVSessionWriter
// 

#ifndef __CVSession_h
#define __CVSession_h

#include "vtkDataObject.h"

#include "CVUtilities.h"

class vtkCamera;

class CVSession : public vtkDataObject
{
public:
  static CVSession *New();
  vtkTypeRevisionMacro( CVSession, vtkDataObject );
  void PrintSelf( ostream& os, vtkIndent indent );
  
  bool operator == ( const CVSession& ) const;
  bool operator != ( const CVSession &rhs ) const { return !( *this == rhs ); }
  
  virtual void DeepCopy( CVSession* );

  void Save( const char* fileName );
  void Load( const char* fileName );

  // Description:
  // Set/get the camera
  virtual vtkCamera* GetCamera() { return this->Camera; }
  virtual void SetCamera( vtkCamera* );
  
protected:
  CVSession();
  ~CVSession();
  
  vtkCamera *Camera;

private:
  CVSession( const CVSession& );  // Not implemented.
  void operator=( const CVSession& );  // Not implemented.
};

#endif
