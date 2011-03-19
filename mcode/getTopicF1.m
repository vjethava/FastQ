%%% getTopicF1.m --- 
%% 
%% Filename: getTopicF1.m
%% Description: 
%% Author: Vinay Jethava
%% Created: Thu Apr 22 02:10:35 2010 (+0200)
%% Last-Updated: Thu Apr 29 18:08:22 2010 (+0200)
%%           By: Vinay Jethava
%%     Update #: 8
%% URL: 
%% Keywords: 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% 
%%% Code:
function [p, r, f, nTrue, nPred] = getTopicF1(result, attrNum, attrStates) 
    ntopic = attrStates;
    ox = result.outX;
    tx = result.trueX;
    bins = [0:1:(attrStates-1)]';
    nTrue = histc(tx(:, attrNum), bins); 
    nPred = histc(ox(:, attrNum), bins); 
    p = zeros(ntopic, 1);
    r =  zeros(ntopic, 1);
    f =  zeros(ntopic, 1);
    for i=1:ntopic
        relevant = (tx(:, attrNum) == (i-1));
        retrieved = (ox(:, attrNum) == (i-1));
        
        if(sum(relevant .* retrieved) > 0)
            p(i) = sum(relevant .* retrieved)/sum(retrieved);
            r(i) =  sum(relevant .* retrieved)/sum(relevant);
            f(i) =2 * p(i) *r(i)/(p(i) + r(i));              
        end
    end
    %  keyboard; 
end



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% getTopicF1.m ends here
