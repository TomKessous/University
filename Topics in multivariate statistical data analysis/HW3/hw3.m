% Tom Kessous
%% ---------- Question 1 --------------
N = 1000;
alpha = 0.05;
p = 5;
SNR = -10:5:10;
probabilities = zeros(1, length(SNR));
threshold = norminv(1-alpha,0,sqrt(8*p*(p+2)));
s = randsrc(N,p,[-1 1; 0.5,0.5]);
sigma_W = 10.^(-SNR./10);

parfor i = 1:length(SNR)
    cnt = 0;
    sigma_W = 10^(-SNR(i)/10);
    for j=1:1000
        X = s + randn(N,p)*sqrt(sigma_W);
        sigma_X = cov(X);
        [U,S,V] = svd(sigma_X);
        inv_std_x = U*1/sqrt(S)*U';
        Z = (X-mean(X))*inv_std_x;
        B = (1/N)*sum(sqrt(sum(Z.^2,2)).^4);
        T = sqrt(N)*(B-p*(p+2));
        if abs(T) > threshold
            cnt = cnt + 1;
        end
    probabilities(i) = cnt/1000;
    end
end

figure(1)
plot(SNR,probabilities);
xlabel("SNR [db]");
ylabel("PD")
title("Mardia’s kurtosis test");

%% ---------- Question 4 --------------
N = 1000;
p = 1;
X = randn(N,p);
W = randn(N,p);
Y = cos(X)+W;
Z = [X,Y];
iters = 100;

T_GLRT = (N - (p+p+3)/2).*( log(det(cov(X)) + log(det(cov(Y))) -log(det(cov(Z)))));


[U,S,V] = svd(cov(Z));
W = U/(sqrt(S))*U.';
Z = Z*W.';

h = (4/(p+2))^(1/(p+4))*N^(-1/(p+4));
h_marg = ((4/(p+p+2))^(1/(p+p+4)))*N^(-1/(p+p+4));

f_XY = zeros(N,1);
f_X = zeros(N,1);
f_Y = zeros(N,1);

parfor i = 1:N
    mu = [Z(i,1),Z(i,2)];
    f_X(i) = ksdensity(Z(:,1),mu(1),'kernel','normal','bandwidth',h_marg);
    f_Y(i) = ksdensity(Z(:,2),mu(2),'kernel','normal','bandwidth',h_marg);
    f_XY(i) = sum(mvnpdf(Z,mu,(h^2)*eye(2,2)))/N;
end

MI = abs(mean(log(f_XY./(f_X.*f_Y))));
T_MI = sqrt(1-exp(-2*MI));

T_PERM_MI = zeros(iters,1);
T_PERM_GLRT = zeros(iters,1);
for j = 1:iters
    ind = randperm(N,N);
    Y = Y(ind);
    Z = [X,Y]; 
    parfor i = 1:N
        mu = [Z(i,1),Z(i,2)];
        f_X(i) = ksdensity(Z(:,1),mu(1),'kernel','normal','bandwidth',h_marg);
        f_Y(i) = ksdensity(Z(:,2),mu(2),'kernel','normal','bandwidth',h_marg);
        f_XY(i) = sum(mvnpdf(Z,mu,(h^2)*eye(2,2)))/N;         
    end
    MI_PERM = abs(mean(log(f_XY./(f_X.*f_Y))));
    T_PERM_MI(j) = sqrt(1-exp(-2*MI_PERM));
    
    T_PERM_GLRT(j) = (N - (p+p+3)/2).*( log(det(cov(X)) + log(det(cov(Y))) -log(det(cov(Z)))));
    disp(j)
end

P_VAL_MI = sum(T_PERM_MI > T_MI)/iters;
P_VAL_GLRT = sum(T_PERM_GLRT > T_GLRT)/iters;

disp(P_VAL_MI)
disp(P_VAL_GLRT)
