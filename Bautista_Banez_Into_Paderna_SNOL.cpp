// Libraries
#include <iostream> 
#include <stdlib.h> 
#include <string> 
#include <regex> 
#include <stack> 
#include <limits>  
#include <unordered_map> 
using namespace std;

// Function Prototypes
void manual();	
void postfixConversion (string expr);	

bool isOperator (char ch);																		
bool isVariable (string ch);																			
bool isDigit (string ch);																	
bool syntaxValidation (string input, int type);												 
bool errorFinder (string postfix); 																													
bool expressionDataType (string postfix);																		
bool numDataType (float num);	

int commands (string input);																																			
int precedence (char value);		

string conversionHelper (stack <char> stack, string infix);											
string evaluateIntExp (stack <float> mystack, string postfix);									
string evaluateFloatExp (stack <float> mystack, string postfix);	

/*
*	Tokenizer - an object class that stores the tokenized input
*/
class Tokenizer 
{ 
	unordered_map<string, string> key; // map that stores the variables and their values

	public:
		/*
		*	varFinder() - function that checks if the variable exists in the map
		*	>> parameter - the input string "variable"
		*	>> return - true if the variable exists, false if it does not
		*/
		bool varFinder(string input) 
		{ 
			if (key.find(input) == key.end())
				return false;	// if the command exists then return 0
			
			else
				return true;
		}

		/*
		*	BEG() - function that handles the BEG command, the input function
		*	>> parameter - the input string "BEG <variable>"
		*	>> return - none
		*/
		void BEG(string input)
		{
			// remove "BEG " from string
			string temp = input.erase(0, 4); 										
			string tokens;
			
			// prompt for input
			cout << "SNOL> Please enter value for [" << temp << "]\nInput: "; 
			cin >> tokens; 			
			cin.clear();

			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear whitespace	

			if (isDigit(tokens)) // if input is digit, assign value to variable
				key[temp] = tokens; 
										 
			else // error if input is not a digit
				cout << "SNOL> Error! [" << temp << "] integer or float values only." << endl;
		}

		/*
		*	PRINT() - function for handles the PRINT command
		*	>> parameter - the input string "PRINT <variable>"
		*	>> return - none
		*/
		void PRINT(string input) 
		{ 
			// remove "PRINT " from string
			string temp = input.erase(0, 6);														
			
			if (varFinder(temp)) // print the variable if it exists
				cout << "SNOL> [" << temp << "] = " << key.at(temp) << endl; 						
			
			else if (isDigit(temp)) // print the digit
				cout << "SNOL> [" << temp << "] = " << temp << endl; 								
			
			else // error if variable does not exist	
				cout << "SNOL> Error! [" << temp << "] is not defined!" << endl;	
		} 

