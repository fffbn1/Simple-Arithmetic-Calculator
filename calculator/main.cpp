/*  Project: Basic Arithmetic Calculator using Infix-Postfix Conversion. 
	
	Goals:
	- Understand and implement Postfix (Reverse Polish Notation), Infix notation and Shunting Yard Algorithm concepts.
	- Implement stack and queues
	- Learn C++ syntax 
	
	Purpose: to calculate expressions in infix notation by first converting it to postfix notation using the Shunting Yard Algorithm and then evaluating it. 

	Notes:
	 - Need to refactor - clean, make it maintainable/revisable (e.g. able to expand operators list like trigo functions, root, logarithm and exponential)
	 - Might implement GUI. 
*/

# include <iostream> //how to display things onto the console and pull feedback from it. 
# include <cmath> //basic math functions
# include <sstream> 
# include "stacko.h" //stack class self-implemented.
# include <queue> 
# include <stack>
using namespace std;

string token;
bool validity;
stacko mystack;

bool isDouble(string str){
	/*int number;
	istringstream check(str);
	if (check >> number && check.eof())
		return true;
	else return false;*/
	int decimal_count = 0;
	
	for (char i : str){
		if (decimal_count > 1)
			return false; 
		else if (isdigit(i) || i == '.') {
			if (i == '.')
				decimal_count += 1; 
		}
		else return false;
	}
	return true;
} 

int precedence (char op) {
	if (op == '+' || op == '-')
		return 1;
	else if (op == '*' || op == '/')
		return 2;
	else if (op == '^')
		return 3;
	else if (op == '!')
		return 4;
	else {
		return 0;
	}
}
bool validOP (string token){
	if (token == "+" || token == "*" || token == "/" || token == "^" || token == "!" || token == "-" || token == "(" || token == ")")
		return true;
	else return false;
}
bool left_assoc (char op){
	if (!(op == '^' || op == '!'))
		return true;
	else return false; 
}

string extractToken(string& exp){ 
	string token;
	int char_counter = -1;
	bool nonDigit = false;
	for (char i : exp){
		char_counter += 1;
		if (!isDouble(std::string (1,i))){ //maybe change to the condition to if its a validOP.
			nonDigit = true;
			break;
		}
	}
	if (char_counter == exp.length() - 1 && !nonDigit){ 
		token = exp.substr(0, exp.length());
		exp.clear();
	}
	else if (char_counter == 0){
		token = exp[0];
		exp = exp.substr(1);
	}
	else {
		token = exp.substr(0, char_counter);
		exp = exp.substr(char_counter);
	}
	return token;
}

string infixToPostfix(string infix){ 

	stack<char> operastack;
	queue<string> rpnqueue;
	string token2, prevToken, lastToken, firstToken, rpnresult;
	double number;
	bool consec, found, first = false; 
	int opencount = 0, closingcount = 0;

	//using char check for each token 
	while (infix.length() != 0){
		token2 = extractToken(infix);
		if (!first){ //checks first token - whether it starts with anything other than a number, besides " ( "
			first = true;
			if (!isDouble(token2) && token2 != "("){ //not number and not + or -
				validity = false; 
				return "a"; //syntax error
			}
		}
		if (token2 == "(" || prevToken == ")" || prevToken == "!") //prevent going to 'else if' invalid operation block. operators can be consecutive depending on specified current token (e.g. '(' / prevToken (e.g. ')' and '!' ) .
			consec = false; 

		if (isDouble(token2)){ // if token is a number, push to rpn queue.
				rpnqueue.push(token2);
				consec = false; 

		} else if (!validOP(token2) || consec) { //if token is not a valid operator or there are UNALLOWED consecutive operators.
			validity = false; 
			return "b"; //syntax error

		} else {  // valid operators
			consec = true;
			if (token2[0] != ')' && token2[0] != '(') { //dealing with all non-parenthesis operators
				while (!operastack.empty() && operastack.top() != '('  && (precedence(operastack.top()) > precedence(token2[0]) || (precedence(operastack.top()) == precedence(token2[0])) && left_assoc(token2[0]))){ 
					rpnqueue.push(std::string (1,operastack.top()));
					operastack.pop();
				}
				operastack.push(token2[0]);

			}
			else if (token2[0] == '('){ //opening parenthesis
				opencount += 1; 
				operastack.push(token2[0]);
			}
			else { //closing parentheses
				if (prevToken == "("){
					validity = false;
					return "c";
				}
				closingcount += 1; 
				found = false; 
				while (!operastack.empty() && !found){
					if (operastack.top() != '(')
						rpnqueue.push(std::string (1,operastack.top()));
					else 
						found = true; 
					operastack.pop();
				}
				if (!found){ // see if can have an overall !found 'if' outside of else. 
					validity = false;
					return "d"; //syntax error
				}
			}
		}	
		prevToken = token2;
	}

	lastToken = prevToken; 
	if (opencount != closingcount || (!isDouble(lastToken) && (lastToken != "!" && lastToken != ")"))){ // if lastToken is not a number and is not ! and )
		validity = false;
		return "e"; //syntax error
	} 
	while (!operastack.empty()){ //popping remaining operators from stack and pushing to RPN queue
		rpnqueue.push(std::string (1,operastack.top()));
		operastack.pop();
	}
	while (!rpnqueue.empty()){ //extracting RPN queue tokens and appending to rpnresult; 
		rpnresult += rpnqueue.front() + " ";
		rpnqueue.pop();
	}
	return rpnresult;
}

void rpnEval(string token){
	double a, b;
	switch(token[0]){
		case '+':
			a = mystack.pop();
			b = mystack.pop();
			mystack.push(b+a); 
			break;

		case '-':
			a = mystack.pop();
			b = mystack.pop();
			mystack.push(b-a); 
			break;

		case '*':
			a = mystack.pop();
			b = mystack.pop();
			mystack.push(b*a); 
			break;

		case '/':
			a = mystack.pop();
			b = mystack.pop();
			if (a == 0){
				cout << "Error: Division by zero. \n"; //math error
				validity = false;
			} else 
				mystack.push(b/a);
			break;

		case '^':
			a = mystack.pop();
			b = mystack.pop();
			mystack.push(pow(b,a));
			break;
		
		case '!':
			a = mystack.pop();
			mystack.push(tgamma(static_cast<int>(a) + 1));
	}
}

int main() {

	//variables
	double number;
	istringstream postfix_stream;
	string infix, postfix; 
	char yn;

	//get user input
	do {
		cout << "-------------------------------------------------------------------";
		cout << "\nWelcome to the Calculator!\nWhat do you wish to calculate? (Infix Notation w/ Spacing) \n";
		cout << "-------------------------------------------------------------------\n";

		validity = true;
		cout << "Infix: ";
		getline(cin, infix);
		postfix = infixToPostfix(infix); //rpn expression in string.
		cout << "-------------------------------------------------------------------\n";
		cout << "Postfix: " << postfix << "\n";
		istringstream postfix_stream(postfix);

		while (postfix_stream >> token && validity){
			istringstream check(token);
			if (check >> number && check.eof())
				mystack.push(stod(token));
			else 
				rpnEval(token);
		}
		if (validity){
			cout << "Answer: " << mystack.peek();
			mystack.clear();
		} else 
			cout << "Invalid Operation.";
		cout << "\n-------------------------------------------------------------------";
		cout << "\nWould you like to continue? (Y/N)";
		cin >> yn; 
		cin.ignore(); //consume new line before expression prompt

	} while (yn == 'Y' || yn == 'y');
	
	cout << "Bye bye!";
}
