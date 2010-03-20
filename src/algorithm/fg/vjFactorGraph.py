'''
Created on 19-Mar-2010

@author: vjethava
'''

import dai
 
class Node(dai.Var):
    def __init__(self, variable, numStates):
        self.id = 
        
class FactorGraph(dai.FactorGraph):
    '''
    This is a thin wrapper around the Lib DAI relevant functionality 
    '''

    def add_factor(self, variables):

    def add_variable(self, variable, numStates):
    
    def get_id(self, variable):
        id = 0
        cname = variable.__class__.__name__
        if(cname == 'Attribute'):
            id = 1000 + variable.id 
            self.type = 'Attribute'
        elif(cname == 'Word'):
            id = 2000 + cname
            self.type = 'Word'
        else:
            self.type = 'Unknown variable type'
        return id 
            
    def __init__(selfparams):
        '''
        Constructor
        '''
        