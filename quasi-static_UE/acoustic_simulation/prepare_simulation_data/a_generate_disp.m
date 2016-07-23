%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Language:  Matlab
% Date:      Date: 2016/07/21 09:49:59 
% Version:   Revision: R2015b 

%      Copyright (c) Jingfeng Jiang, Yu Wang. All rights reserved.
%      See LICENCE file for details.

%      This software is distributed WITHOUT ANY WARRANTY; without even 
%      the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
%      PURPOSE.  See the above copyright notices for more information.

% Function: Generate scatterer displacement
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;
close all;

% generate rectilinear grids in ROI (matlab coordinate)
[X Y Z]=meshgrid(0:0.05:39.95, 0:0.05:22.95, 0:0.05:7.95);

temp=1; 
V=zeros(460, 800, 160);
for k=1:460
    filename=['dz/dispz' num2str(460-k) '.mat'];
    load (filename);
    for i=1:800
        for j=1:160
            V(k, i, j)=(parameter(i,j))*temp;
        end
    end
end

% load random scatters (matlab coordinate)
filename=['x.mat']; 
load (filename);
filename=['y.mat']; 
load (filename);
filename=['z.mat']; 
load (filename);

% interpolate displacement (matlab coordinate)
% "griddata"
for k=1:50000
	disp(k)=interp3(X,Y,Z, V, x(k),y(k), z(k), 'cubic');
end

% save data
save ('displacement.mat', 'disp');
