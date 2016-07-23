/*=========================================================================
                                                                                                                                    
Program:   VMTK
Module:    $RCSfile: vtkvmtkappend.cxx,v $
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

#include "vtkvmtkappend.h"
#include "vtkPolyData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkTriangleFilter.h"
#include "vtkAppendPolyData.h"
#include "vtkCleanPolyData.h"
#include "vtkCubeSource.h"
#include "vtkSphereSource.h"
#include "vtkAppendPolyData.h"
#include "vtkPlaneSource.h"

vtkCxxRevisionMacro(vtkvmtkappend, "$Revision: 1.0 $");
vtkStandardNewMacro(vtkvmtkappend);

vtkvmtkappend::vtkvmtkappend()
{
  this->referencesurface = NULL;
}

vtkvmtkappend::~vtkvmtkappend()
{
  if (this->referencesurface)
  {
    this->referencesurface->Delete();
    this->referencesurface = NULL;
  }
}

void vtkvmtkappend::AssignZones(vtkPolyData *surface,vtkPolyData *temp1,vtkPolyData *output)
{
  // create cube source
  vtkCubeSource *cube = vtkCubeSource::New();
  cube->SetCenter(0, 0, 0);
  cube->SetXLength(45);
  cube->SetYLength(60);
  cube->SetZLength(45);
  cube->Update();

  vtkTriangleFilter *triangle1 = vtkTriangleFilter::New();
  triangle1->SetInput(cube->GetOutput());
  triangle1->Update();
  cube->Delete();

  // create sphere source
  vtkSphereSource *sphere = vtkSphereSource::New();
  sphere->SetCenter(0, 5, 0);
  sphere->SetThetaResolution(10);
  sphere->SetPhiResolution(10);
  sphere->SetRadius(1);
  sphere->Update();

  vtkTriangleFilter *triangle2 = vtkTriangleFilter::New();
  triangle2->SetInput(sphere->GetOutput());
  triangle2->Update();
  sphere->Delete();

  // append
  vtkAppendPolyData *appendFilter = vtkAppendPolyData::New();
  // appendFilter->AddInput(triangle1->GetOutput());
  // appendFilter->AddInput(triangle2->GetOutput());    
  appendFilter->AddInput(surface);  
  appendFilter->AddInput(temp1);  
  appendFilter->Update();

  vtkCleanPolyData *clean = vtkCleanPolyData::New();
  clean->SetInput(appendFilter->GetOutput());
  clean->Update();
     
  output->DeepCopy(clean->GetOutput());
  clean->Delete();
  return;
}

int vtkvmtkappend::RequestData(
    vtkInformation *request,
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector)
{
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  vtkPolyData *input = vtkPolyData::SafeDownCast(
      inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkPolyData *output = vtkPolyData::SafeDownCast(
      outInfo->Get(vtkDataObject::DATA_OBJECT()));

  this->AssignZones(input, this->referencesurface, output);
  return 1;
}

void vtkvmtkappend::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
