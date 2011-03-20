%%% loggerPlot.m --- 
%% 
%% Filename: loggerPlot.m
%% Description: 
%% Author: Vinay Jethava
%% Created: Mon Jan 24 23:53:23 2011 (+0100)
%% Version: 
%% Last-Updated: Mon Jan 24 23:53:41 2011 (+0100)
%%           By: Vinay Jethava
%%     Update #: 1
%% URL: 
%% Keywords: 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% 
%%% Commentary: 
%% 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% 
%%% Change Log:
%% 
%% 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% 
%%% Code:
function []=loggerPlot()
%if nargin < 1
%    csvFileName = '../logger.csv'; 
%end
clear; 
shM = {':', '--', '-.', '-'};
ptM = 'osv<>ph+'; 
colM = 'rgybmckw';
fsize = 20;
lwidth = 6; 
msize = 4;
res_dir = '~/Code/FastQv2/';
out_dir = '../res4'; 
csvFileName = sprintf('%s/logger2.csv', res_dir); 
% P = unique(load(pFileName)); 
P = [0.70; 0.90]; 
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
I = [0; 1; 3]; % not showing CL alone -
L = {'BASE'; 'WN'; 'FastQ'};
% Plotting Hamming
for j=1:length(P)
    hp_mu = [];
    hp_sigma = [];
    figure; hold on; grid on; box on; 
    set(gca, 'FontSize', fsize); 
    set(gca, 'YLim', [0 1.0]);
    set(gca, 'YTick', [0:0.2:1.0]);
    set(gca, 'YTickLabel', [0:20:100]); 
    set(gca, 'XLim', [0 numAttributes]);
    set(gca, 'XTick', [0:1:numAttributes]); 
    set(gca, 'XTickLabel', [0:1:numAttributes]);
    for ii=1:length(I) % for each model type
        i = I(ii); 
        % the indices where prob. is the right value 
        pj = find(p == P(j)); 
        % the indices where the right model. 
        mi = find(model == i); 
        % joint conditions - multiple experiments
        idx = find((model == i) .* (p == P(j)));
        hj_idx = hamming_error(idx, :); 
        hj_mu = mean(hj_idx, 1); 
        hj_sigma = sqrt(var(hj_idx, 0, 1));
        % join for all models
        hp_mu = [hp_mu; hj_mu]; 
        hp_sigma = [hp_sigma; hj_sigma]; 
        mark = sprintf('%c%s%c', ptM(i+1), shM{i+1}, colM(i+1));
        errorbar(h_limits, hj_mu, hj_sigma, ...
            mark, 'LineWidth', lwidth, 'MarkerSize', msize);      
    end
    % title(sprintf('%d %% training data', 100*P(j)), 'FontSize', fsize);   
    ylabel('Cumulative %', 'FontSize', fsize);
    xlabel('Maximum no. of errors', 'FontSize', fsize);
    legend(L, 'Location', 'SouthEast'); 
    s = sprintf('%s/hammingP0x%d', out_dir, floor(100*P(j)));
    % saveas(gcf, s, 'fig');
    print(gcf, '-dpng', s);
    pause(1);
    close(gcf); 
end


% fsize = 25;
% lwidth = 7; 
% msize = 2;
K = 10; 
fig_handles = 1:K;
for k=1:K
    figure(k); 
    
    if(k==1)
        name = 'overall';
    else
        name = attrNames{k-1};
    end
    
    hold on; grid on; box on; 
    pMax = max(P); 
    pGrid = [0.01; 0.05; 0.1; 0.3; 0.5];
    set(gca, 'FontSize', fsize); 
     set(gca, 'YLim', [0 100]);
     set(gca, 'YTick', [0:20:100]);
     set(gca, 'YTickLabel', [0:20:100]); 
     set(gca, 'XLim', [0 pMax]);
     set(gca, 'XTick', pGrid ); 
     set(gca, 'XTickLabel', floor(pGrid*100.0));
end
last_acc = [];
P_ACC_MU = [];
for ii = 1:length(I)
    i = I(ii); 
    p_acc_mu = [];
    p_acc_sigma = [];
    for j=1:length(P)    
        % the indices where prob. is the right value 
        pj = find(p == P(j)); 
        % the indices where the right model. 
        mi = find(model == i); 
        % joint conditions - multiple experiments
        idx = find((model == i) .* (p == P(j)));
        acc_idx = accuracy(idx, :); 
        acc_mu = mean(acc_idx, 1); 
        acc_sigma = sqrt(var(acc_idx, 0, 1));
        % accumulate the probabilities since we want to join later
        p_acc_mu = [p_acc_mu; acc_mu]; 
        p_acc_sigma = [p_acc_sigma; acc_sigma]; 
    end
    for k=1:K
        mark = sprintf('%c%s%c', ptM(i+1), shM{i+1}, colM(i+1));
        figure(fig_handles(k)); 
        m_facet = p_acc_mu(:, k);
        v_facet = p_acc_sigma(:, k); 
        errorbar(gca, P', m_facet', v_facet', ...
            mark, 'LineWidth', lwidth, 'MarkerSize', msize);       
    end
    P_ACC_MU = [P_ACC_MU; p_acc_mu]; 
end
last_acc= mean(p_acc_mu, 1);

for k=1:K
    figure(fig_handles(k));
    if last_acc(k) > 50.0
        location = 'SouthEast';
    else
        location = 'NorthEast';
    end
    legend(L , 'Location', location); 
    if(k==1)
        name = 'overall';
    else
        name = attrNames{k-1};
    end
    %title(sprintf('%s', name), 'FontSize', fsize); 
    ylabel('% accuracy', 'FontSize', fsize);
    xlabel('% training data', 'FontSize', fsize);
    s = sprintf('%s/acc_%s', out_dir, name); 
    % saveas(gcf, s, 'fig');
    print(gcf, '-dpng', s);
    pause(1);
    close(gcf); 
end



%% also take into account logger2.csv
ptM = 'osv<>ph+'; 
colM = 'rgybmckw';
fsize = 20;
lwidth = 6; 
msize = 4;
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
hold on; grid on; box on;
set(gca, 'FontSize', 15); 
% set(gca, 'YLim', [0 1.0]);
% set(gca, 'YTick', [0:0.10:1.00]);
% set(gca, 'YTickLabel', [0:10:100]); 
set(gca, 'XLim', [0 1.00]);
set(gca, 'XTick', [0.01; 0.05; 0.1; 0.3; 0.5; 0.7; 0.9]); 
set(gca, 'XTickLabel', floor([0.01; 0.05; 0.1; 0.3; 0.5; 0.7; 0.9]*100.0));
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
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% loggerPlot.m ends here
