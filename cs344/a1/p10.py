from scipy import stats
import numpy
import math
import pdb
import sys

def mean(vals):
    num_vals = len(vals)
    sum_vals = 0
    
    for i in range(num_vals):
        sum_vals += vals[i]
    
    return float(sum_vals)/float(num_vals)

def stdev(vals):
    mean_vals = mean(vals)

    sum_squared = 0
    for i in range(len(vals)):
        sum_squared += ((mean_vals - float(vals[i])) ** 2)
    
    return math.sqrt(float(sum_squared)/float(len(vals) - 1))

def skewness(vals):
    std_vals = float(stdev(vals)) 
    mean_vals = float(mean(vals))

    numerator = 0
    for i in range(len(vals)):
        numerator += (((float(vals[i]) - mean_vals)/std_vals) ** 3)
    
    return numerator / len(vals)


def kurtosis(vals):
    std_vals = stdev(vals)
    mean_vals = mean(vals)
    
    numerator = 0
    for i in range(len(vals)):
        numerator += (vals[i] - mean_vals) ** 4

    denominator = 0
    for i in range(len(vals)):
        denominator += (vals[i] - mean_vals) ** 2
    
    denominator = denominator ** 2

    return len(vals) * (numerator/denominator)

if __name__ == "__main__":
    
    vals = sys.stdin.read().split('\n')

    my_mean = mean(vals)
    true_mean = numpy.mean(vals)

    my_std = stdev(vals)
    true_std = numpy.std(vals, ddof=1)

    my_skew = skewness(vals)
    true_skew = stats.skew(vals, None, False)
    
    pdb.set_trace()

    my_kurtosis = kurtosis(vals)
    true_kurtosis = stats.kurtosis(vals, None, False, False)
     
