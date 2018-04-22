import csv
import matplotlib.pyplot as plot
import numpy as np

algorithm='KMeans'
# algorithm='DBSCAN'
# algorithm='Spectral'
# algorithm='DenPeak'
# dataset='Aggregation_cluster=7'
# dataset="flame_cluster=2"
dataset="Jain_cluster=2"
# dataset="Pathbased_cluster=3"
# dataset="Spiral_cluster=3"
dataset="data"
filename=algorithm+' - '+dataset+'.txt'#last column is flag; 2D data
header=False

f=open(filename,'r')
r=csv.reader(f)
data=[]
counter=0
#stronge into data
if header:
    for i in r:
        break
for i in r:
    temp=[]
    for j in i:
        temp.append(j)
    data.append(temp)

length=len(data[0])
length-=1

clusters=-1
#转换格式、找到类数
for i in data:
    for j in range(0,length):
        i[j]=float(i[j])
    i[length]=int(i[length])
    if i[length]>clusters:clusters=i[length]

#分配空间
Data=[]
for i in range(0,clusters+1):
    Data.append([[],[]])

#分类
for i in data:
    Data[i[length]][0].append(i[0])
    Data[i[length]][1].append(i[1])

p1=plot.subplot(111)
for i in range(0,clusters+1):
    p1.scatter(Data[i][0],Data[i][1])

plot.show()