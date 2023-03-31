/*Joseph Dunagan (Tue|Thur) &&  Sabien Najim (Mon|Wed) worked on thiis project together
 * Copyright (C) Mohsen Zohrevandi, 2017
 *               Rida Bazzi 2019
 * Do not share this file with anyone
 */
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include "lexer.h"
#include "inputbuf.h"
#include <list>
#include <string>
#include <vector>

using namespace std;
class Rule{
	public:
	string lhs = "";
	int rhsSize = 0;
	
	vector<string> rhs;//rhs of first is first sets of lhs
	vector<string> fol;
	
	Rule(string lhsName){
		lhs = lhsName;
	}
	
	void printRHS(){
		
		for (int i = 0; i <  rhs.size(); i++) {
        	cout << rhs[i] << " ";
    	}
	}
};
//Global Vars
list<Rule> grammer;
vector<Rule> first;
vector<string> terminals, nonTerminals;
string reservedHere[] = { "END_OF_FILE", "ARROW", "STAR", "HASH", "ID", "ERROR" };

void syntax_error()
{
	// outputs error message and exits the program
	cout << "SYNTAX ERROR !!!\n";
	exit(0);
}

// read grammar
void ReadGrammar()
{
	
	LexicalAnalyzer lexer;
	Token token;
	
    //token.Print();
	list<Rule>::iterator itr;
	itr = grammer.begin();
	
    while (token.token_type != END_OF_FILE && token.token_type != 3)
    {
		//Create lhs of new rule
        token = lexer.GetToken();

		if(reservedHere[token.token_type] != "HASH"){
			if(reservedHere[token.token_type] != "ID"){
				syntax_error();
			}
			grammer.push_back(Rule(token.lexeme));
			itr++;
			//cout <<"{ "<<token.lexeme << " TT: " << reservedHere[token.token_type] << " } -> ";

			//Move to the arrow
			token = lexer.GetToken();
			if(reservedHere[token.token_type] != "ARROW"){
				syntax_error();
			}

			//Get rhs
			token = lexer.GetToken();
			if(reservedHere[token.token_type] != "ID" && reservedHere[token.token_type] != "STAR"){
				syntax_error();
			}
			while(reservedHere[(int) token.token_type] != "STAR"){
				itr->rhs.push_back(token.lexeme);
				itr->rhsSize++;
				token = lexer.GetToken();
				if(reservedHere[token.token_type] != "ID" && reservedHere[(int) token.token_type] != "STAR" && reservedHere[(int) token.token_type] != "HASH"){
					syntax_error();
				}
			}
			//itr->printRHS();
			//cout << "\n";
		}
	}
}

bool isTerminal(string check){
	list<Rule>::iterator itrG;
	itrG = grammer.begin();
	int itr = 0;
	
	while(itr < grammer.size()){
		if(check == itrG->lhs){
			return false;
		}
		itr++;
		itrG++;
	}
	return true;
}

bool checkUsed(vector<string> used, string check){
	for(int i = 0; i < used.size(); i++){
		if(check == used[i]){
			return true;
		}
	}
	return false;
}

bool checkUsedRules(vector<Rule> used, string check){
	for(int i = 0; i < used.size(); i++){
		if(check == used[i].lhs){
			return true;
		}
	}
	return false;
}

int findIndex(vector<string> vec, string find){
	for(int i = 0; i < vec.size(); i++){
		if(vec[i] == find){
			return i;
		}
	}
	throw runtime_error("findIndex OUT OF BOUNDS\n");
}

int findRulesIndex(vector<Rule> vec, string find){
	for(int i = 0; i < vec.size(); i++){
		if(vec[i].lhs == find){
			return i;
		}
	}
	throw runtime_error("findIndex OUT OF BOUNDS\n");
}

// Task 1
void printTerminalsAndNoneTerminals(bool shouldPrint)
{
	string T = "", NT = "";
	
	list<Rule>::iterator itrG;
	itrG = grammer.begin();
	
	vector<string> used;
	
	int itrx = 0, itry = 0;
	bool isNonTerminal = false;
	//loop through whole grammer
	while(itrx < grammer.size()){
		if(checkUsed(used, itrG->lhs) == false){
			NT.append(itrG->lhs);
			NT.append(" ");
			used.push_back(itrG->lhs);
			nonTerminals.push_back(itrG->lhs);
		}
		
		for (int i = 0; i < itrG->rhs.size(); i++) {
			if(checkUsed(used, itrG->rhs[i]) == false){
				if(isTerminal(itrG->rhs[i]) == true){
					T.append(itrG->rhs[i]);
					T.append(" ");
					used.push_back(itrG->rhs[i]);
					terminals.push_back(itrG->rhs[i]);
				}else{
					NT.append(itrG->rhs[i]);
					NT.append(" ");
					used.push_back(itrG->rhs[i]);
					nonTerminals.push_back(itrG->rhs[i]);
				}
			}
		}	
		itrx++;
		itrG++;
	}
	if(shouldPrint == true){
		cout << T << NT;
	}
}

