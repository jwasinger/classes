#! /usr/bin/python

import sys, collections
import sort_alpha, sort_num

def parse_words(line):
###
#
# create a list of words present in a string containing a line of input.  spaces and non-letter
# characters delineate words.
#
###
    curr_word = ''
    words = []
    for c in line:
        if c.isalpha():
            curr_word += c
        else:
            if curr_word != '':
                words.append(curr_word)
    
    return words


def create_word_dict():
###
#  
# Create an unordered dictionary of {key: word (str), value: prevalence (int)}
#
###
   word_dict = dict()

    for line in sys.stdin:
        line = line.strip("\n")

        #words = line.split(" ") 
        line = line.tolower()
        words = parse_words(line)

        for word in words:     
            #try and get the word out of the dictionary, if it's not in the dict, add the word
            try:
                word_dict[word] += 1
            except KeyError:
                word_dict[word] = 1

    return word_dict

def comp_words(w1, w2):
###
#
#   compare w1 to w2 alphabetically, return 1 if w1 has higher alphabetic order, return 2 if w2 has higher alphabetic
#   order.  Return 0 if they are the same
#
###
    if w1 == w2:
        return 0

    shortest = (len(w1) if len(w1) < len(w2) else len(w2))
    
    for i in range(shortest):
        if ord(w1[i]) > ord(w2[i]):
            return 1
        elif ord(w1[i]) < ord(w2[i]):
            return 2
        else:
            continue

def insert_kv_into_ordered(kv, ordered):
###
#
#   insert a key/value tuple 'kv' into an ordered collection, 'ordered' where ordering is based on 
#   word prevalence (highest to least) and alphabetically ordered where prevalence of two entries is equal
#
###
    for i in range(len(ordered)):
        #compare against each tuple and add in the appropriate spot
        if(kv[1] > ordered[i][1]):
            ordered.insert(i, kv)
            return 
        if(kv[1] == ordered[i][1]):
            #do alphabetic comparison
            res = comp_words(kv[1], ordered[i][1])
            if res == 1:
                ordered.insert(i, kv)
                return 
            else
                continue
    #tuples have been iterated and the kv tuple has not been placed so add it now (ordered last)
    ordered.append(kv)

def order_words_alpha(word_dict):

def order_words_num(word_dict):

def create_ordered_word_list(word_dict):
    output_dict = collections.OrderedDict()
    items = word_dict.iteritems()
    ordered_items = []
    last_index = 0
    
    for item in items:
        insert_into_kv_ordered(item, ordered_items)
    
    return ordered_items

if __name__ == "__main__":
     