		/*
		*	assignmentOp() - function that handles the assignment command
		*	>> parameter - the input string "variable = expression"
		*	>> return - none
		*/
		void assignmentOp(string input) 
		{ 													
			string temp, expr, var;
			bool flag = false;

			for (int i = 0; i < input.length(); i++) // Traverse input string	
			{ 				
				if (isOperator(input[i])) // if current character is an operator 		
				{																		
					if (input[i] == '-' && isdigit(input[i + 1])) // if the character is a negative integer
					{ 													
						temp += input[i];
						continue;
					}

					if (isVariable(temp)) // if the variable is valid in terms of syntax
					{																				
						if (varFinder(temp)) 
						{																			
							expr += " " + key.at(temp) + " " + input[i]; // add operator/variable to the string									
							flag = true;
						} 

						else 
						{
							cout << "SNOL> Error! [" << temp << "] is not defined!" << endl;					
							return;
						}
					}

					else 
					{
						expr += " " + temp + " " + input[i]; // direct assignment	
						flag = true;
					}

					temp.erase(); // clear temp string
				}

				else if (input[i] == ' ') // if whitespaces
				{ 		
					if ((isdigit(input[i - 1])) && (isdigit(input[i + 1]))) 
					{
						cout << "SNOL> Unknown command! Does not match any valid command!" << endl; // error if there are two digits with no operator in between
						return;
					}

					else continue;
				}	

				else if (input[i] == '=') // add variable name before assignment operation to var string 
				{
					var += temp;	
					temp.erase();
				}

				else
					temp += input[i]; // add char to temp string
			}

			// get the last part of the expression
			if (isVariable(temp)) 
			{
				if (varFinder(temp)) // if there is a variable in the expression
					expr += " " + key.at(temp); 

				else
				{
					cout << "SNOL> Error! [" << temp << "] is not defined!" << endl;
					return;
				}
			}

			else 
				expr += " " + temp;
			
			temp.erase();
			input.erase();
			
			// conditions if the expression is only a digit with no operations
			if (isDigit(expr)) 
				key[var] = expr; // add variable to map with value of the digit
			
			else 
			{
				// convert expression from infix to postfix 
				stack <char> s;
				string postfix = conversionHelper(s, expr);
				string ans = ""; // string to hold the answer
				
				// checks to see if all numbers have the same data types
				if (!errorFinder(postfix)) 
				{
					cout << "Error! Operands must be of the same data type in an arithmetic operation!" << endl;
					return;
				}

				else // determine data type in the expression
				{
					if (expressionDataType(postfix)) // integer postfix expression
					{
						stack <float> i;
						ans = evaluateIntExp(i, postfix);

						if (ans == "Error!") 
						{
							cout << "SNOL> Error! Only int type to perform Modulo!";
							return;
						}
					}

					else // float postfix expression
					{
						stack <float> f;
						ans = evaluateFloatExp(f, postfix);

						if (ans == "ERROR") 
						{
							cout << "SNOL> Error! Only int type to perform Modulo!";	// display an error	if not an integer type		
							return;
						}	
					}
					// store evaluated input into the variable in the map
					key[var] = ans;
				}
			}
		} 
		
		/*
		*	varValidation() - function that checks if the variable is valid
		*	>> parameter - input string
		*	>> return true if the variable is valid, false if not
		*/
		bool varValidation(string input) 
		{ 
			string var; // string to hold the variable name					
			bool flag = false;
			
			for (int i = 0; i < input.length(); i++) // Traverse input string
			{												 
				if (isOperator(input[i])) // if current character is an operator	
				{ 											
					if (isVariable(var) && !varFinder(var)) // guard clause for valid variable 	 
					{ 					
						cout << "SNOL> Error! [" << var << "] is not defined!" << endl; 	
						return false;
					}

					// if the syntax is correct then the string will be erased
					var.erase();

					flag = true;
					continue;
				}

				else if (input[i] == ' ') // check whitespaces in string
				{															
					if ((input[i + 1] == '-') && (isdigit(input[i + 2])) && (flag == false)) // check if input is an operation that involves the variable	
					{   
						cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
						return false;
					}

					else if ((isdigit(input[i - 1])) && (isdigit(input[i + 1]))) // check if the entered command involves two digits with space in between
					{   
						cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl; 
						return false;
					}

					else if ((input[i + 1] == 'B') && (input[i + 2] == 'E') && (input[i + 3] == 'G')) // check if BEG function is after a whitespace
					{
						cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
						return false;
					}

					else if ((input[i + 1] == 'P') && (input[i + 2] == 'R') && (input[i + 3] == 'I') && (input[i + 4] == 'N') && (input[i + 5] == 'T')) // check if PRINT function is after a whitespace
					{
						cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
						return false;
					}
					else continue;
				}

				var += input[i]; // stores the command inputted by the user
				flag = true;
			}
			// checks if the variable is valid
			if (isVariable(var) && !varFinder(var)) 
			{ 												
				cout << "SNOL> Error! [" << var << "] is not defined!" << endl;
				return false;
			}
			return true;
		} 

