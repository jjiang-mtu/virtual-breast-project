/*=========================================================================

Program:   VMTK
Module:    $RCSfile: vtkvmtkappend.h,v $
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

#ifndef __vtkvmtkappend_h
#define __vtkvmtkappend_h

#include "vtkPolyDataAlgorithm.h"
#include "vtkvmtkWin32Header.h"

class VTK_VMTK_MISC_EXPORT vtkvmtkappend : public vtkPolyDataAlgorithm 
{
public:
  static vtkvmtkappend* New();
  vtkTypeRevisionMacro(vtkvmtkappend,vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent); 
  
  void AssignZones(vtkPolyData *input, vtkPolyData *referencesurface, vtkPolyData *output);

  vtkSetObjectMacro(referencesurface,vtkPolyData);
  vtkGetObjectMacro(referencesurface,vtkPolyData);

protected:
  vtkvmtkappend();
  ~vtkvmtkappend();

  virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
  vtkPolyData *referencesurface;

  vtkvmtkappend(const vtkvmtkappend&);  // Not implemented.
  void operator=(const vtkvmtkappend&);  // Not implemented.
};

#endif
