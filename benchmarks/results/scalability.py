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
del data["time"]

all = {}

groups = []
c   = []
c_h = []
c_q = []

cpp = []
cpp_h = []
cpp_q = []


cs  = []
cs_h = []
cs_q = []

erd = []
erd_h = []
erd_q = []


for key, value in data.items():

        if key not in ["./fibonacci", "./knapsack", "./naivepatternmatch", "./primefactors", "./bubblesort", "./helloworld"]:
            continue

        groups.append(key)
        c.append(value[key[2:] + ".c"])
        cs.append(value[key[2:] + ".cs"])
        cpp.append(value[key[2:] + ".cpp"])
        erd.append(value[key[2:] + ".erd"])

for key, value in data.items():

        if key not in ["./fibonacci_quart", "./knapsack_quart", "./naivepatternmatch_quart", "./primefactors_quart", "./bubblesort_quart", "./helloworld"]:
            continue

        c_q.append(value[key.split('_')[0][2:] + ".c"])
        cs_q.append(value[key.split('_')[0][2:] + ".cs"])
        cpp_q.append(value[key.split('_')[0][2:] + ".cpp"])
        erd_q.append(value[key.split('_')[0][2:] + ".erd"])

for key, value in data.items():

        if key not in ["./fibonacci_half", "./knapsack_half", "./naivepatternmatch_half", "./primefactors_half", "./bubblesort_half", "./helloworld"]:
            continue

        c_h.append(value[key.split('_')[0][2:] + ".c"])
        cs_h.append(value[key.split('_')[0][2:] + ".cs"])
        cpp_h.append(value[key.split('_')[0][2:] + ".cpp"])
        erd_h.append(value[key.split('_')[0][2:] + ".erd"])

from statistics import mean

print(erd)
n = 3
r = np.arange(n)
width = 0.2
  
  
plt.plot(["Quarter", "Half", "Full"], [mean(c_q), mean(c_h), mean(c)], color='b', label='C',linewidth=3)
plt.plot(["Quarter", "Half", "Full"], [mean(cpp_q), mean(cpp_h), mean(cpp)], color='r', label='C++',linewidth=3)
plt.plot(["Quarter", "Half", "Full"], [mean(erd_q), mean(erd_h), mean(erd)], color='g', label='Eridu',linewidth=3)

plt.title('Mean Time (s) pr. Test pr. Level of Complexity pr. Language')
plt.ylabel("Time (s)")
plt.grid(linestyle='--')
plt.legend()
plt.show()