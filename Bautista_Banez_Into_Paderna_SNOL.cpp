#include <iostream> 
#include <stdlib.h> 
#include <string> 
#include <regex> 
#include <stack> 
#include <limits>  
#include <unordered_map> 

using namespace std;

// FUNCTION PROTOTYPES
void guide();	
void checkCalc(string expr);	

bool isOperator(char ch);																		
bool isVar(string ch);																			
bool isDigit(string ch);																	
bool syntaxCheck(string input, int type);												 
bool errorChecking(string postfix); 																													
bool dataType(string postfix);																		
bool iftype(float num);	

int commandCheck(string input);																																			
int precedence(char value);		

string inftopost(stack <char> stack, string infix);											
string evalInt(stack <float> mystack, string postfix);									
string evalFloat(stack <float> mystack, string postfix);	

class Tokenizer{ //Object class to store tokenized input
	unordered_map<string, string> key; //a hash type of mapping to identify input and mapped value

	public:

		bool checkVar(string input) { //function for traversing the map if a certain keyword exists
			if(key.find(input) == key.end()){
				return false;	// if the command exists then return 0
			} 
			else{
				return true;
			} 
		}
		// BEG function
		void BEG(string input) {
			// get the BEG function
			string temp = input.erase(0, 4); //remove											
			string tokens;
			
			cout << "SNOL> Please enter value for [" << temp << "]: "; // storing the input value
			cin >> tokens; 			
			cout << "Input: " << tokens << endl;																
			cin.clear();
			// eliminates the whitespace
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); 								
			if(isDigit(tokens)){
				key[temp] = tokens; 	//checks to see if the value entered is really a number	
			} 										 
			else{
				cout << "SNOL> Error! [" << temp << "] integer or float values only." << endl;
			} 
		}
		// PRINT function
		void PRINT(string input) { 
			// get the PRINT function
			string temp = input.erase(0, 6);														
			
			// print the variable if it exists
			if (checkVar(temp)) {
			cout << "SNOL> [" << temp << "] = " << key.at(temp) << endl; 						
			} 
			else if(isDigit(temp)) {  //checks to see if the value entered is really a number
				cout << "SNOL> [" << temp << "] = " << temp << endl; 								
			}
			else{
				cout << "SNOL> Error! [" << temp << "] is not defined!" << endl;	
			} 				
		} 
		// asisgnment of the variable function
		void assignmentCheck(string input) { 													

			string temp, expr, var;
			bool flag = false;
			for (int i = 0; i < input.length(); i++) { 				// loop that will traverse the command												
				if (isOperator(input[i])) {		//if current character is an operator 																		
					if (input[i] == '-' && isdigit(input[i+1])) { 		// if the character is a negative integer											
						temp += input[i];
						continue;
					}
					if (isVar(temp)) {	//if the command is valid in terms of syntax																			
						if (checkVar(temp)) {																			
							expr += " " + key.at(temp) + " " + input[i];	//add operator/variable	to the string									
							flag = true;
						} 
						else {
							cout << "SNOL> Error! [" << temp << "] is not defined!" << endl;					
							return;
						}
					}
					else {
						expr += " " + temp + " " + input[i]; // direct assignment	
						flag = true;
					}
					temp.erase();
				}
				else if (input[i] == ' ') { 		// if whitespaces
					if ((isdigit(input[i - 1])) && (isdigit(input[i + 1]))) {
						cout << "SNOL> Unknown command! Does not match any valid command!" << endl;
						return;
					}
					else continue;
				}	
				else if (input[i] == '=') {
					// get variable name
					var += temp;	
					temp.erase();
				}
				else{
					temp += input[i];	// Add char to part to be assessed 
				} 
			}
			// for the end part of the string
			if(isVar(temp)) {
				if(checkVar(temp)){
					expr += " " + key.at(temp);
				} 
				else{
					cout << "SNOL> Error! [" << temp << "] is not defined!" << endl;
					return;
				}
			}
			else{
				expr += " " + temp;
			} 
			temp.erase();
			input.erase();
			
			//conditions if the expression is only a digit with no operations
			if(isDigit(expr)) {
				key[var] = expr;
			}
			else{
				// infix to postfix conversion
				stack <char> s;
				string postfix = inftopost(s, expr);
				string ans = "";
				
				// checks to see if all numbers have the same data types
				if(!errorChecking(postfix)) {
					cout << "Error!" << endl;
					return;
				}
				else{
					// checks to see the data type in the postfix expression
					if(dataType(postfix)) {
						stack <float> i;
						ans = evalInt(i, postfix);
						if (ans == "Error!") {
							cout << "SNOL> Error! Only int type to perform Modulo!";
							return;
						}
					}
					else{
						// evalutating the postfix that involves a float
						stack <float> f;
						ans = evalFloat(f, postfix);
						if (ans == "ERROR") {
							cout << "SNOL> Error! Only int type to perform Modulo!";	// display an error	if not an integer type		
							return;
						}	
					}
					// stores the evaluated input into the variable in the map.
					key[var] = ans;
				}
			}
		} 
		
		// checks the variable of the command entered by the user
		bool varChec(string input) { 
			string var; 					
			bool flag = false;
			
			// traverses the string
			for (int i = 0; i < input.length(); i++) {												 
				if (isOperator(input[i])) { 					// checks if the character is an operator							
					if (isVar(var) && !checkVar(var)) { 		// checks if the string is a variable in syntax & if it exists					
						cout << "SNOL> Error! [" << var << "] is not defined!" << endl; 	
						return false;
					}
					// if the syntax is correct then the string will be erased
					var.erase();														
					flag = true;
					continue;
				}
				else if(input[i] == ' ') {					// checks to see if there are any whitespaces in the strings										
					if((input[i + 1] == '-') && (isdigit(input[i + 2])) && (flag == false)) {   // checks if the syntax of the input is an operation that involves the variable	
						cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
						return false;
					}
					else if((isdigit(input[i - 1])) && (isdigit(input[i + 1]))) {   // checks if the entered command involves a digit
						cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl; 
						return false;
					}
					else continue;
				}
				var += input[i]; // stores the command inputted by the user
				flag = true;
			}
			// checks the final string
			if(isVar(var) && !checkVar(var)) { 												
				cout << "SNOL> Error! [" << var << "] is not defined!" << endl;
				return false;
			}
			return true;
		} 

		// a function to separates the value and store it
		string getVal(string input) {
			
			string temp, result;
			
			for (int i = 0; i < input.length(); i++) {		// loop that will traverse the command									
				if(input[i] == '-' && isdigit(input[i + 1])) {			// negative number						
					temp += input[i];
					continue;
				}
				if(isOperator(input[i])) {	// checks if the current command is an operator   // checks if the stored string is a variable then stores it with the existing varibable												
					if (isVar(temp)){
						result += " " + key.at(temp) + " " + input[i];           // else, store it with the new command only
					} 
					else{
						result += " " + temp + " " + input[i];
					} 				

					temp.erase();
					continue;
				}
				else if (input[i] == ' ') { // continues for loop if it encounters a whitespace	
					continue;			
				}							
				temp += input[i];
			}
			if(isVar(temp)){
				result += " " + key.at(temp) ;
			} 
			else{
				result += " " + temp;
			} 
			temp.erase();
			return result;
		} 
															
};
							
