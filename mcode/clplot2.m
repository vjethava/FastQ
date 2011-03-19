function []=clplot2(mystr, results, lstr)
% facetData = load('facets_matlab.txt');  
facetData; 
lcolors = ['*-r'; '*-g'; '*-b'; '*-y'];
lc2 =['r'; 'g'; 'b'; 'y']; 
fsize = 15; 
lwidth = 5; 
h = figure;
nres = length(results); 
hcounts = []; 
hacc = []; 
lims = [0:1.0:9]; 
for i=1:nres
    if(size(results(i).accuracy, 1) > 1)
        results(i).accuracy = sum(results(i).accuracy, 1); 
    end
    N = size(results(i).hamming, 1);
    locCount = histc(results(i).hamming, lims); 
    locCount = locCount/sum(locCount)*100.0;
    hcounts = [hcounts locCount];
    results(i).accuracy = (results(i).accuracy/N)*100.0;
    locAcc = results(i).accuracy'; 
    hacc = [hacc locAcc(2:10)]; 
end
% make aggregate counts
for i=2:size(hcounts, 1)
    hcounts(i, :) = hcounts(i, :) + hcounts(i-1, :); 
end
subplot(2, 1, 1); 
set(gca, 'FontSize', fsize); 
set(gca, 'YLim', [0 100.01]);
set(gca, 'YTick', [0:20:100]); 
%set(gcf,'DefaultAxesColorOrder',[0 0 1; 0 1 0; 1 0 0; 1 0 1; 0 1 1; 1 1 0 ]);
plot(lims, hcounts, 'o-', 'MarkerSize', 1.5*lwidth, 'LineWidth', lwidth); 
ColorOrd = get(gca, 'ColorOrder');
grid on;
t = sprintf('Hamming error of Q_{true} vs Q_{pred}'); 
title(t);
xlabel('number of errors', 'FontSize', fsize); 
ylabel('% of samples (cumulative)', 'FontSize', fsize);
legend(lstr, 'Location', 'SouthEast');
subplot(2, 1, 2); 
bar([1:9]', hacc); 
set(gca, 'ColorOrder', ColorOrd);
t  = sprintf('Accuracy of individual facets');
grid on; 
title(t, 'FontSize', fsize); 
set(gca, 'FontSize', fsize); 
xlabel('Facet', 'FontSize', fsize);
ylabel('Accuracy', 'FontSize', fsize); 
set(gca, 'XLim', [0 (numAttributes+1)]);
set(gca, 'XTick', [1:numAttributes]); 
set(gca, 'YTick', [0:20:100]);
set(gca, 'XTickLabel', attrNames);
xticklabel_rotate([], 45, [], 'FontSize',fsize);
legend(lstr, 'Location', 'NorthEast');
mystr2 = sprintf('%s', mystr);
print(h, '-dpng', mystr2);
saveas(h, mystr, 'fig');


