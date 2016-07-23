/*=========================================================================
                                                                                                                                    
Program:   VMTK
Module:    $RCSfile: vtkvmtkFEBioWriter.cxx,v $
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

#include "vtkDoubleArray.h"
#include "vtkIntArray.h"
#include "vtkvmtkFEBioWriter.h"
#include "vtkUnstructuredGrid.h"
#include "vtkCellType.h"
#include "vtkCell.h"
#include "vtkPointData.h"
#include "vtkCellData.h"
#include "vtkIdTypeArray.h"
#include "vtkObjectFactory.h"
#include "vtkvmtkConstants.h"
#include "vtkGeometryFilter.h"
#include "vtkPolyDataNormals.h"
#include "vtkPointLocator.h"
#include "vtkMath.h"
#include <sstream>
#include <stdlib.h>


vtkCxxRevisionMacro(vtkvmtkFEBioWriter, "$Revision: 1.6 $");
vtkStandardNewMacro(vtkvmtkFEBioWriter);

vtkvmtkFEBioWriter::vtkvmtkFEBioWriter()
{
  this->BoundaryDataArrayName = NULL;
  this->BoundaryDataIdOffset = 0;
  this->VolumeDataArrayName = NULL;
  this->PrescribedDisplacement = 0.6; // displacement or force value
}

vtkvmtkFEBioWriter::~vtkvmtkFEBioWriter()
{
  
  if (this->BoundaryDataArrayName)
  {
    delete[] this->BoundaryDataArrayName;
    this->BoundaryDataArrayName = NULL;
  }
  if (this->VolumeDataArrayName)
  {
    delete[] this->VolumeDataArrayName;
    this->VolumeDataArrayName = NULL;
  }
}

bool vtkvmtkFEBioWriter::CheckArrayData(vtkUnstructuredGrid *input,
    vtkIntArray *boundaryDataArray,vtkIntArray *volumeDataArray)
{
  if (this->BoundaryDataArrayName)
  {
    if (input->GetCellData()->GetArray(this->BoundaryDataArrayName))
    {
      boundaryDataArray->DeepCopy(input->GetCellData()->GetArray(this->BoundaryDataArrayName));
    } else {
      vtkErrorMacro(<<"BoundaryDataArray with name specified does not exist");
      boundaryDataArray->Delete(); 
      volumeDataArray->Delete();
      boundaryDataArray = NULL;
      volumeDataArray = NULL;
      return false;
    }
  } else {
    vtkErrorMacro(<<"BoundaryDataArray is not specified.\n");
    boundaryDataArray->Delete();
    volumeDataArray->Delete();
    boundaryDataArray = NULL;
    volumeDataArray = NULL;
    return false;
  }

  if (this->VolumeDataArrayName)
  {
    if (input->GetCellData()->GetArray(this->VolumeDataArrayName))
    {
      volumeDataArray->DeepCopy(input->GetCellData()->GetArray(this->VolumeDataArrayName));
    } else {
      vtkErrorMacro(<<"Volume DataArray with name specified does not exist");
      boundaryDataArray->Delete();
      volumeDataArray->Delete();
      boundaryDataArray = NULL;
      volumeDataArray = NULL;
      return false;
    }
  } else {
    vtkErrorMacro(<<"Volume DataArray is not specified.\n");
    boundaryDataArray->Delete();
    volumeDataArray->Delete();
    boundaryDataArray = NULL;
    volumeDataArray = NULL;
    return false;
  }
  return true;
}

void vtkvmtkFEBioWriter::PrintHeader(ofstream& outfile)
{
  outfile << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n";
  outfile << "<!--\n";
  this->PrintIndentions(4,outfile);
  outfile << "FEBio Input File Converted by customized scripts\n";
  this->PrintIndentions(4,outfile);
  outfile << "Author: Jingfeng Jiang\n";
  this->PrintIndentions(4,outfile);
  outfile << "Dept. of Medical Physics, Univ. of Wisconsin\n";
  outfile << "-->\n"; 
  outfile << "<febio_spec version=\"1.1\">\n";
  this->PrintIndentions(1,outfile);
  outfile << "<Module type=\"solid\"/>\n";
}

void vtkvmtkFEBioWriter::PrintTail(ofstream& outfile)
{
  // this->PrintIndentions(1,outfile);
  outfile << "</febio_spec>\n";
  return;
}

void vtkvmtkFEBioWriter::PrintIndentions(int level,ofstream& outfile)
{
  for(int j=0;j<level;j++)
    outfile << "\t";
  return;
}

void vtkvmtkFEBioWriter::PrintElasticMaterial(ofstream& outfile,int mid,char *name,double rho,double E,double mu)
{
  this->PrintIndentions(2,outfile);
  outfile << "<material id=\"" << mid << "\" name=\"Material 1\" type=\"" << name << "\">\n";
  this->PrintIndentions(3,outfile);
  outfile << "<density>" << rho << "</density>\n";
  this->PrintIndentions(3,outfile);
  outfile << "<E>" << E << "</E>\n";
  this->PrintIndentions(3,outfile);
  outfile << "<v>" << mu << "</v>\n";
  this->PrintIndentions(2,outfile);
  outfile << "</material>\n";
}

void vtkvmtkFEBioWriter::PrintFixBoundaryConditions_y(ofstream& outfile,vtkUnstructuredGrid *input)
{
  int numberOfPoints = input->GetNumberOfPoints();
  double point[3];

  vtkPointLocator *Locator = vtkPointLocator::New();
  Locator->SetDataSet(input);
  Locator->BuildLocator();

  vtkIdList *downList = vtkIdList::New();


  this->PrintIndentions(2,outfile);
  outfile << "<fix>\n";

  for (int i=0; i<numberOfPoints; i++)
  {
    input->GetPoint(i,point);
    if (fabs(point[2]-38)<1e-12)
    {
      vtkIdType downId = Locator->FindClosestPoint(point);
      this->PrintIndentions(3,outfile);
      outfile << "<node id=\"" << downId+1 << "\" bc=\"xyz\"> </node>\n";
    }
  }

  for (int i=0; i<numberOfPoints; i++)
  {
    input->GetPoint(i,point);
    if (fabs(point[1]-70)<1e-12)
    {
      vtkIdType downId = Locator->FindClosestPoint(point);
      this->PrintIndentions(3,outfile);
      outfile << "<node id=\"" << downId+1 << "\" bc=\"xy\"> </node>\n";
    }
  }

  for (int i=0; i<numberOfPoints; i++)
  {
    input->GetPoint(i,point);
    if (fabs(point[0]-180)<1e-12)
    {
      vtkIdType downId = Locator->FindClosestPoint(point);
      this->PrintIndentions(3,outfile);
      outfile << "<node id=\"" << downId+1 << "\" bc=\"xy\"> </node>\n";
    }
  }

  for (int i=0; i<numberOfPoints; i++)
  {
    input->GetPoint(i,point);
    if (fabs(point[0]-195)<1e-12)
    {
      vtkIdType downId = Locator->FindClosestPoint(point);
      this->PrintIndentions(3,outfile);
      outfile << "<node id=\"" << downId+1 << "\" bc=\"xy\"> </node>\n";
    }
  }
  this->PrintIndentions(2,outfile);
  outfile << "</fix>\n";
  downList->Delete();
  return;
}

void vtkvmtkFEBioWriter::PrintFixBoundaryConditions(ofstream& outfile,int numberOfTriangles,
    int type,vtkIdTypeArray *triangleCellIdArray,
    vtkIntArray *boundaryDataArray,vtkUnstructuredGrid *input)
{
  this->PrintIndentions(2,outfile);
  outfile << "<fix>\n";
  vtkIdList *aList = vtkIdList::New();
  for (int i=0; i<numberOfTriangles; i++)
  {
    vtkIdType triangleCellId = triangleCellIdArray->GetValue(i);
    if (boundaryDataArray->GetValue(triangleCellId) != type)
    {
      continue;
    }
    vtkIdList* cellPointIds = input->GetCell(triangleCellId)->GetPointIds();
    vtkIdType id0 = cellPointIds->GetId(0);
    aList->InsertUniqueId(id0);
    vtkIdType id1 = cellPointIds->GetId(1);
    vtkIdType id2 = cellPointIds->GetId(2);
    aList->InsertUniqueId(id1);
    aList->InsertUniqueId(id2);
  }

  cout << "number in aList" << aList->GetNumberOfIds();
  for (int i=0; i<aList->GetNumberOfIds(); i++)
  {
    this->PrintIndentions(3,outfile);
    outfile << "<node id=\"" << aList->GetId(i)+1 << "\" bc=\"xyz\"> </node>\n";
  }
  this->PrintIndentions(2,outfile);
  outfile << "</fix>\n";
  aList->Delete();
  return;
}

void vtkvmtkFEBioWriter::PrintControlSection(ofstream& outfile)
{
  this->PrintIndentions(1,outfile);
  outfile << "<Control>\n";
  this->PrintIndentions(2,outfile);
  outfile << "<title>Tetrahedral Cantilever Beam</title>\n";
  this->PrintIndentions(2,outfile);
  outfile <<	"<time_steps>10</time_steps>\n";
  this->PrintIndentions(2,outfile);
  outfile << "<step_size>0.1</step_size>\n";
  this->PrintIndentions(2,outfile);
  outfile << "<max_refs>50</max_refs>\n";
  this->PrintIndentions(2,outfile);
  outfile << "<max_ups>10</max_ups>\n";
  this->PrintIndentions(2,outfile);
  outfile << "<dtol>0.001</dtol>\n";
  this->PrintIndentions(2,outfile);
  outfile << "<etol>0.01</etol>\n";
  this->PrintIndentions(2,outfile);
  outfile << "<rtol>0</rtol>\n";
  this->PrintIndentions(2,outfile);
  outfile << "<lstol>0</lstol>\n";
  this->PrintIndentions(2,outfile);
  outfile << "<integration>\n";
  this->PrintIndentions(3,outfile);
  outfile << "<rule elem=\"tet4\" type=\"UT4\">\n";
  this->PrintIndentions(3,outfile);
  outfile << "<alpha>0.05</alpha>\n";
  this->PrintIndentions(3,outfile);
  outfile << "<iso_stab>1</iso_stab>\n";
  this->PrintIndentions(3,outfile);
  outfile << "</rule>\n";
  this->PrintIndentions(2,outfile);
  outfile << "</integration>\n";
  this->PrintIndentions(1,outfile);
  outfile << "</Control>\n";
  return;
}

void vtkvmtkFEBioWriter::PrintLoadData(ofstream& outfile)
{
  this->PrintIndentions(1,outfile);
  outfile << "<LoadData>\n";
  this->PrintIndentions(2,outfile);
  outfile <<	"<loadcurve id=\"1\" type=\"smooth\" extend=\"constant\">\n";
  this->PrintIndentions(3,outfile);
  outfile << "<loadpoint>0,0</loadpoint>\n";
  this->PrintIndentions(3,outfile);
  outfile <<	"<loadpoint>1,1</loadpoint>\n";
  this->PrintIndentions(2,outfile);
  outfile <<	"</loadcurve>\n";
  this->PrintIndentions(1,outfile);
  outfile << "</LoadData>\n";
}

void vtkvmtkFEBioWriter::CalculateNormals(int numberOfTriangles,int type,vtkIdTypeArray *triangleCellIdArray,
    vtkIntArray *boundaryDataArray,vtkUnstructuredGrid *input,vtkDoubleArray *Normals)
{

  vtkIdList *aList = vtkIdList::New();

  vtkGeometryFilter *geometryFilter = vtkGeometryFilter::New();
  geometryFilter->SetInput(input);
  geometryFilter->Update(); // extract the surface of the breast tissue


  vtkPolyData *temp = geometryFilter->GetOutput();
  vtkPointLocator *Locator = vtkPointLocator::New();
  Locator->SetDataSet(temp);
  Locator->BuildLocator(); // the surface has been processed


  for (int i=0; i<numberOfTriangles; i++)
  {
    vtkIdType triangleCellId = triangleCellIdArray->GetValue(i);
    if (boundaryDataArray->GetValue(triangleCellId) != type)
    {
      continue;
    }
    vtkIdList* cellPointIds = input->GetCell(triangleCellId)->GetPointIds();
    vtkIdType id0 = cellPointIds->GetId(0);
    aList->InsertUniqueId(id0);
    vtkIdType id1 = cellPointIds->GetId(1);
    vtkIdType id2 = cellPointIds->GetId(2);
    aList->InsertUniqueId(id1);
    aList->InsertUniqueId(id2);
  }

  //calcualte normals for the surface at every point
  vtkPolyDataNormals *normalsFilter = vtkPolyDataNormals::New();
  normalsFilter->SetInput(temp);
  normalsFilter->SetAutoOrientNormals(1);
  normalsFilter->SetFlipNormals(0);
  normalsFilter->SetConsistency(1);
  normalsFilter->SplittingOff();
  normalsFilter->SetComputePointNormals(1);
  normalsFilter->SetComputeCellNormals(0);
  normalsFilter->Update();
  vtkDataArray* NormalVectorsArray = normalsFilter->GetOutput()->GetPointData()->GetNormals();

  //now we need to calcualte the relevant normals
  Normals->SetNumberOfComponents(3);
  Normals->Allocate(3*aList->GetNumberOfIds());
  Normals->SetName("BoundaryNormals");
  Normals->SetNumberOfTuples(aList->GetNumberOfIds());
  for(int i=0;i<aList->GetNumberOfIds();i++)
  {   
    double p[3];
    double v[3];
    input->GetPoints()->GetPoint(aList->GetId(i),p);
    vtkIdType mappedId = Locator->FindClosestPoint(p);
    if(mappedId<0) cout << "Errors ...\n";
    NormalVectorsArray->GetTuple(mappedId,v);
    Normals->SetTuple(i,v);
  } 
  geometryFilter->Delete();
  aList->Delete();
  normalsFilter->Delete();
  Locator->Delete();
  return;
}

// The sign convention is so that a positive prescribed displacement 
// will act along the direction of the normal,
// so it will streth the material. 
void vtkvmtkFEBioWriter::PrintPrescribedBoundaryConditions(ofstream& outfile,int numberOfTriangles,
    int type,vtkIdTypeArray *triangleCellIdArray,
    vtkIntArray *boundaryDataArray,vtkUnstructuredGrid *input,vtkDoubleArray *Normals)
{
  this->PrintIndentions(2,outfile);
  outfile << "<prescribe>\n";
  vtkIdList *aList = vtkIdList::New();
  for (int i=0; i<numberOfTriangles; i++)
  {
    vtkIdType triangleCellId = triangleCellIdArray->GetValue(i);
    if (boundaryDataArray->GetValue(triangleCellId) != type)
    {
      continue;
    }
    vtkIdList* cellPointIds = input->GetCell(triangleCellId)->GetPointIds();
    vtkIdType id0 = cellPointIds->GetId(0);
    aList->InsertUniqueId(id0);
    vtkIdType id1 = cellPointIds->GetId(1);
    vtkIdType id2 = cellPointIds->GetId(2);
    aList->InsertUniqueId(id1);
    aList->InsertUniqueId(id2);
  }
     
  for (int i=0; i<aList->GetNumberOfIds(); i++)
  {
    double dx=0.,dy=0.,dz=0.;
    double v[3];
    Normals->GetTuple(i,v);
    vtkMath::Normalize(v);
    // positive and negative sign represents direction
    dx = 0; // dx = v[0]*this->PrescribedDisplacement;
    dy = 0; // dy = v[1]*this->PrescribedDisplacement;
    dz = (-1)*this->PrescribedDisplacement;
    this->PrintIndentions(3,outfile);
    outfile << "<node id=\"" << aList->GetId(i)+1 << "\" bc=\"x\" lc=\"1\">" << dx << "</node>\n";
    this->PrintIndentions(3,outfile);
    outfile << "<node id=\"" << aList->GetId(i)+1 << "\" bc=\"y\" lc=\"1\">" << dy << "</node>\n";
    this->PrintIndentions(3,outfile);
    outfile << "<node id=\"" << aList->GetId(i)+1 << "\" bc=\"z\" lc=\"1\">" << dz << "</node>\n";
  }
  this->PrintIndentions(2,outfile);
  outfile << "</prescribe>\n";
  aList->Delete();
  return;
}

void vtkvmtkFEBioWriter::PrintOneNode(ofstream& outfile,int id,double x,double y,double z)
{
  this->PrintIndentions(3,outfile);
  outfile << "<node id=\"" << id+1 << "\">" 
      <<setiosflags(ios::fixed)<<setprecision(10)<< x << "," 
      <<setiosflags(ios::fixed)<<setprecision(10)<< y << "," 
      <<setiosflags(ios::fixed)<<setprecision(10)<< z << "</node>\n";
  return;
}

void vtkvmtkFEBioWriter::PrintOneTet4(ofstream& outfile,int id,int mat,int i,int j,int k,int l)
{
  this->PrintIndentions(3,outfile);
  // FEBio format
  outfile << "<tet4 id=\"" << id+1 << "\" mat=\"" << mat << "\">" << i+1 <<"," << j+1 << "," << k+1 << "," << l+1 << "</tet4>\n";
  // Tetview format
  // outfile << "<tet4 id=\"" << id+1  << "\">" << i+1 <<"," << j+1 << "," << k+1 << "," << l+1 << "\" mat=\"" << mat <<"</tet4>\n";
  return;
}


void vtkvmtkFEBioWriter::WriteData()
{
  vtkUnstructuredGrid *input= vtkUnstructuredGrid::SafeDownCast(this->GetInput());

  if (!this->FileName)
  {
    vtkErrorMacro(<<"FileName not set.");
    return;
  }
        
  ofstream out (this->FileName);

  if (!out.good())
  {
    vtkErrorMacro(<<"Could not open file for writing.");
    return;
  }
  
  input->BuildLinks();
  int numberOfPoints = input->GetNumberOfPoints();
  int i = 0;

  this->PrintHeader(out);
  this->PrintControlSection(out);

  vtkIntArray* boundaryDataArray = vtkIntArray::New();
  vtkIntArray* volumeDataArray = vtkIntArray::New();
  this->CheckArrayData(input,boundaryDataArray,volumeDataArray);

  // write material information
  this->PrintIndentions(1,out);
  out << "<Material>\n";
  this->PrintElasticMaterial(out,1,"neo-Hookean",928e-9,10e-3,0.499);
  this->PrintElasticMaterial(out,2,"neo-Hookean",1050e-9,80e-3,0.49);
  this->PrintElasticMaterial(out,3,"neo-Hookean",1030e-9,10e-3,0.49);
  this->PrintElasticMaterial(out,4,"neo-Hookean",1060e-9,300e-3,0.49);
  this->PrintElasticMaterial(out,5,"neo-Hookean",1090e-9,300e-3,0.49);
  this->PrintElasticMaterial(out,6,"neo-Hookean",1040e-9,30e-3,0.49);
  this->PrintIndentions(1,out);
  out << "</Material>\n";

  double point[3];
  this->PrintIndentions(1,out);
  out << "<Geometry>\n";
  // write node information
  this->PrintIndentions(2,out);
  out << "<Nodes>\n";
  for (i=0; i<numberOfPoints; i++)
  {
    input->GetPoint(i,point);
    this->PrintOneNode(out,i,point[0]/10,point[1]/10,point[2]/10);
  }
  this->PrintIndentions(2,out);
  out << "</Nodes>\n";

  // write element information
  this->PrintIndentions(2,out);
  out << "<Elements>\n"; 
  vtkIdTypeArray* tetraCellIdArray = vtkIdTypeArray::New();
  input->GetIdsOfCellsOfType(VTK_TETRA,tetraCellIdArray); 
  int numberOfTetras = tetraCellIdArray->GetNumberOfTuples();
  for (i=0; i<numberOfTetras; i++)
  {
    vtkIdType tetraCellId = tetraCellIdArray->GetValue(i); 
    vtkIdList* cellPointIds = input->GetCell(tetraCellId)->GetPointIds();
    this->PrintOneTet4(out,i,volumeDataArray->GetValue(tetraCellId),cellPointIds->GetId(0),cellPointIds->GetId(1),cellPointIds->GetId(2),cellPointIds->GetId(3));
  }
  this->PrintIndentions(2,out);
  out << "</Elements>\n"; 
  this->PrintIndentions(1,out);
  out << "</Geometry>\n";
 
  vtkIdTypeArray* triangleCellIdArray = vtkIdTypeArray::New();
  input->GetIdsOfCellsOfType(VTK_TRIANGLE,triangleCellIdArray); // triangleCellIdArray
  int numberOfTriangles = triangleCellIdArray->GetNumberOfTuples();
  vtkDoubleArray *Normals = vtkDoubleArray::New();
  this->CalculateNormals(numberOfTriangles,2,triangleCellIdArray,boundaryDataArray,
                         input,Normals);
  
  // write boundary conditions
  this->PrintIndentions(1,out);
  out << "<Boundary>\n"; 
  this->PrintFixBoundaryConditions(out,numberOfTriangles,
      6,triangleCellIdArray,boundaryDataArray,input); // "6" is ID number (bottom)
  this->PrintPrescribedBoundaryConditions(out,numberOfTriangles,
      7,triangleCellIdArray,boundaryDataArray,input,Normals); // "7" is ID number (transducer)
  this->PrintIndentions(1,out);
  out << "</Boundary>\n";

  this->PrintLoadData(out);
  this->PrintTail(out);
  
  if(boundaryDataArray != NULL)
  {
    boundaryDataArray->Delete();
  }
  if(volumeDataArray != NULL) volumeDataArray->Delete();
  triangleCellIdArray->Delete();
  // Normals->Delete();
  tetraCellIdArray->Delete();
  out.close();
}

void vtkvmtkFEBioWriter::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkUnstructuredGridWriter::PrintSelf(os,indent);
}
