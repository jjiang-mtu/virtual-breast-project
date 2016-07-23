function [data]=step_track(Inputfile)

tic;

n_steps=60;

if ~ischar(Inputfile) 
    fprintf('Error: input argumenst must be strings')
    return
end

input_id=fopen(Inputfile);

flag=1;
data=zeros(2,n_steps);

while ~feof(input_id)
  temp=['*Step = ',num2str(flag)];
  tline=fgetl(input_id);
  if strcmp(tline,temp)
    data1=tline;
    data2=fgetl(input_id);
  
  eval([data1,';'])
  eval([data2,';'])
  data(1,flag)=Step;
  data(2,flag)=Time;
  flag=flag+1;
  end
  
 
% 
  if flag>n_steps
    break;
  end
end

tc=toc;
