% read displacement
%
% author: yu wang
% time: 8/31/2015
clear all;
close all;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% input: revise only this part
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
input_id = fopen('inclusion_SSI.txt');
steps=280:2:440; % must be in ascending order


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% read node coordernate and convert to Matlab
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
standard = 'Data = displacement';
X=0;
Y=0;
Z=0;

while ~feof(input_id)
  tline=fgetl(input_id);
  if strcmp(tline,standard)
    data = textscan(input_id,'%f %f %f %f %f %f %f');
    xyz = cell2mat(data);
    temp1=xyz(1:end,2)*10;
    temp2=xyz(1:end,3)*10;
    temp3=xyz(1:end,4)*10;

    X=temp2;
    Y=-temp3;
    Z=-temp1;
  
    break;
  end
end
fclose(input_id)
input_id = fopen('inclusion_SSI.txt');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% interpolation (matlab coordernate) step 1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
interval=0.1; % interval for interpolation [mm]
[Xq Yq Zq]=meshgrid(-4:interval:max(X), -10:interval:10, (min(Z)+max(Z))/2);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% interpolation (matlab coordernate) step 2
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
standard = 'Data = displacement';
n_steps=length(steps);
flag=1;
while ~feof(input_id)
  value=steps(flag);
  temp=['Step = ',num2str(value)];
  tline=fgetl(input_id);
  if strcmp(tline,standard)
    data = textscan(input_id,'%f %f %f %f %f %f %f');
  end
  if ~strcmp(tline,temp)
    continue;
  end

  tline=fgetl(input_id);
  temp_t=tline(8:end);
  time(flag)=str2num(temp_t);
  tline=fgetl(input_id);

  data = textscan(input_id,'%f %f %f %f %f %f %f');
  displace = cell2mat(data);
  temp1=displace(1:end,5);
  temp2=displace(1:end,6);
  temp3=displace(1:end,7);

  VX=temp2;
  VY=-temp3;
  VZ=-temp1;

  % disp_y=griddata(X(:),Y(:),Z(:),VY(:), Xq, Yq, Zq);
  disp_y = 0;
  
  save_y(:,:,flag)=disp_y;
  flag=flag+1;

  if flag>n_steps
    break;
  end
end

displacement = save_y;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% convert data format for curve fitting
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear data;
for i=1:(flag-1)
  temp=displacement(:,:,i);
  data(:,i)=temp(:);
end
clear temp;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% save data
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
save result_new.mat data displacement steps time;











  