		/*
		*	getValue() - a function that separates a value from a command
		*	parameter: string input - the command inputted by the user
		*	return: string result - the value of the command
		*/
		string getValue(string input) 
		{
			string temp, result;
			
			for (int i = 0; i < input.length(); i++) // Traverse input string
			{											
				if (input[i] == '-' && isdigit(input[i + 1])) // negative number
				{									
					temp += input[i];
					continue;
				}

				if (isOperator(input[i])) // operator  
				{	 	
					if (isVariable(temp)) // if temp string is a variable, store it with the new command and its value
						result += " " + key.at(temp) + " " + input[i]; 
					
					else // else, store it with the new command only
						result += " " + temp + " " + input[i];
								

					temp.erase();
					continue;
				}

				else if (input[i] == ' ')  // continue loop if whitespace
					continue;			
											
				temp += input[i]; // store current character in temp string
			}

			if (isVariable(temp)) // if temp string is a variable, store it with the new command and its value
				result += " " + key.at(temp); 

			else // else, store it with the new command only
				result += " " + temp;
			
			temp.erase(); // erase temp string
			return result;
		} 
};

/*
*	manual() - displays the user manual of the SNOL program
*	>> no parameters
*	>> displays the user manual of the SNOL program
*/			
void manual() 
{ 
	cout << endl << ">> SNOL Overview" << endl
		 << "\t1. Formatting >> Spaces are used to separate tokens but are not necessary. A command can have no space in it. All words are also case sensitive." << endl
		 << "\t\tExample: var = 17 -> contains one space between every word" << endl
		 << "\t\tExample: var, VaR, VAR will be addressed differently." << endl

		 << "\t2. Data Type >> There are only two types of values: integer and floating-point. No type declaration; the type of variable is determined by the nature of the value it holds." << endl
		 << "\t\tExample: \n\t\tnum = 5 + 5 -> CORRECT (both int)\n\t\tNUM = 5.5 + 5.5 -> CORRECT (both float)\n\t\tnum = 5 + 5.5 -> INVALID (different data types))" << endl 

		 << "\t3. Arithmetic Operations >> All operands in an operation should be of THE SAME TYPE. Each operation is in infix notation and follows C's precedence and associativity rules." << endl

		 << "\t4. Variables >> Cannot be keywords; simple expressions that evaluate to its corresponding value. It can be a combination of letters and numbers. Variables must be DEFINED FIRST before they can be used." << endl

		 << "\t5. Commands >> A command can be any valid literal, variable, and operation, but there are reserved keywords for certain commands." << endl

 		 << "\t6. SPECIAL KEYWORDS >> " << endl
		 << "\t       > PRINT: Displays the targeted variable or literal." << endl
		 << "\t   		\tExample: num = 8\n\tPRINT num" << endl
		 << "\t       > BEG: Prompts for a user input for a variable" << endl
		 << "\t   		\tExample: BEG var\n\tInput: (user value)" << endl
		 << "\t       > HELP: Displays the user manual for this program." << endl
		 << "\t       > EXIT!: Terminates the program." << endl;
	system("pause"); 
}

/*
*	precedence() - A function that checks the precedence of the operators
*		>> parameter: value - the operator
*		>> return: 1 - lower precedence, 2 - higher precedence, -1 - invalid
*/
int precedence (char value) // function that checks the precedence from infix to postfix
{		 
 	if (value == '+' || value == '-')  // lower precedence
        return 1;

	else if (value == '*' || value == '/' || value == '%') // higher precedence
        return 2;

    else 
		return -1;
	
}

