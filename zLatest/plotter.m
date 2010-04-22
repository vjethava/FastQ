%%% plotter.m --- 
%% 
%% Filename: plotter.m
%% Description: 
%% Author: Vinay Jethava
%% Created: Wed Apr 21 17:46:54 2010 (+0200)
%% Last-Updated: Thu Apr 22 00:59:44 2010 (+0200)
%%           By: Vinay Jethava
%%     Update #: 15
%% URL: 
%% Keywords: 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% 
%%% Code:
function [results] = plotter(exps, fname1) 
    results = []; 
    for i=1:length(exps)
        exp = exps(i); 
        disp(exp);
        result = loadRes(exp);
        results = [results; result];
    end
   clplot2(fname1, results);
end

function [result] = loadRes(exp)
    result = struct('accuracy', [], 'outX', [], 'trueX', [], 'hamming', []); 
    name = sprintf('result/%d/accuracy.txt', exp);
    result.accuracy = load(name); 
    name = sprintf('result/%d/hamming.txt', exp);
    result.hamming = load(name); 
    name = sprintf('result/%d/true.txt', exp);
    result.trueX = load(name); 
    name = sprintf('result/%d/out.txt', exp);
    result.outX = load(name); 
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% plotter.m ends here
