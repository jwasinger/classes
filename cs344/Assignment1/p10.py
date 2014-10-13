#! /usr/bin/python

from scipy import stats
import numpy
import math
import pdb
import sys

def mean(vals):
    num_vals = len(vals)
    sum_vals = 0.0
    
    for i in range(num_vals):
        sum_vals += float(vals[i])
    
    return float(sum_vals)/float(num_vals)


def stdev(vals):
    mean_vals = mean(vals)

    sum_squared = 0.0
    for i in range(len(vals)):
        sum_squared += ((mean_vals - float(vals[i])) ** 2.0)
    
    return math.sqrt(float(sum_squared)/float(len(vals) - 1.0))


def skewness(vals):
    std_vals = float(stdev(vals)) 
    mean_vals = float(mean(vals))
   
    numerator = 0.0
    for i in range(len(vals)):
        numerator += ((vals[i] - mean_vals) ** 3.0)
    
    denominator = 0.0
    for i in range(len(vals)):
        denominator += ((vals[i] - mean_vals) ** 2.0)
    
    denominator = denominator ** (3.0/2.0)

    return (numerator / denominator) * math.sqrt(len(vals))


def kurtosis(vals):
    std_vals = stdev(vals)
    mean_vals = mean(vals)
    
    numerator = 0.0
    for i in range(len(vals)):
        numerator += (vals[i] - mean_vals) ** 4.0

    denominator = 0.0
    for i in range(len(vals)):
        denominator += (vals[i] - mean_vals) ** 2.0
    
    denominator = denominator ** 2.0

    return float(len(vals)) * (numerator/denominator)

if __name__ == "__main__":
    
    vals = sys.stdin.read().split('\n')
    for i in range(len(vals)):
        if not vals[i]:
            del vals[i]
        else:
            vals[i] = float(vals[i])
       
    my_mean = mean(vals)
    true_mean = numpy.mean(vals)

    my_std = stdev(vals)
    true_std = numpy.std(vals, ddof=1)

    my_skew = skewness(vals)
    true_skew = stats.skew(vals, None, True)

    my_kurtosis = kurtosis(vals)
    true_kurtosis = stats.kurtosis(vals, None, False, True)
    
    print "My Calc:  %s %s %s %s %s" % (len(vals), my_mean, my_std, my_skew, my_kurtosis)
    print "Lib Calc: %s %s %s %s %s" % (len(vals), true_mean, true_std, true_skew, true_kurtosis)
    
