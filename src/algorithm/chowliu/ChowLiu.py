'''
Created on 16-Mar-2010

@author: vjethava
'''
import numpy as nm 
from interface.ArffReader import ArffReader
from algorithm.chowliu.MutualInformation import MutualInformation
import networkx as nw

class ChowLiu(object):
    '''
    Generates the chow-liu probability distribution for the given ARFF file.
    '''
    def __init__(self, arff_file=None):
        '''
        Constructor
        '''
        if (arff_file != None):
            self.readFile(arff_file) 
            self.mutual_info = self.getMI(self.attributes, self.instances)
            self.computed = True 
        else: 
            self.singletons= []
            self.mutual_info = [] 
            self.computed = False
            self.edges = None
            self.nE = 0
            self.attributes = None
            self.instances = None 
            self.G = None
            self.graphReady = False 
    
    def getMI(self, attrs1=None, instances1=None):
        if(attrs1 == None):
            attrs1 = self.atttributes
        if(instances1 == None):
            instances1 = self.instances
        mutual_info = MutualInformation(attrs1, instances1).compute()
        return mutual_info 

    def readFile(self, arff_file):
        reader =  ArffReader(arff_file)
        self.attributes = reader.attributes 
        self.instances = reader.instances     
#       miComputer = MutualInformation(self.reader.attributes, self.reader.instances)
#       self.mutual_info = miComputer.compute()
#       self.attributes = self.reader.attributes
#       self.instances = self.reader.instances
#       self.nE = len(self.attributes)  - 1

    def computeEdges(self):   
        self.edges = nm.zeros((self.nE, 2))
        mutual_info1 = self.mutual_info 
        for i in range(self.nE):
            idx = nm.argmax(mutual_info1)
            idx_tuple0 = nm.unravel_index(idx, mutual_info1.shape)
            self.edges[i, 0] = idx_tuple0[0]
            self.edges[i, 1] = idx_tuple0[1]
            mutual_info1[idx_tuple0] = 0
        self.computed = True
        
    def getListOfEdges(self):
        if not (self.computed):
            self.computeEdges()
        return self.edges
    
        
    def getGraph(self):
        '''
        Returns the networkX compatible graph for drawing purpose
        '''
        if self.graphReady == False:
            self.G = nw.Graph()
            # add the edges 
            for myattribute in self.attributes:
                self.G.add_node(myattribute.__hash__())
                self.G[myattribute.__hash__()]['attribute'] = myattribute
                self.G[myattribute.__hash__()]['name'] = myattribute.name
    
            for i in range(self.nE):
                ai = int(self.edges[i, 0]) 
                aj = int(self.edges[i, 1])
                bi = self.attributes[ai].__hash__()
                bj = self.attributes[aj].__hash__()
                self.G.add_edge(bi, bj)
            self.graphReady = True 
        return self.G    
    
if __name__=='__main__':
    arff_file = "/home/vjethava/CODE/webintent/QueryLabels.arff"
    clInstance = ChowLiu()
    clInstance.readFile(arff_file)
    print clInstance.getListOfEdges()
    graph = clInstance.getGraph()
    
