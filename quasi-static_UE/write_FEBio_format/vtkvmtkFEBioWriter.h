/*=========================================================================

Program:   VMTK
Module:    $RCSfile: vtkvmtkFEBioWriter.h,v $
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

#ifndef __vtkvmtkFEBioWriter_h
#define __vtkvmtkFEBioWriter_h

#include "vtkvmtkWin32Header.h"
#include "vtkUnstructuredGridWriter.h"

class vtkCell;
class vtkIdList;
class vtkIntArray;
class vtkIdTypeArray;
class vtkDoubleArray;

class VTK_VMTK_IO_EXPORT vtkvmtkFEBioWriter : public vtkUnstructuredGridWriter
{
public:
  static vtkvmtkFEBioWriter *New();
  vtkTypeRevisionMacro(vtkvmtkFEBioWriter,vtkUnstructuredGridWriter);
  void PrintSelf(ostream& os, vtkIndent indent);

  vtkSetStringMacro(BoundaryDataArrayName);
  vtkGetStringMacro(BoundaryDataArrayName);

  vtkSetMacro(PrescribedDisplacement,double);
  vtkGetMacro(PrescribedDisplacement,double);

  vtkSetStringMacro(VolumeDataArrayName);
  vtkGetStringMacro(VolumeDataArrayName);


  vtkSetMacro(BoundaryDataIdOffset,int);
  vtkGetMacro(BoundaryDataIdOffset,int);

protected:
  vtkvmtkFEBioWriter();
  ~vtkvmtkFEBioWriter();

  void WriteData();
  void PrintHeader(ofstream&);
  void PrintOneNode(ofstream& outfile,int id,double x,double y,double z);
  // void PrintOneTet4(ofstream& outfile,int id,int i,int j,int k,int l);
  void PrintOneTet4(ofstream& outfile,int id,int mat,int i,int j,int k,int l);
  bool CheckArrayData(vtkUnstructuredGrid *input,
                                          vtkIntArray *Array1,vtkIntArray *Array2);
  void PrintTail(ofstream& outfile);
  void PrintIndentions(int level,ofstream& outfile);
  void PrintFixBoundaryConditions_y(ofstream& outfile, vtkUnstructuredGrid *input);
  void PrintFixBoundaryConditions(ofstream& outfile,int numberOfTriangles,
      int type,vtkIdTypeArray *triangleCellIdArray,
      vtkIntArray *boundaryDataArray,vtkUnstructuredGrid *input);
  void PrintPrescribedBoundaryConditions(ofstream& outfile,int numberOfTriangles,
      int type,vtkIdTypeArray *triangleCellIdArray,
      vtkIntArray *boundaryDataArray,vtkUnstructuredGrid *input,vtkDoubleArray *Normals);
  void PrintElasticMaterial(ofstream& outfile,int mid,char *name,double rho,double E,double mu);
  void CalculateNormals(int numberOfTriangles,int type,vtkIdTypeArray *triangleCellIdArray,
      vtkIntArray *boundaryDataArray,vtkUnstructuredGrid *input,vtkDoubleArray *Normals);
  void PrintControlSection(ofstream& outfile);

  void PrintLoadData(ofstream& outfile);
  char* BoundaryDataArrayName;
  int BoundaryDataIdOffset;
  char* VolumeDataArrayName;
  double PrescribedDisplacement;

private:
  vtkvmtkFEBioWriter(const vtkvmtkFEBioWriter&);  // Not implemented.
  void operator=(const vtkvmtkFEBioWriter&);  // Not implemented.
};

#endif
