function accuracy = lstm_classifier(data, train_size)

%Split data into train and test data
data_size = size(data_set,2);
test_size = data_size - train_size;

shuffled_idx = randperm(data_size,data_size);

train_idx = shuffled_idx(1:train_size);
test_idx = shuffled_idx(train_size+1:end);

for idx = train_idx
    train_set = data{idx}
end

for idx = test_idx
    test_set = data{idx}
end

%Create and train network
net = train_lstm(train_set);

%
accuracy = test_lstm(net, test_set)
