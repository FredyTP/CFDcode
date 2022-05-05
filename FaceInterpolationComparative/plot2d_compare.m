function [fig] = plot2d_compare(mesh_type,mesh_size,compare_method,show_analitic,legend_location)
%mesh_type = 0 tri, 1 quad

title_mesh=getMeshElementName(mesh_type);
colors=viridis(length(compare_method)+show_analitic);
marker_size=8;
fig=figure('Renderer', 'painters', 'Position', [10 10 700 500]);
hold on
grid on
legend_name={};

for i=1:length(compare_method)
    n_cell = 4^mesh_size;   
    data=loadLineData(mesh_type,compare_method(i),n_cell);
    x=data(:,1);
    T=data(:,2);
    plot(x,T,'linewidth',2,'color',colors(i,:));
    legend_name{i}=getFaceInterpolationName(compare_method(i));
end
if(show_analitic==1)
    data=csvread("Center_Line_Temperature_Stationary_Second_Order_Upwind_QUAD_262144.csv",1);
    x=data(:,1);
    T=data(:,2);
    plot(x,T,'linewidth',2,'color',colors(length(compare_method)+1,:));
    legend_name{length(compare_method)+1}="High Resolution";
end
figure_title=strcat("Comparativa para ",num2str(n_cell)," ",title_mesh);
title(strcat("\textbf{",figure_title,"}"),"FontSize",14,'Interpreter','latex');
xlim([-0.01,0.11]);
ylim([250,650]);
xlabel("$Position Y $[m]","FontSize",15,'Interpreter','latex')
ylabel("$Temperature $[K]","FontSize",15,'Interpreter','latex')
legend(legend_name,'Interpreter','latex',"FontSize",14,'location',legend_location);
lgd = legend;
lgd.NumColumns = 1;
%saveas(fig,strcat("figures/","FaceInterpolationComparative_",strrep(figure_title," ","_")),'epsc')
end