// Task 2, worth most points
bool RemoveUselessSymbols(bool shouldPrint)
{
	//vector of all LHS no repeats
	vector<string> rules;
	//to iterrate through grammer
	list<Rule>::iterator itrG;
	itrG = grammer.begin();
	
	int itrx = 0;
	while(itrx < grammer.size()){
		if(checkUsed(rules, itrG->lhs) == false){
			rules.push_back(itrG->lhs);
		}
		itrx++;
		itrG++;
	}
	//paralell array of booleans
	bool isGen[rules.size()];
	for(int i = 0; i < rules.size(); i++){
		isGen[i] = false;
	}
	//rules = S C A B, isGen = F F F F
	//BIG ASS ALGORITHM
	
	bool hasChanged = true, ruleIsGen;
	while(hasChanged == true){
		hasChanged = false;
		//check right hand side of current rule, see if has only generating rules or terminals
		
		itrx = 0;
		itrG = grammer.begin();
		
		while(itrx < grammer.size()){
			ruleIsGen = true;
			for(int rhItr = 0; rhItr < itrG->rhs.size() && ruleIsGen == true; rhItr++ ){
				if(isTerminal(itrG->rhs[rhItr]) == false){//first check if terminal
					//determine if rhs has generators or not
					if(isGen[findIndex(rules, itrG->rhs[rhItr])] == false){
						ruleIsGen = false;
					}
				}
			}//all rhs is checked
			if(ruleIsGen == true && isGen[findIndex(rules, itrG->lhs)] != true){
				isGen[findIndex(rules, itrG->lhs)] = true;
				hasChanged = true;
			}
			itrx++;
			itrG++;
		}//end of inner while
		
	}//end of out whilie
	vector<Rule> rulesGen;
	bool rightValid = true;
	itrG = grammer.begin();
	itrx = 0;

	while(itrx < grammer.size()){
		if(isGen[findIndex(rules, itrG->lhs)] == true){
			for(int i = 0; i < itrG->rhs.size(); i++){
				if(isTerminal(itrG->rhs[i]) == false){
					if(isGen[findIndex(rules ,itrG->rhs[i])] == false){
						rightValid = false;
					}
				}
			}
			if(rightValid == true){
				rulesGen.push_back(*itrG);
			}
		}
		rightValid = true;
		itrx++;
		itrG++;
	}
			

	bool isReachable[rulesGen.size()];
	for(int i = 0; i < rulesGen.size(); i++){
		if(rulesGen[i].lhs == grammer.begin()->lhs){
			isReachable[i] = true;
		}else{
			isReachable[i] = false;
		}
		
	}
	//rules = S C A B, isGen = T T F T
	//rulesGen S C S B //isReachable  T F T F
	hasChanged = true;
	while(hasChanged == true){
	hasChanged = false;
		for(int i = 0; i < rulesGen.size(); i++){
			if(isReachable[i] == true){
				for(int r = 0; r < rulesGen[i].rhs.size(); r++){
					if(isTerminal(rulesGen[i].rhs[r]) != true){
						for(int k = 0; k < rulesGen.size(); k++){
							if(rulesGen[k].lhs == rulesGen[i].rhs[r]){
								isReachable[k] = true;
							}
		
						}
							
					}
				}
			}
		}
	}
	
	if(shouldPrint == true){
		string output = "";
		for(int i = 0; i < rulesGen.size(); i++){
			if(isReachable[i] == true){
				output.append(rulesGen[i].lhs);
				output.append(" -> ");
				for(int r = 0; r < rulesGen[i].rhs.size(); r++){
					output.append(rulesGen[i].rhs[r]);
					output.append(" ");
				}
				if(rulesGen[i].rhs.size() == 0){
					output.append("#");
				}
				output.append("\n");
			}
		}
		cout << output;
		return true;
	}else{
		for(int g = 0; g < rules.size(); g++){
			if(isGen[g] == false){
				return false;
			}
	}
		
	}
	return true;
}

