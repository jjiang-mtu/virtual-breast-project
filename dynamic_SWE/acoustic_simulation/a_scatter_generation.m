%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Language:  Matlab
% Date:      Date: 2016/08/21 09:49:59 
% Version:   Revision: R2015b 

%      Copyright (c) Jingfeng Jiang, Yu Wang. All rights reserved.
%      See LICENCE file for details.

%      This software is distributed WITHOUT ANY WARRANTY; without even 
%      the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
%      PURPOSE.  See the above copyright notices for more information.

% Function: Generate point scatterer
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;
close all;

% number of scatters
% number of realizations
N = 76800;
N_folder = 1;

% size of phantom (matlab coordinate system)
x_size = 20;    % [mm]
y_size = 24;    % [mm]
z_size = 8;     % [mm]

for kk=1:N_folder
    rng('shuffle');
    cmd=['mkdir(''loop',num2str(kk),''')'];
    eval(cmd);

    % random scatters (matlab coordinate system)
    x0 = rand(N,1);
    x_original = x0*x_size;
    z0 = rand(N,1);
    z_original = z0*z_size; 
    y0 = rand(N,1);
    y_original = y0*y_size;

    % save data
    cmd = ['save loop',num2str(kk),'/x_original.mat x_original'];
    eval(cmd);
   
    cmd = ['save loop',num2str(kk),'/y_original.mat y_original'];
    eval(cmd);

    cmd = ['save loop',num2str(kk),'/z_original.mat z_original'];
    eval(cmd);

    % load impedance for acoustic simulation
    load impedance_map.mat;
    [R C M] = size(impedance_map);
    dx = 0.1;
    dy = 0.1;
    dz = 0.1;

    centerx_cube = round(x_size/dx/2);
    centery_cube = round(y_size/dy/2);
    centerz_cube = round(z_size/dz/2);

    centerx_impedance = round(C/2);
    centery_impedance = round(R/2);
    centerz_impedance = round(M/2);

    differencex = 0;
    differencey = 0;
    differencez = 0;

    xindex = round(x_original/dx);
    index = find(xindex==0);
    xindex(index)=1;
    xindex = xindex + differencex;

    yindex = round(y_original/dy);
    index = find(yindex==0);
    yindex(index)=1;
    yindex = yindex + differencey;

    zindex = round(z_original/dz);
    index = find(zindex==0);
    zindex(index)=1;
    zindex = zindex + differencez;
    
    % interpolate scatterer impedance
    for i = 1:N
        phantom_amplitudes(i)=impedance_map(yindex(i),xindex(i),zindex(i));
    end

    index_tmp = find(isnan(phantom_amplitudes));
    phantom_amplitudes(index_tmp)=1540*1000;

    cmd=['save loop',num2str(kk),'/phantom_amplitudes.mat phantom_amplitudes'];
    eval(cmd);

    % save data
    for i=1:N
        temp(i,1)=x_original(i,1);
        temp(i,2)=-z_original(i,1);
        temp(i,3)=y_original(i,1);
    end

    for i=1:N
        temp(i,1)=temp(i,1)-0.5*x_size;
        temp(i,2)=temp(i,2)+0.5*y_size;
    end

    for i=1:N
        phantom_positions(i,1)=temp(i,1);
        phantom_positions(i,2)=temp(i,2);
        phantom_positions(i,3)=temp(i,3);
    end

    cmd=['save loop',num2str(kk),'/pht_data_step0.mat phantom_positions phantom_amplitudes'];
    eval(cmd);
end
