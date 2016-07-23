#!/bin/bash

# Run_sims.sh LIST_FILE FOLDERNAME OUTPUTFILENAME

template=D10_cube.feb

xml1=SimList_E01.xml
xml2=SimList_E05.xml
xml3=SimList_E10.xml

fold1=E01_Results
fold2=E05_Results
fold3=E10_Results

outfile=Sim

mkdir -p ./$fold1
mkdir -p ./$fold2
mkdir -p ./$fold3

for i in `seq 1 11`;
do

   python update_sim.py -i $i -lsfile $xml1 -outfile $outfile -sim_file $template
   febio.lnx64 -i CurrentSim.feb
   mv ./$outfile??.txt ./$fold1

   python update_sim.py -i $i -lsfile $xml2 -outfile $outfile -sim_file $template
   febio.lnx64 -i CurrentSim.feb
   mv ./$outfile??.txt ./$fold2

   python update_sim.py -i $i -lsfile $xml3 -outfile $outfile -sim_file $template
   febio.lnx64 -i CurrentSim.feb
   mv ./$outfile??.txt ./$fold3

done
exit
