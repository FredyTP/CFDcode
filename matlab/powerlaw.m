lambda=0.5;
peclet_vec=[-0.01:0.001:0.01];
lambda=[0:0.001:1]
for (i=1:10)
    peclet=peclet_vec(i)
    peclet_val = (exp(peclet * lambda) - 1) ./ (exp(peclet) - 1);
    plot(lambda,peclet_val)
    hold on
   
end