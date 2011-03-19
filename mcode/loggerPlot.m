function []=loggerPlot()
%if nargin < 1
%    csvFileName = '../logger.csv'; 
%end
clear; 
ptM = 'osv<>ph+'; 
colM = 'rgbymckw';
fsize =15;
lwidth = 2; 
msize = 5;
res_dir = '~/Code/FastQv2/';
out_dir = '~/Temp'; 
csvFileName = sprintf('%s/logger.csv', res_dir); 
% P = unique(load(pFileName)); 
P = [0.01; 0.03; 0.05; 0.10; 0.30; 0.50]; 
facetData; 
raw_data = csvread(csvFileName); 
model = raw_data(:, 1);
p = raw_data(:, 2); 
num_errors = raw_data(:, 3:12); 
num_errors_normalized = num_errors ./ (sum(num_errors, 2) * ones(1, 10));
hamming_error = cumsum(num_errors_normalized, 2); 
accuracy = raw_data(:, 13:22); 
h_limits = [0:9]; 
word_stats = raw_data(:, 23:25);  

% for j=1:length(P)
%     hp_mu = [];
%     hp_sigma = [];
%     figure; hold on; grid on; 
%     set(gca, 'FontSize', fsize); 
%     set(gca, 'YLim', [0 1.0]);
%     set(gca, 'YTick', [0:0.1:1.0]);
%     set(gca, 'YTickLabel', [0:10:100]); 
%     set(gca, 'XLim', [0 numAttributes]);
%     set(gca, 'XTick', [0:1:numAttributes]); 
%     set(gca, 'XTickLabel', [0:1:numAttributes]);
%     for i=0:3 % for each model type
%         % the indices where prob. is the right value 
%         pj = find(p == P(j)); 
%         % the indices where the right model. 
%         mi = find(model == i); 
%         % joint conditions - multiple experiments
%         idx = find((model == i) .* (p == P(j)));
%         hj_idx = hamming_error(idx, :); 
%         hj_mu = mean(hj_idx, 1); 
%         hj_sigma = sqrt(var(hj_idx, 0, 1));
%         % join for all models
%         hp_mu = [hp_mu; hj_mu]; 
%         hp_sigma = [hp_sigma; hj_sigma]; 
%         mark = sprintf('%c-%c', ptM(i+1), colM(i+1));
%         errorbar(h_limits, hj_mu, hj_sigma, ...
%             mark, 'LineWidth', lwidth, 'MarkerSize', msize);      
%     end
%     title(sprintf('%d %% training data', 100*P(j)), 'FontSize', fsize);   
%     ylabel('Cumulative %', 'FontSize', fsize);
%     xlabel('Maximum no. of errors', 'FontSize', fsize);
%     legend('BASE', 'WN', 'CL', 'CLWN', 'Location', 'SouthEast'); 
%     s = sprintf('%s/hammingP0x%d', out_dir, floor(100*P(j)));
%     % saveas(gcf, s, 'fig');
%     print(gcf, '-dpng', s);
%     pause(1);
%     close(gcf); 
% end
% 
% 
% K = 10; 
% fig_handles = 1:K;
% for k=1:K
%     figure(k); 
%     
%     if(k==1)
%         name = 'overall';
%     else
%         name = attrNames{k-1};
%     end
%     
%     hold on; grid on; 
%     pMax = max(P); 
%     set(gca, 'FontSize', fsize); 
%      set(gca, 'YLim', [0 100]);
%      set(gca, 'YTick', [0:10:100]);
%      set(gca, 'YTickLabel', [0:10:100]); 
%      set(gca, 'XLim', [0 pMax]);
%      set(gca, 'XTick', P); 
%      set(gca, 'XTickLabel', floor(P*100.0));
% end
% last_acc = [];
% for i=0:3
%     p_acc_mu = [];
%     p_acc_sigma = [];
%     for j=1:length(P)    
%         % the indices where prob. is the right value 
%         pj = find(p == P(j)); 
%         % the indices where the right model. 
%         mi = find(model == i); 
%         % joint conditions - multiple experiments
%         idx = find((model == i) .* (p == P(j)));
%         acc_idx = accuracy(idx, :); 
%         acc_mu = mean(acc_idx, 1); 
%         acc_sigma = sqrt(var(acc_idx, 0, 1));
%         % accumulate the probabilities since we want to join later
%         p_acc_mu = [p_acc_mu; acc_mu]; 
%         p_acc_sigma = [p_acc_sigma; acc_sigma]; 
%     end
%     for k=1:K
%         mark = sprintf('%c-%c', ptM(i+1), colM(i+1));
%         figure(fig_handles(k)); 
%         m_facet = p_acc_mu(:, k);
%         v_facet = p_acc_sigma(:, k); 
%         errorbar(gca, P', m_facet', v_facet', ...
%             mark, 'LineWidth', lwidth, 'MarkerSize', msize);       
%     end
% end
% last_acc= mean(p_acc_mu, 1); 
% for k=1:K
%     figure(fig_handles(k));
%     if last_acc(k) > 50.0
%         location = 'SouthEast';
%     else
%         location = 'NorthEast';
%     end
%     legend('BASE', 'WN', 'CL', 'CLWN' , 'Location', location); 
%     if(k==1)
%         name = 'overall';
%     else
%         name = attrNames{k-1};
%     end
%     title(sprintf('%s', name), 'FontSize', fsize); 
%     ylabel('% accuracy', 'FontSize', fsize);
%     xlabel('% training data', 'FontSize', fsize);
%     s = sprintf('%s/acc_%s', out_dir, name); 
%     % saveas(gcf, s, 'fig');
%     print(gcf, '-dpng', s);
%     pause(1);
%     close(gcf); 
% end



