function test_dtw(data_set, train_range, repeats, k_range)

if(~exist('k_range'))
    k_range = 1;
end

for k = k_range
    for i = train_range
        for j = 1:repeats
            accuracy(i,j,k) = dtw_classifier(data_set,i,k);
        end
    end
end

figure
hold on

for k = k_range
    mean_accuracy = mean(accuracy(:,:,k)');
    plot(mean_accuracy);
end
