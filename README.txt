There are three input files to the propgram: PRPS.txt, KB.txt, ALPHA.txt

PRPS.txt

    This file contains a propositional statement using a very very basic syntax.
        Single character upper case letters are propositional symbols
        Single character lower case letters are arguments to propositional symbols
        Each propositional symbol takes exactly one argument
        Arguments to propositional symbols are surrounded by [ ] brackets. No spaces betwee brackets and other characters.
        Only time you should use a space is between the ] bracket and the operator symbols


        Operator symbols:
            ^ = AND
            v = OR
            -> = IMPLIES
            = = TAUTOLOGY

        Statements can be put in ( ) parentheses. For instance: 
            ((A[a] ^ A[b]) v (B[a] ^ B[b])) = C[c]

ALPHA.txt

    A single propositional symbol with argument as before, except that it is followed either by == 1 or == 0
    This statement is the one that the software is supposed to infer about.
        == 1 means its trying to infer if it is true
        == 0 means its trying to infer if it is false

KB.txt

    This contains the knowledge base of things currently known. Syntax is this:

        First row: first character is a space. 
            After which are a string of lowercase characters with no spaces between
            These represent arguments
        First col: first character is a space.
            After which are a string of uppercase characters with no spaces between
            These represent propositional symbols that take arguments


        Now, these form a grid to identify a knowledge base.


        As an example you have


       1 abc
       2A1  
       3B1  
       4C 1 

        where the 1,2,3,4 represent line numbers as seen in a text editor
        note that A,c coordinate is a space, which represents an unknown truth value for that.
