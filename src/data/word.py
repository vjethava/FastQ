'''
Created on 17-Mar-2010

@author: vjethava
'''

class Word:
    '''
    Class for representing words 
    '''
    count = 0; 
    vocab = dict()
    def __init__(self, word, query_id):
        '''
        Constructor
        '''
        self.word = word 
        if not (self.__class__.vocab.__contains__(word)):
            self.__class__.count = self.__class__.count + 1 
            self.__class__.vocab[word]=[]
            self.queries = []
        # update the list of queries
        new_list = self.__class__.vocab[word]
        new_list.append(query_id)
        self.__class__.vocab.update(word = new_list)
    
    def get_queries_for_word(self,word):
        return self.__class__.vocab[word]
    
    def __str__(self):
        return self.word
