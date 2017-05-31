/*=========================================================================

Program:   VMTK
Module:    $RCSfile: vtkvmtkappendrandom.h,v $
Language:  C++
Date:      $Date: 2016/08/19 09:49:59 $
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
// .NAME vtkvmtkCreatZone. - ..
// .SECTION Description
// ..

#ifndef __vtkvmtkappendrandom_h
#define __vtkvmtkappendrandom_h

#include "vtkPolyDataAlgorithm.h"
#include "vtkvmtkWin32Header.h"

class VTK_VMTK_MISC_EXPORT vtkvmtkappendrandom : public vtkPolyDataAlgorithm {
public:
  static vtkvmtkappendrandom* New();
  vtkTypeRevisionMacro(vtkvmtkappendrandom,vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);   
  void AssignZones(vtkPolyData *input, vtkPolyData *referencesurface1, vtkPolyData *output);

  vtkSetObjectMacro(referencesurface1,vtkPolyData);
  vtkGetObjectMacro(referencesurface1,vtkPolyData);

protected:
  vtkvmtkappendrandom();
  ~vtkvmtkappendrandom();

  virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
  //int IsSecondLayer;
  //int Option;
  //int baseId;
  //double R2;
  //double R1;
  //double Center[3];
  //char *CellEntityIdsArrayName;
  vtkPolyData *referencesurface1;
  vtkvmtkappendrandom(const vtkvmtkappendrandom&);  // Not implemented.
  void operator=(const vtkvmtkappendrandom&);  // Not implemented.
};

#endif
