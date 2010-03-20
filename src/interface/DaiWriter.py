'''
Created on 17-Mar-2010

@author: vjethava
'''
import dai
from helpers.DaiHelper import DaiHelper
from algorithm.chowliu.ChowLiu import ChowLiu
from algorithm.chowliu.Count import Count
class DaiWriter():
    '''
    Implements a DAI FactorGraph from the known structure 
    '''
    def __init__(self):
        self.dataset = None
        self.FactorGraph = dai.FactorGraph()
        self.vars = []
        self.factors = []
        self.daiFactors = dai.VecFactor()
        
    def get_dataset(self):
        return self.dataset
    def set_dataset(self, dataset):
        self.dataset = dataset
    
    def get_cl(self):
        return self.cl 
    def set_cl(self, cl):
        self.cl = cl 
    
    def generate_facet_variables(self):
        self.attributes = self.dataset.attributes
        for attribute in self.attributes:
            aid = DaiHelper().get_id(attribute)
            anum = attribute.numVals
            cfacet  = dai.Var(aid, anum)
            self.vars.append(cfacet)
     
            