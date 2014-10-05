

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
            else:
                continue
    #tuples have been iterated and the kv tuple has not been placed so add it now (ordered last)
    ordered.append(kv)

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
