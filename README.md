# Projects-in-cpp
Project Descriptions:

DNA_Sequence_Alignment: \n
  Program provides 2 functions: 
  1. Outputs a file containing amino acid counts of data from a FASTA file. FASTA file data is parsed using a state machine process.
  2. Outputs a file containing the Needleman-Wunsch score between 2 files of nucleotides. Needleman-Wunsch algorithm is calculated by utilizing dynamic programming concepts.
  
Genetic_Algorithm_Project:
  Functional program implements a genetic algorithm to determine the best tour of LA landmarks, starting and ending at LAX. Initial population was generated based on parameters 
  given through command line inputs. After the initial population was generated, the fitness of each individual was calculated using the Haversine formula. Selection and crossover
  were both done randomly (seeded) throughout each iteration of a new population. Number of iterations was predetermined through command line arguments.
  Most of the functions for the genetic algorithms were implmented using lamdas to follow a functional programming style.
  
RLE_Compression:
  Simple program to perform a run-length encoding form of compression to run on input files referenced from the command line.
  

Virtual_Machine:
  Emulation of a simple virtual machine created using metaprograms,features 15 32-bit registers, 1 KB stack space, and 3-bit color graphics.
  
tbb_Password_Decrypt:
  Program that tries to decript passwords encripted with SHA-1. Performance was optimized using Intel Threading Building Blocks. Passwords were decrypted using either a dictionary
  attack or through brute force. Open addressed hash table was implemented for dictionary attacks and brute force decryption speed was optimized using parallelization with Thread
  Building Blocks.
