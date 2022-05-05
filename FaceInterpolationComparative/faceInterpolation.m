clear all
close all

cds=[]
uds=[]
soup=[]
powerlaw=[]

%%
legend_name = {};
figure
hold on
for i=1:6
    n_cell = 4^i;
    name = strcat("Center_Line_Temperature_Stationary_Central_Diffencing_Scheme_TRI_",num2str(n_cell),".csv");
    cds=csvread(name,1);
    legend_name{i}=num2str(n_cell);
    x=cds(:,1);
    T=cds(:,2);
    plot(x,T)
end
legend(legend_name)
title("CENTRAL DIFFERENCING SCHEME")
%%
figure
hold on
for i=1:6
     n_cell = 4^i;
    name = strcat("Center_Line_Temperature_Stationary_Upwind_Diffencing_Scheme_TRI_",num2str(n_cell),".csv");
    uds=csvread(name,1);
    x=uds(:,1);
    T=uds(:,2);
    plot(x,T)
end
title("UPWIND DIFFERENCING SCHEME")
legend(legend_name)

%%
figure
hold on
for i=1:6
     n_cell = 4^i;
    name = strcat("Center_Line_Temperature_Stationary_Second_Order_Upwind_TRI_",num2str(n_cell),".csv");
    soup=csvread(name,1);
    x=soup(:,1);
    T=soup(:,2);
    plot(x,T)
end
title("SECOND ORDER UPWIND")
legend(legend_name)




%%
figure
hold on
for i=1:6
     n_cell = 4^i;
    name = strcat("Center_Line_Temperature_Stationary_Powerlaw_Diffencing_Scheme_TRI_",num2str(n_cell),".csv");
    powerlaw=csvread(name,1);
    x=powerlaw(:,1);
    T=powerlaw(:,2);
    plot(x,T)
end
title("POWERLAW")
legend(legend_name)
%%
analitic = csvread("Center_Line_Temperature_Stationary_Second_Order_Upwind_QUAD_262144.csv",1);
%%

figure
hold on
title("Comparativa para 4096 elementos")
plot(uds(:,1),uds(:,2));
%%plot(cds(:,1),cds(:,2));
plot(soup(:,1),soup(:,2));
plot(powerlaw(:,1),powerlaw(:,2));
%%plot(analitic(:,1),analitic(:,2));
legend("UDS","SOUP","POWERLAW")%%,"QUAD MESH")