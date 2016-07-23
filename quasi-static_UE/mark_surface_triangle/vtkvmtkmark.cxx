/*=========================================================================
                                                                                                                                    
Program:   VMTK
Module:    $RCSfile: vtkvmtkmark.cxx,v $
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

#include "vtkvmtkmark.h"
#include "vtkCellData.h"
#include "vtkCellLocator.h"
#include "vtkPointLocator.h"
#include "vtkDoubleArray.h"
#include "vtkIntArray.h"
#include "vtkGenericCell.h"
#include "vtkPolyData.h"
#include "vtkPointData.h"
#include "vtkMath.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkGeometryFilter.h"
#include "vtkUnstructuredGrid.h"
#include "vtkCellType.h"
#include "vtkCell.h"
#include "vtkIdTypeArray.h"
#include "vtkXMLUnstructuredGridWriter.h"

vtkCxxRevisionMacro(vtkvmtkmark, "$Revision: 1.4 $");
vtkStandardNewMacro(vtkvmtkmark);

vtkvmtkmark::vtkvmtkmark()
{
  this->ReferenceSurface = NULL;
  this->CellEntityIdsArrayName = NULL;
  this->SetNumberOfOutputPorts(0);
}

vtkvmtkmark::~vtkvmtkmark()
{
  if (this->ReferenceSurface)
  {
    this->ReferenceSurface->Delete();
    this->ReferenceSurface = NULL;
  }
  if (this->CellEntityIdsArrayName)
  {
    delete[] this->CellEntityIdsArrayName;
    this->CellEntityIdsArrayName = NULL;
  }
}

int vtkvmtkmark::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector)
{
  // get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // get the input and output
  vtkPolyData *input = vtkPolyData::SafeDownCast(
      inInfo->Get(vtkDataObject::DATA_OBJECT()));
  // vtkUnstructuredGrid *output = vtkUnstructuredGrid::SafeDownCast(
  // outInfo->Get(vtkDataObject::DATA_OBJECT()));

  vtkIdType i;
  vtkIdType cellId;
  int subId;
  double closestPoint[3];
  vtkIdList *cellIdsofpoint;
  int type=11; // input: mark ID

  double distance2;
  vtkCellLocator *locator;
  vtkGenericCell *genericCell;
  
  vtkIdList *cellPointIds;
  cellPointIds = vtkIdList::New();

  if (!this->ReferenceSurface)
  {
    vtkErrorMacro(<<"No reference surface!");
    return 1;
  }

  // extract surface
  vtkGeometryFilter *geometryFilter = vtkGeometryFilter::New();
  geometryFilter->SetInput(this->ReferenceSurface);
  geometryFilter->Update();
  vtkPolyData *temp = geometryFilter->GetOutput();

  locator = vtkCellLocator::New();
  locator->SetDataSet(temp);
  locator->BuildLocator();
  genericCell = vtkGenericCell::New();

  // find the closest points
  vtkDoubleArray *pointsarray = NULL;
  pointsarray = vtkDoubleArray::New();
  pointsarray->SetName("closestpointarray");
  pointsarray->SetNumberOfComponents(3);

  int numberOfCells = input->GetNumberOfCells();

  for (i=0; i<numberOfCells; i++)
  {
      cellPointIds = input->GetCell(i)->GetPointIds();
      
      double pts0[3],pts1[3],pts2[3],center[3];
      
      input->GetPoints()->GetPoint(cellPointIds->GetId(0),pts0);
      input->GetPoints()->GetPoint(cellPointIds->GetId(1),pts1);
      input->GetPoints()->GetPoint(cellPointIds->GetId(2),pts2);

      center[0] = (pts0[0] + pts1[0] + pts2[0] )/3.;
      center[1] = (pts0[1] + pts1[1] + pts2[1] )/3.;
      center[2] = (pts0[2] + pts1[2] + pts2[2] )/3.;
      
      locator->FindClosestPoint(center,closestPoint,genericCell,cellId,subId,distance2);
      pointsarray->InsertTuple(i,closestPoint);      
  }

  // extract CellId
  vtkIdTypeArray* triangleCellIdArray = vtkIdTypeArray::New();
  (this->ReferenceSurface)->GetIdsOfCellsOfType(VTK_TRIANGLE,triangleCellIdArray); 
  int numberOfTriangles = triangleCellIdArray->GetNumberOfTuples();

  vtkPointLocator *Locatorpoint = vtkPointLocator::New();
  Locatorpoint->SetDataSet(this->ReferenceSurface);
  Locatorpoint->BuildLocator();

  int numberOfPoints = pointsarray->GetNumberOfTuples();

  vtkIdTypeArray* CellIdArray = vtkIdTypeArray::New();
  CellIdArray->SetNumberOfTuples(numberOfPoints);
  CellIdArray->SetNumberOfComponents(1);
  CellIdArray->FillComponent(0, 0);
  
  cellIdsofpoint = vtkIdList::New();
  for (i=0; i<numberOfPoints; i++)   
  {
     double *pointonsurface = pointsarray->GetTuple(i);
     vtkIdType PointId = Locatorpoint->FindClosestPoint(pointonsurface);

     this->ReferenceSurface->GetPointCells(PointId, cellIdsofpoint);
     for (int j=0; j<cellIdsofpoint->GetNumberOfIds(); j++)
     {
       vtkIdType tempId=cellIdsofpoint->GetId(j);
       for (int k=0; k<numberOfTriangles; k++)
       {
         if (tempId != triangleCellIdArray->GetValue(k))
         {
              continue;
         }
         CellIdArray->SetValue(i, triangleCellIdArray->GetValue(k));
         break;
       }
     }
  }                                                     

  vtkIntArray* boundaryDataArray;
  boundaryDataArray = vtkIntArray::SafeDownCast(this->ReferenceSurface->GetCellData()->GetArray(this->CellEntityIdsArrayName));

  for (i=0; i<numberOfPoints; i++)
  {
    vtkIdType tempCellId=CellIdArray->GetValue(i);
    boundaryDataArray->SetValue(tempCellId, type);     
  }

  // write vtkUnstructuredGrid data 
  this->ReferenceSurface->GetCellData()->AddArray(boundaryDataArray);
  vtkXMLUnstructuredGridWriter *writer = vtkXMLUnstructuredGridWriter::New();
  writer->SetInput(this->ReferenceSurface);
  writer->SetFileName("breast_mark.vtu");
  writer->Write();
  writer->Delete();
  cout << "Complete\n";

  locator->Delete();
  genericCell->Delete();
  geometryFilter->Delete();
  pointsarray->Delete();
  cellPointIds->Delete();
  triangleCellIdArray->Delete();
  Locatorpoint->Delete();
  CellIdArray->Delete();
  boundaryDataArray->Delete();
  cellIdsofpoint->Delete();
  return 1;
}

void vtkvmtkmark::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
