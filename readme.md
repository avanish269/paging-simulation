# Paging Simulation
Here, I have simulated a page table. Given a sequence of memory accesses and whether each access was read or write, the program outputs the number of page faults, the number of reads from the disk and the number of writes to the disk. The specifications are given below.

## Details:
1. I have implemented 5 strategies, Optimal Page Replacement (OPT) policy, First In First Out (FIFO) replacement policy, Least Recently Used (LRU) replacement policy, Approximating LRU using clock algorithm (ClOCK), and Random page (RANDOM) replacement policy given in Operating Systems: Three Easy Pieces.

2. The program takes input a trace file, an integer denoting the number of frames available, and a string which would denote the strategy. Example command ./foo trace.in 100 OPT.

3. The trace file is a list of virtual memory addresses followed by letter R or W, denoting type of memory access(read or write).

4. The size of one frame is 4KB and maximum number of frames is 1000.

5. As the trace file is generated by one process, only one page table is needed and also since process never ends pages are freed only for replacement.

6. Initially physical frame is free and frames are numbered starting from 0.

7. No optimization for the number of writes to the disk by taking into account the dirty bit for page replacement has been implemented.

8. For RANDOM the seed has been set to 5635.

9. In addition to the above, there is a verbose option to see which page has been brought in and which page has been dropped/written.