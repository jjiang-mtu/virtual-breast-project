/*=========================================================================

Program:   VMTK
Module:    $RCSfile: vtkvmtkgrape.h,v $
Language:  C++
Date:      $Date: 2006/04/06 16:47:48 $
Version:   $Revision: 1.4 $

  Copyright (c) Luca Antiga, David Steinman. All rights reserved.
  See LICENCE file for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm 
  for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __vtkvmtkgrape_h
#define __vtkvmtkgrape_h

#include "vtkPolyDataAlgorithm.h"
#include "vtkvmtkWin32Header.h"
#include "vtkUnstructuredGrid.h"

class vtkPolyData;

class VTK_VMTK_MISC_EXPORT vtkvmtkgrape : public vtkPolyDataAlgorithm
{
public: 
  vtkTypeRevisionMacro(vtkvmtkgrape,vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent); 

  static vtkvmtkgrape *New();

  // Description:
  // Set/Get the reference surface to compute distance from.
  vtkSetObjectMacro(ReferenceSurface,vtkUnstructuredGrid);
  vtkGetObjectMacro(ReferenceSurface,vtkUnstructuredGrid);

  vtkSetStringMacro(CellEntityIdsArrayName);
  vtkGetStringMacro(CellEntityIdsArrayName);

protected:
  vtkvmtkgrape();
  ~vtkvmtkgrape();  

  virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);
  // virtual int FillInputPortInformation(int port, vtkInformation *info);
  // virtual int FillOutputPortInformation(int port, vtkInformation *info);

private:
  char* CellEntityIdsArrayName;
  vtkUnstructuredGrid *ReferenceSurface;
  
  vtkvmtkgrape(const vtkvmtkgrape&);  // Not implemented.
  void operator=(const vtkvmtkgrape&);  // Not implemented.
};

#endif
