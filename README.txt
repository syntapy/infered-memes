There are two input files to the propgram: PRPS.txt, ALPHA.txt

Input Files

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

    
Inference Algorithm:

    The inference takes place by first reading PRPS.txt into the knowledge base, denoted Kb here, and then reads ALPHA.txt as what will be infered, denoted alpha here.

    Then, the sentece Kb ^ !alpha is constructed as a single sentence, represented in a binary tree.
    Then the tree is converted into conjuctive normal form, and the resolution algorithm is performed on the clauses to find a contradiction in the sentence, which would mean inference takes place.
    If the resolution algorithm yields that the sentence is logically self-consistent, no inference takes place.

How to compile:

    At the root of the source tree, which contains the README.txt file, just type in
        
        make

    at the  prompt

    You'll need colorgcc installed, and ctags also. For these just type in
        
        sudo aptitude install colorgcc ctags

HOW TO RUN

    Simply write your knowledge base in the first line of PRPS.txt
    The statement you wish to test if it is implied from the knowledge base goes into ALPHA.txt
    After compiling type in
        
        ./resolve

    at the prompt

Further work:
    
    The resolution algorithm is done in a basic manner only, and does not have any intelligent means to select which clauses are compared in any way, thus making it very very innefficient for non-trivial logic sentences.

Known Bugs:
    
    None right now. However, it has not been rigorously tested yet, but is shown to work in the following cases:
        
        (A[a] -> B[b]) ^ (B[b] -> C[c]) ^ (C[c] -> D[d]) correctly predicts that (A[a] -> D[d]) is infered
        (A[a] -> B[b]) ^ (B[b] -> C[c]) ^ (C[c] -> D[d]) correctly predicts that !(A[a] -> D[d]) is not infered


        A[a] ^ 4 x{A[x] -> B[x]} correctly predicts that B[a] is infered
        A[a] ^ 4 x{A[x] -> B[x]} correctly predicts that A[a] is not infered
