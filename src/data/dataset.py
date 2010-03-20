'''
Created on 16-Mar-2010

@author: vjethava
'''

class Dataset(object):
    '''
    A dataset containing a list of instances with associated attributes and actual query details 
    '''
    def __init__(self, attribs, instances, queries, vocab=None):
        '''
        Constructor
        '''
        self.attributes = attribs 
        self.instances = instances
        self.queries = queries 
        self.wordset = vocab 

    def get_random_subset(self):