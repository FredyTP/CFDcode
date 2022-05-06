function [] = plot_result(filepath,imagepath,division,Rx,Ry)
%%
if ~exist('Rx','var')
     % third parameter does not exist, so default it to something
      Rx = 1920;
end
 if ~exist('Ry','var')
     % third parameter does not exist, so default it to something
      Ry = 1080;
 end
 
  if ~exist('division','var')
     % third parameter does not exist, so default it to something
      division = 8;
 end
 
%% Read FILE DATA
data = csvread(filepath,1);

%% CONFIG COLORMAP AND LIMITS
T=data(:,3);
color = viridis(255);
maxt = max(T);
mint = min(T);
diff = max(maxt-mint,1);
colors=round((T-mint)/diff*255);
Tmin=mint;
Tmax=maxt;
%% OBTAIN VERTEX DATA
dim = size(data);
n_col = dim(2);
max_node = ceil(n_col-3)/2;
xv=data(:,n_col-2*max_node+1:n_col-max_node)';%x coordinates of vertices
yv=data(:,n_col-max_node+1:n_col)';%y coordinates of vertices

%% PLOT CONTOUR
f = figure('Position',[0 0 Rx Ry],'visible','off');
patch(xv,yv,colors,'EdgeColor','none');
colormap(viridis)
xlabel("X")
ylabel("Y")
%% Configure Colorbar
h = colorbar;
h.Ticks = linspace(0, 255, division); %Create 8 ticks from zero to 1
h.TickLabels = linspace(mint,maxt,division);   
%% OBTAIN ASPECT RATIO
minx = min(min(xv));
maxx = max(max(xv));
miny=min(min(yv));
maxy=max(max(yv));
width = maxx-minx;
height = maxy-miny;
aspect_ratio = width/height;
pbaspect([aspect_ratio 1 1]);
%% SAVE IMG
saveas(f,imagepath);

end

