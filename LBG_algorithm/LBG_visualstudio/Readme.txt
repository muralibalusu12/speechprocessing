
Roll No: 120101043
Name: Murali Raghu Babu B
Assignment 6: LBG algorithm
Visual Studio Version: 2013

Steps to execute the code:
1. You can find the input for the program the file Universe.txt which is present inside the k-means folder.

2. The configuration settings are taken from "configuration.txt". 
   The program take the size of the codebook and the weights to be used in Tokhura's distance calculation as inputs.
   
3. Steps to run: Open the solution file, run F5 to build and run.

4. Outputs: At every stage of the iteration in the LBG algorithm when moving from a particular codebook size to its double a file is generated.
and we move from  1 -> 2 -> 4 -> 8 -> 16 -> 32. So, there are 6 files - Logfile.txt, Distortion.txt, Codebook.txt at every stage.

Logfile.txt -  contains various intermediate outputs like no. of vectors that are allotted to each cluster and the distortion in every iteration.
Distortion.txt - contains the distortion at every stage in t
Codebook.txt - contains the final codebook vectors at every stage of the LBG algorithm

There is also a separate excel file outside the project file named "lbg_distortion_iteration_graph.xlsx" which contains the 
graph of distortion vs the no. of iterations.
