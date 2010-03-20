'''
Created on 17-Mar-2010

@author: vjethava
'''

class DaiHelper(object):
    '''
    Helper class for interfacing with LibDai 
    '''

    def get_id(self, variable):
        id = 0
        cname = variable.__class__.__name__
        if(cname == 'Attribute'):
            id = 1000 + variable.id 
        elif(cname == 'Word'):
            id = 2000 + cname
        else 
            id = '
    
    def update_factor_graph(self, prior,):
    def __init__(self):
        '''
        Constructor
        '''
        