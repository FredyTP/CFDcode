
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
    name = strcat("Center_Line_Temperature_Stationary_Box_Central_Diffencing_Scheme_",num2str(n_cell),".csv");
    cds(:,:,i)=csvread(name,1);
    legend_name{i}=num2str(n_cell);
    x=cds(:,1,i);
    T=cds(:,2,i);
    plot(x,T)
end
legend(legend_name)
title("CENTRAL DIFFERENCING SCHEME")
%%
figure
hold on
for i=1:6
    n_cell = 4^i;
    name = strcat("Center_Line_Temperature_Stationary_Box_Upwind_Diffencing_Scheme_",num2str(n_cell),".csv");
    uds(:,:,i)=csvread(name,1);
    x=uds(:,1,i);
    T=uds(:,2,i);
    plot(x,T)
end
title("UPWIND DIFFERENCING SCHEME")
legend(legend_name)

%%
figure
hold on
for i=1:6
    n_cell = 4^i;
    name = strcat("Center_Line_Temperature_Stationary_Box_Second_Order_Upwind_",num2str(n_cell),".csv");
    soup(:,:,i)=csvread(name,1);
    x=soup(:,1,i);
    T=soup(:,2,i);
    plot(x,T)
end
title("SECOND ORDER UPWIND")
legend(legend_name)




%%
figure
hold on
for i=1:6
    n_cell = 4^i;
    name = strcat("Center_Line_Temperature_Stationary_Box_Powerlaw_Diffencing_Scheme_",num2str(n_cell),".csv");
    powerlaw(:,:,i)=csvread(name,1);
    x=powerlaw(:,1,i);
    T=powerlaw(:,2,i);
    plot(x,T)
end
title("POWERLAW")
legend(legend_name)

%%

figure
hold on
title("Comparativa para 4096 elementos")
plot(uds(:,1,i),uds(:,2,i));
plot(cds(:,1,i),cds(:,2,i));
plot(soup(:,1,i),soup(:,2,i));
plot(powerlaw(:,1,i),powerlaw(:,2,i));
legend("UDS","CDS","SOUP","POWER LAW")