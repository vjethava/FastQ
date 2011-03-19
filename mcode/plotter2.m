%%% plotter2.m ---
%%
%% Filename: plotter2.m
%% Description: Plotter for test2() in ../main.cpp
%% Author: Vinay Jethava
%% Created: Thu Apr 29 13:47:07 2010 (+0200)
%% Last-Updated: Thu Apr 29 18:11:39 2010 (+0200)
%%           By: Vinay Jethava
%%     Update #: 55
%% URL:
%% Keywords:
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
%%% Code:

%% PRF2 modification to get in legible format.
function [results, PRF, PRF2] = plotter2(res_dir, out_dir)
    facetData;
    if nargin < 1
        res_dir = '..'; 
    end
    if nargin < 2
        out_dir = '../images'; 
    end
    T = load(sprintf('%s/T.txt', res_dir));
    P = load(sprintf('%s/P.txt', res_dir));
    mylegend = load(sprintf('%s/mylegend.txt', res_dir));
    P2 = load(sprintf('%s/P2.txt', res_dir));
    n1 = length(P);
    assert(length(mylegend) == length(T));
    lstr = getLgnd(mylegend); 
    n2 = length(mylegend)/n1;
    expCounter = 0;
    assert((n1*n2) == length(T));
    results = [];
    resNum = 0; 
    for i1 = 1:n1
        for i2 = 1:n2
            expCounter = expCounter + 1;
            expNum = T(expCounter);
            resNum = resNum + expNum; 
            result = loadRes(expNum, sprintf('%s/result/', res_dir));
            result.legend = lstr(expCounter);
            result.P = P2(expCounter);
            results = [results; result];
        end
    end
    %%% plot the accuracy p-r plot

    PRF = struct('attribute', [], 'prec', [], 'recall', [], 'f1score', [],  'trPct', [], 'legend', []);
    PRF2 = struct('attribute', [], 'nPred', [], 'nTrue', [], 'prec', [], 'recall', [], 'f1score', [],  'trPct', [], 'legend', []);
    prfCounter = 0;
    prf2Counter = 0; 
    for i=1:numAttributes
        cstates = attrsNumVals{i};
        PRF(i).prec = zeros(n1, n2 * cstates);
        PRF(i).recall = zeros(n1, n2 * cstates);
        PRF(i).f1score = zeros(n1, n2 * cstates);
    end
    for i1=1:n1 % training data percentage
        for i2 = 1:n2 % different models - lines in the plot
            prfCounter = prfCounter + 1;
            cres = results(prfCounter);
            
            for i=1:numAttributes
                cstates = attrsNumVals{i};
                cnum = i;
                [p, r, f, nTrue, nPred] = getTopicF1(cres, cnum, cstates);
                % h = figure;
                t1  =(i2-1)*cstates + 1;
                t2 = i2 * cstates;
                PRF(i).prec(i1, t1:t2) = p';
                PRF(i).recall(i1, t1:t2) = r';
                PRF(i).f1score(i1, t1:t2) = f';
                prf2Counter = prf2Counter + 1; 
                PRF2(prf2Counter).prec = p'; 
                PRF2(prf2Counter).recall = r'; 
                PRF2(prf2Counter).f1score = f';
                PRF2(prf2Counter).attribute = i;
                PRF2(prf2Counter).trPct  = P(i1); 
                PRF2(prf2Counter).legend = lstr(i2); 
                PRF2(prf2Counter).nTrue = nTrue;
                PRF2(prf2Counter).nPred = nPred;
            end
        end
    end
    resName = sprintf('%s/R%d.mat', out_dir, mod(resNum,10000));
    save(resName, 'results', 'PRF2'); 
    l0pr  = getLgnd(mylegend(1:n2, :));
    p0pr = P;
    % plotFacet2(PRF, p0pr, l0pr); 
    plotAccuracy2(results, p0pr, l0pr, out_dir); 
    % plotHamming159(results, p0pr, l0pr); 
end



function [hcounts, hacc, lims] = getAcc2(result)
    hcounts = []; 
    hacc = []; 
    lims = [0:1.0:9]; 
    
        if(size(result.accuracy, 1) > 1)
            result.accuracy = sum(result.accuracy, 1); 
        end
        N = size(result.hamming, 1);
        locCount = histc(result.hamming, lims); 
        locCount = locCount/sum(locCount)*100.0;
        hcounts = [hcounts locCount];
        result.accuracy = (result.accuracy/N)*100.0;
        locAcc = result.accuracy'; 
        hacc = locAcc;
    
    % make aggregate counts
    for i=2:size(hcounts, 1)
        hcounts(i, :) = hcounts(i, :) + hcounts(i-1, :); 
    end
