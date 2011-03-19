%%% plotter.m ---
%%
%% Filename: plotter.m
%% Description:
%% Author: Vinay Jethava
%% Created: Wed Apr 21 17:46:54 2010 (+0200)
%% Last-Updated: Thu Apr 29 18:11:40 2010 (+0200)
%%           By: Vinay Jethava
%%     Update #: 84
%% URL:
%% Keywords:
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
%%% Code:
function [results] = plotter(fname1)
    if nargin < 1
        fname1 = 'trial'; 
    end
    exps = load('../T.txt');
    load('../P2.txt'); 
    load('../mylegend.txt');
    lstr = getLgnd(mylegend); 
    results = [];
    assert(size(mylegend, 1) == length(exps));
    for i=1:length(exps)
        exp = exps(i);
        disp(exp);
        result = loadRes(exp);
        result.legend = lstr(i);
        result.P = P2(i);
        results = [results; result];
        fname2 = sprintf('%s_%d', fname1, mylegend(i));
        % plotPR(result, fname2, 9);
        % plotFacets(result, fname2);
    end
    hold; 
    %    keyboard;
    clplot2(fname1, results, lstr);
end

function [] = plotPR(result, fname1, attrNum)
    facetData; 
    cnumStates = attrsNumVals{attrNum} ;
    [p, r, f] = getTopicF1(result, attrNum, cnumStates);
    fig0 = figure;
    for i=1:cnumStates
        hold on;
        s = sprintf('%s', attributes{attrNum}{i} );
        plot(p(i), r(i), '*');
        text(p(i)+0.03, r(i)+0.03, s);
        %      legend(str(i));
    end
    s = sprintf('%s_pr' , fname1);
    saveas(gcf, s, 'fig');
    print(gcf , '-dpng', s);
    close(gcf); 
end

function [] = plotFacets(result, fname1)
    fsize = 10;
    facetData;
    fig1 = figure;
    n1 = ceil(sqrt(numAttributes));
    n2 = floor(sqrt(numAttributes));
    for i=1:(numAttributes-1) % attributes for
        subplot(n1, n2, i);
        grid on;
        cnumStates = attrsNumVals{i};
        thc = zeros(cnumStates, 1);
        ohc =  zeros(cnumStates, 1);
        for j=1:cnumStates
            thc(j) = sum(result.trueX(:, i) == (j-1));
            ohc(j) = sum(result.outX(:, i) == (j-1));
        end
        bar([1:cnumStates] , [ohc thc]);
        set(gca, 'FontSize', fsize*1.5);
        title(attrNames{i}); 
        set(gca, 'XLim', [0 (cnumStates+1)]);
        set(gca, 'XTick', [1:cnumStates]); 
        set(gca, 'XTickLabel', attributes{i}); 
        xticklabel_rotate([], 45, [], 'FontSize', 10); 
    end
    s  = sprintf('%s_attr', fname1); 
    saveas(gcf, s, 'fig');
    print(gcf, '-dpng', s);  
    close(fig1); 
    %% also plot the last separately
    i = 9;
    fig1b = figure;
    cnumStates = attrsNumVals{i};
    thc = zeros(cnumStates, 1);
    ohc =  zeros(cnumStates, 1);
    for j=1:cnumStates
        thc(j) = sum(result.trueX(:, i) == (j-1));
        ohc(j) = sum(result.outX(:, i) == (j-1));
    end
    bar([1:cnumStates] , [ohc thc]);
    title(attrNames{i}); 
    set(gca, 'XLim', [0 (cnumStates+1)]);
    set(gca, 'XTick', [1:cnumStates]); 
    set(gca, 'XTickLabel', attributes{i}); 
    legend('Pred', 'True');
    set(gca, 'FontSize', fsize);
    xlabel('states', 'FontSize', fsize*1.5);
    ylabel('number of samples', 'FontSize' ,fsize*1.5);
    xticklabel_rotate([], 45, [], 'FontSize', 10); 
    s =  sprintf('%s_topic', fname1); 
    saveas(gcf, s , 'fig');
    print(gcf, '-dpng', s); 
    close(gcf); 
end

function [result] = loadRes(exp)
    result = struct('accuracy', [], 'outX', [], 'trueX', [], 'hamming', []);
    name = sprintf('../result/%d/accuracy.txt', exp);
    result.accuracy = load(name);
    name = sprintf('../result/%d/hamming.txt', exp);
    result.hamming = load(name);
    name = sprintf('../result/%d/true.txt', exp);
    result.trueX = load(name);
    name = sprintf('../result/%d/out.txt', exp);
    result.outX = load(name);
end

%%%%%%%%%%%%%%%%%%%%
%%% plotter.m ends here
