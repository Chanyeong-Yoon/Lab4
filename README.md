# [Lab 4] Memory System Simulation

## Introduction

The goal of this lab is to understand how each cache/memory component in the memory hierarchy interacts to each other at a high level. To do so, you will implement a cycle-level architecture simulator that focuses on the design of a memory hierarchy.

> IMPORTANT: You should use Linux (or WSL) for Lab 4. If you have a Mac with Apple silicon, you may run Linux in UTM. You also need to commit and push your code to your GitHub repo after completing each part. [Tag the commit](https://www.atlassian.com/git/tutorials/inspecting-a-repository/git-tag) for each completing part (e.g., "Part I"). We may grade the code in your repo if you do not complete all parts.

## Part I: `Cache Base`  Implementation

> In Part I, you only need to modify the code in the `cache_base` folder. 

You will first need to implement a `functional` cache simulator that reads a memory trace and performs operations to update the metadata in the `tag store`, depending on a cache hit or a cache miss. You do not need to implement the `data store` throughout this lab. Your `cache base` should keep track of the statistics (e.g., #accesses, #hits, #misses, #writes, #writebacks) and update the statistics for every memory reference. You can modify most of the code throughout this lab if needed (e.g., adding some new variables/functions/signatures/etc.)

### Part I-A: Direct-Mapped Cache

To do so, the first step is to implement a direct-mapped cache. Note that a direct-mapped cache *does not* have to do with cache replacement policy. The initial code is provided for your convenience. You are free to modify most of the code provided throughout this lab.

#### Cache Parameters

The cache simulator takes as inputs the following parameters and initiates the cache structure.

- **Cache Size**: Total cache capacity in bytes
- **Associativity**: For the direct-mapped cache, this will be 1.
- **Line Size**: Cache block size in bytes

These parameter values need to be always powers of two. Your cache needs to be able to print out the cache statistics for different combinations of the parameters.

#### Write-Allocate & Write-Back

Your cache needs to be a `write-allocate`, `write-back` cache. That is,

- A write miss allocates a cacheline in the cache with a **dirty** flag.
- When a line is evicted and the line is dirty, it is written back to memory and the writeback counter increments.

### Part I-B: Set-Associative Cache

The second step is to extend your direct-mapped cache to a set-associative cache with LRU replacement policy. In an `N`-way set-associative cache, each cache set has `N` cache blocks. A memory address is still mapped to a specific set, but the requested data can be found in any of the `N` blocks in the set.

#### Replacement Policy: True LRU

We need to maintain an LRU status of the blocks in each set.
* **Cache Hit**: The cache block is promoted to the MRU (most recently used) position.
* **Cache Miss**: You need to evict the LRU block, bring the new cacheline, and set it to **MRU**. Note that you first need to check if there exists an invalid cache block. If so, you need to use it first.

Note that the code that is needed for the LRU implementation is not included in the initial code. 

### Trace

The sample trace file is located in the **traces** folder. Each line in the trace consists of two fields.

- **1st field**: Indicates whether the memory reference is a data read (0), a data write (1), or an instruction fetch (2)
- **2nd field**: Memory address

> Note: You implement a unified (I/D) cache that caches both instructions and data for Part I.

### Compile & Run

You need to see if your `cache base` correctly works before moving on to the next parts. 

#### Compile Code 
(In the `cache_base` folder)
```
$ make
```

#### Run Simulation
```
./run_base <trace> <cache size (in bytes)> <associativity> <line size (in bytes)>
```

```
$ ./run_base ../traces/sample.trace 8192 2 64
```

### Tips & Cache Operations & Statistics

* You may want to write your own simple trace for which you can verify the answer by hand, and use it to check the results from the simulator.
* You probably need to use GDB (GNU Debugger) for debugging. Below are the links where you can learn GDB. 
  * https://www.youtube.com/watch?v=bWH-nL7v5F4
  * https://www.geeksforgeeks.org/gdb-step-by-step-introduction/
* The number of hits/misses includes all READ/WRITE/IF hits/misses.
* The LRU stack also needs to be updated for all the READ/WRITE/IF requests.
* This document may not describe all the straightforward cache operations. Please think carefully what will happen when you write the code and compute the statistics.

## Part II: Memory System Timing Simulator 

> In Part II, you will mostly need to implement the code in the `memory_system` folder; but, you probably still need to modify your `cache_base`.

In Part II, you implement a cycle-level architecture simulator that consists of a simple core, a `unified, single-level` cache and main memory. The good news is you do not need to modify the core (`core`) and main memory (`memory_controller`) folders, except for debugging purposes. :)

Basically, the timing simulator models the microarchitectural behavior at a cycle level. To do so, in our simulator, the core calls the `run_a_cycle()` function that advances a clock cycle, which hierarchically calls `run_a_cycle()` in the major components that we want to model with a cycle-level behavior.

Note that Part II builds on the `cache_base` that you implemented in Part I. To make your life easier, you first need to implement a timing simulator that matches the cache statistics to the ones in Part I.  With `single_request=1` in the config file, the core issues a memory request and **does not** issue another one to the memory hierarchy until the previous request completes (i.e., modeling sort of data returned to the core). With the flag on, you should see the same statistics as in Part I after completing Part II.

The provided skeleton code is almost complete for the DRAM only (`mem_hierarchy=0` in the config) configuration. But, when you compile the skeleton code and run it (see below), it does not process the trace to completion. Before putting the cache into the memory hierarchy, you first need to make it work for the case where the core sends/receives a request/data directly to/from the main memory (i.e., no cache yet scenario).

### Code Infrastructure for Timing Simulation

You need to understand how the Lab 4 infrastructure works to complete Part II. In addition to reading the comments in the code, you should also read the supplements below. 

* [Documents for Part II](http://comparch.snu.ac.kr/wiki/doku.php?id=class:430.322:lab4)
* [Doxygen](http://comparch.snu.ac.kr/lab4/html/index.html)

> For debugging, we **strongly** recommend you learn/use GDB (e.g., the commands like "b, p, n, s, info, bt, f, etc." will be your life savers.) and the DEBUG macro. Otherwise, you may not be able to complete Part II.

### Compile & Run

#### Compile Code  (In the project root folder)
```
$ make 
```
or (to enable the DEBUG macro)
```
$ make debug
```

#### Run Simulation
```
./memory_sim <trace> <config file>>
```

```
$ ./memory_sim ./traces/sample.trace ./configs/memory.cfg
```

## Part III: Extending Code to Implement Multi-Level Cache Hierarchy

Now, you will need to extend your simulator to model an multi-level cache hierarchy where there exist L1 and L2 caches. All the caches are write-allocate, write-back caches in Part III. 

### Part III-A: Unified L1 and L2

Again, to make your life easier, let's start from the case where we do not have separate L1I and L1D; note that L2 is always unified. The example configuration may look like this:

- **Unified L1 Cache (L1U)**: 2KB, 2-Way SA, LRU, 64B Line Size, 4-cycle latency
- **Unified L2 Cache (L2)**: 16KB, 4-Way SA, **Inclusive**, LRU, 64B Line Size, 10-cycle latency

#### Inclusive L2

Inclusive L2 means that all the cache blocks that reside in L1 are also in L2. That is, L2 is **inclusive** of L1. To implement inclusive L2, you need to implement the following.

#### L2 Cache Miss

If a memory request misses in L2, you need to bring the new block from memory and place it in **both L1 and L2**. Note that an L2 miss naturally implies that L1 does not have the requested block either. 

> For a write miss (e.g., when executing a store instruction), it is first treated as a read miss from the core. And, we bring the data from memory into L1/L2, then the pending write is committed. This makes the L1 block dirty and L2 block clean.

#### L1 Cache Miss + L2 Cache Hit

You can simply bring the cache block from L2 to L1 in this case. Even if the L2 block is dirty, the newly installed block in L1 will be clean; because the L1 and L2 blocks are the same, you do not have to write back the L1 block to L2 on its eviction later. Once there is a write on the L1 block, the L1 block becomes dirty, and it should be written back to L2 on its eviction.

#### L1 Eviction

* If the evicted L1 block is **dirty**, the victim block needs to be moved to L2. In this case, you **do not** update the LRU stack of the L2 cache; because that is not a cache access due to the memory reference. 
* Writeback does not contribute the number of accesses/hits/misses. Note that the number of L2 accesses is the same as the number of L1 misses.

#### L2 Eviction

On an L2 eviction, if there is the same cache block in L1, the L1 block needs to be invalidated, in order not to violate inclusion. This is called **back invalidation**. Note that the invalidated L1 block could be dirty. If that is the case, the invalidated L1 block needs to be written back to **memory** (not to L2), and you need to increment the writeback counter (#writebacks due to back-invalidation) for L1.

### Part III-B: Let's Separate L1-I and L1-D

Now, let's model separate L1I and LID caches as below.

- **L1 Instruction Cache (L1I)**: 2KB, 2-Way SA, LRU, 64B Line Size, 4-cycle latency
- **L1 Data Cache (L1D)**: 2KB, 2-Way SA, LRU, 64B Line Size, 4-cycle latency
- **Unified L2 Cache (L2)**: 16KB, 4-Way SA, **Inclusive**, LRU, 64B Line Size, 10-cycle latency

All the caches are write-back caches, but there will be of course no writebacks from `L1I`.

## Part IV: Enable Multiple Memory Requests

Now, let's enable issuing multiple memory requests by making `single_request=0' in the config file. Do your cache stats look fine? If you find something weird, can you think about why? Do fix your implementation to report the correct cache stats. :)

## Submission

We have **two deadlines** for this lab:
* Part I & II: **June 7 (SAT) 11:59 PM**
* Part III & IV: **June 19 (THU) 11:59 PM**

### Policy
* There will be **no extensions**.
* If you miss the first deadline for Part I & II, you can still submit by the second deadline, but the score for those parts will be capped at **70%**.
* Submissions after the second deadline will receive **zero** credit.

### What to Submit
*  Your code that we can compile and run  
  * Do not include the trace file in the zipped file.
* A **1-page** report that describes your implementation
* (**Part I**) You need to include the cache hit rate for the 16KB direct-mapped cache and 2-/4-/8-way set associative caches with the 64B linesize.
* (**Part II**)
  * Include the stats reported with the configuration of 16KB, 4-way SA, LRU, 4-cycle.
  * The L1 hit rate should be the same as the one in Part I.
* (**Part III**)
  * Include the stats reported with the example configuration.
    * The number of cache blocks that are invalidated in L1I and L1D due to back invalidation.
    * The number of writebacks from L1D to memory due to back invalidation
* (**Part IV**)
  * Include the stats reported with the same configuration as Part III.

### How to Submit
* Upload your compressed file (zip) to eTL
* Format: **YourStudentID_YOURLASTNAME_lab#**
  * e.g., 2025-12345_KIM_lab4.zip
* Please make sure you follow the format (10% penalty for the wrong format)
