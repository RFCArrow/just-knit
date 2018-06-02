data_1 = prepare_data('knit_to_main','knit','to','rob');
data_2 = prepare_data('knit_from_main','knit','from','rob');
data_3 = prepare_data('purl_to_main','purl','to','rob');
data_4 = prepare_data('purl_from_main','purl','from','rob');
data_5 = prepare_data('megan_knit_from_main','knit','from','megan');


data_set = [data_1; data_2; data_3; data_4; data_5];

%Define various channels
periph_quat = [2 3 4 5];
periph_acc  = [6 7 8];
main_quat   = [9 10 11 12];
main_acc    = [13 14 15];


%Initial Test
%test_dtw_cell(data_set,[10:10:200], 10, [1 2 3 4]);

%Add weighting
%Weighting can't be inverse of sorted index because the closest match will
%never be out voted
%test_dtw_cell(data_set,[10:10:50], 10, [1 2 3 4], true);

%Try inverse or gaussian kernel?

%Select templates by find the one that give the highest accuracy for the
%training set

%Select templates that describe the wides breadth of the training set

%Try something like PCA to maximise variation representation

%Could use clustering to try and represent different types of knits


%FFT feature extraction?


%Extract rate of change of quarternion?


%Try justs acc
test_dtw_cell(data_set,[10:10:50], 5, [1 2], false, [periph_acc main_acc]);


%Try just quarterion
test_dtw_cell(data_set,[10:10:50], 5, [1 2], false, [periph_quat main_quat]);


%Try just main needle
data_set = [data_1; data_3];
test_dtw_cell(data_set,[10:20:150], 5, [1], false, [periph_quat periph_acc]);
%test_dtw_cell(data_set,[10:10:50], 5, [1 2], false, [main_quat main_acc]);

%Try main needle with just acc
data_set = [data_1; data_3];
test_dtw_cell(data_set,[10:10:50], 5, [1 2], false, [periph_acc]);
test_dtw_cell(data_set,[10:10:50], 5, [1 2], false, [main_acc]);

%Try main needle with just quat (to data
%Quat only works MUCH better than Acc only
%Quat only seems to work much 
data_set = [data_1; data_3];
test_dtw_cell(data_set,[10:10:150], 20, [1 2], false, [periph_quat]);
test_dtw_cell(data_set,[10:10:150], 20, [1 2], false, [main_quat]);

%Try other way round (from data)
data_set = [data_2; data_4];
test_dtw_cell(data_set,[10:10:150], 10, [1], false, [periph_quat]);

test_dtw_cell(data_set,[10:10:150], 10, [1], false, [main_quat]);

