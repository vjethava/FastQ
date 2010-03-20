'''
Created on 16-Mar-2010

@author: vjethava
'''

from data.word import Word
from data.query import Query

class QueryReader(object):
    '''
    Reads specified query format and initializes
    '''
    def __init__(self, fileName):
        '''
        Constructor
        '''
        self.queries = []
        with open(fileName) as f:
            read_data = f.read()
        lines = read_data.split('\n')
        for line in lines:
            ar = line.split('\t')
            if(ar[0].isdigit()):
                q = Query()
                q.set_id(int(ar[0]))
                ar1 = ar[1].split(' ')
                [WORD, FREQ]= range(2)
                state = WORD
                for l in ar1: 
                    if(state == WORD):
                        word = Word(l)
                    else: # state = FREQ
                        count = int(l)
                        q.words[word] = count 
                    state = 1-state 
                self.queries.append(q)
    
    def get_queries(self):
        return self.queries

if __name__ == '__main__':
    infile = '/home/vjethava/webintent/Matrix'
    queries = QueryReader(infile).get_queries()
    for query in queries:
        print query
    print 'Total number of unique words in vocabulary', Word.count      