void guide() { 
	
	cout << endl << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~SNOL Overview & Guidelines~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl; 
	cout << "\t1. Format >> Separate items with spaces. However, extra spaces will be disregarded." << endl;
	cout << "\t\tExample: var = 17 -> contains one space between every word" << endl;
	cout << "\t2. Case Sensitive >> All words are case sensitive." << endl;
	cout << "\t\tExample: num, NUM, and nUm are distinct from one another. They will be addressed differently." << endl;
	cout << "\t3. Data Type >> There are two data types only for SNOL integer and floating point" << endl;
	cout << "\t\tExample: \n\t\tnum = 10 + 7 -> Correct since both are integers" << endl << "\t\tNUM = 10.3 + 7.5 -> Correct since both are floating-point" << endl << "\t\tnUm = 10 + 7.3 -> Invalid. Their data types are both different" <<endl;
	cout << "\t4. Operands >> Operands must be of the same type for arithemetic operations:" << endl;
	cout << "\t5. Variables >> Variables can be number or letters. However, it should start with letters, not numbers." << endl;
	cout << "\t\tExample: \n\t\tnum -> Correct\n\t\t!num -> Incorrect \n\t\tNote: Variables should be DECLARED first before using." << endl;
	cout << "\t6. Invalid Syntax >> Your command should be logical and acceptable" << endl;
 	cout << "\t7. SPECIAL KEYWORDS >> This program has only 4 special keywords, namely: PRINT, BEG, HELP, and EXIT!" << endl;
	cout << "\t       > PRINT: Display the targeted variable or literal." << endl;
	cout << "\t\t\tExample: num = 8\n\t\t\tPRINT num" << endl;
	cout << "\t       > BEG: Asks for a value" << endl;
	cout << "\t       > HELP: Give the user an idea on how to use the program." << endl;
	cout << "\t       > EXIT!: Terminate the Program" << endl;
	cout << endl << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~SNOL Overview & Guidelines~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl; 
	system("pause"); 
}


