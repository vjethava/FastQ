function []=clplot2(mystr, results)
fsize = 15; 
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
subplot(2, 1, 1); 
set(gca, 'FontSize', fsize); 
bar(lims, hcounts); 
grid on;
t = sprintf('Hamming error of Q_{true} vs Q_{pred}'); 
title(t);
xlabel('number of errors', 'FontSize', fsize); 
ylabel('% of samples', 'FontSize', fsize);
subplot(2, 1, 2); 
bar([1:9]', hacc); 
t  = sprintf('Accuracy of individual facets');
grid on; 
title(t, 'FontSize', fsize); 
set(gca, 'FontSize', fsize); 
xlabel('Facet', 'FontSize', fsize);
ylabel('Accuracy', 'FontSize', fsize); 
mystr2 = sprintf('%s', mystr);
print(gcf, '-dpng', mystr);
legend('CL', 'WORDS', 'WORDS+WN', 'CL+WORDS', 'CL+WORDS+WN');
saveas(gcf, mystr, 'fig');
