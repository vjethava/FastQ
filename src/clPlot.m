function []=clPlot(my_str, hamming, accuracy)
m = sprintf('%s', my_str); 
fsize = 15; 
h = figure; 
if(size(accuracy, 1) > 1)
    accuracy = sum(accuracy, 1); 
end
subplot(2, 1, 1); 
N = size(hamming, 1); 
accuracy = (accuracy/N)*100.0;  

set(gca, 'FontSize', fsize); 
hist(hamming, [0:1.0:9]); 
grid on;
t = sprintf('Hamming error of Q_{true} vs Q_{pred} for %s (%d trials)', my_str, N); 
title(t);
xlabel('Number of errors', 'FontSize', fsize); 
ylabel('Number of samples', 'FontSize', fsize);
subplot(2, 1, 2); 
bar([1:9], accuracy(2:10)); 
t  = sprintf('Accuracy of individual facets');
grid on; 
title(t, 'FontSize', fsize); 
set(gca, 'FontSize', fsize); 
xlabel('Facet', 'FontSize', fsize);
ylabel('Accuracy', 'FontSize', fsize); 
name = sprintf('%s%d', my_str, N); 
print(gcf, '-dpng', name);