int precedence(char value) {		 // function that checks the precedence from infix to postfix
	
 	if (value == '+' || value == '-') { //lower precedence
        return 1;
    }
	else if (value == '*' || value == '/' || value == '%') { //higher precedence
        return 2;
    }
    else {
		return -1;
	}
}

// a function that checks the expression if there is an error
bool errorChecking(string postfix) 
{ 
	
    int prev, curr;
    string tmp, tmp1;
    int j;
    
    
    
    for (int i = 0; i < postfix.length(); i++) {         	 // a loop that checks each character to see if all the numbers have the same data type							
        if (isdigit(postfix[i]) || postfix[i]=='.') {   	 // stores character into a temporary string if number or period							 
            tmp+= postfix[i];
            continue;
        } else {
            if (postfix[i] == ' ' && !tmp.empty()) {    	// if char is space and temporary string is not empty, proceed to the if statement						  
                if (tmp.find(".") >= tmp.length()) { 		// checks to see if the characters are integers							
                    prev= 1;								// chars are integer
                } else {
                    prev = 0; 								// chars are float							
                }
                
                if ((i+1) < postfix.length()) {
                    for (j = i+1; j < postfix.length(); j++){  				// finds the next number to compare with tmp			 
                        if (isdigit(postfix[j]) || postfix[j]=='.') {  		// stores character into a temporary string if num or period		 
                            tmp1+= postfix[j];
                            continue;
                        } else {
                            if (postfix[j] == ' ') {  						// the character will compare the previous number and the next number if it the char is space							  
                                if (tmp1.find(".") >= tmp1.length()) { 				
                                    curr = 1;								// num is integer
                                } else {                               					 
                                    curr = 0;								// num is a float
                                }
                                
                                if (prev != curr) {    				 		// the two data type did not match if prev != curr			
                                    return false;      				 					
                                }
                                
                                tmp = tmp1;            						// gets the current number and stores it into the temporary storage for the previous number							 
                                tmp1.clear();         									 
                                break;
                            }else continue;
                        }
                    }
                    i = j;
                }
            }else continue;
        }
    }
    
    return true;
}


bool dataType(string postfix){ // a boolean function that checks whether the types of values are integer or a floating-point
	
    return (postfix.find('.') >= postfix.length()); 		// returns true = int, false = floating-point				
    
}

