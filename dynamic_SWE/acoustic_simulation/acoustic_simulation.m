%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Language:  Matlab
% Date:      Date: 2016/08/21 09:49:59 
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

f0=10e6;                 %  Transducer center frequency [Hz]
fs=50e6;                 %  Sampling frequency [Hz]
c=1540;                  %  Speed of sound [m/s]
lambda=c/f0;             %  Wavelength [m]
width=0.1/1000;          %  Width of element
element_height=5/1000;   %  Height of element [m]
kerf=0.0/1000;           %  Kerf [m]
focus=[0 0 32]/1000;     %  Fixed focal point [m]
N_active=64;             %  Number of active elements
N_elements=400;          %  Number of physical elements 

for N_folder=1
   for  step_index=280:2:440
        cmd=['load /home/ywang27/research/SWS_breast_model/SSI_inclusion/track/loop',num2str(N_folder),'/pht_data_step',num2str(step_index),'.mat'];
        eval(cmd);

        [point_M point_N]=size(phantom_positions);
        phantom_positions=phantom_positions/1000; % [m]

        for i=1:point_M
            phantom_positions(i,3)=phantom_positions(i,3)+20/1000;
        end

        phantom_amplitudes=phantom_amplitudes';

        %  Set the different focal zones for reception
        focal_zones=[0:1:80]'/1000;
	Nf=max(size(focal_zones));
	focus_times=(focal_zones)/1540;
	z_focus=32/1000;                %  Transmit focus

	%  Set the apodization
	apo=hanning(N_active)';

	%   Do linear array imaging
	no_lines=200;                   %  Number of lines in image
	image_width=20/1000;            %  Size of image sector
	d_x=image_width/no_lines;       %  Increment for image

	% Do imaging line by line
        x = 0;
        N_pre = 0;
        N_post = 0;
        apo_vector = 0;
        rf_data = 0;
        tstart = 0;
        rf_time = cell(1, no_lines);
        rf_env = cell(1, no_lines);
        parfor i=1:no_lines
            path(path,'/share/apps/fieldii/3.24');
            field_init

            %  Set the sampling frequency
            set_sampling(fs);

            %  Generate aperture for emission
            xmit_aperture = xdc_linear_array (N_elements, width, element_height, kerf, 1, 10,focus);

            %  Set the impulse response and excitation of the xmit aperture
            impulse_response=sin(2*pi*f0*(0:1/fs:2/f0));
            impulse_response=impulse_response.*hanning(max(size(impulse_response)))';
            xdc_impulse (xmit_aperture, impulse_response);
            excitation=sin(2*pi*f0*(0:1/fs:2/f0));
            xdc_excitation (xmit_aperture, excitation);

            %  Generate aperture for reception
            receive_aperture = xdc_linear_array (N_elements, width, element_height, kerf, 1, 1,focus);

            %  Set the impulse response for the receive aperture
            xdc_impulse (receive_aperture, impulse_response);
    
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
            
            % save data 
            rf_time{i} = tstart;
            rf_env{i} = rf_data;

            %  Free space for apertures
            xdc_free (xmit_aperture)
            xdc_free (receive_aperture)

        end
	%  Store the result
	cmd=['save /home/ywang27/research/SWS_breast_model/SSI_inclusion/track/loop',num2str(N_folder),'/rf_ln_step',num2str(step_index),'.mat rf_env rf_time'];
	eval(cmd);
    end
end
