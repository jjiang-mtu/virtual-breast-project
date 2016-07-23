/*=========================================================================
                                                                                                                                    
Program:   VMTK
Module:    $RCSfile: vtkvmtkadd.cxx,v $
Language:  C++
Date:      $Date: 2016/07/19 09:49:59 $
Version:   $Revision: 1.6 $
                                                                                                                                    
     Copyright (c) Jingfeng Jiang, Yu Wang. All rights reserved.
     See LICENCE file for details.

     Portions of this code are covered under the VTK copyright.
     See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm 
     for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
                                                                                                                                    
=========================================================================*/

#include "vtkvmtkadd.h"
#include "vtkUnstructuredGrid.h"
#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkMath.h"
#include "vtkXMLUnstructuredGridWriter.h"
#include "vtkDoubleArray.h"
#include "vtkPointData.h"
#include <stdlib.h>

vtkCxxRevisionMacro(vtkvmtkadd, "$Revision: 1.6 $");
vtkStandardNewMacro(vtkvmtkadd);

vtkvmtkadd::vtkvmtkadd()
{
  this->displacementdataname = NULL;
}

vtkvmtkadd::~vtkvmtkadd()
{
  if (this->displacementdataname)
  {
    delete[] this->displacementdataname;
    this->displacementdataname = NULL;
  }
}

void vtkvmtkadd::AssignZones(vtkUnstructuredGrid *input,vtkUnstructuredGrid *output)
{
  //read data from txt
  vtkDoubleArray *displacementdata = NULL;
  displacementdata = vtkDoubleArray::New();

  displacementdata->SetName(this->displacementdataname);
  //warpdata->SetNumberOfTuples(input->GetPoints()->GetNumberOfPoints());
  displacementdata->SetNumberOfComponents(1);
  //displacementdata->FillComponent(0,0.0);
  //displacementdata->FillComponent(1,0.0);
  //displacementdata->FillComponent(2,0.0);

  double pt[3];
  double ptemp;
  double parameter[4]; 

  ifstream infile;
  infile.open("displacement.txt");

  char tempStr[1028];
  double tmp;

  for (int i=0;i<81040;i++)
  {      
    for(int j=0;j<3;j++)
    {
      infile.getline(tempStr,1028,' ');
      parameter[j] = atof(tempStr);
    }
    infile.getline(tempStr,1028);
    parameter[3] = atof(tempStr);
    tmp=parameter[0];

    pt[0] = parameter[1];
    pt[1] = parameter[2];
    pt[2] = parameter[3];
    ptemp=pt[1];
    displacementdata->InsertNextTuple(&ptemp);  
  } 

    input->GetPointData()->AddArray(displacementdata);
    vtkXMLUnstructuredGridWriter *writer = vtkXMLUnstructuredGridWriter::New();
    writer->SetInput(input);
    writer->SetFileName("breast_dz.vtu");
    writer->Write();
    writer->Delete();
    infile.close();

    return;
}

int vtkvmtkadd::FillInputPortInformation(int, vtkInformation *info)
{
  info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkDataSet");
  return 1;
}

int vtkvmtkadd::RequestData(
    vtkInformation *vtkNotUsed(request),
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector)
{
  // get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // get the input and output
  vtkUnstructuredGrid *input = vtkUnstructuredGrid::SafeDownCast(
      inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkUnstructuredGrid *output = vtkUnstructuredGrid::SafeDownCast(
      outInfo->Get(vtkDataObject::DATA_OBJECT()));
  // main function
  this->AssignZones(input,output);

  return 1;
}

void vtkvmtkadd::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