// Task 3, worth most points
void CalculateFirstSets(bool shouldPrint)
{
	printTerminalsAndNoneTerminals(false);
	list<Rule>::iterator itrG;
	itrG = grammer.begin();
	int itrx = 0;

	
	while(itrx < grammer.size()){
		if(checkUsedRules(first, itrG->lhs) == false){
			first.push_back(itrG->lhs);
		}
		
		itrG++;
		itrx++;
	}
	
    // 4, 3, 2, 1, 0
	bool hasChanged = true;
	while(hasChanged == true){
		itrG = grammer.begin();
		itrx = 0;
		
		hasChanged = false;
		while(itrx < grammer.size()){//loop through all rules
			//not checking for terminals here
			
			if(itrG->rhs.empty() == true){
				if(checkUsed(first[findRulesIndex(first, itrG->lhs)].rhs, "#") == false){
					first[findRulesIndex(first, itrG->lhs)].rhs.push_back("#");
					hasChanged = true;
				}
			}else{
				for(int i = 0; i < itrG->rhs.size(); i++){
					bool hasEp = false;
					if(isTerminal(itrG->rhs[i]) == false){
						for(int f = 0; f < first[findRulesIndex(first, itrG->rhs[i])].rhs.size(); f++){
							
							if(checkUsed(first[findRulesIndex(first, itrG->lhs)].rhs, first[findRulesIndex(first, itrG->rhs[i])].rhs[f]) == false 
							   && first[findRulesIndex(first, itrG->rhs[i])].rhs.empty() == false 
							    && first[findRulesIndex(first, itrG->rhs[i])].rhs[f] != "#"){
								
								first[findRulesIndex(first, itrG->lhs)].rhs.push_back(first[findRulesIndex(first, itrG->rhs[i])].rhs[f]);
								hasChanged = true;
							}else if(first[findRulesIndex(first, itrG->rhs[i])].rhs[f] == "#"){
								hasEp = true;
							}
						}
					}else{
						if(checkUsed(first[findRulesIndex(first, itrG->lhs)].rhs, itrG->rhs[i]) == false){
							first[findRulesIndex(first, itrG->lhs)].rhs.push_back(itrG->rhs[i]);
							hasChanged = true;
						}
					}
					if(hasEp == false){
						break;
					}
					
					if(i == itrG->rhs.size() - 1 && checkUsed(first[findRulesIndex(first, itrG->lhs)].rhs, "#") == false){
						first[findRulesIndex(first, itrG->lhs)].rhs.push_back("#");
						hasChanged = true;
					}
					
				}
				
				
			}
			itrG++;
			itrx++;
		}
	}
	
	
	if(shouldPrint == true){
		for(int i = 0; i < nonTerminals.size(); i++){
			cout << "FIRST(" << nonTerminals[i] << ") = { ";
			int size = first[findRulesIndex(first, nonTerminals[i])].rhs.size();
			int numPrinted = 0;
			bool epPrinted = false;
			if(checkUsed(first[findRulesIndex(first, nonTerminals[i])].rhs, "#") == true){
				epPrinted = true;
				cout << "#";
				numPrinted++;
			}
			
			for(int j = 0; j < terminals.size(); j++){
				if(checkUsed(first[findRulesIndex(first, nonTerminals[i])].rhs, terminals[j]) == true ){
					if(numPrinted + 1 == size){
						if(epPrinted == true){
							cout << ", ";
							epPrinted = false;
						}
						cout << terminals[j] << " }\n";
					}else{
						if(epPrinted == true){
							cout << ", ";
							epPrinted = false;
						}
						cout << terminals[j] << ", ";
					}
					
					numPrinted++;
				}
			}
			if(epPrinted == true){
				cout << " }\n";
			}
		}
	}
	
	
	
}

