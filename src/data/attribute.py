'''
Created on 15-Mar-2010

@author: vjethava
'''
import re

class Attribute(object):
    '''
    classdocs
    '''
    count = 0
    
    def __init__(self, line=None):
        '''
        Constructor
        '''
        self.name = None
        self.line = line
        self.values = dict()
        self.__class__.count += 1
        self.numVals = 0
        self.id = self.__class__.count

    
    def get_id(self):
        return self.id
    
    def __hash__(self):
        return self.name 
    
    def __repr__(self):
        return self.name
    
    def getNumVals(self):
        return self.numVals
        
    def __str__(self):
        return str(self.name)
