
colors=viridis(3);

fig=figure('Renderer', 'painters', 'Position', [10 10 700 500]);
hold on
grid on
legend_name={};

  
data=csvread("Center_Line_Temperature_Stationary_Central_Difference_Gradient_TRI_4096.csv",1);
x=data(:,1);
T=data(:,2);
plot(x,T,'linewidth',2,'color',colors(1,:));

 
data=csvread("Center_Line_Temperature_Stationary_Orthogonal_Corrected_Gradient_TRI_4096.csv",1);
x=data(:,1);
T=data(:,2);
plot(x,T,'linewidth',2,'color',colors(2,:));

data=csvread("Center_Line_Temperature_Stationary_Central_Difference_Gradient_QUAD_262144.csv",1);
x=data(:,1);
T=data(:,2);
plot(x,T,'linewidth',2,'color',colors(3,:));


figure_title="Comparativa de los metodos difusivos";
title(strcat("\textbf{",figure_title,"}"),"FontSize",14,'Interpreter','latex');
xlim([-0.01,0.11]);
ylim([290,680]);
xlabel("$Position Y $[m]","FontSize",15,'Interpreter','latex')
ylabel("$Temperature $[K]","FontSize",15,'Interpreter','latex')
legend(["Central Differences","Orthogonal Correction","High Resolution"],'Interpreter','latex',"FontSize",14,'location',"northeast");
lgd = legend;
lgd.NumColumns = 1;
%saveas(fig,"figures/GradientMethodComparative_CDS_OC",'epsc');