/*
*	errorFinder() - A function that checks for an error in the expression
*	>> parameter: postfix - the postfix expression
*	>> return: true - no error, false - error
*/
bool errorFinder (string postfix) 
{ 
    int prev, curr, j;
    string temp, temp1;
    
    for (int i = 0; i < postfix.length(); i++) // traverse postfix string
	{         	 						
        if (isdigit(postfix[i]) || postfix[i]=='.') // store if num or period
		{   	 						 
            temp += postfix[i];
            continue;
        } 

		else 
		{
            if (postfix[i] == ' ' && !temp.empty()) // char is space and temp is not empty
			{    							  
                if (temp.find(".") >= temp.length()) // check for a period in temp string	 								
                    prev = 1; // integer

                else 
                    prev = 0; // float														
                
                if ((i + 1) < postfix.length()) // if the next character is not the last character in the string
				{
                    for (j = i + 1; j < postfix.length(); j++) // find the next character
					{  							 
                        if (isdigit(postfix[j]) || postfix[j] == '.') // if the next character is a number or a period
						{  				 
                            temp1 += postfix[j];
                            continue;
                        } 

						else 
						{
                            if (postfix[j] == ' ') // compare the two data types if the next character is a space
							{  												  
                                if (temp1.find(".") >= temp1.length()) // integer  				
                                    curr = 1;			

                                else // float                            					 
                                    curr = 0;								
                                
                                if (prev != curr) // return false if the data types are not the same    				 				
                                    return false;      				 					
                                   
                                temp = temp1; // store temp1 in temp
                                temp1.clear();       

                                break;
                            }
							else continue;
                        }
                    }
                    i = j; // set i to j
                }
            }
			else continue;
        }
    }
    return true;
}

/*
*	expressionDataType() - A function that checks the data type of the expression
*		>> accepts a postfix string as a parameter
*		>> returns true if the number is an integer, false if it is a floating-point
*/
bool expressionDataType(string postfix) 
{ 
    return (postfix.find('.') >= postfix.length()); // returns true = int, false = floating-point				
}

/*
*	conversionHelper() - A helper function that converts an infix expression to postfix
*	>> accepts an empty character stack and an infix expression 
*	>> returns a postfix expression
*/
string conversionHelper(stack <char> stack, string infix) 
{ 
	string postfix;	// string to hold the postfix expression
	
    for (int i = 0; i < infix.length(); i++) // traverse infix string
	{ 								
        if ((isdigit(infix[i])) || (infix[i] == '.')) 			// if current char is a digit or a period
		{					
            postfix += infix[i];								// append character to postfix		

            if (infix[i + 1] == '.') 
				continue;										// if next char is a period, continue to the next iteration		

            else if (!isdigit(infix[i + 1])) 
				postfix += " ";									// if next char is not a digit, append a space to postfix				
        }

        else if (infix[i] == ' ') 								// if current char is a whitespace then continue to the next iteration		
            continue;
        
        else if (infix[i] == '(') 								// if current char is an open parenthesis, append it to the stack		
            stack.push(infix[i]);											
        
        else if (infix[i] == ')') 								// if current char is a close parenthesis, pop the stack until it meets the open parenthesis
		{									
            while ((stack.top() != '(') && (!stack.empty())) 	
			{			
                char temp = stack.top();
                postfix += temp;								// append the top of the stack to the postfix string		
                postfix += " ";									// add space after each append	
                stack.pop();
            }

            if (stack.top() == '(') 							// if the top of the stack is an open parenthesis, pop it
                stack.pop();
        }

        else 													// if current char is an operator	
		{ 														
            if (infix[i] == '-' && isdigit(infix[i + 1])) 		// if current char & char+1 is a negative number
			{
                postfix += infix[i]; // append							
                continue;
            }

            if (stack.empty()) 									// if stack is empty, push the current char to the stack				
                stack.push(infix[i]);	
            
            else 												
			{															
                if (precedence(infix[i]) > precedence(stack.top())) 		// current operator has higher precedence than the top of the stack
                    stack.push(infix[i]); // append									
                
                else if (precedence(infix[i]) == precedence(stack.top())) 	// equal precedence
				{ 	
                    postfix += stack.top();									
                    postfix += " "; 
                    stack.pop();
                    stack.push(infix[i]); // append									
                }

                else 														// less precedence
				{													
                    while ((!stack.empty()) && (precedence(infix[i]) < precedence(stack.top()))) 
					{
                        postfix += stack.top();
                        postfix += " ";
                        stack.pop(); // pop the stack until the current operator has higher precedence than the top of the stack
                    }

                    stack.push(infix[i]);  // append
                }
            }
        }
    }
    while (!stack.empty()) // pop remaining operators		
	{												
        postfix += stack.top();
        postfix += " ";
        stack.pop();
    }
    return postfix; // return postfix expression
}

