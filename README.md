# speechprocessing
assignments given during the speech processing course. The codes are self explanatory with inline comments. 

Inside Kmeans folder: 
1. kmeans_visual studio - A visual studio project file containing the code for Kmeans algorithm to calculate the codebook for the speech input feature vectors.
2. kmeans_cpp - A cpp file for the kmeans algorithm again for the same purpose and the input and output files.


Inside LBG_algorithm folder: 
1. LBG_visualstudio - A visual studio project file containing the code for LBG algorithm to calculate the codebook for the speech input feature vectors.
2. LBG_cpp - A cpp file for the LBG algorithm again for the same purpose and the input and output files.
3. lbg_distortion_iteration_graph - contains a graph of distortion versus iteration

Inside HiddenMarkovModel folder: 
1. hmm_visualstudio - A visual studio project file containing the code for hidden markov model.
2. hmm_cpp - A cpp file for the hidden markov model.
The initial model is in the a.txt, b.txt, pi.txt files. Two examples of observation sequences of the codebook vector are given in 1.txt and 2.txt. The Hidden Markov model uses the initial model given calculates the initial model likelihood, then the optimal state sequence for that model and the reestimates that model using the Expectation Maximization algorithm. This new model is again used to calculate the new model likelihood and the model is updated again and again in a similar way. The halting condition is to be specified in the code (it could be either until a minor change in the likelihood or based on the number of iterations).


Contact me at: muraliraghubabu1994@gmail.com for any queries.
