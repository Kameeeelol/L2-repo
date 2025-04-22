# README

This project demonstrates how to:

1. **Generate matrix-chain instances** with random dimensions.  
2. **Solve** each instance via a stack-based backtracking algorithm to find the optimal parenthesization.  
3. **Compare** performance and visualize results using a single or multiple results files.

## Directory Structure

```
.
├─ generate_instances.c
├─ solve_backtracking.c
├─ compare_algos_plot.py
├─ README.md
```

### 1. `generate_instances.c`
- **Purpose**: Create a set of random matrix-chain multiplication instances for varying \(n\).  
- **Usage**:
  ```
  ./generate_instances <minN> <maxN> <m> <maxDim> <outputFile>
  ```
  where:
  - `<minN>`: smallest number of matrices
  - `<maxN>`: largest number of matrices
  - `<m>`: number of random instances per value of \(n\)
  - `<maxDim>`: maximum dimension size for any matrix dimension
  - `<outputFile>`: text file in which to store generated instances

- **Output Format**:
  1. First line: total number of instances \(T\).
  2. For each instance:
     - One line with \(n\).
     - One line with \(n+1\) space-separated integers denoting the dimensions array `d[0] ... d[n]`.

### 2. `solve_backtracking.c`
- **Purpose**: Read the generated instances from a text file, solve each via a stack-based backtracking algorithm, and record results.  
- **Usage**:
  ```
  ./solve_backtracking <inputFile> <outputFile>
  ```
  where:
  - `<inputFile>`: the file produced by `generate_instances.c`.
  - `<outputFile>`: text file in which to write results.

- **Output Format**:
  Each line in the results file has the form:
  ```
  n timeSeconds bestCost bestParenthesization
  ```
  - `n`: number of matrices  
  - `timeSeconds`: time taken by the backtracking algorithm for this instance  
  - `bestCost`: the minimum scalar multiplications  
  - `bestParenthesization`: string representation of the optimal parenthesization

### 3. `compare_algos_plot.py`
- **Purpose**: Parse one or more results files, compute average solve times for each \(n\), and produce a plot comparing **time vs. \(n\)** across multiple algorithms or solution approaches.  
- **Usage**:
  ```
  python3 compare_algos_plot.py results_backtracking.txt [results_dp.txt ...]
  ```
  - Reads each results file, expecting lines of the form:
    ```
    n time cost parenthesization
    ```
  - Groups times by \(n\), computes the average for each file, and plots them on a single chart.
  - The figure is saved as `compare_algos_plot.png`, and also shown on screen (if in a GUI environment).

## Compilation and Execution

1. **Compile**:
   ```bash
   gcc generate_instances.c -o generate_instances
   gcc solve_backtracking.c -o solve_backtracking
   ```

2. **Generate Instances**:
   ```bash
   ./generate_instances 2 6 3 100 instances.txt
   ```
   - Produces random instances for \(n\) from 2 to 6.  
   - For each \(n\), 3 random instances are generated (total of \(5 \times 3 = 15\) instances).  
   - Maximum dimension size is 100.  
   - Saves all instances to `instances.txt`.

3. **Solve with Backtracking**:
   ```bash
   ./solve_backtracking instances.txt results_backtracking.txt
   ```
   - Reads the 15 instances from `instances.txt`.  
   - Enumerates \emph{all} parenthesizations, updating the best solution whenever a better cost is found.  
   - Writes final outcomes (time, bestCost, and bestParenthesization) for each instance to `results_backtracking.txt`.

4. **Visualize**:
   ```bash
   python3 compare_algos_plot.py results_backtracking.txt
   ```
   - Reads the file `results_backtracking.txt`.  
   - Groups times by \(n\).  
   - Plots **average time** vs. **\(n\)**.  
   - Saves the figure as `compare_algos_plot.png` and also displays it on screen.

### Example Workflow

1. **Generate**:
   ```
   ./generate_instances 2 6 3 100 instances.txt
   ```
2. **Solve**:
   ```
   ./solve_backtracking instances.txt results_backtracking.txt
   ```
3. **Plot**:
   ```
   python3 compare_algos_plot.py results_backtracking.txt
   ```

The resulting chart (`compare_algos_plot.png`) will illustrate how backtracking performance evolves with \(n\).

**Notes**:
- Backtracking enumerates all possible parenthesizations (Catalan complexity), which becomes infeasible for larger \(n\).  
- You can add more results files (e.g., from DP or other algorithms) and plot them together:  
  ```
  python3 compare_algos_plot.py results_backtracking.txt results_dp.txt
  ```
- Ensure you have [matplotlib](https://matplotlib.org/) installed:
  ```
  pip install matplotlib
  ```
```