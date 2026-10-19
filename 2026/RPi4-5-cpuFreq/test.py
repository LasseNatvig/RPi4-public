# test.py       used in TDT4258 2026 code example RPi4-5  
import subprocess
import sys
import time
import os

testName = "RPi4-5 Demo Program"  
resultsFile = 'results.txt'
# setFreq_kHz with erroneous parameter reports 600000, step + 100000 up to and including 1800000
frequencies = [600000, 700000, 800000, 900000, 1000000, 1100000, 1200000, 1300000, 1400000, 1500000, 1600000, 1700000, 1800000] # in kHz

def setFrequency(f, inkHz):
    freq = str(inkHz)  # freq is in kHz
    subprocess.call(["sudo", "./setFreq_kHz", freq], stdout=f)
    f.flush()
def setGovernor(governor):
    subprocess.call(["sudo", "./setGovernor", governor], stdout=f)
    f.flush()

def part1(f, scale=1):
    f.write("\n\n****** Part 1 ******\n")
    setGovernor("userspace")
    setFrequency(f, 1500000)
    # Demo: the same requested frequency is ignored under ondemand but honoured under userspace
    for governor in ["ondemand", "userspace"]:
        f.write("Tries to set frequency 600000 with " + governor + "\n")
        setGovernor(governor)
        setFrequency(f, 600000)
        f.flush()  # flush before the child process writes to the same file
        subprocess.call(["./cpuFreq", str(scale)], stdout=f)  # cpuFreq is our testprogram written in C
    setFrequency(f, 1800000)

def part2(f, scale=1):
    f.write("\n****** Part 2 ******\n")
    f.flush()
    for freq in frequencies: # loop through all frequencies, see how execution time drops
        setFrequency(f, freq)
        subprocess.call(["./cpuFreq", str(scale)], stdout=f)

def part3(f, scale=1):
    f.write("\n****** Part 3 ******\n")
    f.flush()
    setGovernor("userspace")
    setFrequency(f, 1800000) # loop through governors and see if it affects performance
    for governor in ["conservative", "ondemand", "userspace", "powersave", "performance", "schedutil"]:
        setGovernor(governor)
        f.flush()
        subprocess.call(["./cpuFreq", str(scale)], stdout=f)

def part4(f, scale=1): # Typically used wit scale > 1 to make visible with gkrellm
    f.write("\n****** Part 4 ******\n")
    setGovernor("userspace")
    setFrequency(f, 1800000)
    # Core numbers 1-4 are CPU ids 0-3 in taskset
    for core in range(4): # demonstrate task binding, serially on each core
        f.write("Bound to core " + str(core + 1) + "\n")
        f.flush()
        subprocess.call(["taskset", "-c", str(core), "./cpuFreq", str(scale)], stdout=f)

    f.write("Cores 0 and 1 in parallel\n") # use gkrellm to see how it works
    # Popen runs processes asynchronously (in parallel)
    procs = [subprocess.Popen(["taskset", "-c", str(core), "./cpuFreq", str(scale)], stdout=subprocess.PIPE, text=True)
             for core in (1, 2)] # text=True ensures output returned as a string
    for core, p in zip((1, 2), procs):  # piped, so parallel outputs don't interleave
        f.write("Core " + str(core) + ":\n" + p.communicate()[0])
        # p.communicate retrieves stdout for each process
        # zip pairs each core number with its corresponding process

    f.write("Cores 0, 1, 2 and 3 in parallel\n")
    procs = [subprocess.Popen(["taskset", "-c", str(core), "./cpuFreq", str(scale)], stdout=subprocess.PIPE, text=True)
             for core in (0, 1, 2, 3)] 
    for core, p in zip((0, 1, 2, 3), procs):  # piped, so parallel outputs don't interleave
        f.write("Core " + str(core) + ":\n" + p.communicate()[0])

def part5(f, steps=500000000):
    f.write("\n****** Part 5 ******\n")
    setGovernor("userspace") # demonstrates running  parallelPi on 1,2,3 and 4 threads with all frequencies
    for freq in frequencies:
        setFrequency(f, freq)
        f.write("Frequency (kHz): " + str(freq) + "\n")
        for threads in (1, 2, 3, 4):
            f.flush()
            subprocess.call(["./parallelPi", str(threads), str(steps)], stdout=f)

with open(resultsFile, 'w') as f:
    f.write(testName)
    f.write("\nStarted: " + time.strftime("%Y-%m-%d %H:%M:%S") + "\n")
    part1(f)
    part2(f)
    part3(f)
    part4(f, scale = 30)
    part5(f)
    f.write("\nFinished: " + time.strftime("%Y-%m-%d %H:%M:%S") + "\n")
print("All experiments finished")