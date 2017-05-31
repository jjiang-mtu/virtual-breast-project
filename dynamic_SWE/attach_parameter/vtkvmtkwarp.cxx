/*=========================================================================
                                                                                                                                    
Program:   VMTK
Module:    $RCSfile: vtkvmtkwarp.cxx,v $
Language:  C++
Date:      $Date: 2016/08/19 09:49:59 $
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

#include "vtkvmtkwarp.h"
#include "vtkUnstructuredGrid.h"
#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkMath.h"
#include "vtkXMLUnstructuredGridWriter.h"
#include "vtkDoubleArray.h"
#include "vtkPointData.h"
#include <stdlib.h>
#include <vtkWarpVector.h>

vtkCxxRevisionMacro(vtkvmtkwarp, "$Revision: 1.6 $");
vtkStandardNewMacro(vtkvmtkwarp);

vtkvmtkwarp::vtkvmtkwarp() {
}

vtkvmtkwarp::~vtkvmtkwarp() {
}

void vtkvmtkwarp::AssignZones(vtkUnstructuredGrid *input,vtkUnstructuredGrid *output)
{
  //read data from txt
  vtkDoubleArray *warpdata = NULL;
  warpdata = vtkDoubleArray::New();

  warpdata->SetName(this->warpdataname);
  warpdata->SetNumberOfTuples(input->GetPoints()->GetNumberOfPoints());
  warpdata->SetNumberOfComponents(1);
  warpdata->FillComponent(0,0.0);
  // warpdata->FillComponent(1,0.0);
  // warpdata->FillComponent(2,0.0);

  double pt;        // double pt[3];
  double parameter; // double parameter[1]; 
  ifstream infile;
  infile.open("displacement.txt");
  char tempStr[1028];

  // write data to model (.vtu file)
  for (int i=0;i<201517;i++)
  { 
    infile.getline(tempStr,1028);
    parameter = atof(tempStr);
    pt = parameter;
    warpdata->InsertNextTuple(&pt);  
  } 
  input->GetPointData()->AddArray(warpdata);

  vtkXMLUnstructuredGridWriter *writer = vtkXMLUnstructuredGridWriter::New();
  writer->SetInput(input);
  writer->SetFileName("model_parameter.vtu");
  writer->Write();
  writer->Delete();
  infile.close();
  warpdata->Delete();
  return;
}

int vtkvmtkwarp::FillInputPortInformation(int, vtkInformation *info)
{
  info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkDataSet");
  return 1;
}

int vtkvmtkwarp::RequestData(
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

  this->AssignZones(input,output);
  return 1;
}

void vtkvmtkwarp::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
