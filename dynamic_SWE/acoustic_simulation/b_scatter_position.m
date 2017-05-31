%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Language:  Matlab
% Date:      Date: 2016/08/21 09:49:59 
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

% input parameters
N_folder = 1;
for kk = 1:N_folder
    input_id = fopen('inclusion_SSI.txt');
    steps = 1:10:280; % in ascending order
    % steps=280:2:440;
    % steps=355;

    % size of phantom (matlab coordinate system)
    x_size = 20;   % [mm]
    y_size = 24;   % [mm]
    z_size = 8;    % [mm]

    % read node and convert to matlab system
    standard = 'Data = displacement';
    X = 0;
    Y = 0;
    Z = 0;

    while ~feof(input_id)
        tline = fgetl(input_id);
        if strcmp(tline,standard)
            data = textscan(input_id,'%f %f %f %f %f %f %f');
            xyz = cell2mat(data);
            temp1 = xyz(1:end,2)*10;
            temp2 = xyz(1:end,3)*10;
            temp3 = xyz(1:end,4)*10;
            X = temp2;
            Y = -temp3;
            Z = -temp1;
            break;
        end
    end
    fclose(input_id)
    input_id = fopen('inclusion_SSI.txt');

    % interpolation (matlab coordernate) step 1   
    cmd = ['load loop',num2str(kk),'/x_original.mat'];
    eval(cmd);

    cmd = ['load loop',num2str(kk),'/y_original.mat'];
    eval(cmd);

    cmd = ['load loop',num2str(kk),'/z_original.mat'];
    eval(cmd);

    Xq = x_original(:);
    Yq = y_original(:);
    Zq = z_original(:);

    X_new_move = 4; % [mm]
    Y_new_move = 12; % [mm]
    Z_new_move = 4; % [mm]

    % interpolation (matlab coordernate) step 2
    standard = 'Data = displacement';
    n_steps = length(steps);
    flag = 1;
    while ~feof(input_id)
        value = steps(flag);
        temp = ['Step = ',num2str(value)];
        tline = fgetl(input_id);
        if strcmp(tline,standard)
            data = textscan(input_id,'%f %f %f %f %f %f %f');
        end
        if ~strcmp(tline,temp)
            continue;
        end

        tline = fgetl(input_id);
        temp_t = tline(8:end);
        time(flag) = str2num(temp_t);
        tline = fgetl(input_id);

        data = textscan(input_id,'%f %f %f %f %f %f %f');
        displace = cell2mat(data);
        temp1 = displace(1:end,5);
        temp2 = displace(1:end,6);
        temp3 = displace(1:end,7);

        VX = temp2;
        VY = -temp3;
        VZ = -temp1;
  
        X_new = X(1:5:end) + X_new_move;
        Y_new = Y(1:5:end) + Y_new_move;
        Z_new = Z(1:5:end) + Z_new_move;
        VY_new = VY(1:5:end);
        disp_y = griddata(X_new(:),Y_new(:),Z_new(:),VY_new(:), Xq, Yq, Zq, 'natural');

        index = find(isnan(disp_y));
        disp_y(index) = 0;
  
        save_y(:,flag) = disp_y(:);
        flag = flag+1;

        if flag>n_steps
            break;
        end
    end

    % displacement scale
    displacement = save_y*6;

    % save data
    % save point_scatter_disp.mat displacement steps time;
    cmd=['save loop',num2str(kk),'/point_scatter_disp.mat displacement steps time'];
    eval(cmd);

    fclose(input_id);
end