// Task 4
void CalculateFollowSets(bool shouldPrint)
{	

	CalculateFirstSets(false);
	first[0].fol.push_back("$");//could possibly be an issue
	list<Rule>::iterator itrG;
	itrG = grammer.begin();
	int itrx = 0;
	bool hasChanged = true;
	while(hasChanged == true){
		hasChanged = false;
		
		for(int i = 0; i < first.size(); i++){//loop through first set
			itrG = grammer.begin();
			itrx = 0;
			while(itrx < grammer.size()){//looking for first.lhs inside of all grammer.rhs

				for(int r = itrG->rhs.size() - 1; r >= 0 ; r--){

					if(first[i].lhs == itrG->rhs[r]){//match found at r
						if(r == itrG->rhs.size() - 1){// checking if current pos is EOL
							//add the follow set of lhs of curr rule to the fol set of first[i]
							for(int f = 0; f < first[findRulesIndex(first, itrG->lhs)].fol.size(); f++){
								if(checkUsed(first[i].fol, first[findRulesIndex(first, itrG->lhs)].fol[f]) == false){
									//adds follow set of first[lhs] to first[i]'s follow set
									first[i].fol.push_back(first[findRulesIndex(first, itrG->lhs)].fol[f]);
									hasChanged = true;
								}
							}
						}else if(isTerminal(itrG->rhs[r + 1]) == true){//checking if next symbol is Terminal
							if(checkUsed(first[i].fol , itrG->rhs[r + 1]) == false){
								//add the next symbole (terminal) to [i].fol
								first[i].fol.push_back(itrG->rhs[r + 1]);
								hasChanged = true;
							}
						}else if(isTerminal(itrG->rhs[r + 1]) == false){//checking if next symbol is a nonTerminal
							//from our curr pos = r, loop from next position until the end,  
							for(int R = r + 1; R < itrG->rhs.size(); R++){
								if(isTerminal(itrG->rhs[R]) == true){
									//if next position is a terminal
									if(checkUsed(first[findRulesIndex(first, itrG->rhs[r])].fol, itrG->rhs[R]) == false){
										//add that position and then break
										first[findRulesIndex(first, itrG->rhs[r])].fol.push_back(itrG->rhs[R]);
										hasChanged = true;
									}
									break;
								}else{
									//when next pos = NT, add first set of Nt
									for(int f = 0; f < first[findRulesIndex(first, itrG->rhs[R])].rhs.size(); f++){
										if(first[findRulesIndex(first, itrG->rhs[R])].rhs[f] != "#" 
										  && checkUsed(first[i].fol, first[findRulesIndex(first, itrG->rhs[R])].rhs[f]) == false){
											first[i].fol.push_back(first[findRulesIndex(first, itrG->rhs[R])].rhs[f]);
											hasChanged = true;
										}
									}
									if(checkUsed(first[findRulesIndex(first, itrG->rhs[R])].rhs , "#") == false){
											break;
										}
									if(R == itrG->rhs.size() - 1){// checking if current pos is EOL
										//add the follow set of lhs of curr rule to the fol set of first[i]
										for(int f = 0; f < first[findRulesIndex(first, itrG->lhs)].fol.size(); f++){
											if(checkUsed(first[i].fol, first[findRulesIndex(first, itrG->lhs)].fol[f]) == false){
												first[i].fol.push_back(first[findRulesIndex(first, itrG->lhs)].fol[f]);
												hasChanged = true;
											}
										}

									}
								}
							}
						}else{
							throw runtime_error("NO IF'S WERE REACHED IN FOLLOW\n");
						}
					}
				}
				itrx++;
				itrG++;
			}//while loop end

		}
	}
	if(shouldPrint == true){
		for(int i = 0; i < nonTerminals.size(); i++){
			cout << "FOLLOW(" << nonTerminals[i] << ") = { ";
			int size = first[findRulesIndex(first, nonTerminals[i])].fol.size();
			int numPrinted = 0;
			bool dolPrinted = false;
			if(checkUsed(first[findRulesIndex(first, nonTerminals[i])].fol, "$") == true){
				dolPrinted = true;
				cout << "$";
				numPrinted++;
			}

		
			for(int j = 0; j < terminals.size(); j++){
				if(checkUsed(first[findRulesIndex(first, nonTerminals[i])].fol, terminals[j]) == true ){
					if(numPrinted + 1 == size){
						if(dolPrinted == true){
							cout << ", ";
							dolPrinted = false;
						}
						cout << terminals[j] << " }\n";
					}else{
						if(dolPrinted == true){
							cout << ", ";
							dolPrinted = false;
						}
						cout << terminals[j] << ", ";
					}

					numPrinted++;
				}
			}
			if(dolPrinted == true){
				cout << " }\n";
			}
		}
	}
	

	
}

// Task 5
void CheckIfGrammarHasPredictiveParser()
{	
    if(RemoveUselessSymbols(false) == false){
		cout << "NO\n";
	}
	CalculateFollowSets(false);
	for(int i = 0; i < first.size(); i++){
		if(checkUsed(first[i].rhs, "#") == true){
			for(int r = 0; r < first[i].rhs.size(); r++){
				for(int f = 0; f < first[i].fol.size(); f++){
					if(first[i].rhs[r] == first[i].fol[f]){
						cout << "NO\n";
					}
				}
			}
		}
	}
	cout << "YES\n";
}
    
int main (int argc, char* argv[])
{
    int task;

    if (argc < 2)
    {
        cout << "Error: missing argument\n";
        return 1;
    }

    /*
       Note that by convention argv[0] is the name of your executable,
       and the first argument to your program is stored in argv[1]
     */
    task = atoi(argv[1]);
    ReadGrammar();
	bool eater;
    switch (task) {
        case 1: printTerminalsAndNoneTerminals(true);
            break;
        case 2: 
			eater = RemoveUselessSymbols(true);
            break;

        case 3: CalculateFirstSets(true);
            break;

        case 4: CalculateFollowSets(true);
            break;

        case 5: CheckIfGrammarHasPredictiveParser();
            break;

        default:
            cout << "Error: unrecognized task number " << task << "\n";
            break;
    }
    return 0;
}

