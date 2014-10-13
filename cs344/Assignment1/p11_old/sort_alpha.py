import pdb

def __comp__(w1, w2):
###
#
# compare the alphabetic order of two strings
#
###
    if w1 == w2:
        return 0

    eq_len = len(w1) == len(w2)
    shortest = (len(w1) if len(w1) < len(w2) else len(w2))
    for i in range(shortest):        
        #case where i is at index of shortest word and both words begin the same (example: hello and hellooo)
        #hello should go before hellooo
        if not eq_len:
            if i == shortest - 1:
                if len(w2) == shortest:
                    return 2
                else:
                    return 1

        if ord(w1[i]) > ord(w2[i]):
            return 2
        elif ord(w1[i]) < ord(w2[i]):
            return 1
        else:
            continue 

def __insert_kv_into_ordered__(kv, ordered_list):
    ###
    #
    # insert a key/value tuple into a list with ordering based on alphabetic ordering
    #
    ###
    for i in range(len(ordered_list)):
        if __comp__(ordered_list[i][0], kv[0]) == 2:
            
            ordered_list.insert(i, kv)
            return 
    
    ordered_list.append(kv)

def sort(word_dict):
###
#
# sort an unordered dictionary of key:word(string), value:prevalence(integer)
#
###
    items = word_dict.iteritems()
    ordered_list = []

    for item in items:
        #print "item: %s" % (str(item))
        __insert_kv_into_ordered__(item, ordered_list)
        #print "ordered list: %s" % (str(ordered_list))
    
    return ordered_list
