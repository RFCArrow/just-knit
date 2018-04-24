clearvars -except data
for i = 1:size(data,1)-1
    if( data{i,16} ~= data{i+1,16} )
        if(exist('gesture'))
            gesture{end+1}(1,:) = data{i,:};
        else
            gesture{1}(1,:) = data{i,:};
        end
    else 
        if(exist('gesture'))
            gesture{end}(end+1,:) = data{i,:};
        else
            gesture{1}(1,:) = data{i,:};
        end
    end
end


for i = 1:size(gesture,2)
   if(gesture{i}(1,16) == 0)
       if(exist('knit')) 
           knit{end+1} = gesture{i};
       else
           knit{1} = gesture{i};
       end
   else
       if(exist('no_knit'))
           no_knit{end+1} = gesture{i};
       else
           no_knit{1} = gesture{i};
       end
   end
end
   
    