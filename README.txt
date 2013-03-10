Kim Cooperrider
CS 1550
Project 2
Fall 2013
kac162@pitt.edu

README.txt


--COMPILATION INSTRUCTIONS--
To compile this code, just run the following command:
	gcc mysortapp.c -o mysortapp



--RUNNING INSTRUCTIONS--
Run program with following command: 
	./mysortapp

Alternatively, you can provide flags at the command line for custom initializaton. For a guide to possible flags, type:
	./mysortapp --help
Or run without commands:
	./mysortapp

Additionally, you may run the program with default values provided in my_header.c; they are overwritten by any user-provided flags. The running program will print its values in the shell after initialization. To run the program with defaults, type:
	./mysortapp --defaults
Or 
	./mysortapp --d

When the program is running, you will see some minor log output. To view a more detailed trace of the program's execution, simply open my_header.txt and change the value of DEBUG to 1. By default, the output can be found in output.txt.


--OTHER FILES INCLUDED--
Some test files have been included. Samples include:
	records100.txt
	records_medium.txt
	records5.txt


