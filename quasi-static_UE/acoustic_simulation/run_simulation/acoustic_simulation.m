%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Language:  Matlab
% Date:      Date: 2016/07/21 09:49:59 
% Version:   Revision: R2015b 

%      Copyright (c) Jingfeng Jiang, Yu Wang. All rights reserved.
%      See LICENCE file for details.

%      This software is distributed WITHOUT ANY WARRANTY; without even 
%      the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
%      PURPOSE.  See the above copyright notices for more information.

% Function: show how a linear array B-mode system scans an image
%
% This script assumes that the field_init procedure has been called
% Here the field simulation is performed and the data is stored
% in rf-files; one for each rf-line done. The data must then
% subsequently be processed to yield the image. The data for the
% scatteres are read from the file pht_data.mat, so that the procedure
% can be started again or run for a number of workstations.

% Ver. 1.1: 1/4-98: Procedure xdc_focus_center inserted to use the new
%                    focusing scheme for the Field II program
% Ver. 2.0: 13/8 2007: Parallel version that checks whether the simulation
%                    of a line has been made before, which makes it possible
%                    to run the code in parallel on multiple workstations.

%  Generate the transducer apertures for send and receive
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;
close all;

path(path,'/share/apps/fieldii/3.24');
field_init

f0=6e6;                  %  Transducer center frequency [Hz]
fs=100e6;                %  Sampling frequency [Hz]
c=1540;                  %  Speed of sound [m/s]
lambda=c/f0;             %  Wavelength [m]
width=0.15/1000;         %  Width of element
element_height=5/1000;   %  Height of element [m]
kerf=0.0/1000;           %  Kerf [m]
focus=[0 0 30]/1000;     %  Fixed focal point [m]
N_active=64;             %  Number of active elements
N_elements=350;          %  Number of physical elements 

%  Set the sampling frequency
set_sampling(fs);

%  Generate aperture for emission
xmit_aperture = xdc_linear_array (N_elements, width, element_height, kerf, 1, 1,focus);

%  Set the impulse response and excitation of the xmit aperture
% define properties of the input signal
source_strength = 1e6;    	% [Pa]
tone_burst_freq = 6e6; 	        % [Hz]
tone_burst_cycles = 4;

% create the input signal using toneBurst 
impulse_response = toneBurst(100e6, tone_burst_freq, tone_burst_cycles);
impulse_response = (source_strength./(1540*1000)).*impulse_response;

impulse_response=impulse_response.*hanning(max(size(impulse_response)))';
xdc_impulse (xmit_aperture, impulse_response);

xdc_excitation (xmit_aperture, impulse_response);

%  Generate aperture for reception
receive_aperture = xdc_linear_array (N_elements, width, element_height, kerf, 1, 1,focus);

%  Set the impulse response for the receive aperture
xdc_impulse (receive_aperture, impulse_response);

%   Load the computer phantom
filename=['pht_data.mat'];
load (filename);

phantom_positions=phantom_positions/1000; % [m]
for i=1:50000
  phantom_positions(i,3)=phantom_positions(i,3)+20/1000;
end
phantom_amplitudes=phantom_amplitudes';

%  Set the different focal zones for reception
focal_zones=[0:1:80]'/1000;
Nf=max(size(focal_zones));
focus_times=(focal_zones)/1540;
z_focus=30/1000;          %  Transmit focus

%  Set the apodization
apo=hanning(N_active)';

%   Do linear array imaging
no_lines=256;                   %  Number of lines in image
image_width=40/1000;            %  Size of image sector
d_x=image_width/no_lines;       %  Increment for image

% Do imaging line by line
for i=1:no_lines

  %  Test if the file for the line exist.
  %  Skip the simulation, if the line exits and
  %  go the next line. Else make the simulation
    
  %  Save a file to reserve the calculation
  disp(['Now making line ',num2str(i)])
  
  %  The the imaging direction
  x= -image_width/2 +(i-1)*d_x;

  %   Set the focus for this direction with the proper reference point
  xdc_center_focus (xmit_aperture, [x 0 0]);
  xdc_focus (xmit_aperture, 0, [x 0 z_focus]);
  xdc_center_focus (receive_aperture, [x 0 0]);
  xdc_focus (receive_aperture, focus_times, [x*ones(Nf,1), zeros(Nf,1), focal_zones]);

  %  Calculate the apodization 
  N_pre  = round(x/(width+kerf) + N_elements/2 - N_active/2);
  N_post = N_elements - N_pre - N_active;
  apo_vector=[zeros(1,N_pre) apo zeros(1,N_post)];
  xdc_apodization (xmit_aperture, 0, apo_vector);
  xdc_apodization (receive_aperture, 0, apo_vector);
  
  %   Calculate the received response
  [rf_data, tstart]=calc_scat(xmit_aperture, receive_aperture, phantom_positions, phantom_amplitudes);

  %  Store the result
  cmd=['save rf_ln',num2str(i),'.mat rf_data tstart'];
  eval(cmd);
end

%   Free space for apertures

xdc_free (xmit_aperture)
xdc_free (receive_aperture)

