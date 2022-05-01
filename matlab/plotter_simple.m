clear all
close all

data = csvread("C:\Users\alfre\Desktop\100\Temperature_at_0.05s.csv",1);
dataf = csvread("D:\Alfredo\Academic\MASTER AERO\B\CFD\ProyectoCFD\CodigoCFD\temperature_100_at0.05s.datctr",1);
%%
x=data(:,2);
y=data(:,3);
T=data(:,1);
scatter3(x,y,T)

xf=dataf(:,1);
yf=dataf(:,2);
Tf=dataf(:,3);
scatter3(x,y,T)
hold on
scatter3(xf,yf,Tf)
legend("Luismi","mia")
%%
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



