#!/usr/bin/python3

import matplotlib
import matplotlib.pyplot as plt

t = [1000,100000,1000000] # num records
real_time = [0.008,0.128,0.920]
user_time = [0.001,0.101,2.169]
sys_time = [0.008,0.028,0.228]

fig, ax = plt.subplots()
ax.plot(t, real_time,label='real time')
ax.plot(t, user_time,label='user time')
ax.plot(t,sys_time,label='sys time')
ax.legend()

ax.set(xlabel='# records', ylabel='time (s)',
       title='Time for Sorting Records')
ax.grid()

fig.savefig("sort_act.png")
plt.show()