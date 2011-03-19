%%% getLgnd.m --- 
%% 
%% Filename: getLgnd.m
%% Description: 
%% Author: Vinay Jethava
%% Created: Thu Apr 29 17:37:05 2010 (+0200)
%% Last-Updated: Thu Apr 29 17:37:14 2010 (+0200)
%%           By: Vinay Jethava
%%     Update #: 1
%% URL: 
%% Keywords: 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% 
%%% Code:

function [lstr] = getLgnd(legend0)
    lstr = {};
    for i=1:size(legend0, 1)
        clstr = 'CL ';
        wnstr = ''; 
        wfstr = 'WORD'; 
        if(legend0(i, 1) == 1)
            wnstr = 'WN';
        end
        if(legend0(i, 3) == 1)
            clstr = ''; 
        end
        str = sprintf('%s%s', clstr, wnstr); 
        if(length(str) == 0)
            str = 'BASE';
        end
        lstr{i} = str; 
    end
end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% getLgnd.m ends here
