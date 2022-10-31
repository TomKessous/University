function plot_sample(image_vector,digit)
image_square = reshape(image_vector,28,28);
figure
imshow(image_square)
title(['Digit:' num2str(digit)])
