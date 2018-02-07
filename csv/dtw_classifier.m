function accuracy = dtw_classifier(data_set, train_size, k, weight, selective)
%data_set - dataset to use for training/testing
% train_size - number of samples used in training
% k - k-nearest neighbours classifier
% weight - bool to select to use weighted k-nearest neighbours alg
% selective - select optimal train_set by using keep one out validation

if(~exist('k'))
    k = 1;
elseif(~exist('weight'))
    weight = false;
elseif(~exist('selective'))
    selective = false;
end


data_size = size(data_set,2);
test_size = data_size - train_size;
if(k > train_size)
    error("Train set smaller than k");
end

shuffled_indices = randperm(data_size,data_size);

train_indices = shuffled_indices(1:train_size);
test_indices = shuffled_indices(train_size+1:end);

correct = 0;
for i = 1:length(test_indices)
    for j = 1:length(train_indices)
        dist(i,j) = dtw(data_set{test_indices(i)}(:,2:15)',data_set{train_indices(j)}(:,2:15)');
    end
    [sorted_dist sorted_indices] = sort(dist(i,:));
    for K = 1:k
        vote(K) = data_set{train_indices(sorted_indices(K))}(1,16);
    end
    pred_class(i)=mode(vote);
    true_class(i)=data_set{test_indices(i)}(1,16);
    if(pred_class(i) == true_class(i))
        correct = correct+1;
    end
end

accuracy = correct/test_size;


