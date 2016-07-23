/*=========================================================================
                                                                                                                                    
Program:   VMTK
Module:    $RCSfile: vtkvmtkgrape.cxx,v $
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

#include "vtkvmtkgrape.h"
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
#include "vtkUnstructuredGrid.h"
#include "vtkCellType.h"
#include "vtkCell.h"
#include "vtkIdTypeArray.h"
#include "vtkXMLUnstructuredGridWriter.h"
#include "vtkSelectEnclosedPoints.h"

vtkCxxRevisionMacro(vtkvmtkgrape, "$Revision: 1.4 $");
vtkStandardNewMacro(vtkvmtkgrape);

vtkvmtkgrape::vtkvmtkgrape()
{
  this->ReferenceSurface = NULL;
  this->CellEntityIdsArrayName = NULL;
  this->SetNumberOfOutputPorts(0);
}

vtkvmtkgrape::~vtkvmtkgrape()
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

int vtkvmtkgrape::RequestData(
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

  vtkIdType i, temptype;
  vtkIdType cellId;
  vtkIdType temp;
  int subId;
  double closestPoint[3];
  int type=7; // mark ID 

  double distance2;
  vtkCellLocator *locator;
  vtkGenericCell *genericCell;

  locator = vtkCellLocator::New();
  locator->SetDataSet(this->ReferenceSurface);
  locator->BuildLocator();
  genericCell = vtkGenericCell::New();


  
  vtkIdList *cellPointIds;
  cellPointIds = vtkIdList::New();

  if (!this->ReferenceSurface)
  {
    vtkErrorMacro(<<"No reference surface!");
    return 1;
  }

  vtkSelectEnclosedPoints *selectEnclosedPoints = vtkSelectEnclosedPoints::New();
  selectEnclosedPoints->Initialize(input);

  vtkDoubleArray *Locations = vtkDoubleArray::New();
  Locations->SetNumberOfComponents(3); 
  Locations->SetName("Seed_position");

  vtkIdTypeArray* CellIdArray = vtkIdTypeArray::New();
  CellIdArray->SetNumberOfComponents(1);

  vtkIdTypeArray* tetraCellIdArray = vtkIdTypeArray::New();
  (this->ReferenceSurface)->GetIdsOfCellsOfType(VTK_TETRA,tetraCellIdArray); 
  int numberOftetra = tetraCellIdArray->GetNumberOfTuples();

  vtkIntArray* boundaryDataArray;// = vtkIntArray::New();
  boundaryDataArray = vtkIntArray::SafeDownCast((this->ReferenceSurface)->GetCellData()->GetArray(this->CellEntityIdsArrayName));

  int j=0;

  for (i=0; i<numberOftetra; i++)
  {
    temp = tetraCellIdArray->GetValue(i);
    cellPointIds = (this->ReferenceSurface)->GetCell(temp)->GetPointIds();
      
    double pts0[3],pts1[3],pts2[3],pts3[3],center[3];
    double displacement;

    (this->ReferenceSurface)->GetPoints()->GetPoint(cellPointIds->GetId(0),pts0);
    (this->ReferenceSurface)->GetPoints()->GetPoint(cellPointIds->GetId(1),pts1);
    (this->ReferenceSurface)->GetPoints()->GetPoint(cellPointIds->GetId(2),pts2);
    (this->ReferenceSurface)->GetPoints()->GetPoint(cellPointIds->GetId(3),pts3);

    center[0] = (pts0[0] + pts1[0] + pts2[0] + pts3[0])/4.;
    center[1] = (pts0[1] + pts1[1] + pts2[1] + pts3[1])/4.;
    center[2] = (pts0[2] + pts1[2] + pts2[2] + pts3[2])/4.;

    if(selectEnclosedPoints->IsInsideSurface(center))
    {
      Locations->InsertNextTuple(center);
      locator->FindClosestPoint(center,closestPoint,genericCell,cellId,subId,distance2);
      boundaryDataArray->SetValue(cellId, type);
    }
  }
  (this->ReferenceSurface)->GetCellData()->AddArray(boundaryDataArray);
  vtkXMLUnstructuredGridWriter *writer = vtkXMLUnstructuredGridWriter::New();
  writer->SetInput(this->ReferenceSurface);
  writer->SetFileName("breast_new.vtu");
  writer->Write();
  writer->Delete();
  locator->Delete();
  genericCell->Delete();
  cellPointIds->Delete();
  CellIdArray->Delete();
  boundaryDataArray->Delete();
  return 1;
}

void vtkvmtkgrape::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
