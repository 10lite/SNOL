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
bool syntaxCheck (string input, int type);												 
bool errorChecking (string postfix); 																													
bool expressionDataType (string postfix);																		
bool numDataType (float num);	

int commands (string input);																																			
int precedence (char value);		

string conversionHelper (stack <char> stack, string infix);											
string evalInt (stack <float> mystack, string postfix);									
string evalFloat (stack <float> mystack, string postfix);	

/*
*	Tokenizer - an object class that stores the tokenized input
*/
class Tokenizer 
{ 
	unordered_map<string, string> key; // a hash type of mapping to identify input and mapped value

	public:
		/*
		*	checkVar() - function that checks if the variable exists in the map
		*	>> parameter - the input string "variable"
		*	>> return - true if the variable exists, false if it does not
		*/
		bool checkVar(string input) 
		{ 
			if (key.find(input) == key.end())
				return false;	// if the command exists then return 0
			
			else
				return true;
		}

		/*
		*	BEG() - function that handles the BEG command
		*	>> parameter - the input string "BEG <variable>"
		*	>> return - none
		*/
		void BEG(string input)
		{
			// remove "BEG" from string
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
			// remove "PRINT" from string
			string temp = input.erase(0, 6);														
			
			if (checkVar(temp)) // print the variable if it exists
				cout << "SNOL> [" << temp << "] = " << key.at(temp) << endl; 						
			
			else if (isDigit(temp)) // print the digit
				cout << "SNOL> [" << temp << "] = " << temp << endl; 								
			
			else // error if variable does not exist	
				cout << "SNOL> Error! [" << temp << "] is not defined!" << endl;	
						
		} 

