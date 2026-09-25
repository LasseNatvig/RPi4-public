# plot_sorting.py
import os
import re
import matplotlib.pyplot as plt
from datetime import datetime

# Paths
res_dir = "res"
plots_dir = "plots"

# Find the latest .txt file in res folder that contains "Experiment: sorting"
latest_file = None
latest_time = datetime.min

if not os.path.exists(res_dir):
    print(f"Directory {res_dir} does not exist")
    exit(1)

if not os.path.exists(plots_dir):
    os.makedirs(plots_dir)

# Get all .txt files in res directory
for filename in os.listdir(res_dir):
    if filename.endswith(".txt"):
        filepath = os.path.join(res_dir, filename)
        try:
            timestamp_str = filename[4:19]
            file_time = datetime.strptime(timestamp_str, "%Y%m%d-%H%M%S")
            
            with open(filepath, 'r') as f:
                content = f.read()
                if "Experiment: sorting" in content:
                    if file_time > latest_time:
                        latest_time = file_time
                        latest_file = filepath
        except Exception as e:
            continue

if latest_file is None:
    print("No sorting experiment file found in res directory")
    exit(1)

print(f"Found sorting experiment file: {latest_file}")

# Extract timestamp for output filename
basename = os.path.basename(latest_file)
timestamp = basename[4:19]

# Counter names - all 8 counters
counter_names = ["Cycles", "Instructions", "Cache refs", "Cache misses", "Branch misses", "cacheMissRate", "IPC", "branchMissesPerMillionInstructions"]

# Data structures to store counter values
insertion_data = {}
bubble_data = {}
current_sort = None
current_size = None
current_counter_values = []

# Regular expressions for parsing counter values
counter_regex = {
    "Cycles": r"Cycles:\s*mean=([\d.]+)M",
    "Instructions": r"Instructions:\s*mean=([\d.]+)M",
    "Cache refs": r"Cache refs:\s*mean=([\d.]+)k",
    "Cache misses": r"Cache misses:\s*mean=([\d.]+)",
    "Branch misses": r"Branch misses:\s*mean=([\d.]+)",
    "cacheMissRate": r"cacheMissRate:\s*([\d.]+)",
    "IPC": r"IPC:\s*([\d.]+)",
    "branchMissesPerMillionInstructions": r"(?i)branchMissesPerMillionInstructions:\s*([\d.]+)"
}

with open(latest_file, 'r') as f:
    for line in f:
        line = line.strip()
        
        # Save previous counter data when new section starts
        if line.startswith("insertionSort: size=") or line.startswith("bubbleSort: size="):
            if current_sort and current_size is not None and len(current_counter_values) == len(counter_names):
                if current_sort == "insertionSort":
                    insertion_data[current_size] = current_counter_values.copy()
                else:
                    bubble_data[current_size] = current_counter_values.copy()
            current_counter_values = []
        
        # Check for sort type and size
        if line.startswith("insertionSort: size="):
            match = re.search(r'insertionSort: size=\s*(\d+)', line)
            if match:
                current_size = int(match.group(1))
                current_sort = "insertionSort"
        elif line.startswith("bubbleSort: size="):
            match = re.search(r'bubbleSort: size=\s*(\d+)', line)
            if match:
                current_size = int(match.group(1))
                current_sort = "bubbleSort"
        
        # Extract counter values
        for counter_idx, counter_name in enumerate(counter_names):
            pattern = counter_regex[counter_name]
            match = re.search(pattern, line)
            if match:
                value = float(match.group(1))
                # Apply unit conversions for specific counters
                if counter_name == "Cycles" or counter_name == "Instructions":
                    value *= 1000000.0  # M to actual
                elif counter_name == "Cache refs":
                    value *= 1000.0  # K to actual
                current_counter_values.append(value)
                break

# Save the last set of counters
if current_sort and current_size is not None and len(current_counter_values) == len(counter_names):
    if current_sort == "insertionSort":
        insertion_data[current_size] = current_counter_values.copy()
    else:
        bubble_data[current_size] = current_counter_values.copy()

print(f"Insertion sort: {len(insertion_data)} data points")
print(f"Bubble sort: {len(bubble_data)} data points")

# Get all sorted sizes
all_sizes = sorted(set(list(insertion_data.keys()) + list(bubble_data.keys())))

# Create the plot with 8 subplots (2 rows x 4 columns)
fig, axes = plt.subplots(2, 4, figsize=(20, 10))
fig.suptitle('Sorting Algorithm All 8 Counter Statistics', fontsize=16)

# Flatten axes for easy iteration
axes = axes.flatten()

for i, counter_name in enumerate(counter_names):
    if i >= len(axes):
        break
    ax = axes[i]
    
    # Prepare data for this counter
    ins_sizes = []; ins_vals = []
    bub_sizes = []; bub_vals = []
    
    for size in all_sizes:
        if size in insertion_data and i < len(insertion_data[size]):
            ins_sizes.append(size)
            ins_vals.append(insertion_data[size][i])
        if size in bubble_data and i < len(bubble_data[size]):
            bub_sizes.append(size)
            bub_vals.append(bubble_data[size][i])
    
    # Plot both algorithms
    if ins_sizes:
        ax.plot(ins_sizes, ins_vals, 'b-o', label='Insertion Sort', markersize=4)
    if bub_sizes:
        ax.plot(bub_sizes, bub_vals, 'r-s', label='Bubble Sort', markersize=4)
    
    ax.set_xlabel('Array Size')
    ax.set_ylabel(counter_name)
    ax.set_title(counter_name)
    ax.legend()
    ax.grid(True, linestyle='--', alpha=0.7)

plt.tight_layout()
output_png = os.path.join(plots_dir, f"sort-counters-{timestamp}.png")
plt.savefig(output_png, dpi=300, bbox_inches='tight')
plt.close()
print(f"Counter statistics plot saved to: {output_png}")

# Also create the original timing plot
insertion_sizes = []; insertion_times = []
bubble_sizes = []; bubble_times = []
current_sort = None; current_size = None

with open(latest_file, 'r') as f:
    for line in f:
        line = line.strip()
        if line.startswith("insertionSort: size="):
            match = re.search(r'insertionSort: size=\s*(\d+)', line)
            if match:
                current_size = int(match.group(1))
                current_sort = "insertionSort"
        elif line.startswith("bubbleSort: size="):
            match = re.search(r'bubbleSort: size=\s*(\d+)', line)
            if match:
                current_size = int(match.group(1))
                current_sort = "bubbleSort"
        elif line.startswith("Mean:"):
            match = re.search(r'Mean:\s*([\d.]+)\s*seconds', line)
            if match and current_size is not None:
                time_value = float(match.group(1))
                if current_sort == "insertionSort":
                    insertion_sizes.append(current_size)
                    insertion_times.append(time_value)
                elif current_sort == "bubbleSort":
                    bubble_sizes.append(current_size)
                    bubble_times.append(time_value)

output_time_png = os.path.join(plots_dir, f"sort-{timestamp}.png")
plt.figure(figsize=(10, 6))
plt.plot(insertion_sizes, insertion_times, 'b-o', label='Insertion Sort', markersize=4)
plt.plot(bubble_sizes, bubble_times, 'r-s', label='Bubble Sort', markersize=4)
plt.xlabel('Array Size')
plt.ylabel('Execution Time (seconds)')
plt.title('Sorting Algorithm Performance: Insertion Sort vs Bubble Sort')
plt.legend()
plt.grid(True, linestyle='--', alpha=0.7)
plt.savefig(output_time_png, dpi=300, bbox_inches='tight')
plt.close()
print(f"Timing plot saved to: {output_time_png}")
