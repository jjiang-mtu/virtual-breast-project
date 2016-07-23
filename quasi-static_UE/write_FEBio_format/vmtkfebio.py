#!/usr/bin/env python

## Program:   VMTK
## Module:    $RCSfile: vmtkfebio.py,v $
## Language:  Python
## Date:      $Date: 2016/07/19 09:49:59 $
## Version:   $Revision: 1.6 $

##      Copyright (c) Jingfeng Jiang, Yu Wang. All rights reserved.
##      See LICENCE file for details.

##      This software is distributed WITHOUT ANY WARRANTY; without even 
##      the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
##      PURPOSE.  See the above copyright notices for more information.

import vtk
import vtkvmtk
import sys

import pypes

vmtkfebio = 'vmtkfebiowrite'

class vmtkfebiowrite(pypes.pypeScript):

    def __init__(self):

        pypes.pypeScript.__init__(self)
        
        self.Surface = None        
        self.SetScriptName('vmtkfebiowrite')
        self.SetScriptDoc('interpolates the point data of a reference surface onto the input surface based on minimum distance criterion')
        self.SetInputMembers([
            ['Surface','i','vtkUnstructuredGrid',1,'','the mesh surface','vmtkmeshreader']
            ])
        self.SetOutputMembers([])
            # ['dSurface','o','vtkUnstructuredGrid',1,'','the output surface','vmtkmeshwriter']

    def Execute(self):

        if self.Surface == None:
            self.PrintError('Error: No Surface.')

        self.PrintLog('Computing projection.')
        surfaceProjection = vtkvmtk.vtkvmtkFEBioWriter()
        surfaceProjection.SetInput(self.Surface)
        surfaceProjection.SetBoundaryDataArrayName('CellEntityIds')  
        surfaceProjection.SetVolumeDataArrayName('VolId')
        surfaceProjection.SetFileName('breast.feb')
        surfaceProjection.Write()
        surfaceProjection.Update()
      
if __name__=='__main__':
    main = pypes.pypeMain()
    main.Arguments = sys.argv
    main.Execute()
