% =========================================
%       Homework on K-Nearest Neighbors
% =========================================
% Course: Introduction to Information Theory
% Lecturer: Haim H. Permuter.
%
% NOTE:
% -----
% Please change the variable ID below to your ID number as a string.
% Please do it now and save this file before doing the assignment
clear all;
ID = '206018749';

%% Loading and plot a sample of the data
% ---------------------------------------
% The MNIST database contains in total of 60000 train images and 10000 test images
% of handwritten digits.
% This database is a benchmark for many classification algorithms, including neural networks.
% For further reading, visit http://yann.lecun.com/exdb/mnist/
%
% You will implement the KNN algorithm to classify two digits from the database: 3 and 5.
% First, we will load the data from .MAT file we have prepared for this assignment: MNIST_3_and_5.mat
load('MNIST_3_and_5.mat')
% The data is divided into 2 pairs:
% (Xtrain, Ytrain) , (Xvalid, Yvalid)
% In addition, you have unlabeled test sample in Xtest.
%
% Each row of a X matrix is a sample (gray-scale picture) of dimension 784 = 28^2,
% and the digit (number) is in the corresponding row of the Y vector.
%
% To plot the digit, see attached function 'plot_sample.m'
sampleNum = 1902;
plot_sample(Xtest(sampleNum,:),5)


% Build a KNN classifier based on what you've learned in class:
%
% 1. The classifier should be given a train dataset (Xtrain, Ytain),  and a test sample Xtest.
% 2. The classifier should return a class for each row in test sample in a column vector Ytest.
%
% Finally, your results will be saved into a <ID>.txt file, where each row is an element in Ytest.
%
% Note:
% ------
% For you conveniece (and ours), give you classifications in a 1902 x 1 vector named Ytest,
% and set the variable ID at the beginning of this file to your ID.
%========================================================================================================
%========================================================================================================
%========================================================================================================
%This segment validtate if the prediction is correct
Y = zeros(1,1522);      %empty vector.
for i=1:1522            %for all Xvalid predict the digit and store in Y(i)
   Y(i)=Three_NN(Xvalid(i,:),Xtrain,Ytrain);
end
Y = Y';          
dist=abs(Y-Yvalid);  %diffrences
cnt=0;               %counting how much pictures i miss.
for j=1:1522
    if dist(j) ~= 0 
        cnt=cnt+1;
    end
end
%According to the validation segment i get a 1.5 precent miss prediction
%on the valid data base.
%For 1-NN i get 2.5 precet miss prediction and for 10-NN i get 1.5 precent
%miss.
%===========================================================================================================
%Start Prediction for Xtest.
Ytest = zeros(1,1902);
for i=1:1902
   Ytest(i)=Three_NN(Xtest(i,:),Xtrain,Ytrain);
end
Ytest = Ytest';    
% save classification results
disp('saving')
csvwrite([ID '.txt'], Ytest)
disp('done')

function ret = Three_NN(x,y,Ytrain)          %Three nearest neighbor, return value.
min = [99999999,99999998,999999997];                     %min=[min0,min1,min2],Initial value.
tmp = zeros(1,784);                 %Empty vector.
index=[-1,-1,-1];                   %vector Indexs, index=[Idx0,Idx1,Idx2].
for i=1:11552                       %for every picture in Xtrain
    tmp = sum(abs(x-y(i,:)));       
    %caculating the distance between the given picture and the i-th picture in Xtrain pixel by pixel.
    %and then sum all the distances of every pixels.
     l = min==max(min);                 %find the Index of the max value in min vector, store that index at l.
    if tmp<min(l)                      %If the distance is smaller then the max value in the min vector
        min(l)=tmp;                    %Replace them
        index(l)=i;                   %Store the Index of the min distance
       
    end
end
three=0;
five=0;
for k=1:3                   %count how munch five and three.
    if Ytrain(index(k))==3 
        three=three+1;
    else
        five=five+1;
    end
end
if three > five             %If three is the majority then return 3
    ret=3;
else
    ret=5;                  %else return 5
end  
end