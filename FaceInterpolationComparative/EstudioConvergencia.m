clear all 
close all

Tasyn=386.078;
data = csvread("../MeshConvergence.csv",1);
n_cell = data(:,1);
h=0.1./sqrt(n_cell);
n_cell_log=[1,2,3,4,5,6];
UDS_TRI= data(:,2);
CDS_TRI= data(:,3);
PL_TRI= data(:,4);
SOUP_TRI= data(:,5);

UDS_QUAD= data(:,6);
CDS_QUAD= data(:,7);
PL_QUAD= data(:,8);
SOUP_QUAD= data(:,9);

UDS_QUAD=abs(UDS_QUAD-Tasyn)./Tasyn*100;
CDS_QUAD=abs(CDS_QUAD-Tasyn)./Tasyn*100;
PL_QUAD=abs(PL_QUAD-Tasyn)./Tasyn*100;
SOUP_QUAD=abs(SOUP_QUAD-Tasyn)./Tasyn*100;

figure
%plot(n_cell_log,UDS_TRI)
hold on
grid on
%plot(n_cell_log,CDS_TRI)
%plot(n_cell_log,PL_TRI)
%plot(n_cell_log,SOUP_TRI)
colors = viridis(3);

plot(n_cell_log,UDS_QUAD,'--','linewidth',2,'color',colors(1,:))
plot(n_cell_log,CDS_QUAD,'--','linewidth',2,'color',colors(2,:))
plot(n_cell_log,SOUP_QUAD,'--','linewidth',2,'color',colors(3,:))
%plot([0.75,6.25],[Tasyn,Tasyn],'linewidth',2,'color',colors(4,:))

plot(n_cell_log,UDS_QUAD,'o','linewidth',1,'color',colors(1,:))
plot(n_cell_log,CDS_QUAD,'o','linewidth',1,'color',colors(2,:))
plot(n_cell_log,SOUP_QUAD,'o','linewidth',1,'color',colors(3,:))
title("\textbf{Independencia de Malla}","FontSize",14,'Interpreter','latex');
xlim([0.5,6.5])
ylim([-0.1,3])
xlabel("$log_4(n cells)$","FontSize",15,'Interpreter','latex')
ylabel("$|\epsilon|\;$[\%]","FontSize",15,'Interpreter','latex')
legend(["Upwind Differencing Scheme","Central Differencing Scheme","Second Order Upwind","High Resolution"]...
    ,'Interpreter','latex',"FontSize",14,'location','northeast')

%% CALCULO DE GDI I EXTRAPOLACION DE RICHARSON

h=0.1./sqrt(n_cell);


h1=h(6);
h2=h(5);
h3=h(4);

w1=SOUP_QUAD(6);
w2=SOUP_QUAD(5);
w3=SOUP_QUAD(4);


syms C p;

eq1= w1-w2==C*(h1^p-h2^p)
eq2= w2-w3==C*(h2^p-h1^p)

[c,p]=vpasolve([eq1,eq2],[C,p])






