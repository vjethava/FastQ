'''
Created on 17-Mar-2010

@author: vjethava
'''
from interface import ArffReader

class TestCL(object):
    '''
    This is the main class for testing of C-L accuracy and recall 
    '''

    
    def __init__(self):
        '''
        Constructor
        '''
        self.arff_file = '../../data/queryClassification.arff' 
        self.query_file = '../../data/Matrix'
    
    def read_arff(self, arff_file):
        self.areader = ArffReader(arff_file)
   
    def read_query(self, query_file):
        self.qreader = ArffReader(query_file)
        
    def read_dataset(self, arff_file, query_file):
        '''
        Read an coordinated dataset and corresponding query words  
        '''
        self.read_arff(arff_file)
        self.read_query(query_file)
        # assign the query to the virtual counts 
        
if __name__ == '__main__':
    tester  = TestCL()
    reader = ArffReader("/home/vjethava/CODE/webintent/QueryLabels.arff")
    