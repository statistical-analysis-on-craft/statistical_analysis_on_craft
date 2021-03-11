clear;
N=2^58;
syms x;
R=@(x)(2.*(exp(-(x-N.*2.^(-54)).^2/(N.*2.^(-53))))/(sqrt(N.*2.^(-53).*pi))).*normcdf((x-16)/4);
W=@(x)(2.*(exp(-(x-N.*2.^(-56)).^2/(N.*2.^(-55))))/(sqrt(N.*2.^(-55).*pi))).*normcdf((x-N*2^(-56))/sqrt(N*2^(-56)));
P=integral(R,0,10.00213);
Q=integral(W,10.00213,inf);
S=P+Q;
fprintf('P = %d\n',P);
fprintf('Q = %d\n',Q);
fprintf('SUM = %d\n',S);