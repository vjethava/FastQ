function [ctPrf] = table2(a)
if nargin < 1
    a = 2; % select training data 30% 
end
load wsdmRPRF;
ctPrf = getPRF(prf2, a, 3, 2); % select model CL WN, attribute TASK
load p3mod.mat; 
taskTable = [p3mod(:, 2) p3mod(:, 1) ctPrf.prec' ctPrf.recall'  ctPrf.f1score']; 
colLegend = {'True'; 'Predicted'; 'Precision'; 'Recall'; 'F1 score'} ;
rowLegend = {'Informational'; 'Not Informational'; 'Both'};
matrix2latex(taskTable, 'table2.tex', 'rowLabels', rowLegend, 'columnLabels', colLegend, 'format', '%0.2f'); 
save taskTable.mat taskTable;