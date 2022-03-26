%%

data = csvread("Result.txt",1);
% K=convhull(data)
% figure
% trisurf(K,data(:,1),data(:,2),data(:,3))

figure
scatter3(data(:,1),data(:,2),data(:,3))
colorbar
daspect(1.0e+03*[ 0.00167 0.00167    5.0000])
xlabel("X")
ylabel("Y")
zlabel("T")
