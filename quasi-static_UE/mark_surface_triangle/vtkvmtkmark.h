/*=========================================================================

Program:   VMTK
Module:    $RCSfile: vtkvmtkmark.h,v $
Language:  C++
Date:      $Date: 2016/07/19 09:49:59 $
Version:   $Revision: 1.4 $

     Copyright (c) Jingfeng Jiang, Yu Wang. All rights reserved.
     See LICENCE file for details.

     Portions of this code are covered under the VTK copyright.
     See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm 
     for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __vtkvmtkmark_h
#define __vtkvmtkmark_h

#include "vtkPolyDataAlgorithm.h"
#include "vtkvmtkWin32Header.h"
#include "vtkUnstructuredGrid.h"

class vtkPolyData;

class VTK_VMTK_MISC_EXPORT vtkvmtkmark : public vtkPolyDataAlgorithm
{
public: 
  vtkTypeRevisionMacro(vtkvmtkmark,vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent); 
  static vtkvmtkmark *New();

  vtkSetObjectMacro(ReferenceSurface,vtkUnstructuredGrid);
  vtkGetObjectMacro(ReferenceSurface,vtkUnstructuredGrid);

  vtkSetStringMacro(CellEntityIdsArrayName);
  vtkGetStringMacro(CellEntityIdsArrayName);

protected:
  vtkvmtkmark();
  ~vtkvmtkmark();  
  virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
  char* CellEntityIdsArrayName;
  vtkUnstructuredGrid *ReferenceSurface;
  
  vtkvmtkmark(const vtkvmtkmark&); // Not implemented.
  void operator=(const vtkvmtkmark&); // Not implemented.
};

#endif