		/*
		*	assignmentCheck() - function that handles the assignment command
		*	>> parameter - the input string "variable = expression"
		*	>> return - none
		*/
		void assignmentCheck(string input) 
		{ 													
			string temp, expr, var;
			bool flag = false;

			for (int i = 0; i < input.length(); i++) // Traverse input command		
			{ 				

				if (isOperator(input[i])) // if current character is an operator 		
				{																		
					if (input[i] == '-' && isdigit(input[i+1])) // if the character is a negative integer
					{ 													
						temp += input[i];
						continue;
					}

					if (isVariable(temp)) // if the command is valid in terms of syntax
					{																				
						if (checkVar(temp)) 
						{																			
							expr += " " + key.at(temp) + " " + input[i];	// add operator/variable	to the string									
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
						cout << "SNOL> Unknown command! Does not match any valid command!" << endl;
						return;
					}

					else continue;
				}	

				else if (input[i] == '=') // get variable name
				{
					var += temp;	
					temp.erase();
				}

				else
					temp += input[i]; // Add char to part to be assessed 
			}

			// for the end part of the string
			if (isVariable(temp)) 
			{
				if(checkVar(temp))
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
			
			//conditions if the expression is only a digit with no operations
			if (isDigit(expr)) 
				key[var] = expr;
			
			else
			{
				// infix to postfix conversion
				stack <char> s;
				string postfix = conversionHelper(s, expr);
				string ans = "";
				
				// checks to see if all numbers have the same data types
				if(!errorChecking(postfix)) 
				{
					cout << "Error!" << endl;
					return;
				}
				else
				{
					// checks to see the data type in the postfix expression
					if(expressionDataType(postfix)) 
					{
						stack <float> i;
						ans = evalInt(i, postfix);

						if (ans == "Error!") 
						{
							cout << "SNOL> Error! Only int type to perform Modulo!";
							return;
						}
					}

					else
					{
						// evalutating the postfix that involves a float
						stack <float> f;
						ans = evalFloat(f, postfix);

						if (ans == "ERROR") 
						{
							cout << "SNOL> Error! Only int type to perform Modulo!";	// display an error	if not an integer type		
							return;
						}	
					}

					// store the evaluated input into the variable in the map.
					key[var] = ans;
				}
			}
		} 
		
		/*
		*	varCheck() - function that checks if the variable is valid
		*	>> parameter - input string
		*	>> return true if the variable is valid, false if not
		*/
		bool varCheck(string input) 
		{ 
			string var; 					
			bool flag = false;
			
			for (int i = 0; i < input.length(); i++) // Traverse input command
			{												 
				if (isOperator(input[i])) // checks if the character is an operator	
				{ 											
					if (isVariable(var) && !checkVar(var)) // checks if the string is a variable in syntax & if it exists		 
					{ 					
						cout << "SNOL> Error! [" << var << "] is not defined!" << endl; 	
						return false;
					}

					// if the syntax is correct then the string will be erased
					var.erase();

					flag = true;
					continue;
				}

				else if (input[i] == ' ') // checks to see if there are any whitespaces in the strings
				{															
					if ((input[i + 1] == '-') && (isdigit(input[i + 2])) && (flag == false)) // checks if the syntax of the input is an operation that involves the variable	
					{   
						cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
						return false;
					}

					else if ((isdigit(input[i - 1])) && (isdigit(input[i + 1]))) // checks if the entered command involves a digit
					{   
						cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl; 
						return false;
					}

					else continue;
				}

				var += input[i]; // stores the command inputted by the user
				flag = true;
			}

			// check the final string
			if (isVariable(var) && !checkVar(var)) 
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
			
			for (int i = 0; i < input.length(); i++) // loop that will traverse the command
			{											
				if (input[i] == '-' && isdigit(input[i + 1])) // negative number
				{									
					temp += input[i];
					continue;
				}

				if (isOperator(input[i])) // checks if the current command is an operator  
				{	 	
					if (isVariable(temp)) // checks if the stored string is a variable then stores it with the existing varibable
						result += " " + key.at(temp) + " " + input[i]; 
					
					else // else, store it with the new command only
						result += " " + temp + " " + input[i];
								

					temp.erase();
					continue;
				}

				else if (input[i] == ' ')  // continues for loop if it encounters a whitespace	
					continue;			
											
				temp += input[i]; // stores the command inputted by the user
			}

			if (isVariable(temp))
				result += " " + key.at(temp); 

			else
				result += " " + temp;
			
			temp.erase();
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
		 << "\t1. Format >> Separate items with spaces. However, extra spaces will be disregarded." << endl
		 << "\t\tExample: var = 17 -> contains one space between every word" << endl

		 << "\t2. Case Sensitive >> All words are case sensitive." << endl
		 << "\t\tExample: num, NUM, and nUm are distinct from one another. They will be addressed differently." << endl

		 << "\t3. Data Type >> There are two data types only for SNOL integer and floating point" << endl
		 << "\t\tExample: \n\t\tnum = 10 + 7 -> Correct since both are integers\n\t\tNUM = 10.3 + 7.5 -> Correct since both are floating-point\n\t\tnUm = 10 + 7.3 -> Invalid. Their data types are both different" << endl 

		 << "\t4. Operands >> Operands must be of the same type for arithemetic operations:" << endl

		 << "\t5. Variables >> Variables can be number or letters. However, it should start with letters, not numbers." << endl
		 << "\t\tExample: \n\t\tnum -> Correct\n\t\t!num -> Incorrect \n\t\tNote: Variables should be DECLARED first before using." << endl

		 << "\t6. Invalid Syntax >> Your command should be logical and acceptable" << endl

 		 << "\t7. SPECIAL KEYWORDS >> This program has only 4 special keywords, namely: PRINT, BEG, HELP, and EXIT!" << endl
		 << "\t       > PRINT: Display the targeted variable or literal." << endl
		 << "\t\t\t   	Example: num = 8\n\t\t\tPRINT num" << endl
		 << "\t       > BEG: Asks for a value" << endl
		 << "\t       > HELP: Give the user an idea on how to use the program." << endl
		 << "\t       > EXIT!: Terminate the Program" << endl;
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
*	errorChecking() - A function that checks for an error in the expression
*	>> parameter: postfix - the postfix expression
*	>> return: true - no error, false - error
*/
bool errorChecking (string postfix) 
{ 
    int prev, curr, j;
    string tmp, tmp1;
    
    for (int i = 0; i < postfix.length(); i++) // a loop that checks each character to see if all the numbers have the same data type	
	{         	 						
        if (isdigit(postfix[i]) || postfix[i]=='.') // stores character into a temporary string if number or period	
		{   	 						 
            tmp += postfix[i];
            continue;

        } 

		else 
		{
            if (postfix[i] == ' ' && !tmp.empty()) // if char is space and temporary string is not empty, proceed to the if statement
			{    							  
                if (tmp.find(".") >= tmp.length()) // checks to see if the characters are integers	 								
                    prev= 1; // chars are integer

                else 
                    prev = 0; // chars are float															
                
                if ((i+1) < postfix.length()) 
				{
                    for (j = i+1; j < postfix.length(); j++) // finds the next number to compare with tmp
					{  							 
                        if (isdigit(postfix[j]) || postfix[j]=='.') // stores character into a temporary string if num or period
						{  				 
                            tmp1 += postfix[j];
                            continue;
                        } 

						else 
						{
                            if (postfix[j] == ' ') // the character will compare the previous number and the next number if it the char is space	
							{  												  
                                if (tmp1.find(".") >= tmp1.length()) // num is integer  				
                                    curr = 1;			

                                else // num is a float                               					 
                                    curr = 0;								
                                
                                if (prev != curr) // the two data type did not match if prev != curr	    				 				
                                    return false;      				 					
                                   
                                tmp = tmp1; // gets the current number and stores it into the temporary storage for the previous number
                                tmp1.clear();       

                                break;
                            }

							else continue;
                        }
                    }

                    i = j;
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
	string postfix;	// a string that will hold the postfix expression
	
	// Traverse the infix expression
    for (int i = 0; i < infix.length(); i++) 
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
*	evalInt() - A function that evaluates an integer postfix expression
*	>> accepts a stack and a postfix expression
*	>> returns the result of the expression
*/
string evalInt(stack <float> mystack, string postfix)
{ 
	string num;
	
	// traverses through the expression
	for (int i = 0; i < postfix.length(); i++)
	{																		
	 	if(isdigit(postfix[i])) // if the char is a digit	
		{																					
            num += postfix[i]; // stores into the string																			
        	if (!isdigit(postfix[i + 1])) num += " "; // stores the whitespace if the next char is not a digit														
        }

        else if (postfix[i]==' ' && postfix[i+1] != 0) 
		{	    																					
			int b=atoi(num.c_str()); // if the character is a whitespace then store the integer from a string																				
        	mystack.push(b);
            num.erase();
            continue;
        }

        else if (postfix[i] == '-' && isdigit(postfix[i + 1])) // if it has a negative operand
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
        	
        	if (postfix[i]=='%')									// proceeds to the function if a modulo is encountered	
			{																					
        		if (numDataType(val1)==true && numDataType(val2)==true)		// checks if both values are int
				{													
	        		int v1=val1; 
	        		int v2=val2;
					mystack.push(v2%v1);							// pushes the modulo result
				}

				else
					return "Error!";
			}
			
			// where operators are evalutated
			// operators in the stack with equivalent arithmetic operations will be pushed        	
        	switch(postfix[i]) 
			{ 
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
	 
	// stores the top of the stack
	float ans=mystack.top();
	 
	// returning the evaluated expressions
	if (numDataType(ans)==true)
	{
		int ans2=mystack.top();
		mystack.pop();
		string ret= to_string(ans2);
		return ret;
	}

	else
	{
		mystack.pop();
		string ret= to_string(ans);
		return ret;
	}
} 

/*
*	evalFloat() - A function that evaluates a floating-point postfix expression
*	>> accepts a stack and a postfix expression
*	>> returns the result of the expression
*/
string evalFloat(stack <float> mystack, string postfix)
{ 
	string num;
	
	// traverses the expression
	for (int i = 0; i < postfix.length(); i++)
	{							
	 	if ((isdigit(postfix[i])) || (postfix[i] == '.')) 		// checks if the char has a period or a digit
		{					
            num += postfix[i];									// stores it			
            if (postfix[i + 1] == '.') continue;				// continues if next char has a period						
            else if (!isdigit(postfix[i + 1])) num += " ";		// stores a whitespace if char is not a digit				
        }

        else if (postfix[i]==' ' && postfix[i+1]!=0) 
		{												
			float a=atof(num.c_str()); 							//if the character is a whitespace then store the integer from a string				
    		mystack.push(a);
            num.erase();
            continue;
        }
		
        else if (postfix[i] == '-' && isdigit(postfix[i + 1])) 	// if it has a negative operand	
		{			
            num += postfix[i];													
            continue;
        }
		
        else if (postfix[i] == '%') 							// invalid imput if a modulo exists in a floating-point
            return "Error!";
        
        else 
		{
        	float val1 = mystack.top();
        	mystack.pop();

        	float val2 = mystack.top();	
        	mystack.pop();
			
			// where operators are evalutated
			// operators in the stack with equivalent arithmetic operations will be pushed       	
        	switch (postfix[i]) 
			{ 											
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
	// stores the top of the stack
	float ans = mystack.top();
	
	// returning the evaluated expressions
	if (numDataType(ans) == true)
	{
		int ans2 = mystack.top();
		mystack.pop();

		string ret = to_string(ans2);
		return ret;
	}

	else
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
	regex beg("BEG [^\\|]+");		// BEG characters[any] to be a BEG command
	regex disp("PRINT [^\\|]+");	// PRINT characters[any] to be a PRINT command
	
	int j = input.length();
	
	if (regex_match(input, beg))
		return 1;

	else if (regex_match(input, disp)) 
		return 2;

	else if (input == "EXIT!") 
		return 3;
	 
	else if (input == "HELP")
		return 6;
	
	for (int i = 0; i < j; i++) // checks if the input is an assignment
	{  
		if (isOperator(input[i]))		// calculate if it starts with an operator
			return 4; 					
		 
		if (input[i] == '=')			// it is an assignment if it starts with an equal "=" sign
			return 5;	
	}
	return 0;	
} 

/*
*	syntaxCheck() - A function that checks if the input is a valid expression
*	>> accepts a string input and an integer type
*	>> returns true if the input is valid, false if not
*/
bool syntaxCheck(string input, int type) 
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

	else if (type == 4) 
	{ 
		for (int i = 0; i < input.length(); i++) 
		{
			if (parenthesis < 0) // if parenthesis has no pari
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
			if (parenthesis < 0) // Error check for unpaired paranthesis
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

			else if (input[i] == '=') 
			{	
				equals++;

				if (equals > 1) // Error check for more thane one equal sign
				{
					cout << "SNOL> Invalid! more than one '=' in the expression." << endl;
					return false;
				}

				if (!regex_match(temp, var)) // if temp is not match in the formate
				{	
					cout << "SNOL> Error! Invalid variable name syntax." << endl;
					return false;
				}

				temp.erase();	
			}

			else if (isOperator(input[i])) // checks if input is an operator
			{	
				if (input[i] == '-' && temp.length() == 0) // checks if input is a negative number
				{ 
					temp += input[i];
					continue;
				}

				if (temp.size() == 0 || equals == 0) // empty token
				{	
					cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
					return false;
				}

				if (!(regex_match(temp, var) || regex_match(temp, digit))) // checks if temp is not a variable or temp is a number	
				{ 
					cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl;
					return false;
				}

				temp.erase(); // delete temp
			}

			else if (input[i] == ' ') // ignore white spaces
				continue;
			
			else temp += input[i]; // add input to temp
		}

		if (parenthesis != 0) // Error check for unpaired paranthesis
		{ 
			cout << "SNOL> Missing parenthesis pair!" << endl;
			return false;
		}

		if (temp.size() == 0) // Error check for empty temp
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
		return true; // returns true if the string match
	
	else
		return false; // returns false if not	
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

	if (errorChecking(postfix) == false) // check for data type consistency
	{
		cout << "SNOL> Error! Operands must be of the same data type in an arithmetic operation!" << endl;
		return;
	}

	else // check if expression data type is int or float
	{
		if (expressionDataType(postfix)) // int data type
		{
			stack <float> i;
			string ans = evalInt(i, postfix);

			if (ans == "ERROR") 
			{
				cout << "SNOL> Error! Operands must be int type to perform Modulo!"; 			
				return;
			}
		}

		else // float data type
		{
			stack <float> f;
			string ans = evalFloat(f, postfix);

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
	string input;  		//user string input
	int type = 0; 		//variable to use to store what type of command
	Tokenizer tokens; 	//object from Tokenizer class	
	
	// Message for program start
	cout << "\n\nThe SNOL environment is now active, you may proceed with giving your commands." << endl;
	
	while (input != "EXIT!") // loops until user input is "EXIT!""
	{ 
		// Get user input
		cout << "\nCommand: ";   
		getline(cin, input); 
		
		// Command validation
		type = commands(input); // checks whether user input is an appropriate command
		
		// Switch case for the command specified by the user
		switch (type) 
		{ 		
			case 0: // Invalid command input
					cout << "SNOL> Unknown command! Does not match any valid command of the language." << endl; 
					break;

			case 1: // BEG command
					if (syntaxCheck(input, type)) // Input syntax validation
						tokens.BEG(input); 		   

					break;  

			case 2: // PRINT command
					if (syntaxCheck(input, type)) // Input syntax validation
						tokens.PRINT(input);

					break;
				
			case 3: // EXIT command
					cout << "\nInterpreter is now terminated..." << endl;
					return 0;

					break;

			case 4:
					if (!syntaxCheck(input, type))  // Input syntax guard clause
						break;
					
					if (tokens.varCheck(input)) // checks if the input has a variable
					{ 
						input = tokens.getValue(input); // get the value of the variable
						postfixConversion(input); // the input will be then converted into postfix 
					}
					break;

			case 5: // Check assigned operations
					if (syntaxCheck(input, type)) // Input syntax validation
						tokens.assignmentCheck(input);	
					
					break;

			case 6:
					manual(); // user manual
					break;
		}
	}
}

