/*=========================================================================

Program:   VMTK
Module:    $RCSfile: vtkvmtkwarp.h,v $
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
  // .NAME vtkvmtkSurfaceProjection - ...
  // .SECTION Description
  // .

#ifndef __vtkvmtkwarp_h
#define __vtkvmtkwarp_h

#include "vtkPolyDataAlgorithm.h"
#include "vtkvmtkWin32Header.h"
#include "vtkUnstructuredGrid.h"

class vtkPolyData;

class VTK_VMTK_MISC_EXPORT vtkvmtkwarp : public vtkPolyDataAlgorithm
{
  public: 
  vtkTypeRevisionMacro(vtkvmtkwarp,vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent); 

  static vtkvmtkwarp *New();

  vtkGetStringMacro(warpdataname);
  vtkSetStringMacro(warpdataname);
  
  void AssignZones(vtkUnstructuredGrid *input,vtkUnstructuredGrid *output);

  protected:
  vtkvmtkwarp();
  ~vtkvmtkwarp();  

  virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);
  virtual int FillInputPortInformation(int port, vtkInformation *info);
  // virtual int FillOutputPortInformation(int port, vtkInformation *info);

  private:
  char *warpdataname;
  vtkvmtkwarp(const vtkvmtkwarp&);     // Not implemented.
  void operator=(const vtkvmtkwarp&);  // Not implemented.
};

#endif
