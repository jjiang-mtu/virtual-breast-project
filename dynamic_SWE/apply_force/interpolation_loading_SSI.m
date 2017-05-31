% interpolate node's pressure
% 
% author: yu wang
% time: 9/11/2015

clear all;
close all;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% read node coordernate and convert to Matlab
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
node_id = fopen('barout_quad.node');
data = textscan(node_id,'%f %f %f %f');
nodes = cell2mat(data);
NUM = 399824;
temp1 = nodes(1:end,2)*10;
temp2 = nodes(1:end,3)*10;
temp3 = nodes(1:end,4)*10;

x_n=temp2;
y_n=-temp3;
z_n=-temp1;

% write .txt file
file_id=fopen('FEBio_loading_format.txt','w');
fprintf(file_id,'		<force>\r\n');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% load pressure and convert to Matlab
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
load pressure.mat;
count=2; % inital value must be 2
check_matrix=0;
sum_pressure=0;
for delta = -10:1.1:10 % [mm]
  [x y z] = meshgrid((-0.5):0.02:(0.5),(-0.5+delta):0.02:(0.5+delta),-0.5:0.02:0.5);

  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  % select nodes within pressure
  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  x_min = min(x(:));
  x_max = max(x(:));

  y_min = min(y(:));
  y_max = max(y(:));

  z_min = min(z(:));
  z_max = max(z(:));

  k=1;
  for i=1:NUM
      if x_n(i)>x_min && x_n(i)<x_max ... 
         && y_n(i)>y_min && y_n(i)<y_max ... 
         && z_n(i)>z_min && z_n(i)<z_max
       
         index(k)=i;
         k=k+1;
      end
  end

  x_new = x_n(index);
  y_new = y_n(index);
  z_new = z_n(index);

  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  % interpolation
  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  pressure_new = interp3(x,y,z,pressure,x_new,y_new,z_new,'cubic')*1.3e13;

  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  % format for FEBio
  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  if count == 2
      check_matrix = index;
      for i=1:length(index(:))
        fprintf(file_id,'			<node id="%d" bc="z" lc="%d">-%f</node>\r\n',index(i), count,pressure_new(i)); 
        sum_pressure=sum_pressure+pressure_new(i);
      end
      count=count+1;
      fix_sum = sum_pressure
      continue;
  end
 
  number=0;
  for i=1:length(index(:))
      if isempty(find(check_matrix==index(i)))
        number=number+1;
        pressure_filter(number)=pressure_new(i);
        index_filter(number)=index(i);
      end
  end

  pressure_filter=pressure_filter*(fix_sum/sum(pressure_filter(:)));

  for i=1:number
      fprintf(file_id,'			<node id="%d" bc="z" lc="%d">-%f</node>\r\n',index_filter(i), count,pressure_filter(i));
      sum_pressure=sum_pressure+pressure_filter(i);
  end

  count = count + 1;
  check_matrix=cat(2,check_matrix,index);
  clear pressure_filter;
  clear index_filter;
  clear pressure_new;
  clear index;
end

fprintf(file_id,'		</force>\r\n');
sum_pressure
fclose(file_id);














