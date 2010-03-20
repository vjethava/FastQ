'''
Created on 15-Mar-2010

@author: vjethava
'''

import re
from data.attribute import Attribute
from data.instance import Instance
from data.dataset import Dataset
class ArffReader(object):
    '''
    Reads the WEKA ARFF Format -  prototype can be replaced later 
    '''
    def __init__(self, fname=None):
        '''
        Constructor
        @param fname: ARFF File name 
        '''
        self.fileName = fname 
        self.__reset()
        if not (self.fileName == None):
            self.readFile()

    def __reset(self):
        self.dataset = None
        self.relation = None
        self.attributes = []
        self.instances = []
    
    def parseInstance(self, line, id=None):
        instance = Instance()
        instance.id = id
        instance.line = line 
        instance.facet = [] 
        
    def parseAttribute(self, line):
        result = Attribute(line)
        count = 0
        p = re.match('(\@attribute) (\w*) \{(.*)\}', line)
        result.name = p.group(2)
        vals = p.group(3).split(',')
        for val in vals:
            count = count + 1; 
            result.values[val.strip()] = count;
        result.numVals = count 
#        print  self.__class__.__name__ + str(self.numVals)
        return result 
          
    def readFile(self, fileName=None, queryIdFile=None):
        if not (fileName == None):
            self.fileName = fileName
            self.__reset()
        else:
            fileName = self.fileName
        with open(fileName, 'r') as f:
            read_data = f.read()
        lines = read_data.split('\n')
        [RELATION, ITER, DATA] = range(3)
        state = RELATION
        for line in lines: 
            if state == RELATION:
                matches = re.match('(@relation)(\s*)(\w*)', line)
                if not (matches == None):
                    self.relation = matches.group(3)
                    state = ITER
            elif state == ITER:
                if not (re.match('\@attribute', line) == None):
                    attribute = self.parseAttribute(line)
                    self.attributes.append(attribute)
                elif not(re.match('\@data', line) == None):
                    state = DATA
            elif (state == DATA): 
                cline = line.strip().split(',')
                if len(cline) == len(self.attributes):
                    count = 0
                    instance = Instance()
                    for attribute in self.attributes:    
                        instance.append(attribute.values[cline[count] ])
                        count = count + 1 
                    self.instances.append(instance)
        self.dataset = Dataset(self.instances, self.attributes, None)            

    def getRelations(self):
        return self.relation

    def getNumAttributes(self):
        return len(self.attributes)
    
    def getNumInstances(self):
        return len(self.instances)

    def getAttributes(self):
        attrs = ""
        for attribute in self.attributes:
            attrs = attrs + attribute.__str__() + "\n" 
        return attrs 
    
    def getInstances(self):
        res = ""
        for instance in self.instances:
            res = res + str(instance) + "\n" 
        return res
    
if __name__=="__main__": 
    reader = ArffReader("/home/vjethava/CODE/webintent/QueryLabels.arff")
    print reader.getAttributes()
#     print reader.getInstances()
    
    