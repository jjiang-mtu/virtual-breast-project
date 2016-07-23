#!/usr/bin/env python

## Program:   VMTK
## Module:    $RCSfile: vmtkappend.py,v $
## Language:  Python
## Date:      $Date: 2016/07/19 09:49:59 $
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

vmtkappend = 'vmtkAssignZone'

class vmtkAssignZone(pypes.pypeScript):

    def __init__(self):      

        pypes.pypeScript.__init__(self)
        self.surface = None
        self.referencesurface = None
        self.dSurface = None
        self.SetScriptName('Smoothing polydata surface')
        self.SetScriptDoc('write surface for tetgen processing')

        self.SetInputMembers([
            ['surface','i','vtkPolyData',1,'','the input surface','vmtksurfacereader'],
            ['referencesurface','o','vtkPolyData',1,'','the input surface','vmtksurfacereader'],
            ])
        self.SetOutputMembers([
            ['dSurface','d','vtkPolyData',1,'','the output mesh','vmtksurfacewriter']
            ])

    def TestGrid(self):

        appendFilter = vtkvmtk.vtkvmtkappend()
        appendFilter.SetInput(self.surface)
        appendFilter.Setreferencesurface(self.referencesurface)      
        appendFilter.Update()
        self.dSurface = appendFilter.GetOutput()
        
    def Execute(self):

        self.TestGrid()
        
if __name__=='__main__':
    main = pypes.pypeMain()
    main.Arguments = sys.argv
    main.Execute()
