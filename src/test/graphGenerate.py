'''
Created on 17-Mar-2010

@author: vjethava
'''
import unittest
import sys 
import os
from helpers import DaiWriter
from interface.ArffReader import ArffReader
from interface.QueryReader import QueryReader

class GraphGenerateTestCase(unittest.TestCase):
    def setUp(self):
        self.ARFF_FILE = '../../data/queryClassification.arff'
        self.QUERY_FILE = '../../data/Matrix'
    
    def test_arff_file(self):
        self.assertTrue(os.path.isfile(self.ARFF_FILE))
        
    def test_query_file(self):
        self.assertTrue(os.path.isfile(self.QUERY_FILE))
    
    def test_dai(self):
        clInstance = ChowLiu()
        
if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testGraphGenerate']
    unittest.main()
