# 472-Project-1

COMP SCI 472 Project 1

Steven Le

10/23/2024

Fall 2024

Professor Janghoon Yang

# Project Description
In this project, we are tasks with the objective of developing a system that can process multiple large files by using a few ideas that we discussed in class. We will be using techniques such as multithreading, multiprocessing and inter-process communication or IPC for short. From doing this project, we will be able to further dive into the positive and negative sides of these topics and how they may compare. We will also be using thread ideas such as fork() and child process to process each file to count the frequency of specific words. 

# How to use program
To use this program, we will need to firstly load up the main code into an ide that supports the C programming language. For my ide of choice, I used Google Collab for this project. After you have the code set up, you must upload or import the calgary_corpus files into the ide. Make sure they contain the seven files needed for this program to execute correctly. With all these aspects set up, you can now run the code and the system will print out the analytics of the file processes. 

# Code Structure
The code is mostly inspired by the class notes and Google Collabs that we learned with. So it is written in C, it brings all the libraries we need at the top of the program. We set up the threads so that there is a maximum of 4 and each one of these threads will have a data chunk of 1024 given to process their tasks. There is a function made to process and read each of the seven files that we need to check. When the threads process each file, they will seach for the specified words that we set up in the program and if the specific word is found, then it will be stored in an array and the number of times it shows up in that file. There is a struct that will contain all the analytics that we need to keep count of as part of the requirements. For our performance evaluations, it will print out the time it took, how much memory was used, and counts for each target word from the caguray corpus files. In addition, we have method that allows for multiple threads to work within a single process. There are POSIX, parent threads and child threads that are set up to perform this multitasking for each file. "fork()" will be responsible for creating child process so it can run through the process_single_file function. The POSIX pthreads are split into different chunks to process the files for parallel processing. For the functions of IPC, the child process communicates with the parent process by sending them the word counts back to them. "pipe()" is the system call that has each child process write its statistics for the parent to read from. Error handling includes elements such as file operation like reading and opening, pipe creations, memory operations and checks for things including if pid is equal to -1 to print if this call fails.

# Diagram of Code Structure

![472 Project Diagram drawio](https://github.com/user-attachments/assets/0562a310-4524-4543-9d48-ac4d7541f6df)


# Samples of word processing from the files

![Screenshot 2024-10-23 215808](https://github.com/user-attachments/assets/10c8bbf8-a8be-45b4-a15e-683bf84a94f7)

![Screenshot 2024-10-23 215905](https://github.com/user-attachments/assets/2f74588d-c294-4e88-a9dd-22f770b52e53)

# Findings and discussions
As for the findings for this implmentation, the challenges had to be correctly implementing the IPC and resource managing processes. Needing to carefully manage the allocation of each resource and the process of making sure the functions worked was one of the more challenging details in this project. Another challenge, was activily trying to change the data so we can get values above 0 for time measurements, CPU usage, and etc. For some reason, the program will not spit out an accurate display of these analytics. This is something that I want to improve on since it is important and I cannot seem to get it to work. I am not sure why but perhaps it could be the file sizes are too small or large. Whatever the issue is, I want to know how I can change the values, especially time measurement because I even set that to calculate in nanoseconds. Some advantages of multiprocessing, is that it is highly scalable and it has better memory cleanup on process termination. Some disadvantages of this processing method is that it uses more memory, it is a slower process compared to threads and has limited sharing capabilities. On the other hand multithreading is great for efficient memory sharing and has a lower memory threshhold. The downsides to this is that it is susceptible to deadlocks, has risks for corruption since one thread crashing is dangerous for all of them. 


