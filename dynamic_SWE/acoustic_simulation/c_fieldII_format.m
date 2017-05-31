%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Language:  Matlab
% Date:      Date: 2016/08/21 09:49:59 
% Version:   Revision: R2015b 

%      Copyright (c) Jingfeng Jiang, Yu Wang. All rights reserved.
%      See LICENCE file for details.

%      This software is distributed WITHOUT ANY WARRANTY; without even 
%      the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
%      PURPOSE.  See the above copyright notices for more information.

% Function: FEILD II format
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;
close all;

% load data
N_folder = 1;
for kk=1:N_folder
    cmd = ['load loop',num2str(kk),'/x_original.mat'];
    eval(cmd);

    cmd = ['load loop',num2str(kk),'/y_original.mat'];
    eval(cmd);

    cmd = ['load loop',num2str(kk),'/z_original.mat'];
    eval(cmd);
    
    cmd = ['load loop',num2str(kk),'/point_scatter_disp.mat'];
    eval(cmd);

    cmd = ['load loop',num2str(kk),'/phantom_amplitudes.mat'];
    eval(cmd);

    % Matlab ==> Field II, x, y, z coordernate
    x_size = 20;    % [mm]
    y_size = 8;     % [mm]
    z_size = 24;    % [mm]
    N = 76800;

    for i=1:N
        temp(i,1) = x_original(i,1);
        temp(i,2) = -z_original(i,1);
        temp(i,3) = y_original(i,1);
    end

    for i=1:N
        temp(i,1) = temp(i,1)-0.5*x_size;
        temp(i,2) = temp(i,2)+0.5*y_size;
    end

    for i=1:N
        phantom_positions(i,1) = temp(i,1);
        phantom_positions(i,2) = temp(i,2);
        phantom_positions(i,3) = temp(i,3);
    end

    cmd = ['save loop',num2str(kk),'/pht_data_step0.mat phantom_positions phantom_amplitudes'];
    eval(cmd);

    for k =1:length(steps)
        for i=1:N
            temp(i,1) = x_original(i,1);
            temp(i,2) = -z_original(i,1);
            temp(i,3) = y_original(i,1) + displacement(i,k)*10; % unit conversion (from cm to mm)
        end

        for i=1:N
            temp(i,1) = temp(i,1)-0.5*x_size;
            temp(i,2) = temp(i,2)+0.5*y_size;
        end

        for i=1:N
	    phantom_positions(i,1) = temp(i,1);
            phantom_positions(i,2) = temp(i,2);
            phantom_positions(i,3) = temp(i,3);
        end

        cmd = ['save loop',num2str(kk),'/pht_data_step',num2str(steps(k)),'.mat phantom_positions phantom_amplitudes'];
        eval(cmd);
    end
end
