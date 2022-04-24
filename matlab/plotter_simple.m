data = csvread("../static_solutionOC.txt",1);


x=data(:,1);
y=data(:,2);
Tc=data(:,3);
figure
scatter3(x,y,Tc)
hold on

data = csvread("../static_solutionCDs.txt",1);
x=data(:,1);
y=data(:,2);
T=data(:,3);

scatter3(x,y,T)

legend("ORTH-CORR","CDs")

figure
scatter3(x,y,Tc-T)