// a function for infix to postfix
string inftopost(stack <char> stack, string infix) { 
    
	string postfix;
	
	// checks the string per character
    for (int i = 0; i < infix.length(); i++) { 								
        if ((isdigit(infix[i])) || (infix[i] == '.')) {		// checks to see if the current character is a period or a digit				
            postfix += infix[i];							// appends the current character to the postfix string				
            if (infix[i + 1] == '.') continue;				// checks to see if the next character is a period				
            else if (!isdigit(infix[i + 1])) postfix += " ";	// checks to see if the next character is not a digit				
        }
        else if (infix[i] == ' ') {								// if the current char is a whitespace then ignore			
            continue;
        }
        else if (infix[i] == '(') {								// checks for the open parenthesis 			
            stack.push(infix[i]);											
        }
        else if (infix[i] == ')') {								// checks for the close parenthesis			
            while ((stack.top() != '(') && (!stack.empty())) {		// loop until it meets the pair or if the stack is empty		
                char temp = stack.top();
                postfix += temp;									// append the top of the stack to the postfix string		
                postfix += " ";										// adds spaces after each append	
                stack.pop();
            }
            if (stack.top() == '(') {								// pops if the pair parenthesis meet up		
                stack.pop();
            }
        }
        else { 													// if the current infix is an operator		
            if (infix[i] == '-' && isdigit(infix[i + 1])) {
                postfix += infix[i];
                continue;
            }
            if (stack.empty()) {										
                stack.push(infix[i]);
            }
            else {															
                if (precedence(infix[i]) > precedence(stack.top())) {		// checks precedence
                    stack.push(infix[i]);								// push to the stack if the precedence of the top of the stack < current character
                }
                else if (precedence(infix[i]) == precedence(stack.top())) { 	// checks to see if the precedence of the current character is equal to the stack top
                    postfix += stack.top();									
                    postfix += " ";
                    stack.pop();
                    stack.push(infix[i]);									
                }
                else {								// checks to see if the precedence is less than the top of the stack						
                    while ((!stack.empty()) && (precedence(infix[i]) < precedence(stack.top()))) {
                        postfix += stack.top();
                        postfix += " ";
                        stack.pop();
                    }
                    stack.push(infix[i]);
                }
            }
        }
    }
    
    while (!stack.empty()) {								// pops the remaining operators						
        postfix += stack.top();
        postfix += " ";
        stack.pop();
    }
    
    return postfix;
}


bool iftype(float num){		//a function that checks if the value is an integer											
	
	return num == (float)(int)num;  		// returns true if num = int
																	
} 

// a function for elevating the integer postfix
string evalInt(stack <float> mystack, string postfix){ 
	
	string num;
	
	// traverses through the expression
	 for (int i = 0; i < postfix.length(); i++){																		
	 	if(isdigit(postfix[i])) {		// if the char is a digit																				
            num += postfix[i];			// stores into the string																			
        	if (!isdigit(postfix[i + 1])) num += " ";		// stores the whitespace if the next char is not a digit														
        }
        else if (postfix[i]==' ' && postfix[i+1] != 0) {	    																					
			int b=atoi(num.c_str()); 					//if the character is a whitespace then store the integer from a string															
        	mystack.push(b);
            num.erase();
            continue;
        }
        else if (postfix[i] == '-' && isdigit(postfix[i + 1])) {		// if it has a negative operand										
            num += postfix[i];
            continue;
        }
        else {
        	int val1 = mystack.top();
        	mystack.pop();
        	int val2 = mystack.top();	
        	mystack.pop();
        	
        	if(postfix[i]=='%'){									// proceeds to the function if a modulo is encountered													
        		if(iftype(val1)==true && iftype(val2)==true){			// checks if both values are int											
	        		int v1=val1; 
	        		int v2=val2;
					mystack.push(v2%v1);							// pushes the modulo result
				}
				else{
					return "Error!";
				}
			}
			
			// where operators are evalutated
			// operators in the stack with equivalent arithmetic operations will be pushed        	
        	switch(postfix[i]) { 
	            case '+': 
					mystack.push(val2+val1); 
					break;
	            case '-': 
					mystack.push(val2-val1); 
					break;  
	            case '*': 
					mystack.push(val2*val1); 
					break; 
	            case '/': 
					mystack.push(val2/val1); 
					break; 
            }
            i++;
		}
 }
	 
	float ans=mystack.top();
	 
	// returning th evaluated expressions
	if(iftype(ans)==true){
		int ans2=mystack.top();
		mystack.pop();
		string ret= to_string(ans2);
		return ret;
	}
	else{
		mystack.pop();
		string ret= to_string(ans);
		return ret;
	}
} 

