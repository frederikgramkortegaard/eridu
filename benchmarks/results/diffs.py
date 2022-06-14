import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import json
import os

data = json.load(open('2022-05-31 23:14:08.json', 'r'))

extension_to_language = {
    'c': 'C',
    'cpp': 'C++',
    'cs': 'C#',
    'erd': 'Eridu',
}

from statistics import mean, median

groups = []
c   = []
cpp = []
cs  = []
erd = []

del data["time"]

for key, value in data.items():

        if key not in ["./fibonacci_half", "./knapsack_half", "./naivepatternmatch_half", "./primefactors_half", "./bubblesort_half", "./helloworld"]:
            continue
        print(key)

        groups.append(key)
        c.append(value[key.split('_')[0][2:] + ".c"])
        cs.append(value[key.split('_')[0][2:] + ".cs"])
        cpp.append(value[key.split('_')[0][2:] + ".cpp"])
        erd.append(value[key.split('_')[0][2:] + ".erd"])

print(erd)
n = len(groups)
r = np.arange(n)
width = 0.2
  
def p(a,b):
        print(100 - a/b*100)
        return 100 - a/b*100

cn = c
erdn = [p(a,b) for (a,b) in zip(cn, erd)]
print(erdn)

plt.bar(r + width , erdn, color = 'b',
        width = width, edgecolor = 'black')
plt.bar(r, [0,0,0,0,0,0], color = 'r',
        width = width, edgecolor = 'black', label="Mean (27.38%)")
plt.bar(r, [0,0,0,0,0,0], color = 'darkorange',
        width = width, edgecolor = 'black', label="Median (26.05%)")

plt.ylabel("Deviation (%)") 
plt.title("Eridu Mean Deviation from C (% Slower than C, Half Tests)")
print(mean(erdn))
print(median(erdn))
plt.grid(linestyle='--')
plt.xticks(r + width/2, groups)
plt.legend()
plt.ylim(0, 100)
plt.axhline(y=median(erdn), color='darkorange', linestyle="-.", label="Mean", marker="o")

plt.axhline(y=mean(erdn), color='r', linestyle="-.", label="Mean", marker="o")
plt.show()