#! /usr/bin/python

import sys, collections, pdb

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
                curr_word = ''
    
    #append the last word
    if curr_word != '':
        words.append(curr_word)
    
    return words

def create_word_dict(text):
###
#  
# Create an unordered dictionary of {key: word (str), value: prevalence (int)}
#
###
    word_dict = dict()
    lines = text.split('\n')

    for line in lines:
        #line = line.strip('\n')

        #words = line.split( ) 
        line = line.lower()
        words = parse_words(line)
        
        for word in words:     
            #try and get the word out of the dictionary, if it's not in the dict, add the word
            try:
                word_dict[word]  += 1
            except KeyError:
                word_dict[word] = 1

    return word_dict

def create_vowel_dict(text):
    vowel_dict = dict()
    vowel_dict['a'] = 0
    vowel_dict['e'] = 0
    vowel_dict['i'] = 0
    vowel_dict['o'] = 0
    vowel_dict['u'] = 0

    for c in text:
        c_low = str(c).lower()
        
        if   c_low == 'a':
            vowel_dict['a'] += 1
        elif c_low == 'e': 
            vowel_dict['e'] += 1
        elif c_low == 'i':
            vowel_dict['i'] += 1
        elif c_low == 'o':
            vowel_dict['o'] += 1
        elif c_low == 'u':
            vowel_dict['u'] += 1

    return vowel_dict
         
def print_sorted(l):
    for item in l:
        print str(item[0]) + ' ' + str(item[1])

if __name__ == "__main__":
    text = sys.stdin.read()
    word_dict = create_word_dict(text)
    
    print("=== PRINTED IN NUMERIC-ORDER ===")
    items = list(word_dict.iteritems())
    
    items.sort(key=lambda x: x[0])
    print_sorted(items)
    
    print("=== PRINTED IN ALPHA-ORDER ===")
     
    items.sort(key=lambda x: x[1], reverse=True)
    
    print_sorted(items)

    print("=== VOWEL COUNT ===")

    vowel_dict = create_vowel_dict(text)
    vowel_list = list(vowel_dict.iteritems())
    vowel_list.sort()

    print_sorted(vowel_list)
