function [accTable]  = table1()
clear; 
% [res, prf, prf2] = plotter2();  
% save wsdmRPRF res prf2; 

load wsdmRPRF; 
facetData; 
predTasks = {}; 
trueTasks = {}; 
P =[]; 
clwnR = []; 
for i=1:length(res) 
    if(strcmp(res(i).legend , 'CL WN'))
        clwnR = [clwnR; res(i)]; 
    end
end
accTable = []; 
for i=1:length(res)
    if(strcmp(res(i).legend , 'CL ') == 0)
    if((res(i).P == 0.1) || (res(i).P==0.3) || (res(i).P == 0.5) || (res(i).P == 0.9))
        csum = res(i).accuracy; 
        cacc =mean(csum * 100.0)';
        accTable = [accTable cacc]; 
    end
    end
end
acc2 = accTable(2:10, :)
matrix2latex(acc2, 'table1.tex', 'rowLabels', pcAttrNames, 'format', '%0.2f'); 
end
