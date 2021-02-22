# Turing Machine Project
## A non-deterministic of a Turing Machine using a set of rules and an input, passed by the user.

# **Structure and classes**

# 1. Tokenizer
## The tokenizer is responisble for parsing the given input(rules and input tape) into tokens and distribute them into structures respectively.

# 2. Graph
## The graph class represents the rules linked between each other with their respective edges. An example for such a graph is the following: 
![Instruction Graph sample](https://github.com/Bitnutz/Turing-Machine-1.0/blob/main/sampleGraph.png)

# 3. Tape
## The tape class represents the tape of the machine. Two stacks are used, the left hadn-side is storing the symbols on the left of the tape,while the right hand-side is storing the the head and the symbols to the right.

# 4. Machine
## This class is the main engine of the project, using all of the above classes.

# Sample program for maiking all 0s and 1s Xs. (The sample is the same as the image above)

__Instructions__:
```init:{start}
X{start}->X{start}R
0{start}->X{B}R
1{start}->X{C}R
_{start}->_{halt}R
0{B}->0{B}R
X{B}->X{B}R
1{B}->X{D}L
1{D}->1{D}L
0{D}->0{D}L
X{D}->X{D}L
_{D}->_{start}R
0{C}->X{D}L
X{C}->X{C}R
1{C}->1{C}R
```
__Input tape__:
`
0011
`\
__Output tape__:
`_ _ _ _ _ _ X X X X _ _ _ _ _ 
`\

