clear;
syms x;
y=(0:2^(-30):10^(-4));
N = 2^22;
W=exp(-(y-2.^(-15)/sqrt(pi)-2.^(-30)).^2/(2.^(-29)*(1-1/pi)))/sqrt(2.^(-29)*(pi-1));
hold on;
plot(y,W,'--r','Linewidth',1.7);
