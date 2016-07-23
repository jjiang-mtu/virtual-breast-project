%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Language:  Matlab
% Date:      Date: 2016/07/21 09:49:59 
% Version:   Revision: R2015b 

%      Copyright (c) Jingfeng Jiang, Yu Wang. All rights reserved.
%      See LICENCE file for details.

%      This software is distributed WITHOUT ANY WARRANTY; without even 
%      the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
%      PURPOSE.  See the above copyright notices for more information.

% Function: Generate scatterer amplitudes
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;
close all;

% number of scatters
N=50000;

% size of phantom (Field II coordinates system)
x_size=39.95;   % [mm]
y_size=7.95;    % [mm]
z_size=22.95;   % [mm]
z_start=0;      % [mm]

dx=0.05;        % [mm]
dy=0.05;        % [mm]
dz=0.05;        % [mm]

% load impedance map (amplitude)
filename=['impedance.mat']; 
load (filename);

% load scatters (matlab coordinate)
filename=['x.mat']; 
load (filename);
filename=['y.mat']; 
load (filename);
filename=['z.mat']; 
load (filename);

% convert coordinate
for i=1:N
        x_temp(i,1)=x(i,1);
        y_temp(i,2)=-z(i,1);
        z_temp(i,3)=y(i,1);
end

for i=1:N
        x(i,1)=x_temp(i,1);
        y(i,1)=y_temp(i,2);
        z(i,1)=z_temp(i,3);
end

% assign amplitude (Field II coordinates)
xindex = round((x)/dx + 1);
yindex = round(abs((y)/dy) + 1);
zindex = round((z - z_start)/dz + 1);

for i=1:N
	phantom_positions(i,1)=x(i,1);
        phantom_positions(i,2)=y(i,1);
        phantom_positions(i,3)=z(i,1);

	phantom_amplitudes(i)=impedance_map(zindex(i,1),xindex(i,1),yindex(i,1));
end

% save data
cmd=['save pht_amplitudes.mat phantom_amplitudes'];
eval(cmd);