/*
*	numDataType() - A function that checks if the value is an integer or a floating-point
*	>> accepts a number
*	>> returns true if the value is an integer, false if it is a floating-point
*/
bool numDataType(float num)
{													
	return num == (float)(int)num; 
} 

/*
*	evaluateIntExp() - A function that evaluates an integer postfix expression
*	>> accepts a stack and a postfix expression
*	>> returns the result of the expression
*/
string evaluateIntExp(stack <float> mystack, string postfix)
{ 
	string num;
	
	for (int i = 0; i < postfix.length(); i++) // traverse postfix expression
	{																		
	 	if(isdigit(postfix[i])) // if the char is a digit	
		{																					
            num += postfix[i]; // stores into the string																			
        	if (!isdigit(postfix[i + 1])) num += " "; // store a whitespace after the number													
        }

        else if (postfix[i] == ' ' && postfix[i + 1] != 0)  // if the char is a whitespace and the next char is not a null terminator
		{	    																					
			int b = atoi(num.c_str()); // convert the string to an integer
			mystack.push(b); // push the integer to the stack
			num.erase(); // clear the string
			continue;			
        }

        else if (postfix[i] == '-' && isdigit(postfix[i + 1])) // negative number
		{												
            num += postfix[i];
            continue;
        }

        else 
		{
        	int val1 = mystack.top();
        	mystack.pop();
        	int val2 = mystack.top();	
        	mystack.pop();
        	
        	if (postfix[i]=='%') // modulo
			{																					
        		if (numDataType(val1)==true && numDataType(val2)==true) // if both values are integers
				{													
	        		int v1 = val1; 
	        		int v2 = val2;
					mystack.push(v2 % v1);
				}

				else // modulo can not be performed if one of the values is a floating-point
					return "Error!";	
			}
			
        	switch(postfix[i]) // evaluate appropriate operator
			{ 
	            case '+': // addition
					mystack.push(val2 + val1); 
					break;

	            case '-': // subtraction
					mystack.push(val2 - val1); 
					break;  

	            case '*': // multiplication
					mystack.push(val2 * val1); 
					break; 

	            case '/': // division
					mystack.push(val2 / val1); 
					break; 
            }
            i++;
		}
 	}
	 
	// store top of stack to ans
	float ans = mystack.top();
	 
	// return evaluated expressions
	if (numDataType(ans) == true)
	{
		int ans2 = mystack.top(); // integer
		mystack.pop();
		string ret = to_string(ans2);
		return ret;
	}

	else // if the value is a floating-point
	{
		mystack.pop(); 
		string ret = to_string(ans);
		return ret;
	}
} 

