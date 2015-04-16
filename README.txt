OVERVIEW

    This project performs logical inference on logic sentences which are read from file. 
    One logic sentence is placed in PRPS.txt, and another is placed in ALPHA.txt. 
    The software then reads both and deduces whether or not the sentence in ALPHA.txt 
    is infered from the sentence in PRPS.txt.  When it comes to parsing logic sentences, 
    the software fully supports existential and universal quantifers which can be nested 
    to arbitrary depth in other quantifier statements.

    Denote the sentence in PRPS.txt as KB, and the sentence in ALPHA.txt as alpha.
    After reading and parsing the sentences, the logical equivalent of them is stored
    in RAM as a binary tree. Specifically, the tree represents KB ^ !alpha, where
    the exclammation denotes negation.

    Then, this tree is converted into conjuctive normal form and the resolution algorithm
    is used to determined whether it is a contradictory statement or not.
    
    If KB ^ !alpha is contradictory, then inference takes place, otherwise no inference
    takes place.

    The resolution of each clause in the conjunctive normal form tree is done in a naive
    way, however, making the algorithm very inneficient, and the NP-Complete nature of it
    easily becomes apparent on a non-trivial textbook problem (see the README.txt for
    details on the syntax):

        KB:
            
            4 x{ 4 y{Animal[y] -> Loves[x, y]} -> 3 y{Loves[y, x]}} ^ 4 x{3 z{Animal[z] ^ Kills[x, z]} -> 4 y{!Loves[y, x]}} ^ 4 x{Animal[x] -> Loves[jack, x]} ^ Kills[jack, tuna] v Kills[curiosity, tuna] ^ Cat[tuna] ^ 4 x{Cat[x] -> Animal[x]}
        alpha:
            
            Kills[curiosity, tuna]

    For this input, the 4GB RAM computer runs out of memory after a few days of computation.

    However, for more trivial examples, the program runs:
        KB: (A[a] -> B[b]) ^ (B[b] -> C[c])
        alpha: (A[a] -> C[c])

        KB: A[a] ^ 4 x{A[x] -> B[x]}
        alpha: B[a]

    and inference takes place for these cases


    To solve the first, and complex, textbook problem, binning heuristics need to be 
    used to intelligently decide which clauses to resolve during the resolution process.


    The algorithm has been shown to produce correct result in the following cases:
        
        (A[a] -> B[b]) ^ (B[b] -> C[c]) ^ (C[c] -> D[d]) correctly predicts that (A[a] -> D[d]) is infered
        (A[a] -> B[b]) ^ (B[b] -> C[c]) ^ (C[c] -> D[d]) correctly predicts that !(A[a] -> D[d]) is not infered


        KB: A[a] ^ 4 x{A[x] -> B[x]} 
        alpha: B[a]
        correctly predicts that B[a] is infered

        KB: A[a] ^ 4 x{A[x] -> B[x]}
        alpha: A[a] 
        correctly predicts that A[a] is not infered

INPUT FILES AND THEIR SYNTAX

    Note: in both PRPS.txt and ALPHA.txt, only the first line is read. All other lines are ingored.
    ===============================================================================================

    PRPS.txt

        This file contains a propositional sentence using a particular basic syntax.
            A single character upper case letter followed by any number of a-z characters is a propositional symbol / function
            Propositional symbols are followd by square brackets with any number of arguments inside them
            Each argument consists of a string of a-z lowercase characters of arbitrary length
            Each propositional symbol takes exactly any number of arguments, each seperated by a comma, and a space after the comma

            Foral symbols are represented by the numeral 4; existential operators are represented by numeral 3. 
                Each is followd by one or more argument symbols and then the containing sentence in curly braces
                    For instance: 4 creature, animal{(Eats[animal, creature] ^ Insect[creature]) -> Insectivor[animal]}
                    OR: 4 boy{3 girl{Teaches[girl, boy]}}

                    This latter one says "For every boy, there is a girl who teaches that boy."

            Operator symbols:
                ^  -- AND
                v  -- OR
                -> -- IMPLIES
                =  -- TAUTOLOGY

                !  -- Negation, i.e. !Car[object] or !(Plane[object])

            Statements can be put in ( ) parentheses. For instance: 
                ((Car[vehicle] ^ Belongs[vehicle, ted]) v (B[a] ^ B[b])) = C[c]

    ALPHA.txt

        A single propositional sentence with as described in PRPS.txt instructions above
        This statement is the one that the software is supposed to infer about.
        It can be a single symbol such as Red[car] or a full fledged sentence (once the bugs are worked out of course)

    
    In both files, only the first line is read by the program

    
How to compile:

    At the root of the source tree, which contains the README.txt file, just type in
        
        make

    at the  prompt

    You'll need colorgcc installed, and ctags also. For these just type in
        
        sudo aptitude install colorgcc ctags

HOW TO RUN
    Note: this program was written and tested only in Linux i386 architecture.

    Simply write your knowledge base in the first line of PRPS.txt
    The statement you wish to test if it is implied from the knowledge base goes into ALPHA.txt
    After compiling type in
        
        ./resolve

    at the prompt

FURTHER WORK:
    
    The resolution algorithm is done in a basic manner only, and does not have any intelligent means to select which clauses are compared in any way, thus making it very very innefficient for non-trivial logic sentences.

KNOWN BUGS:
    
    None right now. However, it has not been rigorously tested yet.