%% also take into account logger2.csv
P = [0.01; 0.03; 0.05; 0.10; 0.3;0.5;0.7; 0.9;0.95;0.99];
raw1 = csvread('~/Code/FastQv2/logger.csv'); 
raw2 = csvread('~/Code/FastQv2/logger2.csv'); 
raw_data = [raw1; raw2];
model = raw_data(:, 1);
p = raw_data(:, 2); 
num_errors = raw_data(:, 3:12); 
num_errors_normalized = num_errors ./ (sum(num_errors, 2) * ones(1, 10));
hamming_error = cumsum(num_errors_normalized, 2); 
accuracy = raw_data(:, 13:22); 
h_limits = [0:9]; 
word_stats = raw_data(:, 23:25);  
p_wn_sigma = [];
p_wn_mu = []; 

figure;
hold on; grid on; 
set(gca, 'FontSize', 10); 
% set(gca, 'YLim', [0 1.0]);
% set(gca, 'YTick', [0:0.10:1.00]);
% set(gca, 'YTickLabel', [0:10:100]); 
set(gca, 'XLim', [0 1.00]);
set(gca, 'XTick',P); 
set(gca, 'XTickLabel', floor(P*100.0));
for j=1:length(P)
    % the indices where prob. is the right value
    pj = find(p == P(j));
    % the indices where the right model.
    mi = find((model == i) + (model == 3));
    % joint conditions - multiple experiments
    idx = find(((model == 1)) .* (p == P(j)));
    words_idx = word_stats(idx, :)
    % wn_idx = words_idx(:, 2)./words_idx(:, 1); 
    wn_idx = [words_idx(:, 1) (words_idx(:, 1) - words_idx(:, 2))]; 
    wn_mu = mean(wn_idx, 1);
   %  keyboard; 
    X = P(j)*ones(length(idx), 1);
    % plot(X, words_idx(:, 1), ...
    %     'bh', 'MarkerSize', msize*1.5, 'MarkerFaceColor', [0 0 1]);
    % plot(X, words_idx(:, 1) - words_idx(:, 2), ...
    %     'gp', 'MarkerSize', msize*1.5, 'MarkerFaceColor', [0 1 0] );
    wn_sigma = sqrt(var(wn_idx, 0, 1));
    % cumulate the probabilities since we want to join later
    p_wn_mu = [p_wn_mu; wn_mu];
    p_wn_sigma = [p_wn_sigma; wn_sigma];    
    % plotyy(X, words_idx(:, 1:2), P(j), wn_mu*100.0, @aplot1, @aplot2);
end
errorbar(P', p_wn_mu(:, 1)', p_wn_sigma(:, 1)', ...
             'bh-', 'LineWidth', lwidth, 'MarkerSize', msize, 'MarkerFaceColor', [0 0 1]);
errorbar(P', p_wn_mu(:, 2)', p_wn_sigma(:, 2)', ...
             'gp-', 'LineWidth', lwidth, 'MarkerSize', msize, 'MarkerFaceColor', [0 1 0]);    

legend('New Words', 'Found in WN'); 
xlabel('% of training data', 'FontSize', fsize);
ylabel('Number of words', 'FontSize', fsize);
s = sprintf('%s/wn', out_dir); 
print(gcf, '-dpng', s); 
close(gcf); 
% figure(14); 
% hold on; grid on; 
% set(gca, 'FontSize', fsize); 
% set(gca, 'YLim', [0 1.0]);
% set(gca, 'YTick', [0:0.10:1.00]);
% set(gca, 'YTickLabel', [0:10:100]); 
% set(gca, 'XLim', [0 1.00]);
% set(gca, 'XTick',P); 
% set(gca, 'XTickLabel', floor(P*100.0));
% errorbar(P, p_wn_mu, p_wn_sigma, 'r-*', 'LineWidth', lwidth, 'MarkerSize', msize*1.5, 'MarkerFaceColor', [1 0 0]);
% 

save(sprintf('%s/loggerPlotData.mat', out_dir)); 
end

function [a]=aplot1(X, Y) 
   
    plot(X, Y(:, 1), 'bh', 'MarkerSize', 10, 'MarkerFaceColor', [0 0 1]);
    a= plot(X, Y(:, 2), 'gp', 'MarkerSize', 10, 'MarkerFaceColor', [0 1 0]);
end

function [a] = aplot2(X, Y)    
    a = plot(X, Y, ...
        'r-*', 'MarkerSize', 10, 'MarkerFaceColor', [1 0 0]);
end