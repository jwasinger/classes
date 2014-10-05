
def __comp__(w1, w2):
###
#
# compare the alphabetic order of two strings
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
    

def __insert_kv_into_ordered__(kv, ordered_list):
    ###
    #
    # insert a key/value tuple into a list with ordering based on alphabetic ordering
    #
    ###
    for i in range(len(ordered_list)):
        if __comp__(ordered_list[i][0], kv[0]) == 1:
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
        __insert_kv_into_ordered__(item, ordered_list)

    return items