end

function [] = plotAccuracy2(results, P, L, out_dir)
    ptM = 'osp^v<>ph+'; 
    colM = 'rgbymckw';
    np = length(P); 
    nl = length(L); 
    facetData ;
    fsize =15;
    lwidth = 3.5; 
    msize = 5;
    acc0 = zeros(np, nl, numAttributes);
    hc0 = zeros(np, nl); 
    for i1=1:np
        % histogram figure
        fig0 = figure; 
        hold on;
        grid on; 
        set(gca, 'FontSize', fsize); 
        set(gca, 'YLim', [0 100.01]);
        set(gca, 'YTick', [0:20:100]); 
        %set(gcf,'DefaultAxesColorOrder',[0 0 1; 0 1 0; 1 0 0; 1 0 1; 0 1 1; 1 1 0 ]);
    
        t = sprintf('Hamming error of Q_{true} vs Q_{pred} (%d%% tr data)', floor(100.0*P(i1))); 
        title(t);
        xlabel('number of errors', 'FontSize', fsize); 
        ylabel('% of samples (cumulative)', 'FontSize', fsize);
         
        for i2=1:nl
            idx = (i1-1)*nl + i2;
            cres = results(idx); 
            [hcounts, hacc, lims] = getAcc2(cres);
            mark = sprintf('%c-%c',ptM(i2), colM(i2));
            plot(lims, hcounts, mark, 'MarkerSize', msize, 'LineWidth', lwidth); 
            for ia=1:numAttributes % plot individual facet accuracies
                acc0(i1, i2,  ia)  = hacc(ia+ 1); 
            end
        end
    
        s = sprintf('%s/hammingP0x%d', out_dir, floor(100*P(i1)));
        legend(L, 'Location', 'SouthEast');
        saveas(gcf, s, 'fig');
        print(gcf, '-dpng', s);
        % close(fig0); 
    end
    
    for i=1:numAttributes
        s = sprintf('%s/acc_%s', out_dir, attrNames{i});
        acc1 = acc0(:, :, i);
        fig1 = figure;
        hold on;
        grid on; 
        set(gca, 'FontSize', fsize); 
        set(gca, 'YLim', [0 100.01]);
        set(gca, 'YTick', [0:20:100]);
        pMax = max(P); 
        set(gca, 'XLim', [0.0 pMax]);
        set(gca, 'XTick', [0:0.1:1.0]*pMax); 
        set(gca, 'XTickLabel', [0:10:100]*pMax);
        %set(gcf,'DefaultAxesColorOrder',[0 0 1; 0 1 0; 1 0 0; 1 0 1; 0 1
        %1; 1 1 0 ]);
        t = sprintf('%s', attrNames{i}); 
        title(t);
        xlabel('% tr. data ', 'FontSize', fsize); 
        ylabel('accuracy', 'FontSize', fsize);
        
        for i2=1:nl
            mark = sprintf('%c-%c',ptM(i2), colM(i2));
            plot(P, acc1(:, i2), mark, 'MarkerSize',msize, 'LineWidth', lwidth);
        end
        legend(L, 'Location', 'SouthEast');
        saveas(gcf, s, 'fig');
        print(gcf, '-dpng', s);
        % close(fig1);
    end
       
        
end

