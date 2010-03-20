'''
Created on 16-Mar-2010

@author: vjethava
'''
from numpy import zeros
from numpy import array
import numpy 
class Count:
    '''
    Internal class to preserve counts between two atttributes 
    '''
    def __init__(self, nx, ny):
        '''
        @param nx: the number of possible values for attribute x
        @param ny: the number of possible values for attribute y
        '''
        self.nx = nx
        self.ny = ny
        self.freq = zeros((nx, ny))
                
    def isValid(self, c, maxc, minc=0):
        return ((c >= minc) and (c < maxc))
    
    def incr(self, x, y):
        assert((self.isValid(x, self.nx) and self.isValid(y, self.ny)))
        self.freq[x][y] = self.freq[x][y] + 1
    
    def normalize(self, row=None):
        if(row == None):
            msum = numpy.sum(self.freq)
            if(msum > 0):
                self.freq = self.freq/msum 
        else:
            msum = numpy.sum(row)
            if(msum > 0.0):
                row = row/msum
            
#        rsum = 0.0
#        if(len(row[0]) == 1):
#           for i in row:
#                rsum  = rsum + i
#           if(rsum > 0.0):
#                for i in range(len(row)):
#                   row[i] = row[i]/rsum
#        else:
#            for i in row: 
#                for j in i:
#                    rsum = rsum + j
#            if(rsum > 0.0):
#                for i in range(len(row)):
#                    for j in range(len(row[i])):
#                        row[i][j] = row[i][j]/rsum
#                        if(row[i][j] > 0.0):

    def get(self, x=None, y=None):
        if (y == None) and (x == None):
            return self.freq
        elif(y == None):
            return self.freq[x]
        elif(x == None):
            res = []
            for i in range(self.nx):
                res.append(self.freq(i, y))
            return res
        else:
            return self.freq[x][y]
    
    def __str__(self):
        res = "["
        for i in range(self.nx):
            res = res + "\n\t" + str(self.freq[i]) + "\t"
        res = res + "]"
        return res
    

if __name__ == '__main__':
    myc = Count(2, 3)
    print myc
    myc.incr(1, 2)
    myc.incr(0, 1)
    myc.normalize()
    print myc