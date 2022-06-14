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


groups = []
c   = []
cpp = []
cs  = []
erd = []

del data["time"]

for key, value in data.items():

        if key not in ["./fibonacci_half", "./knapsack_half", "./naivepatternmatch_half", "./primefactors_half", "./bubblesort_half"]:
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
  
  
plt.bar(r, c, color = 'b',
        width = width, edgecolor = 'black',
        label='C')

plt.bar(r + width , cpp, color = 'r',
        width = width, edgecolor = 'black',
        label='C++')

plt.bar(r + width * 2, erd, color = 'g',
        width = width, edgecolor = 'black',
        label='Eridu')


plt.ylabel("Time (s)") 
plt.title("Average Time (s) pr. (Half) Test pr. Language (10,000 runs)")
  
plt.grid(linestyle='--')
plt.xticks(r + width/2, groups)
plt.legend()
plt.show()