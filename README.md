# GrammerAnalysis
Parses through an input grammar, finds and removes rules with useless symbols or unreachable rules.  Calculates First and Follow sets of each rule and determines if the grammar has a predictive parser.

Utilizes a makefile with following commands.
clean: deletes current executables.
make: compiles code into new executable files
run: runs exectuables while piping in a txt file.
to utilize different functions within the program modify th ecurrent makefiles run command and change the middle number to 1-5
e.g.
./$(EXEC) <1-5> < <*inpute File>
