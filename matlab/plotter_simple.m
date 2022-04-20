data = csvread("static_solutionUDS.txt",1);


x=data(:,1);
y=data(:,2);
T=data(:,3);
figure
scatter3(x,y,T)