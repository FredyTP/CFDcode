

data50 = csvread("..\\center_line_temp50.csv",1);
data50_nocorr = csvread("..\\center_line_temp50_nocorr.csv",1);
data500 = csvread("..\\center_line_temp500.csv",1);
data1668 = csvread("..\\center_line_temp1668.csv",1);
s50 = data50(:,1);
T50= data50(:,2);
s50_nocorr = data50_nocorr(:,1);
T50_nocorr= data50_nocorr(:,2);
s500=data500(:,1);
T500=data500(:,2);

s1668=data1668(:,1);
T1668=data1668(:,2);
plot(s50,T50)
hold on
plot(s50_nocorr,T50_nocorr)

%plot(s500,T500)

plot(s1668,T1668)

legend("50 corrected","50 no corrected","full data");