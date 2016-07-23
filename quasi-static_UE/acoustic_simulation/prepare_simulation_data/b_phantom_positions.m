%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Language:  Matlab
% Date:      Date: 2016/07/21 09:49:59 
% Version:   Revision: R2015b 

%      Copyright (c) Jingfeng Jiang, Yu Wang. All rights reserved.
%      See LICENCE file for details.

%      This software is distributed WITHOUT ANY WARRANTY; without even 
%      the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
%      PURPOSE.  See the above copyright notices for more information.

% Function: Generate scatterer position after deformation
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;
close all;

% load scatterer (matlab coordinate)
filename=['x.mat']; 
load (filename);
filename=['y.mat']; 
load (filename);
filename=['z.mat']; 
load (filename);

% load scatter displacement (matlab coordinate)
load displacement.mat;

% generate scatterer position after deformation
% the deformation of the interface between transducer
% and tissue surface should be zero
for i=1:50000
	y(i,1)=y(i,1)+displacement(i)+3.2362;
end

% convert coordinate (Field II coordinate)
x_size=39.95;   % [mm]
y_size=7.95;    % [mm]
z_size=22.95;   % [mm]

for i=1:50000
        temp(i,1)=x(i,1);
        temp(i,2)=-z(i,1);
        temp(i,3)=y(i,1);
end

for i=1:50000
        temp(i,1)=temp(i,1)-0.5*x_size;
        temp(i,2)=temp(i,2)+0.5*y_size;
end

for i=1:50000
	phantom_positions(i,1)=temp(i,1);
        phantom_positions(i,2)=temp(i,2);
        phantom_positions(i,3)=temp(i,3);
end

% save data
cmd=['save pht_positions.mat phantom_positions'];
eval(cmd);
