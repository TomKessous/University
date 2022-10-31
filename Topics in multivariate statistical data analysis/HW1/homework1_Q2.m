I = imread('Castle.jpg');           %load image
I = double(rgb2gray(I));            %Convert to gray scale
mu = (mean(I.').')*ones(1,3872);    %Expected value
figure(1);
imshow(uint8(I));                   %Original Image
I = I - mu;                         %Subtracting the mu
sigma_x = cov(I.');                 %Cov matrix
[U,S,V] = svd(sigma_x);             %Eigen vectors matrix
compress_ratio = cumsum(diag(S))/sum(diag(S));
figure(2);
plot(compress_ratio);
title("Compress ratio");
xlabel("Number of eigen values");
ylabel("Ratio");
M95 = 52;             %0.95 reach with the first 52 eigen values
M65 = 3;              %0.65 reach with the first 3 eigen values
U95 = U(1:2592,1:M95);           %The transform matrix for 0.95
U65 = U(1:2592,1:M65);           %The transform matrix for 0.65
I95 = U95.'*I;                   %apply the transform
I65 = U65.'*I;                   %apply the transform
R95 = U95*I95 + mu;              %Reverse the transform
R65 = U65*I65 + mu;              %Reverse the transform
figure(3);
imshow(uint8(R95));
figure(4);
imshow(uint8(R65));