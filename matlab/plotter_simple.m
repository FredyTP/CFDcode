clear all
close all

data = csvread("static_solution.txt",1);

%%
x=data(:,1);
y=data(:,2);
T=data(:,3);
xv=data(:,4:7)';%x coordinates of vertices
yv=data(:,8:11)';%y coordinates of vertices
zv=[T';T';T'];

color = colormap;
maxt = max(T);
mint = min(T);
diff = max(maxt-mint,1);
colors=round((T-mint)/diff*255);

minx = min(min(xv));
maxx = max(max(xv));
miny=min(min(yv));
maxy=max(max(yv));
width = maxx-minx;
height = maxy-miny;
pbaspect([width/height 1 1])
patch(xv,yv,colors,'EdgeColor','none');



