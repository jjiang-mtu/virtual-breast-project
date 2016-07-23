%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Language:  Matlab
% Date:      Date: 2016/07/21 09:49:59 
% Version:   Revision: R2015b 

%      Copyright (c) Jingfeng Jiang, Yu Wang. All rights reserved.
%      See LICENCE file for details.

%      This software is distributed WITHOUT ANY WARRANTY; without even 
%      the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
%      PURPOSE.  See the above copyright notices for more information.

% Function: orginize rf data to a matrix for speckle tracking
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;
close all;

% orginize rf lines
fs=50e6; % [Hz]
no_lines=256;
for i =1:no_lines
  cmd=['load rf_data/rf_ln',num2str(i),'.mat'];
  eval(cmd);
  rf_env_new = [zeros(round(tstart*fs),1); rf_data];
  env(1:max(size(rf_env_new)),i)=rf_env_new;
end

% save data
data = env;
cmd = ['save deformation.mat data'];
eval(cmd);
