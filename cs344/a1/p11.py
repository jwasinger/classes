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

if __name__ == "__main__":
    word_dict = create_word_dict()
    sorted_alpha = sort_alpha.sort()
    sorted_num = sort_num.sort()

    print("=== PRINTED IN ALPHA-ORDER ===")
    print(sorted_alpha)

    print("=== PRINTED IN NUMERIC-ORDER===")
    print(sorted_num)