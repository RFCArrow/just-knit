function net = train_lstm(data);

%transpose data in data set
data = cellfun(@transpose,data','UniformOutput',false);



%Sort gestures by length

numObs = size(data,1)
for i=1:numObs
    observation = data{i};
    X{i} = observation(2:end-1,:);
    Y(i) = observation(end,1)+1;
    obsLength(i) = size(observation,2);
end


[obsLength,idx] = sort(obsLength);
X = X(idx)';
Y = Y(idx)';

Y = categorical(Y);


miniBatchSize = 5;
miniBatchLocations = miniBatchSize+1:miniBatchSize:numObs;
XLocations = repmat(miniBatchLocations,[2 1]);
YLocations = repmat([0;30],[1 9]);

%Create layers

inputSize = 14;
outputSize = 100;
outputMode = 'last';
numClasses = 2;

layers = [ ...
    sequenceInputLayer(inputSize)
    lstmLayer(outputSize,'OutputMode',outputMode)
    fullyConnectedLayer(numClasses)
    softmaxLayer
    classificationLayer]

%Configure settings

maxEpochs = 150;
shuffle = 'never';

options = trainingOptions('sgdm', ...
    'MaxEpochs',maxEpochs, ...
    'MiniBatchSize',miniBatchSize, ...
    'Shuffle', shuffle);

%Train network

net = trainNetwork(X,Y,layers,options);







