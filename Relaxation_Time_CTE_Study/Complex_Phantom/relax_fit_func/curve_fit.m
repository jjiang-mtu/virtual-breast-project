function T=curve_fit(Inputfile)

if ~isstr(Inputfile)
    fprintf('Error: input argumenst must be strings')
    return
end

load(Inputfile);
[m n]=size(data);
for k=1:m % number of points index
  x=time(1:end); % time
  y=data(k,1:end); % strain
                                         %previously [.05 .05 2]
  [bestcoeffs,fval,exitflag]=fminsearch(@calculation,[y(1) y(end)-y(1) 2],[],x,y);
  yfit=bestcoeffs(1)+bestcoeffs(2).*(1-exp(-x./bestcoeffs(3)));

  data_matrix(k)=bestcoeffs(3);
  %plot(x,y,x,yfit,'r')
end

[M N P]=size(strain_y);
temp=1;
for j=1:N
  for i=1:M
    T(i,j)=data_matrix(temp);
    temp=temp+1;
  end
end

imagesc(T);colormap(gray); axis image;
end