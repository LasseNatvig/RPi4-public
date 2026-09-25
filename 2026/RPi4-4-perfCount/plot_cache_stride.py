#!/usr/bin/env python3
# plot_cache_stride.py - Plot execution time mean values as a function of stride
# for the latest cache experiment results in the res/ folder

import os
import glob
import re
import matplotlib.pyplot as plt


def find_latest_cache_file(res_dir='res'):
    """Find the latest .txt file in res/ directory that contains 'Experiment: cache'"""
    txt_files = glob.glob(os.path.join(res_dir, '*.txt'))
    txt_files.sort(reverse=True)  # Sort by timestamp (newest first)
    
    for filepath in txt_files:
        try:
            with open(filepath, 'r') as f:
                content = f.read()
                if 'Experiment: cache' in content:
                    print(f"Found cache experiment file: {filepath}")
                    # Extract timestamp from filename
                    basename = os.path.basename(filepath)
                    timestamp = basename.replace('res-', '').replace('.txt', '')
                    return filepath, content, timestamp
        except Exception as e:
            print(f"Error reading {filepath}: {e}")
            continue
    
    print("No cache experiment file found in res/ directory")
    return None, None, None


def extract_strides_and_times(content):
    """Extract stride values and their corresponding mean execution times"""
    strides = []
    times = []
    
    # Parse the arguments line to understand the stride range
    # Example: "Arguments E, start, step, end : 1,  1, 1, 70"
    args_match = re.search(r'Arguments E, start, step, end : \d+,\s*(\d+),\s*(\d+),\s*(\d+)', content)
    
    # Alternative: find all stride sections
    # Pattern: "Cache-strided: stride= X" followed by "Mean: Y"
    stride_pattern = re.compile(r'Cache-strided: stride=\s*(\d+).*?Mean:\s*([\d.]+)\s*seconds', re.DOTALL)
    
    # Also get sequential
    seq_pattern = re.compile(r'Cache-sequential:.*?Mean:\s*([\d.]+)\s*seconds', re.DOTALL)
    
    # Extract sequential time first
    seq_match = seq_pattern.search(content)
    if seq_match:
        seq_time = float(seq_match.group(1))
        strides.append(0)  # or 1 for sequential
        times.append(seq_time)
        print(f"Found sequential time: {seq_time}")
    
    # Extract all stride times
    stride_matches = stride_pattern.finditer(content)
    for match in stride_matches:
        stride = int(match.group(1))
        time = float(match.group(2))
        strides.append(stride)
        times.append(time)
        print(f"Found stride {stride}: {time} seconds")
    
    return strides, times


def plot_stride_times(strides, times, output_file):
    """Create a plot of execution time vs stride"""
    if not strides or not times:
        print("No data to plot")
        return
    
    plt.figure(figsize=(12, 6))
    plt.plot(strides, times, 'bo-', label='Execution Time', linewidth=2, markersize=6)
    
    # Add grid and labels
    plt.grid(True, alpha=0.3)
    plt.xlabel('Stride Value', fontsize=12)
    plt.ylabel('Execution Time (seconds)', fontsize=12)
    plt.title('Cache Experiment: Execution Time vs Stride Value', fontsize=14)
    plt.legend(fontsize=10)
    
    # Add data labels
    for i, (stride, time) in enumerate(zip(strides, times)):
        # Only label some points to avoid clutter
        if i == 0 or i == len(strides) - 1 or i % 5 == 0:
            plt.annotate(f'{time:.2f}', (stride, time), 
                        textcoords="offset points", xytext=(0,10), 
                        ha='center', fontsize=8)
    
    plt.tight_layout()
    plt.savefig(output_file, dpi=150)
    print(f"Plot saved to {output_file}")
    plt.close()


def main():
    # Create plots directory if it doesn't exist
    os.makedirs('plots', exist_ok=True)
    
    # Find the latest cache file
    filepath, content, timestamp = find_latest_cache_file()
    
    if not filepath or not content or not timestamp:
        print("No cache experiment file found.")
        return
    
    # Extract stride and time data
    strides, times = extract_strides_and_times(content)
    
    if not strides or not times:
        print("No stride/time data found in the file.")
        return
    
    # Plot the data with timestamp in filename
    output_file = os.path.join('plots', f'cache-{timestamp}.png')
    plot_stride_times(strides, times, output_file)


if __name__ == '__main__':
    main()