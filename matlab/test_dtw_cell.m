function test_dtw_cell(data_set, train_range, repeats, k_range, weight)

if(~exist('weight'))
    weight = false;
    if(~exist('k_range'))
    k_range = 1;
    end
end

if max(train_range)>size(data_set,1)
    error("train_range bigger than data set!");
end

if min(train_range) < max(k_range)
    error("k_range bigger than data set!");
end

problem_size = length(k_range)*length(train_range)*repeats;
progress_bar = waitbar(0, 'Initialising');
counter = 0;
timer = 0;


for k = k_range
    for j = 1:repeats
        tic;
        for i = train_range
            accuracy(i,j,k) = dtw_classifier_cell(data_set,i,k,weight);
            counter = counter+1;
            progress = counter/problem_size;
            time_remaining = floor(timer*(problem_size-counter)/repeats);
            s = mod(time_remaining, 60);
            time_remaining = (time_remaining -s)/60;
            m = mod(time_remaining,60);
            h = floor((time_remaining-m)/60);
            wait_text = sprintf('Testing in progress. \n Time Remaining: %i hrs, %i mins and %i sec',h,m,s);
            waitbar(progress,progress_bar, wait_text);
        end
        timer = toc;
    end
end

close(progress_bar);

figure
hold on

for k = k_range
    mean_accuracy = mean(accuracy(:,:,k)');
    err = std(accuracy(:,:,k)');
    errorbar(train_range,mean_accuracy(train_range),err(train_range),'*');
    legend_text(find(k==k_range)) = sprintf("k = %i",k);
    title('Dynamic Time Warping');
    xlabel('Template Library Size');
    ylabel('Accuracy (%/100)');
end

legend(legend_text);
