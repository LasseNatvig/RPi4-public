# TDT4258-2026-RPi4-5 CPU Frequencies and more

This code has been developed for and on a RPi4 partly with assistance of AI, mainly Mistral Medium running on Idun@NTNU. All code has been reviewed, and commented. Please send comments to your teacher in e-mail if you have ideas for improvements. Last updated 2026-10-19, Lasse Natvig.

The code demonstrates CPU frequency scaling, governor modes, task binding (CPU affinity), and parallel execution. The main purpose of the example is to demonstrate how system calls can be done from C, and how HW features such as CPU frequency can be controlled by accessing special systems files exposed by the linux kernel. We also demonstrate CPU governors that is a related concept partly covered by Di Lius guest lecture (Lecture 9)

It is only C-code that is part of your readling list (relevant for exam), and is elaborated at the end of this README files.

---

## 📁 Files and Tools

| File/Tool               | Description                                                                                     |
|-------------------------|-------------------------------------------------------------------------------------------------|
| `test.py`               | Main test script. Runs experiments to analyze CPU frequency, governors, and core binding.       |
| `cpuFreq`               | C program to measure execution time of a CPU-bound task.                                        |
| `parallelPi`            | C program to compute π in parallel across threads (uses `pthreads`).                            |
| `setFreq_kHz`           | Utility to set CPU frequency (in kHz). **Requires `sudo` and `userspace` governor.**            |
| `setGovernor`           | Utility to set CPU governor mode. **Requires `sudo`.**                                          |
| `plot.py`               | Generates a plot (`plot.png`) of execution time vs. CPU frequency from `results.txt`.           |
| `parPlot.py`            | Generates a plot (`parPlot.png`) of execution time vs. threads for different frequencies.       |
| `Makefile`              | Builds all C programs and provides shortcuts for running tests and generating plots.            |
| `results.txt`           | Output log from `test.py` experiments.                                                          |

---

## 🔧 Setup and Usage

### Build

```bash
make all
```

Compiles:

- `cpuFreq` (CPU-bound task benchmark)
- `parallelPi` (multi-threaded π calculation)
- `setFreq_kHz` (frequency setter)
- `setGovernor` (governor mode setter)

### Run Tests

```bash
make test          # Run all experiments (output to results.txt)
make plot           # Generate plot.png from results.txt
make parPlot        # Generate parPlot.png from results.txt
```

### Manual Usage

1. **Set Governor** (e.g., to `userspace`):

   ```bash
   sudo ./setGovernor userspace
   ```

2. **Set Frequency** (e.g., to 1500 MHz):

   ```bash
   sudo ./setFreq_kHz 1500000
   ```

3. **Run CPU Benchmark**:

   ```bash
   ./cpuFreq 30       # Scale factor (higher = longer runtime)
   ```

4. **Run Parallel π Calculation**:

   ```bash
   ./parallelPi 4 500000000  # 4 threads, 500M steps
   ```

---

## Experiments in `test.py`

| Part | Description                                                                                     |
|------|-------------------------------------------------------------------------------------------------|
| 1    | Shows that the `userspace` governor must be used to be able to set frequency                    |
| 2    | Tests execution time across all available CPU frequencies (600–1800 MHz).                       |
| 3    | Measures performance under all governor modes (`performance`, `powersave`, etc.).               |
| 4    | Demonstrates **CPU core binding** (task affinity) using `taskset` for serial/parallel runs.     |
| 5    | Tests parallel π calculation (`parallelPi`) at different frequencies and thread counts (1–4).   |

---

## Key Concepts

### CPU Governors

- **`performance`**: Always max frequency.
- **`powersave`**: Always min frequency.
- **`userspace`**: Manual frequency control (required for `setFreq_kHz`).
- **`ondemand`**: Dynamically scales frequency based on load.
- **`conservative`**: Like `ondemand` but more gradual.
- **`schedutil`**: Integrates with the Linux scheduler.

### CPU Frequency

- Valid frequencies (in kHz): `600000`, `700000`, ..., `1800000`.
- Requires **`userspace` governor** and **`sudo`** to modify.

### Task Binding

- Use `taskset -c <core>` to bind a process to a specific CPU core (e.g., `taskset -c 0 ./cpuFreq`).
- Prevents the OS from migrating the process between cores.

## Which parts of the example are part of the TDT4258-2026 reading list

- The helper function utils/setFreq_kHz.c
- The helper function utils/setGovernor.c
- cpuFreqTest.c

## Comments

- If you have comments to the example, please send them in an e-mail to the TDT4258 course teacher.
