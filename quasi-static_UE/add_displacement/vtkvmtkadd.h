/*=========================================================================

Program:   VMTK
Module:    $RCSfile: vtkvmtkadd.h,v $
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

#ifndef __vtkvmtkadd_h
#define __vtkvmtkadd_h

#include "vtkPolyDataAlgorithm.h"
#include "vtkvmtkWin32Header.h"
#include "vtkUnstructuredGrid.h"

class vtkPolyData;

class VTK_VMTK_MISC_EXPORT vtkvmtkadd : public vtkPolyDataAlgorithm
{
public: 
  vtkTypeRevisionMacro(vtkvmtkadd,vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent); 

  static vtkvmtkadd *New();

  vtkGetStringMacro(displacementdataname);
  vtkSetStringMacro(displacementdataname);
  
  void AssignZones(vtkUnstructuredGrid *input,vtkUnstructuredGrid *output);

protected:
  vtkvmtkadd();
  ~vtkvmtkadd();  

  virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);
  virtual int FillInputPortInformation(int port, vtkInformation *info);
  // virtual int FillOutputPortInformation(int port, vtkInformation *info);

private:
  char *displacementdataname;
  vtkvmtkadd(const vtkvmtkadd&);  // Not implemented.
  void operator=(const vtkvmtkadd&);  // Not implemented.
};

#endif
