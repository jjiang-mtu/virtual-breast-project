%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Language:  Matlab
% Date:      Date: 2016/07/21 09:49:59 
% Version:   Revision: R2015b 

%      Copyright (c) Jingfeng Jiang, Yu Wang. All rights reserved.
%      See LICENCE file for details.

%      This software is distributed WITHOUT ANY WARRANTY; without even 
%      the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
%      PURPOSE.  See the above copyright notices for more information.

% Function: Generate acoustic simulation format
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;
close all;

filename=['pht_positions.mat']; 
load (filename);

filename=['pht_amplitudes.mat']; 
load (filename);

cmd=['save pht_data.mat phantom_positions phantom_amplitudes'];
eval(cmd);

