function [] = plot_result(filepath,imagepath,Rx,Ry)
%%
if ~exist('Rx','var')
     % third parameter does not exist, so default it to something
      Rx = 1920;
end
 if ~exist('Ry','var')
     % third parameter does not exist, so default it to something
      Ry = 1080;
 end
 
data = csvread(filepath,1);
T=data(:,3);

f = figure('Position',[0 0 Rx Ry],'visible','off');

color = colormap;
maxt = max(T);
mint = min(T);
diff = max(maxt-mint,1);
colors=round((T-mint)/diff*255);

xv=data(:,4:6)';%x coordinates of vertices
yv=data(:,7:9)';%y coordinates of vertices
patch(xv,yv,colors,'EdgeColor','none');
xlabel("X")
ylabel("Y")
h = colorbar;
set(h, 'ylim', [mint maxt])
saveas(f,imagepath)

end

