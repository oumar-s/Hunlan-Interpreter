# This program is an interpreter for the Hunlan programming language.

# How to run this program
* This program runs in the terminal
* This program requires a C++ compiler. 

1. Clone this repo.
2. Compile using your C++ compiler.
Ex: `g++ -std=c++11 -o interpreter interpreter.cpp main.cpp`
3. run the program by provinding a Hunlan file (txt file) 
Ex: `./interpreter "hello_world.hun`

# Hunlan Language Descriptions

## Instructions
Accepts one instruction per line
Allows empty lines with no instructions

## Comment

Everything starting with # till the end of the line is ignored. In other words, # is a symbol of one-line comment.

## Printing
PRINT command prints to the console.
`PRINT 10`
`STRING str`
`str = "Hello World"`
`PRINT str`

## Variables
Has variables of two datatypes: NUMBER and STRING
NUMBER is a 8-Byte integer
STRING is a string
Variable declaration looks like this
datatype  variable_name
Example: 
`NUMBER num`
`num = 10`
`PRINT num`
 

# Mathematical Expressions

`NUMBER num1`
`NUMBER num2`
`NUMBER _result`
`num1 = 3 + 2`
`num2 = 12 * 1`
`_result = num1 + num2`
`PRINT _result`
`PRINT "\n"`

 
