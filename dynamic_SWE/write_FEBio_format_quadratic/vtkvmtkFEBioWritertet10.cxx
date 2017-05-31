/*=========================================================================
                                                                                                                                    
Program:   VMTK
Module:    $RCSfile: vtkvmtkfebiowytet10.cxx,v $
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

#include "vtkDoubleArray.h"
#include "vtkIntArray.h"
#include "vtkvmtkFEBioWritertet10.h"
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
#include "vtkSphereSource.h"
#include "vtkSelectEnclosedPoints.h"
#include "vtkCellLocator.h"
#include "vtkGenericCell.h"
#include "vtkPolyData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"

vtkCxxRevisionMacro(vtkvmtkFEBioWritertet10, "$Revision: 1.6 $");
vtkStandardNewMacro(vtkvmtkFEBioWritertet10);

vtkvmtkFEBioWritertet10::vtkvmtkFEBioWritertet10()
{
  //this->BoundaryDataArrayName = NULL;
  //this->BoundaryDataIdOffset = 0;
  this->VolumeDataArrayName = NULL;
  this->PrescribedDisplacement = 3;
}

vtkvmtkFEBioWritertet10::~vtkvmtkFEBioWritertet10()
{
  if (this->VolumeDataArrayName)
  {
    delete[] this->VolumeDataArrayName;
    this->VolumeDataArrayName = NULL;
  }
}

bool vtkvmtkFEBioWritertet10::CheckArrayData(vtkUnstructuredGrid *input,
      vtkIntArray *volumeDataArray) {
  if (this->VolumeDataArrayName)
  {
    if (input->GetCellData()->GetArray(this->VolumeDataArrayName))
    {
      volumeDataArray->DeepCopy(input->GetCellData()->GetArray(this->VolumeDataArrayName));
    } else {
      vtkErrorMacro(<<"Volume DataArray with name specified does not exist");
      volumeDataArray->Delete();
      // boundaryDataArray = NULL;
      volumeDataArray = NULL;
      return false;
    }
  } else {
    vtkErrorMacro(<<"Volume DataArray is not specified.\n");
    volumeDataArray->Delete();
    //boundaryDataArray = NULL;
    volumeDataArray = NULL;
    return false;
  }
  return true;     
}

void vtkvmtkFEBioWritertet10::PrintHeader(ofstream& outfile)
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

void vtkvmtkFEBioWritertet10::PrintTail(ofstream& outfile)
{
  //this->PrintIndentions(1,outfile);
  outfile << "</febio_spec>\n";
  return;
}

void vtkvmtkFEBioWritertet10::PrintIndentions(int level,ofstream& outfile)
{
  for(int j=0;j<level;j++)
    outfile << "\t";
  return;
}

void vtkvmtkFEBioWritertet10::PrintElasticMaterial(ofstream& outfile,int mid,char *name,double rho,double E,double mu)
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

void vtkvmtkFEBioWritertet10::PrintFixBoundaryConditions(ofstream& outfile,vtkUnstructuredGrid *input)
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
    if (fabs(point[2]+35)<1e-12)   //        if (fabs(point[2]+10.)<1e-12)
    {
      vtkIdType downId = Locator->FindClosestPoint(point);
      this->PrintIndentions(3,outfile);
      outfile << "<node id=\"" << downId+1 << "\" bc=\"xyz\"> </node>\n";
    }
  }

  this->PrintIndentions(2,outfile);
  outfile << "</fix>\n";

  downList->Delete();
  return;
}

void vtkvmtkFEBioWritertet10::PrintFixBoundaryConditions_x(ofstream& outfile,vtkUnstructuredGrid *input)
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
    if (fabs(point[2]-50)<1e-12)   //        if (fabs(point[2]+10.)<1e-12)
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

void vtkvmtkFEBioWritertet10::PrintFixBoundaryConditions_y(ofstream& outfile,vtkUnstructuredGrid *input)
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
    if (fabs(point[2]+30)<1e-12)
    {
      vtkIdType downId = Locator->FindClosestPoint(point);
      this->PrintIndentions(3,outfile);
      outfile << "<node id=\"" << downId+1 << "\" bc=\"xyz\"> </node>\n";
    }
  }

  this->PrintIndentions(2,outfile);
  outfile << "</fix>\n";

  downList->Delete();
  return;
}

void vtkvmtkFEBioWritertet10::PrintFix_forceBoundaryConditions(ofstream& outfile,vtkUnstructuredGrid *input)
{
  int numberOfPoints = input->GetNumberOfPoints();
  double point[3];

  vtkPointLocator *Locator = vtkPointLocator::New();
  Locator->SetDataSet(input);
  Locator->BuildLocator();

  vtkIdList *downList = vtkIdList::New();
  this->PrintIndentions(2,outfile);
  outfile << "<prescribe>\n";
  int nnn=0;
  for (int i=0; i<numberOfPoints; i++)
  {
    input->GetPoint(i,point);
    if (fabs(point[2]-13)<1e-12 && point[0]<=2.5 && point[0]>=-2.5 && point[1]<=17 && point[1]>=-17)
    {
      vtkIdType downId = Locator->FindClosestPoint(point);
      this->PrintIndentions(3,outfile);
      double force_z=-5.2;
      outfile << "<node id=\"" << downId+1 << "\" bc=\"z\" lc=\"1\">" << force_z << "</node>\n";
    }
  }

  this->PrintIndentions(2,outfile);
  outfile << "</prescribe>\n";

  downList->Delete();
  return;
}

void vtkvmtkFEBioWritertet10::PrintControlSection(ofstream& outfile)
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

void vtkvmtkFEBioWritertet10::PrintLoadData(ofstream& outfile)
{
  this->PrintIndentions(1,outfile);
  outfile << "<LoadData>\n";
  this->PrintIndentions(2,outfile);
  outfile <<	"<loadcurve id=\"1\" type=\"smooth\" extend=\"constant\">\n";

  double parameter[5]; 
  ifstream infile;
  infile.open("force_data.txt");
  char tempStr[1028];
  for (int i=0;i<600;i++)
  {
    infile.getline(tempStr,1028,' ');
    parameter[0] = atof(tempStr);

    infile.getline(tempStr,1028,' ');
    parameter[1] = atof(tempStr);

    infile.getline(tempStr,1028,' ');
    parameter[2] = atof(tempStr);

    infile.getline(tempStr,1028,' ');
    parameter[3] = atof(tempStr);

    infile.getline(tempStr,1028);
    parameter[4] = atof(tempStr);

    this->PrintIndentions(3,outfile);
    outfile << "<loadpoint>"<<setiosflags(ios::fixed)<<setprecision(10)<<parameter[3]<<","
	<<setiosflags(ios::fixed)<<setprecision(10)<<parameter[4]<<"</loadpoint>\n";
  }

  this->PrintIndentions(2,outfile);
  outfile <<	"</loadcurve>\n";
  this->PrintIndentions(1,outfile);
  outfile << "</LoadData>\n";
  cout<<parameter[0]<<endl;
  cout<<parameter[1]<<endl;
  cout<<parameter[2]<<endl;
  cout<<parameter[3]<<endl;
  cout<<parameter[4]<<endl;
}

void vtkvmtkFEBioWritertet10::PrintOutput(ofstream& outfile,vtkUnstructuredGrid *input)
{
  this->PrintIndentions(1,outfile);
  outfile << "<Output>\n";
  this->PrintIndentions(2,outfile);
  outfile <<	"<logfile file = \"displacement_test.txt\">\n";
  this->PrintIndentions(3,outfile);
  outfile << "<node_data data=\"x;y;z;ux;uy;uz\" name = \"displacement\">\n";
  this->PrintIndentions(5,outfile);

  int numberOfPoints = input->GetNumberOfPoints();
  double point[3];

  vtkPointLocator *Locator = vtkPointLocator::New();
  Locator->SetDataSet(input);
  Locator->BuildLocator();

  vtkIdList *downList = vtkIdList::New();
  int m=1;
  for (int i=0; i<numberOfPoints; i++)
  {
    input->GetPoint(i,point);

    if ( point[0]<=4 && point[0]>=-4 && point[1] <= 16 && point[1] >= -4 && point[2]<=15 && point[2]>=-15)   //        if (fabs(point[2]+10.)<1e-12)
    {
      if (m<=8)
      {
        vtkIdType downId = Locator->FindClosestPoint(point);
        outfile << downId+1 << ",";
        m=m+1;
      } else {
        vtkIdType downId = Locator->FindClosestPoint(point);
        outfile << downId+1 << ",\n";
        this->PrintIndentions(5,outfile);
        m=1;
      }
    }
  }

  this->PrintIndentions(3,outfile);
  outfile <<	"</node_data>\n";
  this->PrintIndentions(2,outfile);
  outfile <<	"</logfile>\n";
  this->PrintIndentions(1,outfile);
  outfile << "</Output>\n";
}

void vtkvmtkFEBioWritertet10::PrintPrescribedBoundaryConditions(ofstream& outfile,vtkUnstructuredGrid *input)
{
  this->PrintIndentions(2,outfile);
  outfile << "<force>\n";
  vtkSphereSource *sphere = vtkSphereSource::New();
  sphere->SetCenter(0, 0, 0);
  sphere->SetRadius(2);
  sphere->Update();

  vtkSelectEnclosedPoints *selectEnclosedPoints = vtkSelectEnclosedPoints::New();
  selectEnclosedPoints->Initialize(sphere->GetOutput());

  vtkIdTypeArray* tetraCellIdArray_1 = vtkIdTypeArray::New();
  input->GetIdsOfCellsOfType(VTK_TETRA,tetraCellIdArray_1); 
  int numberOftetra = tetraCellIdArray_1->GetNumberOfTuples();
  double force_z;
  force_z=10e-3;
  vtkIdType i, temptype;
  vtkIdType temp;
  vtkCellLocator *locator;
  vtkIdList *cellPointIds;
  cellPointIds = vtkIdList::New();
  int count=0;
  int Id_store[600];

  for (i=0; i<numberOftetra; i++)
  {
    temp = tetraCellIdArray_1->GetValue(i);
    cellPointIds = input->GetCell(temp)->GetPointIds();
      
    double pts0[3],pts1[3],pts2[3],pts3[3],center[3];
    input->GetPoints()->GetPoint(cellPointIds->GetId(0),pts0);
    input->GetPoints()->GetPoint(cellPointIds->GetId(1),pts1);
    input->GetPoints()->GetPoint(cellPointIds->GetId(2),pts2);
    input->GetPoints()->GetPoint(cellPointIds->GetId(3),pts3);

    center[0] = (pts0[0] + pts1[0] + pts2[0] + pts3[0])/4.;
    center[1] = (pts0[1] + pts1[1] + pts2[1] + pts3[1])/4.;
    center[2] = (pts0[2] + pts1[2] + pts2[2] + pts3[2])/4.;

    if(selectEnclosedPoints->IsInsideSurface(center))
    {
      if (count==0)
      {
        Id_store[count]=cellPointIds->GetId(0);
        count=count+1;
        Id_store[count]=cellPointIds->GetId(1);
        count=count+1;
        Id_store[count]=cellPointIds->GetId(2);
        count=count+1;
        Id_store[count]=cellPointIds->GetId(3);
        count=count+1;

        this->PrintIndentions(3,outfile);
        outfile << "<node id=\"" << cellPointIds->GetId(0)+1 << "\" bc=\"z\" lc=\"1\">" << force_z << "</node>\n";
        this->PrintIndentions(3,outfile);
        outfile << "<node id=\"" << cellPointIds->GetId(1)+1 << "\" bc=\"z\" lc=\"1\">" << force_z << "</node>\n";
        this->PrintIndentions(3,outfile);
        outfile << "<node id=\"" << cellPointIds->GetId(2)+1 << "\" bc=\"z\" lc=\"1\">" << force_z << "</node>\n";
        this->PrintIndentions(3,outfile);
        outfile << "<node id=\"" << cellPointIds->GetId(3)+1 << "\" bc=\"z\" lc=\"1\">" << force_z << "</node>\n";
      }

      for (int j=0; j<count; j++)
      {
        if (Id_store[j] == cellPointIds->GetId(0))
          break;
          if (j == (count-1))
          {
	    this->PrintIndentions(3,outfile);
            outfile << "<node id=\"" << cellPointIds->GetId(0)+1 << "\" bc=\"z\" lc=\"1\">" << force_z << "</node>\n";

	    Id_store[count]=cellPointIds->GetId(0);
	    count=count+1;
          }
      }

      for (int j=0; j<count; j++)
      {
        if (Id_store[j] == cellPointIds->GetId(1))
	  break;
        if (j == (count-1))
        {
	  this->PrintIndentions(3,outfile);
          outfile << "<node id=\"" << cellPointIds->GetId(1)+1 << "\" bc=\"z\" lc=\"1\">" << force_z << "</node>\n";

	  Id_store[count]=cellPointIds->GetId(1);
	  count=count+1;
        }
      }

      for (int j=0; j<count; j++)
      {
        if (Id_store[j] == cellPointIds->GetId(2))
	  break;
        if (j == (count-1))
        {
	  this->PrintIndentions(3,outfile);
          outfile << "<node id=\"" << cellPointIds->GetId(2)+1 << "\" bc=\"z\" lc=\"1\">" << force_z << "</node>\n";

	  Id_store[count]=cellPointIds->GetId(2);
	  count=count+1;
        }
      }

      for (int j=0; j<count; j++)
      {
        if (Id_store[j] == cellPointIds->GetId(3))
	  break;
        if (j == (count-1))
        {
	  this->PrintIndentions(3,outfile);
          outfile << "<node id=\"" << cellPointIds->GetId(3)+1 << "\" bc=\"z\" lc=\"1\">" << force_z << "</node>\n";

	  Id_store[count]=cellPointIds->GetId(3);
	  count=count+1;
        }
      }
    }
  }

  this->PrintIndentions(2,outfile);
  outfile << "</force>\n";
  return;
}

void vtkvmtkFEBioWritertet10::PrintPrescribedBoundaryConditions_z(ofstream& outfile,vtkUnstructuredGrid *input)
{
  int numberOfPoints = input->GetNumberOfPoints();
  double point[3];

  double force_z;
  force_z=-0.05;

  vtkPointLocator *Locator = vtkPointLocator::New();
  Locator->SetDataSet(input);
  Locator->BuildLocator();

  vtkIdList *downList = vtkIdList::New();
  this->PrintIndentions(2,outfile);
  outfile << "<force>\n";

  for (int i=0; i<numberOfPoints; i++)
  {
    input->GetPoint(i,point);
    if (fabs(point[2]-5)<1e-12 && point[0]<=2 && point[0]>=-2 && point[1]<=2 && point[1]>=-2)
    {
      vtkIdType downId = Locator->FindClosestPoint(point);
      this->PrintIndentions(3,outfile);
      outfile << "<node id=\"" << downId+1 << "\" bc=\"z\" lc=\"1\">" << force_z << "</node>\n";
    }
  }

  this->PrintIndentions(2,outfile);
  outfile << "</force>\n";

  downList->Delete();
  return;
}

void vtkvmtkFEBioWritertet10::PrintOneNode(ofstream& outfile,int id,double x,double y,double z)
{
  this->PrintIndentions(3,outfile);
  outfile << "<node id=\"" << id+1 << "\">" <<setiosflags(ios::fixed)<<setprecision(10)<< x <<"," 
	<<setiosflags(ios::fixed)<<setprecision(10)<< y << "," 
	<<setiosflags(ios::fixed)<<setprecision(10)<< z  << "</node>\n";
  return;
}

void vtkvmtkFEBioWritertet10::PrintOneTet4(ofstream& outfile,int id,int mat,int i,int j,int k,int l, 
	int l1, int l2, int l3, int l4, int l5, int l6) {
  this->PrintIndentions(3,outfile);
  outfile << "<tet10 id=\"" << id+1 << "\" mat=\"" << mat << "\">" << i+1 <<"," 
	<< j+1 << "," << k+1 << "," << l+1 << "," << l1+1 << "," 
	<< l2+1 <<  "," << l3+1 <<  "," << l4+1 << "," << l5+1 << "," << l6+1 
	<<"</tet10>\n";  // ID: i+1
  return;
}


void vtkvmtkFEBioWritertet10::WriteData()
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
  cout<< input->GetNumberOfPoints() <<endl;
  //int numberOfCells = input->GetNumberOfCells();
  int i = 0;

  this->PrintHeader(out);
  this->PrintControlSection(out);

  //vtkIntArray* boundaryDataArray = vtkIntArray::New();
  vtkIntArray* volumeDataArray = vtkIntArray::New();
  this->CheckArrayData(input,volumeDataArray); // just for setting array

  this->PrintIndentions(1,out);
  out << "<Material>\n";
  this->PrintElasticMaterial(out,1,"neo-Hookean",1.04e-6,0.03,0.49);
  //this->PrintElasticMaterial(out,2,"neo-Hookean",1.06e-6,0.3,0.49);
  this->PrintIndentions(1,out);
  out << "</Material>\n";

  double point[3];
  this->PrintIndentions(1,out);
  out << "<Geometry>\n";
  //write out all node information
  this->PrintIndentions(2,out);
  out << "<Nodes>\n";
 
  for (i=0; i<numberOfPoints; i++)
  {
    input->GetPoint(i,point);
    this->PrintOneNode(out,i,point[0]/10,point[1]/10,point[2]/10);
  }

  this->PrintIndentions(2,out);
  out << "</Nodes>\n";
  //write out all element information
  this->PrintIndentions(2,out);

  out << "<Elements>\n"; 

  vtkIdTypeArray* tetraCellIdArray = vtkIdTypeArray::New();
  input->GetIdsOfCellsOfType( VTK_QUADRATIC_TETRA ,tetraCellIdArray); 
  int numberOfTetras = tetraCellIdArray->GetNumberOfTuples();
  cout<< tetraCellIdArray->GetNumberOfTuples() <<endl;
  cout << "\nnumber of numberOfTetras\n" << numberOfTetras<<endl;

  for (i=0; i<numberOfTetras; i++)
  {
    vtkIdType tetraCellId = tetraCellIdArray->GetValue(i); 
    vtkIdList* cellPointIds = input->GetCell(tetraCellId)->GetPointIds();
    this->PrintOneTet4(out,i,volumeDataArray->GetValue(tetraCellId),cellPointIds->GetId(0),
	cellPointIds->GetId(1),cellPointIds->GetId(2),cellPointIds->GetId(3),cellPointIds->GetId(4),
	cellPointIds->GetId(5),cellPointIds->GetId(6),cellPointIds->GetId(7),cellPointIds->GetId(8),
	cellPointIds->GetId(9));
  }

  this->PrintIndentions(2,out);
  out << "</Elements>\n"; 

  this->PrintIndentions(1,out);
  out << "</Geometry>\n";

  this->PrintIndentions(1,out);
  out << "<Boundary>\n"; 
  //this->PrintFixBoundaryConditions(out,input);
  //this->PrintFixBoundaryConditions_x(out,input);
  this->PrintFixBoundaryConditions_y(out,input);
  // this->PrintPrescribedBoundaryConditions(out,input);
  this->PrintPrescribedBoundaryConditions_z(out,input);
  // this->PrintFix_forceBoundaryConditions(out,input);
  this->PrintIndentions(1,out);
  out << "</Boundary>\n";
  cout<<"boundarys"<<endl;

  // this->PrintLoadData(out);
  this->PrintOutput(out,input);
  this->PrintTail(out);

  if(volumeDataArray!=NULL) volumeDataArray->Delete();
  //triangleCellIdArray->Delete();
  //Normals->Delete();
  tetraCellIdArray->Delete();
  out.close();
}

void vtkvmtkFEBioWritertet10::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkUnstructuredGridWriter::PrintSelf(os,indent);
}
