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


groups = ["C", "C++","Eridu"]
c   = []
cpp = []
cs  = []
erd = []

del data["time"]




n = len(groups)
r = np.arange(n)
width = 0.2

print(data["./program_setup_teardown"]["setup_teardown.c"])

plt.bar(0.1, data["./program_setup_teardown"]["setup_teardown.c"], color = 'b',width = width,edgecolor = 'black')
plt.bar(1.1, data["./program_setup_teardown"]["setup_teardown.cpp"], color = 'r',edgecolor = 'black',width = width)
plt.bar(2.1, data["./program_setup_teardown"]["setup_teardown.erd"], color = 'g',edgecolor = 'black',width = width)


plt.ylabel("Time (s)") 
plt.title("Average Time (s) for Dummy Program Setup & Teardown (10,000 runs)")


plt.grid(linestyle='--')
plt.xticks(r + width/2, groups)
plt.legend()
plt.show()