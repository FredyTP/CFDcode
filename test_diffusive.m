
%%


quad=csvread("C:\Users\alfre\Desktop\100\Temperature_at_0.05s.csv",1);
x=quad(:,2);

y=quad(:,3);
T=quad(:,1);
figure
scatter3(x,y,T)
hold on

quad=csvread("C:\Users\alfre\Desktop\100\Temperature_at_0.10s.csv",1);
x=quad(:,2);

y=quad(:,3);
T=quad(:,1);
scatter3(x,y,T)

quad=csvread("C:\Users\alfre\Desktop\100\Temperature_at_0.15s.csv",1);
x=quad(:,2);

y=quad(:,3);
T=quad(:,1);
scatter3(x,y,T)


quad=csvread("C:\Users\alfre\Desktop\100\Temperature_at_1.00s.csv",1);
x=quad(:,2);

y=quad(:,3);
T=quad(:,1);
scatter3(x,y,T)