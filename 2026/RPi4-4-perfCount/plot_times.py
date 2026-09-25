# plot_times.py
import sys
import os
import glob
import re
import matplotlib.pyplot as plt

def extract_times_from_file(filepath):
    """Extract time values from a file."""
    with open(filepath, 'r') as file:
        content = file.read()
    
    # Extract times from the "Times:" line
    times_line = re.search(r'Times:\s*(.+)', content)
    if not times_line:
        return None
    
    times_str = times_line.group(1).strip()
    times = [float(t.strip()) for t in times_str.split(',')]
    return times

def find_newest_file_with_experiment(directory, experiment_name):
    """Find the newest .txt file in directory that contains 'Experiment: <name>'."""
    txt_files = glob.glob(os.path.join(directory, '*.txt'))
    matching_files = []
    
    for filepath in txt_files:
        with open(filepath, 'r') as file:
            content = file.read()
            if f"Experiment: {experiment_name}" in content:
                matching_files.append(filepath)
    
    if not matching_files:
        return None
    
    # Return the newest file
    newest_file = max(matching_files, key=os.path.getmtime)
    return newest_file

def plot_times(times, experiment_name, timestamp):
    """Plot the time values and save as an image file."""
    plt.figure(figsize=(10, 6))
    runs = list(range(1, len(times) + 1))
    plt.plot(runs, times, marker='o', linestyle='-', color='b')
    plt.title(f'Time Values for Experiment: {experiment_name}')
    plt.xlabel('Run')
    plt.ylabel('Time (seconds)')
    
    # Use max 20 evenly distributed x-axis ticks
    num_ticks = min(len(runs), 20)
    tick_positions = [runs[int(i * (len(runs) - 1) / (num_ticks - 1))] for i in range(num_ticks)] if num_ticks > 1 else runs
    plt.xticks(tick_positions)
    
    plt.grid(True, axis='x')
    
    # Save the plot to a file with the same timestamp as the .txt file
    output_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'plots')
    os.makedirs(output_dir, exist_ok=True)
    output_file = os.path.join(output_dir, f'times_{experiment_name}_{timestamp}.png')
    plt.savefig(output_file)
    print(f"Plot saved to: {output_file}")
    plt.close()

def main():
    if len(sys.argv) < 2:
        print("Usage: python plot_times.py <experiment_name>")
        sys.exit(1)
    
    experiment_name = sys.argv[1]
    res_directory = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'res')
    
    newest_file = find_newest_file_with_experiment(res_directory, experiment_name)
    if not newest_file:
        print(f"No file found with Experiment: {experiment_name}")
        sys.exit(1)
    
    # Extract timestamp from the filename (e.g., res-20260919-163935.txt -> 20260919-163935)
    filename = os.path.basename(newest_file)
    timestamp = filename.replace('res-', '').replace('.txt', '')
    
    times = extract_times_from_file(newest_file)
    if not times:
        print(f"No time values found in file: {newest_file}")
        sys.exit(1)
    
    print(f"Plotting time values from: {newest_file}")
    print(f"Times: {times}")
    plot_times(times, experiment_name, timestamp)

if __name__ == '__main__':
    main()