# parPlot.py
# Code written by AI
import re
import matplotlib
matplotlib.use("Agg")  # no display needed on the Pi
import matplotlib.pyplot as plt

text = open("results.txt").read().split("Part 5")[1]
series = {}  # frequency in kHz -> (threads, times)
freq = None
for line in text.splitlines():
    if m := re.match(r"Frequency \(kHz\): (\d+)", line):
        freq = int(m.group(1))
        series[freq] = ([], [])
    elif freq is not None and (m := re.match(r"Threads: (\d+),.*?Execution time: ([\d.]+) ms", line)):
        series[freq][0].append(int(m.group(1)))
        series[freq][1].append(float(m.group(2)))

colors = plt.cm.viridis([i / max(len(series) - 1, 1) for i in range(len(series))])
for color, (freq, (threads, times)) in zip(colors, series.items()):
    plt.plot(threads, times, "o-", color=color, label=f"{freq / 1000:g} MHz")
plt.xticks([1, 2, 3, 4])
plt.xlabel("Number of threads")
plt.ylabel("Execution time (ms)")
plt.legend(title="CPU frequency", fontsize="small")
plt.grid(True)
plt.savefig("parPlot.png", dpi=150)
print("Wrote parPlot.png")
