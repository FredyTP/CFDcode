%%

data = csvread("static_solution.txt",1);


x=data(:,1);
y=data(:,2);
T=data(:,3);

color = colormap;
maxt = max(T);
mint = min(T);
diff = max(maxt-mint,1);
colors=round((T-mint)/diff*255);




scatter3(x,y,T)
%%
%hold on
f = figure;

xv=data(:,4:6)';%x coordinates of vertices
yv=data(:,7:9)';%y coordinates of vertices
zv=[T';T';T'];
patch(xv,yv,colors,'EdgeColor','none');
colorbar

xlabel("X")
ylabel("Y")



%set(h, 'ylim', [mint maxt])
saveas(f,"Results.jpg")
