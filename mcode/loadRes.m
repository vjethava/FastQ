%%% loadRes.m --- 
%% 
%% Filename: loadRes.m
%% Description: 
%% Author: Vinay Jethava
%% Created: Thu Apr 29 16:44:28 2010 (+0200)
%% Last-Updated: Thu Apr 29 16:44:37 2010 (+0200)
%%           By: Vinay Jethava
%%     Update #: 1
%% URL: 
%% Keywords: 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% 
%%% Code:

function [result] = loadRes(exp, res_dir)
    result = struct('P', [], 'legend', [], 'query', [] , 'accuracy', [], 'outX', [], 'trueX', [], 'hamming', []);
    name = sprintf('%s/%d/accuracy.txt', res_dir, exp);
    result.accuracy = load(name);
    name = sprintf('%s/%d/hamming.txt', res_dir, exp);
    result.hamming = load(name);
    name = sprintf('%s/%d/true.txt', res_dir, exp);
    result.trueX = load(name);
    name = sprintf('%s/%d/out.txt', res_dir, exp);
    result.outX = load(name);
end



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% loadRes.m ends here
