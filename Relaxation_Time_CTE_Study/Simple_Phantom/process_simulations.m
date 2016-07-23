addpath('./relax_fit_func')

strt=10; %start time (end of compression)
d1=dir(pwd);

if exist('./barout.node','file')~=2
    disp('Error: no node file')
    return
else
    Nodefile='./barout.node';
end

for i=1:length(d1)
    if d1(i).isdir
        if exist(['./',d1(i).name,'/run.txt'],'file')==2
            d2=dir(['./',d1(i).name]);
            txfile=fopen(['./',d1(i).name,'/run.txt']);
            tx=textscan(txfile,'%s');
            for j=1:length(d2)
                if strncmp(tx{1},d2(j).name,length(tx{1}))
          
                Inputfile=['./',d1(i).name,'/',d2(j).name];
                Outputfile1=['./',d1(i).name,'/','strain_',d2(j).name(1:end-4),'.mat'];
                Outputfile2=['./',d1(i).name,'/','T_',d2(j).name(1:end-4),'.mat'];
                    
                outfile=ResultsConfigure(Inputfile);
                strain_gen_files2(outfile,Nodefile,Outputfile1,strt)
                T=curve_fit(Outputfile1);
                
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% contrast calculation: method 1
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
i_area = T(41:60,41:60); % inclusion
b_area_1 = T(23:32,10:19); % background
b_area_2 = T(24:33,76:85); % background
b_area_3 = T(74:83,11:20); % background
b_area_4 = T(73:82,72:81); % background
b_area = [b_area_1(:); b_area_2(:); b_area_3(:); b_area_4(:)];
inclusion = sum(i_area(:))/length(i_area(:));
background = sum(b_area(:))/length(b_area(:));
i_b_contrast_1 = inclusion/background

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% contrast calculation: method 2
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
inclusion = 0;
N_i = 0;
background = 0;
N_b = 0;
center = [50;50];
for k=1:99
  for l=1:101
    if ((k-center(1))^2+(l-center(2))^2) < 181
      inclusion = inclusion + T(k,l);
      N_i = N_i + 1;
    end
    if ((k-center(1))^2+(l-center(2))^2) > 340 && ((k-center(1))^2+(l-center(2))^2) <= (340+181)
      background = background + T(k,l);
      N_b = N_b + 1;
    end
  end
end
i_b_contrast_2 = (inclusion/N_i)/(background/N_b)
                
% dim=size(T);
% bxwidth=[20,20];
% center(1,:)=[50,50];
% center(2,:)=[25,15];
% box(:,:,1)=T(center(1,1)-bxwidth(1)/2:center(1,1)+bxwidth(1)/2,...
%             center(1,2)-bxwidth(1)/2:center(1,2)+bxwidth(1)/2);
%         
% box(:,:,2)=T(center(2,1)-bxwidth(1)/2:center(2,1)+bxwidth(1)/2,...
%             center(2,2)-bxwidth(1)/2:center(2,2)+bxwidth(1)/2);
% 
%         
% contrast=mean(mean(box(:,:,1)))/mean(mean(box(:,:,2)));

save(Outputfile2,'T','i_b_contrast_2','i_b_contrast_1')
 clear Inputfile Outputfile1 Outputfile2 ib_contrast_1 ib_contrast_2 T 
                end
            end
        end
    end
end