/*
*	evaluateFloatExp() - A function that evaluates a floating-point postfix expression
*	>> accepts a stack and a postfix expression
*	>> returns the result of the expression
*/
string evaluateFloatExp(stack <float> mystack, string postfix)
{ 
	string num;
	
	for (int i = 0; i < postfix.length(); i++) // traverse postfix expression
	{							
	 	if ((isdigit(postfix[i])) || (postfix[i] == '.')) // if the char is a digit or a period
		{					
            num += postfix[i];										
            if (postfix[i + 1] == '.') // continue if the next char is a period
				continue;									
            else if (!isdigit(postfix[i + 1])) // store whitespace after the number
				num += " ";					
        }

        else if ((postfix[i] == ' ') && (postfix[i + 1] != 0)) // if the char is a whitespace and the next char is not a null terminator
		{												
			float a = atof(num.c_str()); // convert the string to a float										
    		mystack.push(a);
            num.erase();
            continue;
        }
		
        else if (postfix[i] == '-' && isdigit(postfix[i + 1])) // negative number
		{			
            num += postfix[i];													
            continue;
        }
		
        else if (postfix[i] == '%') // error if modulo is used							
            return "Error! Modulo cannot be used on floating-point numbers!";
        
        else  // if the char is an operator
		{
        	float val1 = mystack.top();
        	mystack.pop();

        	float val2 = mystack.top();	
        	mystack.pop();
			
        	switch (postfix[i]) // evaluate appropriate operator
			{ 											
	            case '+': // addition
					mystack.push(val2 + val1); 
					break;
	            case '-': // subtraction
					mystack.push(val2 - val1); 
					break;   					
	            case '*': // multiplication
					mystack.push(val2 * val1); 
					break; 
	            case '/': // division
					mystack.push(val2 / val1); 
					break; 
            }
            i++;
		}
	}
	float ans = mystack.top(); // store top of stack to ans
	
	// return evaluated expressions
	if (numDataType(ans) == true)
	{
		int ans2 = mystack.top();
		mystack.pop();

		string ret = to_string(ans2);
		return ret;
	}

	else // if the value is a floating-point
	{
		mystack.pop();
		string ret = to_string(ans);
		return ret;
	}	
} 

/*
*	commands() - A function that checks if the input is a command
*	>> accepts a string input
*	>> returns an integer for a switch case
*/
int commands(string input) 
{
	regex beg("BEG [^\\|]+");		// regex for a BEG command
	regex disp("PRINT [^\\|]+");	// regex for a PRINT command
	
	int j = input.length(); 
	
	if (regex_match(input, beg)) // check if input is a BEG command
		return 1;

	else if (regex_match(input, disp)) // check if input is a PRINT command
		return 2;

	else if (input == "EXIT!") // check if input is an EXIT command
		return 3;
	 
	else if (input == "HELP") // check if input is a HELP command
		return 6;
	
	else if (isVariable(input) || isDigit(input)) // simple expression
		return 7;
	
	for (int i = 0; i < j; i++) // traverse through the input
	{  
		if (isOperator(input[i]))		// check if input is an operator
			return 4; 					
		 
		if (input[i] == '=')			// assignment operation with the presence of an '='
			return 5;	
	}
	return 0;	
} 

