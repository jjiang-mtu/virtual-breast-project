/*=========================================================================
                                                                                                                                    
Program:   VMTK
Module:    $RCSfile: vtkvmtkParameterWriter.cxx,v $
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

#include "vtkCellArray.h"
#include "vtkUnstructuredGrid.h"
#include "vtkvmtkParameterWriter.h"
#include "vtkPolyData.h"
#include "vtkPointData.h"
#include "vtkIdList.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkPointLocator.h"
#include "vtkDoubleArray.h"
#include "vtkIntArray.h"
#include "vtkMath.h"
#include "vtkDelaunay3D.h"
#include "vtkAssignAttribute.h"
#include <stdlib.h>
#include <string.h>
#include "vtkPointLocator.h"
#include "vtkProbeFilter.h"
#include "vtkIdList.h"
#include "vtkCharArray.h"
#include "vtkCellData.h"
#include "vtkCleanPolyData.h"
#include "vtkMergePoints.h"
#include "vtkImageData.h"
#include "vtkDelaunay3D.h"
#include "vtkProbeFilter.h"
#include "vtkvmtkCFDConverter.h"
#include <string>
#include <sstream>
using namespace std;

vtkCxxRevisionMacro(vtkvmtkParameterWriter, "$Revision: 1.0 $");
vtkStandardNewMacro(vtkvmtkParameterWriter);

vtkvmtkParameterWriter::vtkvmtkParameterWriter()
{
  this->Resolution = 1; // outpute file resolution
  this->ResultFileName = NULL;
  this->ParameterArrayName = NULL;
  this->Option = 0;
}

vtkvmtkParameterWriter::~vtkvmtkParameterWriter()
{
  if (this->ResultFileName)
  {
    delete[] this->ResultFileName;
    this->ResultFileName = NULL;
  }
  
  if (this->ParameterArrayName)
  {
    delete[] this->ParameterArrayName;
    this->ParameterArrayName = NULL;
  }  
}


void vtkvmtkParameterWriter::ResampleAndWrite(vtkUnstructuredGrid *grid)
{
  // region of interesting (volume)
  double pt[6];
  // x direction
  pt[0] = 183;
  pt[1] = 190;
  // y direction
  pt[2] = 80;
  pt[3] = 90;
  // z direction
  pt[4] = 40;
  pt[5] = 50;

  double res[3];
  res[0] = this->Resolution;
  res[1] = this->Resolution;
  res[2] = this->Resolution;
   
  int xNumber;
  int yNumber;
  int zNumber; 
  switch(this->Option) 
  {
    case 0:
       xNumber = ceil((pt[1] - pt[0])/res[0]);
       yNumber = ceil((pt[3] - pt[2])/res[1]);
       zNumber = ceil((pt[5] - pt[4])/res[2]);
       break;
    case 1: //for the y-axis @ y =0 for 10 slices
       xNumber = ceil((pt[1] - pt[0])/res[0]);
       yNumber = 6;
       zNumber = ceil((pt[5] - pt[4])/res[2]);
       pt[2] = -this->Resolution * (double)yNumber/2.;
       break;
    case 2: //for the x-axis @ x =0 for 10 slices
       xNumber = 6;
       yNumber = ceil((pt[3] - pt[2])/res[1]);
       zNumber = ceil((pt[5] - pt[4])/res[2]);
       pt[0] = -this->Resolution * (double)xNumber/2.;
       break;
    case 3: //for the z-axis @ z =0 for 10 slices
       xNumber = ceil((pt[1] - pt[0])/res[0]);
       yNumber = ceil((pt[3] - pt[2])/res[1]);
       zNumber = 6;
       pt[4] = -this->Resolution * (double)zNumber/2.;
       break;
    default:
       cout << "Something is wrong!!!\n";
       break;
  } 

   
  cout << xNumber << "\t" << yNumber << "\t" << zNumber << endl;
  vtkImageData *resampledData = vtkImageData::New(); 
  cout << "Forming image data ...\n";

  resampledData->SetOrigin(pt[0],pt[2],pt[4]);
  resampledData->SetDimensions(xNumber,yNumber,zNumber);
  resampledData->SetSpacing(res);
  resampledData->UpdateInformation();
   
  vtkProbeFilter *probe = vtkProbeFilter::New();
  probe->SetSource(grid);
  probe->SetInput(resampledData);
  probe->Update();

  vtkProbeFilter *probewy = vtkProbeFilter::New();
  probewy->SetInput(probe -> GetOutput());
  probewy->SetSource(grid);
  probewy->Update();
   
  int dims[3];

  vtkImageData *aImage = vtkImageData::SafeDownCast(probewy->GetOutput());
  aImage->GetDimensions(dims);
  cout << dims[0] << "\t" << dims[1] << "\t" << dims[2] << endl;

  vtkDoubleArray *scalars = vtkDoubleArray::SafeDownCast(aImage->GetPointData()->GetArray(this->ParameterArrayName));


  if(!scalars) cout << "No array found\n";
  int nparameters = scalars->GetNumberOfTuples();
  int iter;
  cout << "Writing out Matlab output ....\t";
  for(int i=0;i<dims[2];i++)
  {
    double *temp;
    temp = new double[dims[0]*dims[1]];
    for(int cnt1=0 ; cnt1<dims[1]; cnt1++)
    {
      for(int cnt0=0 ; cnt0<dims[0]; cnt0++) 
      {
        iter = i*dims[0]*dims[1]+cnt1*dims[0]+cnt0;
        double storage[3];
        scalars->GetTuple(iter,storage);
        temp[cnt1*dims[0]+cnt0] = storage[0];
      }
    }
    stringstream out;
    out << i;
    string s(this->ResultFileName);
    string s1 = out.str();
    s.append(s1);
    s.append(".mat",0,4);
    // "parameter" is the array name in the output file
    vtkvmtkCFDConverter::writematmatrix(&s[0], "parameter",temp, dims[1],dims[0], 0);

    cout << "finished iteration \n"<< i << "\">\n";
  }
  cout << "finished!\n";
  resampledData->Delete();
  probe->Delete();
  //isoAttribute->Delete();
  //constructor->Delete();
  return;
}

void vtkvmtkParameterWriter::WriteData()
{
  vtkUnstructuredGrid *input= vtkUnstructuredGrid::SafeDownCast(this->GetInput());
  input->BuildLinks();
  if(!this->ParameterArrayName) 
  {
    vtkErrorMacro("Parameter Data Array Name is not specified!");
    return;
  }
   
  vtkDoubleArray *DataArray = vtkDoubleArray::New();
  DataArray->DeepCopy(input->GetPointData()->GetArray(this->ParameterArrayName));
  this->ResampleAndWrite(input);
   
  DataArray->Delete();
  return;
}

void vtkvmtkParameterWriter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
