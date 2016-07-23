function B=curve_display(strain_y,time,p)

B=zeros(size(p,2),length(time));
for k=1:size(p,2) % number of points index
  x=time; % time
  y=permute(strain_y(p(k,1),p(k,2),:),[2,3,1]); % strain

  [bestcoeffs,fval,exitflag]=fminsearch(@calculation,[y(1) y(end)-y(1) 2],[],x,y);
  yfit=bestcoeffs(1)+bestcoeffs(2).*(1-exp(-x./bestcoeffs(3)));

B(k,:)=yfit;
end



