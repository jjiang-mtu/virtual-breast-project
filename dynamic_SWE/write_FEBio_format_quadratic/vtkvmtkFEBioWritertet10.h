/*=========================================================================

Program:   VMTK
Module:    $RCSfile: vtkvmtkfebiowytet10.h,v $
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

// .NAME vtkvmtkDolfinWriter - 
// .SECTION Description
// vtkvmtkDolfinWriter writes Dolfin files - www.fenics.org

// .SECTION See Also

#ifndef __vtkvmtkFEBioWritertet10_h
#define __vtkvmtkFEBioWritertet10_h

#include "vtkvmtkWin32Header.h"
#include "vtkUnstructuredGridWriter.h"

class vtkCell;
class vtkIdList;
class vtkIntArray;
class vtkIdTypeArray;
class vtkDoubleArray;

class VTK_VMTK_IO_EXPORT vtkvmtkFEBioWritertet10 : public vtkUnstructuredGridWriter
{
public:
  static vtkvmtkFEBioWritertet10 *New();
  vtkTypeRevisionMacro(vtkvmtkFEBioWritertet10,vtkUnstructuredGridWriter);
  void PrintSelf(ostream& os, vtkIndent indent);

  vtkSetMacro(PrescribedDisplacement,double);
  vtkGetMacro(PrescribedDisplacement,double);

  vtkSetStringMacro(VolumeDataArrayName);
  vtkGetStringMacro(VolumeDataArrayName);

protected:
  vtkvmtkFEBioWritertet10();
  ~vtkvmtkFEBioWritertet10();

  void WriteData();
  void PrintHeader(ofstream&);
  void PrintOneNode(ofstream& outfile,int id,double x,double y,double z);
  // void PrintOneTet4(ofstream& outfile,int id,int i,int j,int k,int l);
  void PrintOneTet4(ofstream& outfile,int id,int mat,int i,int j,int k,int l, int l1, int l2, int l3, int l4, int l5, int l6);
  bool CheckArrayData(vtkUnstructuredGrid *input, vtkIntArray *Array2);
  void PrintTail(ofstream& outfile);
  void PrintIndentions(int level,ofstream& outfile);
  void PrintFixBoundaryConditions(ofstream& outfile, vtkUnstructuredGrid *input);
  void PrintFixBoundaryConditions_x(ofstream& outfile, vtkUnstructuredGrid *input);
  void PrintFixBoundaryConditions_y(ofstream& outfile, vtkUnstructuredGrid *input);
  void PrintFix_forceBoundaryConditions(ofstream& outfile, vtkUnstructuredGrid *input);

  void PrintPrescribedBoundaryConditions(ofstream& outfile, vtkUnstructuredGrid *input);
  void PrintPrescribedBoundaryConditions_z(ofstream& outfile, vtkUnstructuredGrid *input);
  void PrintElasticMaterial(ofstream& outfile,int mid,char *name,double rho,double E,double mu);
  //void CalculateNormals(int numberOfTriangles,int type,vtkIdTypeArray *triangleCellIdArray,
  //                                                vtkIntArray *boundaryDataArray,
  //                                                vtkUnstructuredGrid *input,vtkDoubleArray *Normals);
  void PrintControlSection(ofstream& outfile);

  void PrintLoadData(ofstream& outfile);
  void PrintOutput(ofstream& outfile, vtkUnstructuredGrid *input);

  char* VolumeDataArrayName;
  double PrescribedDisplacement;

private:
  vtkvmtkFEBioWritertet10(const vtkvmtkFEBioWritertet10&);  // Not implemented.
  void operator=(const vtkvmtkFEBioWritertet10&);  // Not implemented.
};

#endif
