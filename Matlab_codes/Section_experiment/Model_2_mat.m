clear;
N = 2^19:2^18:2^24;
for i=1:length(N)
F=@(x)((exp(-(x-N(i).*2.^(-17)).^2./(N(i).*2.^(-16))))./(sqrt(N(i).*pi.*2.^(-16))));
P(i)=1-(integral(F,-inf,8));
Q(i)=1-(integral(F,-inf,16));
O(i)=1-(integral(F,-inf,24));
R(i)=1-(integral(F,-inf,32));
end 
X=log2(N);  
plot(X,P,'g','Linewidth',1.7);
hold on;
plot(X,Q,'--r','Linewidth',1.3);
plot(X,O,'-.b','Linewidth',1.3);
plot(X,R,':m','Linewidth',1.3);
ylim([0 1]);
legend('T=8','T=16','T=24','T=32');
set(gcf, 'PaperPosition', [-0.75 0.6 40 24]);
set(gcf, 'PaperSize', [39 24]);
saveas(gcf,'D:\1A所有文件\Model_2_mat.pdf')