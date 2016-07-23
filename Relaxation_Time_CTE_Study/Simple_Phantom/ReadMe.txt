This folder contains all the necessary files for producing the results found in figures 5 and 6 in "Virtual Breast Quasi-Static Elastography(VBQE): A Case Study in Contrast Transfer Efficiency of Viscoelastic Imaging" (David Rosen et al 2016, Ultrasonic Imaging).  In order to run the simulations, you will need a 64 bit linux operating system (Ubuntu or Redhat), FEBio version 1.5 or higher, and python 2.7.7 or higher.  The most straightforward way to run the simulations is to copy the folder directly to your desired location without moving or deleting any of its files and running Run_sims.sh from a terminal with its directory set to the folder location. Run_sims.sh will create a folder within the directory where it will place the simulation results as text files in the new folder. Simulations are ran in a loop (up to 11 in a session) and each simulation can be lengthy, so it is adviced that you leave yourself adequate computational resources to do your other work or run the session over night or on a weekend. Also, since Run_sims.sh is a relatively simple script, it is not likely to be robbust to the subtles of your system (e.g. line 17 may need adjustment depending on how your system calls FEBio from the terminal window).  Small adjustements can be made to the file to suite the needs of your system.

 In order to use strain and relaxation time image workflow used in the previously mentioned publication, you will need Matlab version 2015 available on your system (older or newer versions may work, but there may be subtle differences between version which disrupt the workflow). From an open matlab window, set the current directory to this folder.  Move the file named run.txt into the simulation results subfolder you wish to process and run the matlab script named process_simulations.m
The results are processed in a loop and processing may take 30min-1hr.


Important Simulation Files
-------------------------------------

  Run_sims.sh-- Bash script that automates simulation workflow. Run from terminal window set to this directory.

  upd_sim_bp.py-- python script that adjusts model parameters (i.e. relaxation time contrast) of a template .feb file (benign.feb) for the current simulation. Outputs/overwrites file named CurrentSim.feb as the template file with adjusted parameters. Note: python version 2.7.7 or higher is required to run this file.

  SimList_E*.xml-- XML files containing parameters used by upd_sim_bp.py for template .feb file. The three different files correspond to the three elastic contrast ratios 

  D10_cube.feb-- template .feb file. File contains the finite element model for a simple 10mm diameter spherical lesion embedded in cubic phantom. template files for 5mm and 10 mm diameter files can be found in the folder named "Other Models".  To run these other template files, adjusted the template name specified as a variable in Run_sims.sh.

Generated Simulation Files
--------------------------------------------
  CurrentSim.feb-- See above
  Sim**.txt--results file for given simulation. Note: the base name (i.e. Sim) may be adjusted.  remember to also adjust it in run.txt if you intend to use the available image creation workflow.
If the base name is changed, all instances down the workflow will be changed as well (e.g. Sim**_dat.mat becomes (new basename)**_dat.mat
 E**_Results(folder)-- simulation results folder.

  
Important Relaxation Time Image Creation Files
-----------------------------------------------

process_simulations.m--Matlab script that loops through folders in current directory and creates strain images and relaxation time images from simulation results.

relax_fit_func (folder)--Contains the Matlab functions used by process_simulations.m. process_simulations.m will set this folder to the Matlab pathway automatically when ran.

barout.txt--file containing node numbers and locations. Needed to process nodel data into image results.

run.txt--File used as a flag for process_simulations.m indicating that the folder this file resides in contains the simulation results. As process_simulations.m runs through its while loop, it searches for this file win order to deside whether or not to attempt processing on files in the subfolder.  The text in run.txt is the base name of the simulation results.


Generated Processing Files
--------------------------------------------

Sim**_dat.mat--nodal data imported and saved as a .mat file.

strain_Sim**.mat--time resolved strain data 

T_Sim**.mat--Relaxation time image data and contrast calculations.
 
