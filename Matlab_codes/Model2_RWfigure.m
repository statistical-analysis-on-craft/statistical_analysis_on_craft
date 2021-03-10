clear;
N=2^58;
x=(0:50)
R_E=N*2^(-54)
R_W=N*2^(-56)
R=(exp(-(x-2*N*2^(-54)).^2/(64)))/(8*sqrt(pi));
W=(exp(-(x-2*N*2^(-56)).^2/(16)))/(4*sqrt(pi));
plot(x,R,'--r','Linewidth',1.7)
hold on;
plot(x,W,':g','Linewidth',1.7)
legend('Correct key','Wrong key');
set(gcf, 'PaperPosition', [-0.75 0.2 26.5 15]);
set(gcf, 'PaperSize', [25 23]);
saveas(gcf,'D:\1A所有文件\Model_2_sum.pdf')
