<h2>*Emptiness check: Algorithms based on depth-first search*</h2>

We present two emptiness algorithms that explore A using depth-first search (DFS):

    1. Nested -- used by NBA only

    2. Two-stacked - used by default for both automatone types (NBA/NGA)

 NBA -- nondeterministic Büchi automaton;

 NGA -- nondeterministic Generalized Büchi automaton;

This program also provide NGA-to-NBA converter. For greater compatibility.

The AIM of the program is to answer if A is nonempty and if it has an accepting lasso!
'THIS_BINARY' usage:

- --generator         [number > 0]    enable generation mode. See more info by calling it with help parameter;
- --help              [NONE/bool]     Show info about this binary (programm);
- --nba               [NONE/bool]     Works only with NBA (converts NGA if needed);
- --non_optimal_only  [NONE/bool]     Invokes Nested algorithm. We use Two-stack by default (because optimal);
- --in_file           [text]          Input file name where we store interested automaton;
- --out_file          [text]          Output file name where we will dump converted automaton (if will exist);

Return true or false for selected algorithm

************************

<h2>*Statistic generator. Could show comparative characteristic for all methods*</h2>

    "--generation" parameter will set up number of repetition for each approach. 0 - means disabled
        
There are some additional parameteres to work with generation mode. 

'THIS_BINARY --generated 1' usage:

-- -help          [NONE/bool]     Show info about this this generated mode;
- --states        [number]        Number of power of 10 for automaton states. Will be generated 10^0, 10^1,..,10^n
- --trees         [number]        Number of generated trees for the transition table. Default value is a minimal for really similar table (2);
- --sets          [number]        Number of sets in final states container. 
                                  Default value for NBA (1);
- --edges         [number]        Number of the edges for each (not leaf and not pre-leaf) vertex in the tree. 
                                  Default value for binary tree (2);
- --out_file      [text]          Output file name where we will dump generated information;

************************

<h3>BFS Search will be supported later</h3>
