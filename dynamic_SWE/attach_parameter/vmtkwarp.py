#!/usr/bin/env python

## Program:   VMTK
## Module:    $RCSfile: vmtkwarp.py,v $
## Language:  Python
## Date:      $Date: 2016/08/19 09:49:59 $
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

vmtkwarp = 'vmtkimagedatawrite'

class vmtkimagedatawrite(pypes.pypeScript):

    def __init__(self):

        pypes.pypeScript.__init__(self)
        
        self.Surface = None 
        self.dSurface = None       
        self.SetScriptName('vmtkimagedatawrite')
        self.SetScriptDoc('convert unstructruredgrid to imagedata through vtkprobefilter')
        self.SetInputMembers([
            ['Surface','i','vtkUnstructuredGrid',1,'','the input mesh','vmtkmeshreader']
            ])
        # self.SetOutputMembers([
        #   ['dSurface','d','vtkUnstructuredGrid',1,'','the output mesh','vmtkmeshwriter']
        #   ])

    def Execute(self):

        if self.Surface == None:
            self.PrintError('Error: No Surface.')

        self.PrintLog('Computing convertion')
        convertion = vtkvmtk.vtkvmtkwarp()
        convertion.SetInput(self.Surface)
        convertion.Setwarpdataname('ARF')
        convertion.Update()
      
if __name__=='__main__':
    main = pypes.pypeMain()
    main.Arguments = sys.argv
    main.Execute()
