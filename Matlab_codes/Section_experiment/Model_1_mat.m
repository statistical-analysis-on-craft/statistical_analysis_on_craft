clear;
N = 2^19:2^18:2^24;
for i=1:length(N)
F=@(x)((exp(-(x-N(i).*2.^(-18)).^2./(N(i).*2.^(-17))))./(sqrt(N(i).*pi.*2.^(-17))));
P(i)=1-(integral(F,-inf,4))^2;
Q(i)=1-(integral(F,-inf,8))^2;
O(i)=1-(integral(F,-inf,12))^2;
R(i)=1-(integral(F,-inf,16))^2;
end 
X=log2(N);  
plot(X,P,'g','Linewidth',1.7);
hold on;
plot(X,Q,'--r','Linewidth',1.3);
plot(X,O,'-.b','Linewidth',1.3);
plot(X,R,':m','Linewidth',1.3);
ylim([0 1]);
legend('T=4','T=8','T=12','T=16');
set(gcf, 'PaperPosition', [-0.75 0.6 40 24]);
set(gcf, 'PaperSize', [39 24]);
saveas(gcf,'D:\1A?????ļ?\Model_1_mat.pdf')