# plot.py
# Code written by AI

import re
import matplotlib
matplotlib.use("Agg")  # no display needed on the Pi
import matplotlib.pyplot as plt

text = open("results.txt").read().split("Part 2")[1].split("Part 3")[0].split("Part 5")[0]  # Part 1 is the governor demo
freqs = [int(m) / 1000 for m in re.findall(r"Starts with CPU frequency: (\d+)", text)]
times = [float(m) for m in re.findall(r"Execution time: ([\d.]+) ms", text)]

plt.plot(freqs, times, "o-")
plt.xlabel("CPU frequency (MHz)")
plt.ylabel("Execution time (ms)")
plt.grid(True)
plt.savefig("plot.png", dpi=150)
print("Wrote plot.png")
