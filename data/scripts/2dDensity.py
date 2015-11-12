#!/usr/bin/python
import sys
from numpy import *

option = int(sys.argv[1])
#print option
data_file = '../agent.dat'
expl_file = '../performgvl.dat'
before_file = '../before_histogram.dat'
after_file = '../after_histogram.dat'
#data_file = '../data_histogram.dat'

num_per_meter = 5.0 
offset = (1/num_per_meter)*0.5 

# Here, angles are defined from [-180 to 180[ degrees.
# You could choose another domain. Points will be automatically
# wrapped inside that domain. This can be useful for symetrical
# side chains, for instance. Be careful to use an appropriate domain,
# otherwise the wrapping will produce meaningless data.
x_min, x_max, y_min, y_max = -10.0+offset, 10.0+offset, -10.0+offset, 10.0+offset
x_range = x_max-x_min 

# Number of 2D regions in which the plot is divided.
x_resolution, y_resolution = x_range*num_per_meter, x_range*num_per_meter
#print x_range
#print x_resolution, y_resolution

def read_angles(line):
    tokens = line.split()
    trial = float(tokens[0])
    x = float(tokens[2])
    y = float(tokens[3])
    while x < x_min:
        x = x_max - (x_min - x)
    while x >= x_max:
        x = x_min + (x - x_max)
    while y < y_min:
        y = y_max - (y_min - y)
    while y >= y_max:
        y = y_min + (y - y_max)
    return [x, y, trial]

points = [read_angles(line) for line in open(data_file)]
count = len(points)
histogram = zeros([x_resolution, y_resolution])
x_interval_length = (x_max - x_min) / x_resolution
y_interval_length = (y_max - y_min) / y_resolution
interval_surface = x_interval_length * y_interval_length
increment = 1. / count
M = loadtxt(expl_file, skiprows=1)

for i in points:
    x = int((i[0] - x_min) / x_interval_length)
    y = int((i[1] - y_min) / y_interval_length)
    if M[i[2]-1,1]>0.5:	
    	histogram[x,y] += (1-option)*increment
    if M[i[2]-1,1]<0.5:	
    	histogram[x,y] += (option)*increment

x_intervals = arange(x_min, x_max, (x_max - x_min) / x_resolution)
y_intervals = arange(y_min, y_max, (y_max - y_min) / y_resolution)

if option==0:
    o = open(before_file, 'w')
if option==1:
    o = open(after_file, 'w')
for i, x in enumerate(x_intervals):
    for j, y in enumerate(y_intervals):
        o.write('%f %f %f \n' % (x, y, histogram[i,j]))
    o.write('\n')
print histogram.max()
