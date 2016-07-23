#!/usr/bin/python

#David Rosen
#Usage: ./update_sim.py -i SIM_NUMBER -lsfile SIMULATION_LIST.xml -outfile BASE_NAME_SIM_OUTFILE

import xml.etree.ElementTree as ET
import math
import argparse
# Collect argument from commandline


parser=argparse.ArgumentParser(description='something')
parser.add_argument('-i',type=int,default='1')
parser.add_argument('-lsfile',type=str,default='Sim_list.xml')
parser.add_argument('-outfile',type=str,default='dis_sim')
parser.add_argument('-infile',type=str,default='cube.feb')
args=parser.parse_args()
#Future corrections: check -i is int and -lsfile is filename



print args.i
print args.lsfile
print args.infile

# Parse febio file
# store element as simulation
simulation=ET.parse(args.infile)

T1back=float(simulation.findtext('Material/material[@name="glandular"]/t1'))

# Parse simulation list file
# store as simlist
simlist=ET.parse(str(args.lsfile))
simprof=simlist.find('Sim[@id="'+str(args.i)+'"]')

T1r=float(simprof.findtext('T1r'))
snum=simprof.findtext('snum')



# Calculate parameters for simulation

# Number of points for each time region
npoints=float(10)
compress_time=float(1) #seconds

#  T1=1st relaxation time
T1sph=T1r*T1back

if T1sph>T1back:
    T1max=T1sph
    T1min=T1back
else:
    T1max=T1back
    T1min=T1sph

# ts=time steps of simulation  
#  currently hardwired for stepsize 0.1 s
# always rounds up 
ts=int(math.ceil(2*3*10*T1max)+10*compress_time)

#iniialize dmax & dedge lists
dmax=[0,0,0]
dedge=[0,0,0]


# Calculate max timestep size
# and duration for various 
if T1min<0.5*T1max:
    dmax[0]=2*T1min/npoints
    dedge[0]=compress_time+2*T1min

    dmax[1]=2*(T1max-T1min)/npoints
    dedge[1]=compress_time+2*T1max
    
else:
    dmax[0]=2*T1max/(npoints*2)
    dedge[0]=compress_time+2*T1min

    dmax[1]=dmax[0]
    dedge[1]=compress_time+2*T1max

dmax[2]=(2*T1max)/npoints
dedge[2]=compress_time+6*T1max

print '\nbackground (T1,E,G1):'
print T1back

print '\nsphere (T1, E, G1):' 
print T1sph


print '\nTime Stepping'
print 'total time'
print ts
print '\ndmax'
print dmax
print '\ndedge'
print dedge

#Set duration of simulation
time_steps=simulation.find('Control/time_steps')
time_steps.text=str(ts)


#Set material properties of simulation
#SphereMaterial=simulation.find('Material/material[@name="sphere"]')
t1=simulation.find('Material/material[@name="tumor"]/t1')
t1.text=str(T1sph)


counter=0
#Set load
for loadpoint in simulation.iterfind('LoadData/loadcurve/loadpoint'):
    s=loadpoint.text
    s2=s.split(',')[0]
    if float(s2)>compress_time:
        loadpoint.text=str(dedge[counter])+','+str(dmax[counter])
        counter=counter+1
        

noddat=simulation.find('Output/logfile/node_data')
noddat.set('file',str(args.outfile)+snum+'.txt')

simulation.write('CurrentSim.feb',encoding='ISO-8859-1',xml_declaration=True)
