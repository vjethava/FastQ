function [prf] = getPRF(prf2, trPctIdx, modelIdx, attributeIdx)
%% reads PRF2 frrom plotter2 
assert( ((attributeIdx > 0) && (attributeIdx < 10)), 'getPRF: attributeIdx is 1...9');
assert( (modelIdx >=0) && (modelIdx < 4), 'getPRF: modelIdx is zero indexed'); 
assert( (trPctIdx >=0) && (trPctIdx < 4), 'getPRF: trPctIdx is zero indexed'); 
prf = prf2(trPctIdx*4*9 + modelIdx*9 + attributeIdx);
end