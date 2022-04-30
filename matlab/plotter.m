%%

data_uds = csvread("static_solutionUDS.txt",1);
data_cds = csvread("static_solutionCDS.txt",1);
data_pl = csvread("static_solutionPOWERLAW.txt",1);
data_soup = csvread("static_solutionSOUP.txt",1);

x=data_uds(:,1);
y=data_uds(:,2);
T_uds=data_uds(:,3);
T_cds=data_cds(:,3);
T_pl=data_pl(:,3);
T_soup=data_soup(:,3);

T=T_uds;
color = colormap;
maxt = max(T);
mint = min(T);
diff = max(maxt-mint,1);
colors=round((T-mint)/diff*255);

figure

scatter(y,T_uds)
hold on
scatter(y,T_cds)
scatter(y,T_pl)
scatter(y,T_soup)
legend("UDS","CDS","POWER-LAW","SOUP")
%%
[sy,sind]=sort(y);
lasty=[sy(1)];
ind=[1];
cont=1;
for(i=1:length(sy))
    if(lasty~=sy(i))
        cont=cont+1;
        ind(cont)=i;
        lasty=sy(i);
    end  
end
y=sy(ind);
T_uds=T_uds(sind);
T_uds=T_uds(ind);

T_cds=T_cds(sind);
T_cds=T_cds(ind);

T_pl=T_pl(sind);
T_pl=T_pl(ind);

T_soup=T_soup(sind);
T_soup=T_soup(ind);


figure

scatter(y,T_uds)
hold on
scatter(y,T_cds)
scatter(y,T_pl)
scatter(y,T_soup)
legend("UDS","CDS","POWER-LAW","SOUP")
%%
scatter3(x,y,T_uds)
hold on 
%scatter3(x,y,T_cds)
%scatter3(x,y,T_pl)
scatter3(x,y,T_soup)

legend("UDS","SOUP")

%%
%hold on
f = figure;

xv=data(:,4:6)';%x coordinates of vertices
yv=data(:,7:9)';%y coordinates of vertices
zv=[T';T';T'];
patch(xv,yv,colors,'EdgeColor','none');
colorbar

xlabel("X")
ylabel("Y")



%set(h, 'ylim', [mint maxt])
saveas(f,"Results.jpg")