// a function for elevating the integer postfix
string evalFloat(stack <float> mystack, string postfix){ 
	
	string num;
	
	// traverses the expression
	 for (int i = 0; i < postfix.length(); i++){							
	 	if ((isdigit(postfix[i])) || (postfix[i] == '.')) {		// checks if the char has a period or a digit			
            num += postfix[i];									// stores it			
            if (postfix[i + 1] == '.') continue;				// continues if next char has a period						
            else if (!isdigit(postfix[i + 1])) num += " ";		// stores a whitespace if char is not a digit				
        }
        else if (postfix[i]==' ' && postfix[i+1]!=0) {												
			float a=atof(num.c_str()); 							//if the character is a whitespace then store the integer from a string				
    		mystack.push(a);
            num.erase();
            continue;
        }
        else if (postfix[i] == '-' && isdigit(postfix[i + 1])) {			// if it has a negative operand	
            num += postfix[i];													
            continue;
        }
        else if (postfix[i] == '%') {										// invalid imput if a modulo exists in a floating-point
            return "Error!";
        }
        else {
        	float val1 = mystack.top();
        	mystack.pop();
        	float val2 = mystack.top();	
        	mystack.pop();
			
			// where operators are evalutated
			// operators in the stack with equivalent arithmetic operations will be pushed       	
        	switch(postfix[i]) { 											
	            case '+': 
					mystack.push(val2+val1); 
					break;
	            case '-': 
					mystack.push(val2-val1); 
					break;   					
	            case '*': 
					mystack.push(val2*val1); 
					break; 
	            case '/': 
					mystack.push(val2/val1); 
					break; 
            }
            i++;
		}
	 }
	

	float ans=mystack.top();
	
	// returning th evaluated expressions
	if(iftype(ans)==true){
		int ans2=mystack.top();
		mystack.pop();
		string ret= to_string(ans2);
		return ret;
	}
	else{
		mystack.pop();
		string ret= to_string(ans);
		return ret;
	}	
} 


int commandCheck(string input) {
	
	regex beg("BEG [^\\|]+");	// BEG characters[any] to be a BEG command
	regex disp("PRINT [^\\|]+");	// PRINT characters[any] to be a PRINT command
	
	int j = input.length();
	
	if (regex_match(input, beg)){
		return 1;
	} 
	else if (regex_match(input, disp)){
		return 2;
	} 
	else if (input == "EXIT!"){
		return 3;
	} 
	else if (input == "HELP"){
		return 6;
	}
	for (int i = 0; i < j; i++) {  
		if (isOperator(input[i])){		// calculate if it starts with an operator
			return 4; 					
		} 
		if (input[i] == '='){			// it is an assignment if it starts with an equal "=" sign
			return 5;	
		} 
		
	}
	return 0;	
} 

