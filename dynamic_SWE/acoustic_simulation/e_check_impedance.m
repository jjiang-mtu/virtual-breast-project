%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Language:  Matlab
% Date:      Date: 2016/08/21 09:49:59 
% Version:   Revision: R2015b 

%      Copyright (c) Jingfeng Jiang, Yu Wang. All rights reserved.
%      See LICENCE file for details.

%      This software is distributed WITHOUT ANY WARRANTY; without even 
%      the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
%      PURPOSE.  See the above copyright notices for more information.

% Function: check impedance
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;
close all;

load loop1/phantom_amplitudes.mat;
load loop1/x_original.mat;
load loop1/y_original.mat;
load loop1/z_original.mat;

X = x_original(:);
Y = y_original(:);
Z = z_original(:);
V = phantom_amplitudes(:);
[Xq,Yq,Zq] = meshgrid(min(X):0.2:max(X),min(Y):0.2:max(Y),min(Z):0.2:max(Z));
Vq = griddata(X,Y,Z,V,Xq,Yq,Zq);
imagesc(Vq(:,:,20)); axis image
