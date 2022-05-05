
     
name = strcat("temperature_trimesh",".datctr");
tri=csvread(name,1);
x=tri(:,1);

y=tri(:,2);
T=tri(:,3);
figure
scatter3(x,y,T)
%%

name = strcat("temperature_quadmesh",".datctr");
quad=csvread(name,1);
x=quad(:,1);

y=quad(:,2);
T=quad(:,3);
figure
scatter3(x,y,T)