/*
*	syntaxValidation() - A function that checks if the input is a valid expression
*	>> accepts a string input and an integer type
*	>> returns true if the input is valid, false if not
*/
bool syntaxValidation(string input, int type) 
{ 
	// regex format that follows EBNF rule for variable - letter{(letter|digit)}
	regex var("\\(*-?[A-Za-z][A-Za-z0-9]*\\)*"); 	

	// regex format that follows EBNF rule of 
	// int - [-]digit{digit} 
	// float - [-]digit{digit}.{digit} 	
	regex digit("\\(*-?[0-9][0-9]*(\\.[0-9]+)?\\)*"); 	
	
	int parenthesis = 0; 
	string temp; 

	// BEG Command 
	if (type == 1) 
	{
		temp = input.erase(0, 4); 	// deletes the content of the string in this range (Deletes the word BEG)

		if (regex_match(temp, var)) // returns true if the regular expression var match against temp variable 
			return true;

		else  // else return false and print error
		{
			cout << "SNOL> [" << temp << "]" << " is not a valid variable name!" << endl;
			return false;
		}
	}

	// PRINT Command
	else if (type == 2) 
	{	
		temp = input.erase(0, 6); // deletes the content of the string in this range (Deletes the word PRINT)

		if (regex_match(temp, var) || regex_match(temp, digit)) // returns true if the regular expression var match against temp variable 
			return true; 
		 
		else // otherwise returns false and print error
		{ 
			cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
			return false;
		}
	}

	else if (type == 4) // if input is an expression
	{ 
		for (int i = 0; i < input.length(); i++) 
		{
			if (parenthesis < 0) // if parenthesis has no pair
			{ 
				cout << "SNOL> Missing parenthesis pair!" << endl; 
				return false;
			}

			else if (input[i] == '(') 
			{
				parenthesis++;
				temp += input[i];
			}

			else if (input[i] == ')') 
			{
				parenthesis--;
				temp += input[i];
			}

			else if (isOperator(input[i])) // checks if input is an operator
			{
				if (input[i] == '-' && isdigit(input[i+1])) // if input is a negative number
				{ 
					temp += input[i];
					continue;
				}

				if (temp.size() == 0) // if temp is empty
				{	
					cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
					return false;
				}

				if (!(regex_match(temp, var) || regex_match(temp, digit))) // checks if temp is not a variable or temp is a number
				{  
					cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
					return false;
				}

				temp.erase(); // empty the temp
			}

			else if (input[i] == ' ')  // ignore whitespaces
				continue;	
			
			else 
				temp += input[i];	
		}

		if (parenthesis != 0) // Error check for unpaired paranthesis
		{ 
			cout << "SNOL> Missing parenthesis pair!" << endl;
			return false;
		}

		if (temp.size() == 0) // if empty size return false
		{	
			cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
			return false;
		}

		if (!(regex_match(temp, var) || regex_match(temp, digit))) // checks if temp is not a variable or temp is a number
		{  
			cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
			return false;
		}

		temp.erase();
		return true;
	}

	else if (type == 5) // Assignment Operation
	{ 
		for (int i = 0, equals = 0; i < input.size(); i++) 
		{
			if (parenthesis < 0) // guard clause for unpaired paranthesis
			{ 
				cout << "SNOL> Missing parenthesis pair!" << endl;
				return false;
			}

			else if (input[i] == '(') 
			{
				parenthesis++;
				temp += input[i];
			}

			else if (input[i] == ')') 
			{
				parenthesis--;
				temp += input[i];
			}

			else if (input[i] == '=') // guard clause for more than one '=' in the expression
			{	
				equals++;

				if (equals > 1) 
				{
					cout << "SNOL> Invalid! More than one '=' in the expression." << endl;
					return false;
				}

				if (!regex_match(temp, var)) // if temp is not a valid variable
				{	
					cout << "SNOL> Error! Invalid variable name syntax." << endl;
					return false;
				}

				temp.erase();	
			}

			else if (isOperator(input[i])) // operator
			{	
				if (input[i] == '-' && temp.length() == 0) // negative number
				{ 
					temp += input[i];
					continue;
				}

				if (temp.size() == 0 || equals == 0) // empty token
				{	
					cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
					return false;
				}

				if (!(regex_match(temp, var) || regex_match(temp, digit))) // check if temp is neither a valid variable nor number
				{ 
					cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
					return false;
				}

				temp.erase(); // delete temp
			}

			else if (input[i] == ' ') // ignore whitespaces
				continue;
			
			else temp += input[i]; // add input to temp
		}

		if (parenthesis != 0) // guard clause for unpaired parenthesis
		{ 
			cout << "SNOL> Missing parenthesis pair!" << endl;
			return false;
		}

		if (temp.size() == 0) // empty temp
		{	
			cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
			return false;
		}

		if (!(regex_match(temp, var) || regex_match(temp, digit))) // temp is not a valid variable nor digit
		{ 
			cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
			return false;
		}
		temp.erase();
		return true;
	}
	else return false;
} 

