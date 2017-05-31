#!/usr/bin/env python

## Program:   VMTK
## Module:    $RCSfile: vmtkappendrandom.py,v $
## Language:  Python
## Date:      $Date: 2016/08/19 09:49:59 $
## Version:   $Revision: 1.6 $

##      Copyright (c) Jingfeng Jiang, Yu Wang. All rights reserved.
##      See LICENCE file for details.

##      This software is distributed WITHOUT ANY WARRANTY; without even 
##      the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
##      PURPOSE.  See the above copyright notices for more information.

import vtk
import sys
import vtkvmtk
import vmtkscripts
import pypes

vmtkappendrandom = 'vmtkAssignZone'

class vmtkAssignZone(pypes.pypeScript):

    def __init__(self):      

        pypes.pypeScript.__init__(self)
        self.surface = None
        self.referencesurface1 = None
        self.dSurface = None
        self.SetScriptName('Smoothing polydata surface')
        self.SetScriptDoc('write surface for tetgen processing')
        self.SetInputMembers([
            ['surface','i','vtkPolyData',1,'','the input surface','vmtksurfacereader'],
            ['referencesurface1','o','vtkPolyData',1,'','the input surface','vmtksurfacereader'],
            ])
        self.SetOutputMembers([
            ['dSurface','d','vtkPolyData',1,'','the output mesh','vmtksurfacewriter']
            ])

    def TestGrid(self):

        appendFilter = vtkvmtk.vtkvmtkappendrandom()       
        appendFilter.SetInput(self.surface)
        appendFilter.Setreferencesurface1(self.referencesurface1)
        appendFilter.Update()
        self.dSurface = appendFilter.GetOutput()        
        
    def Execute(self):

        self.TestGrid()
        
if __name__=='__main__':
    main = pypes.pypeMain()
    main.Arguments = sys.argv
    main.Execute()
