data_1 = prepare_data('knit_to_main','knit','to','rob');
data_2 = prepare_data('knit_from_main','knit','from','rob');
data_3 = prepare_data('megan_knit_from_main','knit','from','megan');
data_4 = prepare_data('purl_from_main','purl','from','rob');
data_5 = prepare_data('purl_to_main','purl','to','rob');

data_set = [data_1; data_2; data_3; data_4; data_5];

diff_data = data_set;
diff_data(:,1) = cellfun( @diff, data_set(:,1), 'UniformOutput', false) 

%Initial Test
%test_dtw_cell(data_set,[10:10:200], 10, [1 2 3 4]);

%Add weighting
%Weighting can't be inverse of sorted index because the closest match will
%never be out voted

%Select templates by find the one that give the highest accuracy for the
%training set

%Select templates that describe the wides breadth of the training set

%Try something like PCA to maximise variation representation

%Could use clustering to try and represent different types of knits


%FFT feature extraction?


%Extract rate of change of quarternion?
test_dtw_cell(diff_data,[10:10:200], 10, 1);


%Try justs acc


%Try just quarterion

%Try just one needle

