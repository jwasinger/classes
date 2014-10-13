#! /usr/bin/python2.7

import sys, collections, pdb
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

def print_vowels(text):
    vowels = {}
    vowels['a'] = 0
    vowels['e'] = 0
    vowels['i'] = 0
    vowels['o'] = 0 
    vowels['u'] = 0

    for c in text:
        if c == 'a':
            vowels['a'] += 1 
        elif c == 'e':
            vowels['e'] += 1
        elif c == 'i':
            vowels['i'] += 1
        elif c == 'o':
            vowels['o'] += 1
        elif c == 'u':
            vowels['u'] += 1
    
    print vowels

if __name__ == '__main__':

    text = sys.stdin.read()    
    word_dict = create_word_dict(text)
    sorted_alpha = sort_alpha.sort(word_dict)
    #sorted_num = sort_num.sort(word_dict)

    print("=== PRINTED IN ALPHA-ORDER ===")
    print(sorted_alpha)

    #print("=== PRINTED IN NUMERIC-ORDER===")
    #print(sorted_num)

    print("=== VOWEL COUNT ===")
    print_vowels(text)