//Checks if the input is a valid command
bool syntaxCheck(string input, int type) { 
	
	regex var("\\(*-?[A-Za-z][A-Za-z0-9]*\\)*"); //regex format that follows EBNF rule for variable: letter{(letter|digit)}
	regex digit("\\(*-?[0-9][0-9]*(\\.[0-9]+)?\\)*"); //regex format that follows EBNF rule of data type int: [-]digit{digit}  
													  // and for data type float is :[-]digit{digit}.{digit} 
	
	int parenthesis = 0; 
	string temp; 

	//BEG Command 
	if (type == 1) {
		temp = input.erase(0, 4); //deletes the content of the string in this range (Deletes the word BEG)
		if (regex_match(temp, var)){ //returns true if the regular expression var match against temp variable 
			return true;
		} 
		else { //otherwise returns false and print error
			cout << "SNOL> [" << temp << "]" << " is not a valid variable name!" << endl;
			return false;
		}
	}
	//PRINT Command
	else if (type == 2) {	
		temp = input.erase(0, 6); //deletes the content of the string in this range (Deletes the word BEG)
		if (regex_match(temp, var) || regex_match(temp, digit)){//returns true if the regular expression var match against temp variable 
			return true; 
		} 
		else { //otherwise returns false and print error
			cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
			return false;
		}
	}
	else if (type == 4) { 
		for (int i = 0; i < input.length(); i++) {
			if (parenthesis < 0) { //if parenthesis has no pari
				cout << "SNOL> Missing parenthesis pair!" << endl; 
				return false;
			}
			else if (input[i] == '(') {
				parenthesis++;
				temp += input[i];
			}
			else if (input[i] == ')') {
				parenthesis--;
				temp += input[i];
			}
			else if (isOperator(input[i])) {//checks if input is an operator
				if (input[i] == '-' && isdigit(input[i+1])) { //if input is a negative number
					temp += input[i];
					continue;
				}
				if (temp.size() == 0) {	//if temp is empty
					cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
					return false;
				}
				if (!(regex_match(temp, var) || regex_match(temp, digit))) {  // checks if temp is not a variable or temp is a number
					cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
					return false;
				}
				temp.erase(); //empty the temp
			}
			else if (input[i] == ' ') { //ignore whitespaces
				continue;	
			}
			else {
				temp += input[i];	
			}
		}

		if (parenthesis != 0) { //Error check for unpaired paranthesis
			cout << "SNOL> Missing parenthesis pair!" << endl;
			return false;
		}

		
		if (temp.size() == 0) {	//if empty size return false
			cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
			return false;
		}
		if (!(regex_match(temp, var) || regex_match(temp, digit))) {  // checks if temp is not a variable or temp is a number
			cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
			return false;
		}
		temp.erase();
		return true;
	}
	else if (type == 5) { //Assignment Operation
		for (int i = 0, equals = 0; i < input.size(); i++) {
			if (parenthesis < 0) { //Error check for unpaired paranthesis
				cout << "SNOL> Missing parenthesis pair!" << endl;
				return false;
			}
			else if (input[i] == '(') {
				parenthesis++;
				temp += input[i];
			}
			else if (input[i] == ')') {
				parenthesis--;
				temp += input[i];
			}
			else if (input[i] == '=') {	
				equals++;
				if (equals > 1) {//Error check for more thane one equal sign
					cout << "SNOL> Invalid! more than one '=' in the expression." << endl;
					return false;
				}
				if (!regex_match(temp, var)) {	//if temp is not match in the formate
					cout << "SNOL> Error! Invalid variable name syntax." << endl;
					return false;
				}
				temp.erase();	
			}
			else if (isOperator(input[i])) {	//checks if input is an operator
				if (input[i] == '-' && temp.length() == 0) { //checks if input is a negative number
					temp += input[i];
					continue;
				}
				if (temp.size() == 0 || equals == 0) {	//empty token
					cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
					return false;
				}
				if (!(regex_match(temp, var) || regex_match(temp, digit))) { // checks if temp is not a variable or temp is a number	
					cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
					return false;
				}
				temp.erase(); //delete temp
			}
			else if (input[i] == ' ') { //ignore white spaces
				continue;
			}	
			else temp += input[i];
		}

		if (parenthesis != 0) { //Error check for unpaired paranthesis
			cout << "SNOL> Missing parenthesis pair!" << endl;
			return false;
		}

		if (temp.size() == 0) {	//Error check for empty temp
			cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
			return false;
		}
		if (!(regex_match(temp, var) || regex_match(temp, digit))) { // checks if temp is not a variable or temp is a number
			cout << "SNOL> Unknown command! Does not match any valid command of the language."<< endl;
			return false;
		}
		temp.erase();
		return true;
	}

	else return false;
} 

