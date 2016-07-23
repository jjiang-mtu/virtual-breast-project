#!/bin/bash

# Run_sims.sh LIST_FILE FOLDERNAME OUTPUTFILENAME

xml=SimList.xml # XML file containing list of simulations and their adjusted parameters

outfile=Sim # base name of output file from simulation.

mkdir -p ./SimulationResults # creaters folder for results

# loop runs simulations listed in XML file in the range.

for i in `seq 1 10`;
do

   python upd_sim_bp.py -i $i -lsfile $xml -infile benign.feb -outfile $outfile
   febio.lnx64 -i CurrentSim.feb -silent
   mv ./$outfile$i.txt ./SimulationResults/$outfile$i.txt

done
exit


