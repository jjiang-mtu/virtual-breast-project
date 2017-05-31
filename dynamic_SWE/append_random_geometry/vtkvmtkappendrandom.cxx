/*=========================================================================
                                                                                                                                    
Program:   VMTK
Module:    $RCSfile: vtkvmtkappendrandom.cxx,v $
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

#include "vtkvmtkappendrandom.h"
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
#include <stdlib.h> 
#include <vector>
#include <sstream>
using namespace std;

vtkCxxRevisionMacro(vtkvmtkappendrandom, "$Revision: 1.0 $");
vtkStandardNewMacro(vtkvmtkappendrandom);

vtkvmtkappendrandom::vtkvmtkappendrandom()
{
  this->referencesurface1 = NULL;
}


vtkvmtkappendrandom::~vtkvmtkappendrandom()
{
  if (this->referencesurface1)
    {
      this->referencesurface1->Delete();
      this->referencesurface1 = NULL;
    }
}

void vtkvmtkappendrandom::AssignZones(vtkPolyData *surface,vtkPolyData *temp1,vtkPolyData *output)
{
  // generate cube source
  double xLength = 9;
  double yLength = 9;
  double zLength = 9;
  vtkCubeSource *cube = vtkCubeSource::New();
  cube->SetCenter(4.8, 4.8, 4.8);
  cube->SetXLength(xLength);
  cube->SetYLength(yLength);
  cube->SetZLength(zLength);
  cube->Update();

  vtkTriangleFilter *triangle1 = vtkTriangleFilter::New();
  triangle1->SetInput(cube->GetOutput());
  triangle1->Update();
  cube->Delete();

  vtkAppendPolyData *appendFilter = vtkAppendPolyData::New();
  // appendFilter->AddInput(triangle1->GetOutput());

  double sphereNum = 1000;
  double sphereR = 0.5;
  vector<double> xvector;
  vector<double> yvector;
  vector<double> zvector;
  for (int i = 0; i < sphereNum; i++) {
    double xP, yP, zP;
    xP = ((double) rand() / (RAND_MAX)) * xLength - (xLength / 2);
    yP = ((double) rand() / (RAND_MAX)) * yLength - (yLength / 2);
    zP = ((double) rand() / (RAND_MAX)) * zLength - (zLength / 2);

    if (xP > xLength/2 - 1.1*sphereR || xP < -xLength/2 + 1.1*sphereR ||
	yP > yLength/2 - 1.1*sphereR || yP < -yLength/2 + 1.1*sphereR ||
	zP > zLength/2 - 1.1*sphereR || zP < -zLength/2 + 1.1*sphereR) {
      continue;
    }
	
    if (xvector.empty()) {
      xvector.push_back(xP);
      yvector.push_back(yP);
      zvector.push_back(zP);
    } else {
      int count = 0;
      for (int j = 0; j < xvector.size(); j++) {
        if ((xP-xvector.at(j))*(xP-xvector.at(j)) + 
	    (yP-yvector.at(j))*(yP-yvector.at(j)) + 
	    (zP-zvector.at(j))*(zP-zvector.at(j)) > (2*sphereR)*(2*sphereR)) {
	  count++;
	} else {
	  break;
	} 
      }
      if (count == xvector.size()) {
        xvector.push_back(xP);
	yvector.push_back(yP);
	zvector.push_back(zP);
      }  
    }
  }

  for (int i = 0; i < xvector.size(); i++) {
    vtkSphereSource *sphere = vtkSphereSource::New();
    sphere->SetCenter(xvector.at(i)+4.8, yvector.at(i)+4.8, zvector.at(i)+4.8);
    sphere->SetThetaResolution(10);
    sphere->SetPhiResolution(10);
    sphere->SetRadius(sphereR);
    sphere->Update();

    vtkTriangleFilter *triangle2 = vtkTriangleFilter::New();
    triangle2->SetInput(sphere->GetOutput());
    triangle2->Update();
    sphere->Delete();

    appendFilter->AddInput(triangle2->GetOutput()); 
    triangle2->Delete();   
  }

  vtkCleanPolyData *clean = vtkCleanPolyData::New();
  clean->SetInput(appendFilter->GetOutput());
  clean->Update();
     
  output->DeepCopy(clean->GetOutput());
  clean->Delete();

  ofstream outputfile;

  // record the center of the generated spheres
  outputfile.open("sphereCenter.txt");
  for (int i = 0; i < xvector.size(); i++) {
    outputfile << xvector.at(i)+4.8 << endl;
  }
  for (int i = 0; i < yvector.size(); i++) {
    outputfile << yvector.at(i)+4.8 << endl;
  }
  for (int i = 0; i < zvector.size(); i++) {
    outputfile << zvector.at(i)+4.8 << endl;
  }

  outputfile << zvector.size() << endl;
  outputfile.close();
  return;
}

int vtkvmtkappendrandom::RequestData(
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

  this->AssignZones(input, this->referencesurface1, output);
  return 1;
}

void vtkvmtkappendrandom::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