function [] = plotFacet2(PRF, P, L )

    fsize = 10;
    ptM = 'osp^v<>ph+'; 
    colM = 'rgbymckw';
    facetData;
    
    n1 = ceil(sqrt(numAttributes));
    n2 = floor(sqrt(numAttributes));
    np = length(P); 
    nl = length(L); 
    for p1 = 1:np % plot at each tr pct
        
       
        for i=1:(numAttributes) % attributes for
            fig1 = figure;
            cnumStates = attrsNumVals{i};
            thc = zeros(cnumStates, nl);
            for j=1:cnumStates
                for l1=1:nl 
                    idx1 = (l1-1)*cnumStates + j; 
                    thc(j, l1) = PRF(i).f1score(p1, idx1); 
                end
            end
            bar([1:cnumStates]' , thc);
            set(gca, 'FontSize', fsize*1.5);
            s = sprintf('%s (tr. data = %d %% )', attrNames{i}, floor(100*P(p1)));
            title(s); 
            set(gca, 'YLim', [0 1]); 
            set(gca, 'XLim', [0 (cnumStates+2)]);
            set(gca, 'XTick', [1:cnumStates]); 
            set(gca, 'XTickLabel', attributes{i}); 
            xticklabel_rotate([], 45, [], 'FontSize', 10); 
            fname1 = sprintf('../images/%sP0x%d', attrNames{i} , floor(10*P(p1)) ); 
            xlabel('states', 'FontSize', fsize*1.5);
            ylabel('F1-score', 'FontSize' ,fsize*1.5);
            %if(i==1)
            %    legend(L);
            %end
            legend(L, 'Location', 'SouthEast');
            saveas(gcf, fname1, 'fig');
            print(gcf, '-dpng', fname1);
            
            % keyboard; 
            % close(fig1); 
        end
    end
end


% %% Plots 10,50 and 90% hamming plots on the same plot 
% function [] = plotHamming159(results, P, L)
%     ptM = 'sp^v<>h+'; 
%     colM = 'rgmbckw';
%     lineM = {'-'; '-.'; '--'}
%     np = length(P); 
%     nl = length(L); 
%     facetData ;
%     fsize =15;
%     lwidth = 3.5; 
%     msize = 5;
%     acc0 = zeros(np, nl, numAttributes);
%     hc0 = zeros(np, nl); 
%     % histogram figure
%     fig0 = figure; 
%     hold on;
%     grid on;
%     
%     for i1=1:np
%         if( (P(i1) == 0.1 ) || (P(i1) == 0.9))
%             set(gca, 'FontSize', fsize);
%             set(gca, 'YLim', [0 100.01]);
%             set(gca, 'YTick', [0:20:100]);
%             %set(gcf,'DefaultAxesColorOrder',[0 0 1; 0 1 0; 1 0 0; 1 0 1; 0 1 1; 1 1 0 ]);
%             
%             t = sprintf('Hamming error of Q_{true} vs Q_{pred} (%d%% tr data)', floor(100.0*P(i1)));
%             % title(t);
%             xlabel('number of errors', 'FontSize', fsize);
%             ylabel('% of samples (cumulative)', 'FontSize', fsize);
%             
%             %%% HACK: Don't show the CL, just CLWN directly
%             I22  =[1 2 4] ;
%             for i2sm=1:length(I22) %% original: 1:nl
%                 i2 = I22(i2sm); 
%                 idx = (i1-1)*nl + i2;
%                 cres = results(idx);
%                 [hcounts, hacc, lims] = getAcc2(cres);
%                 mark = sprintf('%c%s%c',ptM(i2), lineM{mod(i1,length(lineM))+1},colM(i2));
%                 plot(lims, hcounts, mark, 'MarkerSize', msize, 'LineWidth', lwidth);
%                 for ia=1:numAttributes % plot individual facet accuracies
%                     acc0(i1, i2,  ia)  = hacc(ia+ 1);
%                 end
%             end
%         end
%         
%     end
%     s = sprintf('../images/hammingWSDM');
%     % legend(L, 'Location', 'SouthEast');
%     saveas(gcf, s, 'fig');
%     print(gcf, '-dpng', s);
%     close(fig0); 
% end
% 
% 
% function [] = plotPrf1(PRF, P, L)
%     facetData;
%     fsize = 10; 
%     ptM = 'osp^v<>ph+'; 
%     colM = 'rgbcmykw';
%     n1 = length(P); 
%     n2 = length(L); 
%     for i=1:numAttributes
% 
%         leg0 = {}; 
%         mark0 = {}; 
%         counter0 = 0;
%         cstates = attrsNumVals{i};
%         for c0=1:cstates
%             for i2 = 1:n2       
%                 counter0  = counter0 + 1; 
%                 currCol = colM(c0); 
%                 currPt = ptM(i2); 
%                 s1 = sprintf('%c-%c', currCol , currPt);
%                 s2 = sprintf('%s %s', L{i2}, attributes{i}{c0}); 
%                 mark0{counter0} = s1; 
%                 leg0{counter0} = s2;
%             end
%         end
%         myhandles = {}; 
%         fig0  = figure; 
%         grid on; 
%         hold on; 
%         for p1 = 1:(cstates*n2)
%             hh = plot('v6', P, PRF(i).f1score(:, p1), mark0{p1}, 'MarkerSize', fsize/2, 'LineWidth', fsize/3);                
%             myhandles{p1} = hh; 
%         end
%         
%         legend(leg0); 
%         set(gca, 'FontSize', fsize); 
%         keyboard; 
%         close(fig0); 
%         
%     end 
% end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% plotter2.m ends here
