load acc4;
P = [0.01, 0.05, 0.10]; 
creq  = [];
for j=1:9
    req = [];
    for i=4:4
        req = [req P_base(i, j)];
        req = [req P_wn(i,j)];
        req = [req P_fastq(i, j)]; 
    end
    creq = [creq; req]; 
end
