'''
Created on 17-Mar-2010

@author: vjethava
'''

class Word(str):
    '''
    Class for representing words 
    '''
    count = 0; 
    vocab = set()
    def __init__(self, word):
        '''
        Constructor
        '''
        str.__init__(word)
        if not (self.__class__.vocab.__contains__(word)):
            self.__class__.count = self.__class__.count + 1 
            self.__class__.vocab.add(word)
        