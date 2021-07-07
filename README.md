OVERVIEW
--------

This is a logical inference engine and associated parser I did for a college class

The user places the input in the files `PRPS.txt` and `ALPHA.txt` using a simple 
syntax for relational logic sentences specific to this program as described below

The software reads both files and deduces whether or not the sentence in `ALPHA.txt` 
is infered from the sentence in `PRPS.txt`.

Only the 1st line of each input file is read by the program

HOW IT WORKS
------------
The code reads the contents of `PRPS.txt` as one sentence, denoted here as `KB`,
and the contents of `ALPHA.txt` as another sentence, denoted as `alpha`.

After acquiring `KB` and `alpha`, inference is determined by proving whether `KB & !alpha` 
is a contradictory (i.e. impossible) statement. If it is contradictory, then `alpha` has to be
true if `KB` is true

The proof is done by converting the sentence `KB & !alpha` into conjunctive normal form,
and then reducing disjunctive clauses until a simple statement is reached that is obviously
impossible or obviously true

ALGORITHM OVERVIEW
------------------
Sentences in the program are represented as binary trees in memory. Leaf nodes 
are relational logic symbols, and internal nodes are logic operators from the set
(AND, OR, IMPLIES, EQUALS, FORALL, EXISTS). All nodes can be accompanied by a negate 
operator.

For instance, the statement `A[a] ^ B[b]` would be converted to a 3 element binary
tree with the AND operator as the root node, and `A[a]` being the left child, and `B[b]` 
being the right child

Each node in the tree is a pointer to a struct, which contains info on whether that node
is a logic operator or a relational logic symbol, and which of either it is

The parser recursivively handles subsentences in parantheses as sub-trees

Universal quantifiers are processed using skolemization. This involves replicating the 
universally quantified subtree across all possible combinations of logic functions and 
their respective arguments, where the space of all possible symbols is limited to those 
seen during parsing

Existential quantifiers are done by simply generating a new symbol not seen during parsing
(and not generated yet either)

It is straightforward to combine both quantifier types in a sentence

The recursive sentence parsing allows quantifiers to be nested to arbitrary depth
(as limited by recursive stack size and memory)

The resolution of each clause in the conjunctive normal form tree is done in a naive
way, however, making the algorithm very inneficient, and the NP-Complete nature of it
easily becomes apparent on the following textbook problem where:

`KB = 4 x{ 4 y{Animal[y] -> Loves[x, y]} -> 3 y{Loves[y, x]}} ^ 4 x{3 z{Animal[z] ^ Kills[x, z]} -> 4 y{!Loves[y, x]}} ^ 4 x{Animal[x] -> Loves[jack, x]} ^ Kills[jack, tuna] v Kills[curiosity, tuna] ^ Cat[tuna] ^ 4 x{Cat[x] -> Animal[x]}`

and `alpha = Kills[curiosity, tuna]`

For this input, a 4GB RAM computer runs out of memory after a few days of computation.

However, for more trivial examples, the program runs correctly:
 - `KB = (A[a] -> B[b]) ^ (B[b] -> C[c])` and `alpha = (A[a] -> C[c])`
 - `KB = A[a] ^ 4 x{A[x] -> B[x]}` and `alpha = B[a]`

The algorithm has been shown to produce correct result in the following cases as well:
 -  `KB = (A[a] -> B[b]) ^ (B[b] -> C[c]) ^ (C[c] -> D[d])` predicts that `alpha = (A[a] -> D[d])` is infered
 -  `KB = (A[a] -> B[b]) ^ (B[b] -> C[c]) ^ (C[c] -> D[d])` predicts that `alpha = !(A[a] -> D[d])` is not infered
 -  `KB = A[a] ^ 4 x{A[x] -> B[x]}` predicts that `alpha = B[a]` is infered
 -  `KB = A[a] ^ 4 x{A[x] -> B[x]}` predicts that `alpha = A[a]` is not infered

INPUT FILES AND THEIR SYNTAX
----------------------------

*Note: in both `PRPS.txt` and `ALPHA.txt`, only the first line is read. All other lines are ingored.*

**`PRPS.txt`**

This file contains a relational sentence using a particular basic syntax.
 -  A single character upper case letter followed by any number of a-z characters is a relational symbol / function
 -  Propositional symbols are followd by square brackets with any number of arguments inside them
 -  Each argument consists of a string of a-z lowercase characters of arbitrary length
 -  Each relational symbol takes exactly any number of arguments, each seperated by a comma, and a space after the comma

 -  Foral symbols are represented by the numeral 4; existential operators are represented by numeral 3. 
   - Each is followd by one or more argument symbols and then the containing sentence in curly braces
         For instance: 

          - `4 creature, animal{(Eats[animal, creature] ^ Insect[creature]) -> Insectivor[animal]}`

          - `4 boy{3 girl{Teaches[girl, boy]}}`

         The second one says "For every boy, there is a girl who teaches that boy."

   - Operator symbols:

        `^`  -- AND

        `v`  -- OR

        `->` -- IMPLIES

        `=`  -- TAUTOLOGY

        `!`  -- Negation, i.e. `!Car[object]` or `!(Plane[object])`


   - Statements can be put in `(` `)` parentheses. For instance: 

        `((Car[vehicle] ^ Belongs[vehicle, ted]) v (B[a] ^ B[b])) = C[c]`

**`ALPHA.txt`**

A single relational sentence with as described in `PRPS.txt` instructions above
This statement is the one that the software is supposed to infer about.
It can be a single symbol such as `Red[car]` or a full fledged sentence

COMPILING
---------

This was tested on debian-based system

Dependencies are just gcc, ctags, and gnu make:
    
    `sudo apt install gcc exuberant-ctags make`

To compile just type:

    `make`


USAGE
-----

Note: this program was written and tested only in Linux i386 architecture.

Simply write your knowledge base in the first line of `PRPS.txt`
The statement you wish to test if it is implied from the knowledge base goes into `ALPHA.txt`
After compiling type in
    
    ./resolve

at the prompt

FURTHER WORK
------------
    
The resolution algorithm is done in a basic manner only, and does not have any intelligent means to select which clauses are compared in any way, thus making it very very innefficient for non-trivial logic sentences.

KNOWN BUGS
----------
    
None right now. However, it has not been rigorously tested yet.
