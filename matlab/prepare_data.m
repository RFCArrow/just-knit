function out_data = prepare_data(folder_name, stitch, direction, knitter)

prev_dir = pwd;

if ispc
    root_dir = 'C:\Users\Rob\Documents\just-knit\csv\';
    cd(char(strcat(root_dir,folder_name,'\')))
    list = ls;
    files = string(list(3:end,:));
elseif isunix
    root_dir = "/home/rob/just-knit/csv/"  ;
    cd(char(strcat(root_dir,folder_name,'/')));
    files=strsplit(ls);
    files = files(1:end-1);
end

for i = 1:(length(files))
    table = readtable(files{i},'ReadVariableNames',false,'Format','%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f'); 
    table = rmmissing(table(:,1:16),1);
    array = table2array(table(:,1:16));
    
    transitions = find( diff(array(:,16)==1) | diff(array(:,16)==-1) );
    transitions = transitions+1;
    
    if transitions(1) ~= 1
        transitions = [1; transitions];
    end
    
    if transitions(end) ~= size(array,1)+1
        transitions = [transitions; size(array,1)+1];
    end
    
    Cell = mat2cell(array,diff(transitions),16);
    
    if i == 1
        data = Cell;
    else
        data = [data; Cell];
    end
end
    

isNotStitch = find(cellfun( @(x) x(1,16), data(:,1) ) == 0);
isStitch =  find(cellfun( @(x) x(1,16), data(:,1) ) == 1);


data(isNotStitch,2) = cellstr('none');
data(isStitch,2) = cellstr(stitch);

data(:,3) = cellstr(direction);
data(:,4) = cellstr(knitter);
   
cd(prev_dir);
out_data = data;
end
    