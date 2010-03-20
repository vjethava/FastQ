'''
Created on 17-Mar-2010

@author: vjethava
'''

class Variable(object):
    '''
    This models a variable node
    '''
    def __hash__(self):
        return hash(self.id)
    
    def __init__(self, id, numVals):
        '''
        Constructor
        '''
        self.id = id 
        self.numVals = numVals 