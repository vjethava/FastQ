'''
Created on 15-Mar-2010

@author: vjethava
'''
from interface.ArffReader import ArffReader
from data.attribute import Attribute
from algorithm.chowliu.Count import Count
import sys
import numpy 
import networkx as nw

class MutualInformation(object):
    '''
    Computes the mutual information based on ARFF data 
    '''
    def __init__(self, attributes, instances=None):
        '''
        Constructor
        '''
        self.singletons = [] 
        self.entropy = []
        self.attributes = attributes
        assert(not (attributes == None))
        assert(attributes[0].__class__.__name__ =='Attribute')
        nAttrs = len(attributes)
        # TODO: convert to array
        self.counts = [] 
        for i in range(nAttrs): # initialising the counts 
            crow = []
            singletonCount = Count(nx, 1)
            nx = (attributes[i]).getNumVals()
            for j in range(nAttrs):
                ny = attributes[j].numVals
                currCount = Count(nx, ny)
                crow.append(currCount)
            self.counts.append(crow)
            self.singletons.append(singletonCount)
            
        sys.stderr.write('%s: Initializing nAttr: %d\n' % (self.__class__.__name__,nAttrs))
        if not (instances == None):
            self.update(instances)
            
    def update(self, instances):
        nAttr = len(self.attributes)
        for instance in instances:
            assert(len(instance) == nAttr)
            for i in range(nAttr):
                x = instance[i]
                self.singletons[i].incr(x-1, 0)
                for j in range(i+1, nAttr):
                    y = instance[j]
                    self.counts[i][j].incr(x-1, y-1)
                    ## keeps the matrix symmetric
                    self.counts[j][i].incr(y-1, x-1)
        for i in range(nAttr):
            self.singletons[i].normalize()

    def getEntropy(self, freq):
        nx = freq.shape[0]
        ny = freq.shape[1]
        # note the reversal of indices while  summing
        py = numpy.sum(freq, 0)
        px = numpy.sum(freq, 1)
        assert(px.size == nx)
        assert(py.size == ny)
        result = 0.0
        for i in range(nx):
            for j in range(ny):
                if(freq[i][j] > 0.0):
                    term = freq[i][j]/(px[i]*py[j])
                    term = freq[i][j] * numpy.log( term )
                    result = result + term 
        return result
    
    def getSingletonProb(self, x):
        return self.singletons[x-1]

                           
    def compute(self):
        nAttr = len(self.attributes)
        self.mutual_information = numpy.zeros((nAttr, nAttr)) 
        for i in range(nAttr):
            for j in range(i+1, nAttr):
                self.counts[i][j].normalize()
#                print self.counts[i][j]
                self.mutual_information[i, j] = self.getEntropy(self.counts[i][j].freq)
#               print self.mutual_information[i][j]
        return self.mutual_information
    
if __name__ == '__main__':
    reader = ArffReader("/home/vjethava/CODE/webintent/QueryLabels.arff")
#    print reader.getInstances()
    mi = MutualInformation(reader.attributes, reader.instances)
    mi.compute()
