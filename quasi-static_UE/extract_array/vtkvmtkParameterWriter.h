/*=========================================================================

Program:   VMTK
Module:    $RCSfile: vtkvmtkParameterWriter.h,v $
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

#ifndef __vtkvmtkParameterWriter_h
#define __vtkvmtkParameterWriter_h
#include "vtkUnstructuredGrid.h"
#include "vtkUnstructuredGridWriter.h"
#include "vtkvmtkWin32Header.h"

class vtkPolyData;
class vtkPoints;
class vtkDoubleArray;
class vtkUnstructuredGrid;
class vtkImageData;
// class ofstream;

class VTK_VMTK_MISC_EXPORT vtkvmtkParameterWriter: public vtkUnstructuredGridWriter
{
public: 
  vtkTypeRevisionMacro(vtkvmtkParameterWriter,vtkUnstructuredGridWriter);
  void PrintSelf(ostream& os, vtkIndent indent); 

  static vtkvmtkParameterWriter *New();

  vtkSetStringMacro(ParameterArrayName);
  vtkGetStringMacro(ParameterArrayName);

  vtkGetStringMacro(ResultFileName);
  vtkSetStringMacro(ResultFileName);

  vtkGetMacro(Resolution,double);
  vtkSetMacro(Resolution,double);

  vtkGetMacro(Option,int);
  vtkSetMacro(Option,int);

  void ResampleAndWrite(vtkUnstructuredGrid *grid);

protected:
  vtkvmtkParameterWriter();
  ~vtkvmtkParameterWriter();  

  void WriteData();

private:
  char* ParameterArrayName;
  char *ResultFileName;
  int Option;
  double Resolution;
  vtkvmtkParameterWriter(const vtkvmtkParameterWriter&);  // Not implemented.
  void operator=(const vtkvmtkParameterWriter&);  // Not implemented.
  //vtkUnstructuredGrid *data1;
};

#endif
