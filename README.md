# Parallel-computing-course

The main tasks of the credit module.  
According to the requirements of the program of the academic discipline,   
after mastering the credit module, students must demonstrate the following learning outcomes:

Knowledge  
* basic principles of building systems for parallel and distributed computing;
* leading features of parallel numerical methods, conditions of their use, possibilities
adaptation to specific tasks, technologies for their implementation;  

Skill
* design of parallel and distributed systems;
* mastery of parallel programming technology;  

# Lab tasks

lab1

* Using parallelization, solve the problem of filling a square matrix with random numbers and placing the maximum row element 
on the main diagonal.   

* Use parallelization on a fixed number of threads: 2 times less than the number of physical cores, on the number equal to 
the number of physical cores, on the number equal to the number of logical cores, on the number that is 2, 4, 8, 16 times 
the number of logical cores, and using different data dimensions.

lab2

* Implement a configurable thread pool that is served by 4 worker threads and has a single execution queue. 
Jobs are added immediately to the end of the execution queue or are discarded if the execution time of all 
the jobs in the queue and the potential job to be added exceeds 60 seconds. The task is executed from the 
buffer when a free workstream appears. The task takes a random time between 5 and 12 seconds.

* Do some testing and calculate the number of threads created and the average time a thread spends in the waiting state. 
Determine the maximum and minimum time until the queue is filled, and the number of rejected tasks.