/*
*	isOperator() - a function that checks if the character is an operator
*	parameter: char ch - the character to be checked
*	return: true if the character is an operator, false if not
*/
bool isOperator (char ch) 
{	
	switch (ch) 
	{
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

/*
*	isVariable() - a function that checks if the string is a valid variable
*	parameter: string ch - the string to be checked
*	return: true if the string is a variable, false if not
*/
bool isVariable(string ch) 
{	
	// regex format that follows EBNF rule for 
	// variable: 	letter{(letter|digit)}
	regex var(" *\\(*-?[A-Za-z][A-Za-z0-9]*\\)*"); 

	if (regex_match(ch, var))
		return true; // return true if the string matches the regex format
	
	else
		return false; // return false otherwise
} 

/*
*	isDigit() - a function that checks if the string is a valid digit
*	parameter: string ch - the string to be checked
*	return: true if the string is a digit, false if not
*/ 
bool isDigit(string ch) 
{	
	// regex format that follows EBNF rule for
	// data type int: 	[-]digit{digit}
	// data type float: [-]digit{digit}.{digit}
	regex digit(" *\\(*-?[0-9][0-9]*(\\.[0-9]+)?\\)*");	  

	if (regex_match(ch, digit)) 
		return true;
		
	else 
		return false;
}

/*
*	postfixConversion() - a function that converts an infix expression to postfix expression 
*	parameter: string expr - the infix expression
*	return: none
*/
void postfixConversion(string expr)
{
	stack <char> s;
	string postfix = conversionHelper(s, expr);

	if (errorFinder(postfix) == false) // check for data type consistency
	{
		cout << "SNOL> Error! Operands must be of the same data type in an arithmetic operation!" << endl;
		return;
	}

	else // check if expression data type is int or float
	{
		if (expressionDataType(postfix)) // int data type
		{
			stack <float> i;
			string ans = evaluateIntExp(i, postfix);

			if (ans == "ERROR") 
			{
				cout << "SNOL> Error! Operands must be int type to perform Modulo!"; 			
				return;
			}
		}

		else // float data type
		{
			stack <float> f;
			string ans = evaluateFloatExp(f, postfix);

			if (ans == "ERROR") 
			{
				cout << "SNOL> Error! Operands must be int type to perform Modulo!"; 			
				return;
			}
		}
		return;
	}
} 

/*
*	main() - The main function
*	parameter: none
*/
int main() 
{ 
	string input;  		// user string input
	int type = 0; 		// variable to use to store type of command
	Tokenizer tokens; 	// object from Tokenizer class	
	
	// Message for program start
	cout << "\n\nThe SNOL environment is now active, you may proceed with giving your commands." << endl;
	
	while (input != "EXIT!") // loop until user input is "EXIT!"
	{ 
		// Get user input
		cout << "\nCommand: ";   
		getline(cin, input); 
		
		// Command validation
		type = commands(input);
		
		switch (type) // switch case for command type
		{ 		
			case 0: // Invalid command input
				cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl; 
				break;

			case 1: // BEG command
				if (syntaxValidation(input, type)) // Input syntax validation
					tokens.BEG(input); 		   

				break;  

			case 2: // PRINT command
				if (syntaxValidation(input, type)) // Input syntax validation
					tokens.PRINT(input);

				break;
				
			case 3: // EXIT command
				cout << "\nInterpreter is now terminated..." << endl;
				return 0;

				break;

			case 4: // Expression
				if (!syntaxValidation(input, type))  	// Input syntax guard clause
					break;
				
				if (tokens.varValidation(input)) 		// checks if the input has a variable
				{ 
					input = tokens.getValue(input); 	// get the value of the variable
					postfixConversion(input); 			// convert to postfix and evaluate
				}
				break;

			case 5: // Assignment operation
				if (syntaxValidation(input, type)) // Input syntax validation
					tokens.assignmentOp(input);	
				
				break;

			case 6: // User manual
				manual(); 
				break;

			case 7: // Simple expression
				break;
		}
	}
}