// Check if the character is a operator. 
// Return true if it is a operator.
// Return false if not.
bool isOperator(char ch) {	
	switch (ch) {
		case '+': 
			return true;
		case '-': 
			return true;
		case '*': 
			return true; 
		case '/':
			 return true;
		case '%': 
			return true;
		default: 
			return false;
	}
} 

//Checks if the string is a variable
bool isVar(string ch) {	

	regex var(" *\\(*-?[A-Za-z][A-Za-z0-9]*\\)*"); // regex format that follows EBNF rule for variable: letter{(letter|digit)}

	if (regex_match(ch, var)) {
		return true; //returns true if the string match
	} 
	else {
		return false;//returns false if not
	} 
	
} 

//Checks if the string is a digit
bool isDigit(string ch) {	

	regex digit(" *\\(*-?[0-9][0-9]*(\\.[0-9]+)?\\)*");	 // regex format that follows EBNF rule of data type int: [-]digit{digit}  
														 // and for data type float is :[-]digit{digit}.{digit} 
	if (regex_match(ch, digit)) return true;
	else return false;
	
}

//Function that converts the expression to postfix
void checkCalc(string expr){
	
	stack <char> s;
	string postfix = inftopost(s, expr);

	//Checks if they have same data type
	if (errorChecking(postfix) == false) {
		cout << "SNOL> Error! Operands must be of the same data type in an arithmetic operation!" << endl;
		return;
	}
	//Checks whether the data type is float or int
	else {
		
		//if int data type, it will then evaluate/calculate
		if (dataType(postfix)) {
			stack <float> i;
			string ans = evalInt(i, postfix);
			if (ans == "ERROR") {
				cout << "SNOL> Error! Operands must be int type to perform Modulo!"; 			
				return;
			}
		}
		//if float data type, it will then evaluate/calculate
		else {
			stack <float> f;
			string ans = evalFloat(f, postfix);
			if (ans == "ERROR") {
				cout << "SNOL> Error! Operands must be int type to perform Modulo!"; 			
				return;
			}
		}
		return;
	}
} 

//main function of the program
int main() { 
	

	string input;  //user string input
	int type = 0; //variable to use to store what type of command
	Tokenizer tokens; //object from Tokenizer class	
	
	//This message appears if the SNOL starts running
	cout << "The SNOL environment is now active, type HELP to get an overview before proceeding of giving your commands." << endl;
	
		
	while (input != "EXIT!") { //loops until user inputs EXIT!
		
		//user input
		cout << "\nCommand: ";   
		getline(cin, input); 
		
		type = commandCheck(input); //checks whether  user input has its appropriate command
		
		switch (type) { 
			
			case 0:
					cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl; //Prints if the input is invalid command 
					break;
			case 1:
					if (syntaxCheck(input, type)){ //Checks if the input is a valid syntax for the command 
						tokens.BEG(input); 		   //If valid, do the BEG command
					} 
					break;  
			case 2:
					
					if (syntaxCheck(input, type)){ //Checks if the input is a valid syntax for the command 
						tokens.PRINT(input);	   //If valid, do the PRINT command
					}  
					break;
			case 3:
					cout << "\nInterpreter is now terminated..." << endl; //Exits program if the input is EXIT!
					return 0;
					break;
			case 4:
					if (!syntaxCheck(input, type)) { //If it is not a valid syntax, break in the loop
						break;
					}
					else if (tokens.varChec(input)) { //checks if the input has a variable
						
					input = tokens.getVal(input); //the input will go to getVal function 
					checkCalc(input); //the input will be then converted into postfix 
					
					}
					break;
			case 5:
					if (syntaxCheck(input, type)){ //Checks if the input is a valid syntax for the command 
						tokens.assignmentCheck(input);	//If valid, check the assigned operations
					} 
					break;
			// case 6:
					// help(); //help function
					// break;

			
		}
	}
}

