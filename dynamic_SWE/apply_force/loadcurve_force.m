% load curve for dispersion
% author: yu wang
% time: 1/24/2016

clear all;
close all;

times=19;
file_id=fopen('loadcurve_force.txt','w');
for i=2:(times+1)
  x_t=[0 1e-6 29e-6 30e-6 39e-6]+(i-2)*40e-6;
  y_d=[0 1 1 0 0];

  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  % format for FEBio
  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  fprintf(file_id,'		<loadcurve id="%d" type="linear" extend="constant">\r\n', i);
  for i=1:length(x_t(:))
    fprintf(file_id,'			<loadpoint> %f,%f </loadpoint>\r\n',x_t(i),y_d(i));
  end
  fprintf(file_id,'		</loadcurve>\r\n');
end

