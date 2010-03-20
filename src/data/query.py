'''
Created on 16-Mar-2010

@author: vjethava
'''

class Query(object):
    '''
        Implements a query
    ''' 
    query_list = dict()
    def __init__(self):
        
        self.id = None
        self.words = dict()
        
    def set_id(self, id):
        self.id = id
        self.__class__.query_list[self.id] = self
     
    def get_id(self):
        return self.id 
    
    def get_query(self, id):
        return self.__class__.query_list[id]

    def __str__(self):
        result = "" + str(self.id) + " ["
        for word, count in self.words.iteritems():
            result = result + " " + word + " " + str(count)
        result = result + " ]" 
        return result 
        
if __name__=='__main__':
    q1 = Query()
    q1.set_id(2)
    print Query().get_query(2)