function [fig] = plot2d_mesh(mesh_type,face_interpolation,legend_location)
%mesh_type = 0 tri, 1 quad
title_mesh = getMeshElementName(mesh_type);

title_face = getFaceInterpolationName(face_interpolation);
        
colors=viridis(6);

fig=figure('Renderer', 'painters', 'Position', [10 10 700 500]);
hold on
grid on
legend_name={};
for i=1:6
     n_cell = 4^i;
    data=loadLineData(mesh_type,face_interpolation,n_cell)
    x=data(:,1);
    T=data(:,2);
    plot(x,T,'linewidth',2,'color',colors(i,:));
    legend_name{i}=num2str(n_cell);
end
figure_title=strcat(title_face," ",title_mesh);
title(strcat("\textbf{",figure_title,"}"),"FontSize",14,'Interpreter','latex');
xlim([-0.01,0.11]);
ylim([250,650]);
xlabel("$Position Y $[m]","FontSize",15,'Interpreter','latex')
ylabel("$Temperature $[K]","FontSize",15,'Interpreter','latex')
legend(legend_name,'Interpreter','latex',"FontSize",14,'location',legend_location);
lgd = legend;
lgd.NumColumns = 1;
saveas(fig,strcat("figures/","MeshComparative_",strrep(figure_title," ","_")),'epsc')
end

