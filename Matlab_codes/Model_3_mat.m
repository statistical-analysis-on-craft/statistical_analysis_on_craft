clear;
N = 2^19:2^8:2^24;
for i=1:length(N)
F=@(x,y)(exp(sqrt(x.*y)+sqrt(y-x.*y))+exp(sqrt(x.*y)-sqrt(y-x.*y))+exp(-sqrt(x.*y)+sqrt(y-x.*y))+exp(-sqrt(x.*y)-sqrt(y-x.*y))).*exp((-(N(i).^2).*2^(-35)-y)./(N(i)*2^(-17)))./(N(i)*2^(-15).*pi.*sqrt(x.*(1-x)));
P(i)=1-(integral2(F,0,1,0,32));
Q(i)=1-(integral2(F,0,1,0,128));
O(i)=1-(integral2(F,0,1,0,288));
R(i)=1-(integral2(F,0,1,0,512));
end;
X=log2(N);  
plot(X,P,'g','Linewidth',1.7);
hold on;
plot(X,Q,'--r','Linewidth',1.3);
plot(X,O,'-.b','Linewidth',1.3);
plot(X,R,':m','Linewidth',1.3);
ylim([0 1]);
legend('T=32','T=128','T=288','T=512');
set(gcf, 'PaperPosition', [-0.75 0.6 40 24]);
set(gcf, 'PaperSize', [39 24]);
saveas(gcf,'D:\1A所有文件\Model_3_mat.pdf')

