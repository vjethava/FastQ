'''
Created on 16-Mar-2010

@author: vjethava
'''

class Instance(list):
    '''
    Facet classification for known query 
    '''
    def __init__(self):
        '''
        Constructor
        '''
        list.__init__(self)
        self.id = None
        self.line = None
        self.facet = None
    
    def get_id(self):
        return self.id
    def set_id(self, id):
        self.id = id
    
    def get_line(self):
        return self.line
    def set_line(self, line):
        self.line = line
    
    def get_facet(self):
        return self.facet
    def set_facet(self, facet):
        self.facet = facet 
        