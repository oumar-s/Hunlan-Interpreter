#include "interpreter.h"
#include <cctype>
#include <climits>
#include <vector>
#include <stack>


//Data Class
Data::Data(const DataType &type, const std::string &name) : mType(type), mName(name) {};
std::string Data::getName() const
{
    return mName;
};
DataType Data::getDataType() const
{
    return mType;
};

Data::~Data() {};

//Number Class
Number::Number(const DataType &type, const std::string &name) : Data(type, name){};
Number::~Number(){

};
int Number::getNum()
{
    return mNumber;
};
int Number::getNumberValue() const
{
    return mNumber;
};

std::string Number::getStringValue() const
{
    return "";
};
void Number::setVariableValue(std::string value)
{
    mNumber = std::stoll(value);
};


//String Class
String::String(const DataType &type, const std::string &name) : Data(type, name) {};
String::~String(){

};

std::string String::getStr()
{
    return mString;
};
int String::getNumberValue() const
{
    return 0;
};

std::string String::getStringValue() const
{
    return mString;
};
void String::setVariableValue(std::string value)
{
    mString = value;  
};


//Parser Class
Parser::Parser() {};
                 
std::string Parser::trim(const std::string &str, char *seperators)
{
    int firstChar = str.find_first_not_of(seperators);
    int lastChar = str.find_last_not_of(seperators);
    int trimmedStr =  lastChar - firstChar + 1;
    return str.substr(firstChar, trimmedStr);
};
bool Parser::isAllSpace(const std::string &line)
{
    for(int i = 0; i < line.size(); i++)
    {
        if(isalnum(line[i]))
        {
            return false;
        }
    }
    return true;
};
bool Parser::isQuote(char c)
{
    if(c == '"')
    {
        isQuoteMark = !isQuoteMark;
        return isQuoteMark;
    }
    else
    {
        return isQuoteMark;
    }
};

bool Parser::isAssignment(const std::string &line)
{
    for(int i = 0; i < line.size(); i++)
    {
        if(line[i] == '=')
        {
            return true;
        }
    }
    return false;

};
std::vector<std::string> Parser::parseAssignment(const std::string &line)
{
    std::vector<std::string> tokens;
    std::string token = "";

    for(int i = 0; i < line.size(); i++)
    {
        if(line[i] == '=')
        {
            tokens.push_back(trim(token, mSeparators));
            token.clear(); 
        }
        else
        {
            if(isQuote(line[i]) == false && line[i] == '#') 
            {
                tokens.push_back(trim(token, mSeparators));
                tokens.push_back("=");

                return tokens;
            }
            else
            {
                token += line[i];
            }
        }
    }
    tokens.push_back(trim(token, mSeparators));
    tokens.push_back("=");

    return tokens;
};


std::vector<std::string> Parser::parse(const std::string &line) 
{
    std::vector<std::string> tokens;
    if(isAllSpace(line))
    {
        return tokens;
    }
    std::string trimmedLine = trim(line, mSeparators);
    std::string token = "";

    if (isAssignment(trimmedLine))
    {
        return parseAssignment(trimmedLine);
    }
    
    for(int i = 0; i < trimmedLine.size(); i++)
    {
        if(isQuote(trimmedLine[i]))
        {
            token += trimmedLine[i];
        }
        else
        {
            if(trimmedLine[i] == '#')
            {
                return tokens;
            }
            if(trimmedLine[i] != mSeparators[0] && trimmedLine[i] != mSeparators[1])
                token += trimmedLine[i];
            else
            {
                if(token.size() == 0)
                    continue;
                else
                {
                    tokens.push_back(token);
                    token.clear();
                }
            }
        }
    }
    tokens.push_back(token);
    return tokens;

};
std::vector<std::string> Parser::parseExpression(const std::string &expression)
{
    std::vector<std::string> tokens;
    std::string token = "";

    for(int i = 0; i < expression.size(); i++)
    {
        if(expression[i] == '+' || expression[i] == '-' || expression[i] == '*')
        {
            if(token.empty())
            {
                tokens.push_back(std::string(1,expression[i]));
                continue;
            }
            if(isAllSpace(token))
            {
                tokens.push_back(std::string(1,expression[i]));
                continue;
            }
            else
            {
                tokens.push_back(trim(token, mSeparators));
                tokens.push_back(std::string(1,expression[i]));
                token.clear();
            }
        }
        else
        {
            token += expression[i];
        }
    }
    tokens.push_back(trim(token, mSeparators));

    return tokens;
};



//Interpreter Class
Interpreter::Interpreter() {};
Interpreter::~Interpreter()
{
    for (int i = 0; i < variables.size(); i++)
    {
        delete variables[i];
        variables[i] = nullptr;
    }
    
};

bool Interpreter::interpret(const std::string &line)
{
    if(line == "\r")
        return true;
    if(line[0] == '#')
    {
        return true;
    }
    if(quotesInString(line))
    {
        return false;
    }
    std::string extraChars = line.substr(line.size()-1, 2);
    std::vector<std::string> tokens;
    Parser parser;
    if(extraChars == "\r")
    {
        std::string fixedLine = line.substr(0,line.size()-1);
        tokens = parser.parse(fixedLine);
    }
    else
    {
        tokens = parser.parse(line);
    }
    if(tokens.empty())
    {
        return true;
    }
    if(tokens.size() < 3)
    {
        tokens.push_back("filling");
    }
    
    //Var declaration
    if(isDeclaration(tokens[0], tokens[1]))
    {
        //Check for double declaration
        for (int i = 0; i < variables.size(); i++)
        {
            if(variables[i]->getName() == tokens[1])
            {
                return false;
            }
        }
        if(declareVar(tokens))
            return true;
        return false;

    }
    
    //variable assignment
    if(isAssignment(tokens) && isArithmetic(tokens) == false)
    {
        if(!isNumberVariable(tokens[0]) && !isStringVariable(tokens[0]))
            return false;
        if(AssignVar(tokens) == false)
            return false;

        return true;

    }

    //variable assignment with arithment
    if(isAssignment(tokens) && isArithmetic(tokens) == true)
    {
        if(tokens[1][0] == '-' || tokens[1][0] == '+' || tokens[1][0] == '*')
            return false;
        if(!isNumberVariable(tokens[0]))
            return false;
        for (int i = 0; i < tokens[1].size(); i++)
        {
            if(tokens[1][i] == '/')
                return false;
        }
        
        std::vector<std::string> terms =parser.parseExpression(tokens[1]);
        
        for (int i = 0; i < terms.size(); i++)
        {
            if(isStringVariable(terms[i]))
                return false;

            if(terms[i] == "+" || terms[i] == "-" || terms[i] == "*")
            {
                if(terms[i] == terms[i+1])
                    return false;
            }
        }
        

        assignVarWithArithmetic(tokens, terms);
        return true;
    }

    //print
    if(isPrintStatement(tokens[0]))
    {
        if(PRINT(tokens[1]))
            return true;
        return false;
    }

   
    return false;

}; 
bool Interpreter::isNumberVariable(const std::string &line)
{
    for (int i = 0; i < variables.size(); i++)
    {
        if(variables[i]->getName() == line && variables[i]->getDataType() == DataType::NUMBER )
        {
            return true;
        }
    }    
    return false;
};
bool Interpreter::isStringVariable(const std::string &line)
{
    for (int i = 0; i < variables.size(); i++)
    {
        if(variables[i]->getName() == line && variables[i]->getDataType() == DataType::STRING )
        {
            return true;
        }
    }
    return false;

};

bool Interpreter::declareVar(const std::vector<std::string> &tokens)
{
    std::string varName = tokens[1];
    if(varName == "NUMBER" || varName == "STRING" || varName == "PRINT")
        return false;
    if(!std::isalpha(varName[0]) && varName[0] != '_')
        return false;
    for (int i = 0; i < varName.size(); i++)
    {
        if(!std::isalnum(varName[i]) && varName[i] != '_')
            return false;
    }
    
    if(tokens[0] == "NUMBER")
    {
        Data* newNumber = new Number(DataType::NUMBER, tokens[1]);
        variables.push_back(newNumber);
        return true;
    }
    else
    {
        Data* newString = new String(DataType::STRING, tokens[1]);
        variables.push_back(newString);
        return true;
    }
    
    
};
bool Interpreter::AssignVar(const std::vector<std::string> &tokens)
{
    if(isNumberVariable(tokens[1]))
    {
        if(isStringVariable(tokens[0]))
            return false;

        long long rightSideValue;
        for (int i = 0; i < variables.size(); i++)
        {
            if(variables[i]->getName() == tokens[1])
            {
                rightSideValue = variables[i]->getNumberValue();
            }
        }

        for (int i = 0; i < variables.size(); i++)
        {
            if(variables[i]->getName() == tokens[0])
            {
                variables[i]->setVariableValue(std::to_string( rightSideValue));
                return true;
            }
        }
    }
    if(isStringVariable(tokens[1]))
    {
        if(isNumberVariable(tokens[0]))
            return false;
        std::string rightSideValue;
        for (int i = 0; i < variables.size(); i++)
        {
            if(variables[i]->getName() == tokens[1])
            {
                rightSideValue = variables[i]->getStringValue();
            }
        }

        for (int i = 0; i < variables.size(); i++)
        {
            if(variables[i]->getName() == tokens[0])
            {
                variables[i]->setVariableValue(rightSideValue);
                return true;
            }
        }
    }
    if(isNumberVariable(tokens[0]))
    {
        for (int i = 0; i < tokens[1].size(); i++)
        {
            if(!std::isdigit(tokens[1][i]))
                return false;
        }
        
        for (int i = 0; i < variables.size(); i++)
        {
            if(variables[i]->getName() == tokens[0])
            {
                variables[i]->setVariableValue(tokens[1]);
                return true;
            }
        }
    }
    if(isStringVariable(tokens[0]))
    {
        std::string str = tokens[1];
        if(str[0] != '\"' || str[str.size()-1] != '\"')
            return false;
        for (int i = 0; i < variables.size(); i++)
        {
            if(variables[i]->getName() == tokens[0])
            {
                variables[i]->setVariableValue(tokens[1].substr(1,tokens[1].size()-2));
                return true;
            }
        }
    }
    return false;

};

long long Interpreter::evaluateExpression(std::vector<std::string> expression)
{
    for (int i = 0; i < expression.size(); i++)
    {
        if(isNumberVariable(expression[i]))
        {
            for (int j = 0; j < variables.size(); j++)
            {
                if(variables[j]->getName() == expression[i])
                {
                    expression[i] = std::to_string(variables[j]->getNumberValue());
                }
            }
        }
    }

    std::string str = "";
    
    for (int i = 0; i < expression.size(); i++)
    {
        str += expression[i];
    }

    int currentNum = 0;
    long long result = 0;
    std::stack<int> terms;
    char operation = '+';
    for (int i = 0; i < str.size(); i++)
    {
        if(std::isdigit(str[i]))
        {
            currentNum = (currentNum * 10) + (str[i] - '0');
        }
        if(!std::isdigit(str[i]) || i == str.size()-1)
        {
            if(operation == '-')
            {
                terms.push(-currentNum);
            }
            else if(operation == '+')
            {
                terms.push(currentNum);
            }
            else if(operation == '*')
            {
                int top = terms.top();
                terms.pop();
                terms.push(top * currentNum);
            }
            operation = str[i];
            currentNum = 0;
        }

    }
    
    while (terms.size() != 0)
    {
        result += terms.top();
        terms.pop();
    }
    
    return result;
};
void Interpreter::assignVarWithArithmetic(const std::vector<std::string> &tokens, std::vector<std::string> expression)
{
    long long resultOfExpression = evaluateExpression(expression);
    if(isNumberVariable(tokens[0]))
    {
        for (int i = 0; i < variables.size(); i++)
        {
            if(variables[i]->getName() == tokens[0])
            {
                variables[i]->setVariableValue(std::to_string(resultOfExpression));
                return;
            }
        }
    }
};
bool Interpreter::PRINT(const std::string &str)
{
    if(isNumberVariable(str))
    {
        for (int i = 0; i < variables.size(); i++)
        {
            if(variables[i]->getName() == str)
            {
                std::cout << variables[i]->getNumberValue();
            }
        }
        return true;
    }
    else if(isStringVariable(str))
    {
         for (int i = 0; i < variables.size(); i++)
        {
            if(variables[i]->getName() == str)
            {
                std::string newStr = variables[i]->getStringValue();
                for (int i = 0; i < newStr.length(); i++)
                {
                    if(newStr[i] == '\\' && newStr[i+1] == 'n')
                    {
                        std::cout << std::endl;;
                        i++;
                    }
                    else
                    {
                        std::cout << newStr[i];
                    }
                }
            }
        }
        return true;
    }
    else if(str[0] == '\"' && str[str.size()-1] == '\"')
    {
        for (int i = 1; i < str.length()-1; i++)
        {
            if(str[i] == '\\' && str[i+1] == 'n')
            {
                std::cout << std::endl;
                i++;
            }
            else
            {
                std::cout << str[i];
            }
        }
        return true;
    }
    for (int i = 0; i < str.size(); i++)
    {
        if(std::isalpha(str[i]))
            return false;
    }
    
    std::cout << str;
        return true;

};

bool Interpreter::isDeclaration(const std::string &type, const std::string &name)
{
    
    if(type == "NUMBER" || type == "STRING")
    {
        return true;
    }
    else
    {    
        return false;
    }

};
bool Interpreter::isAssignment(const std::vector<std::string> &tokens)
{
    if(tokens[2] == "=")
    {
        return true;
    }
    return false;
};
bool Interpreter::isArithmetic(const std::vector<std::string> &tokens)
{
    if(tokens[1][0] == '\"' && tokens[1][tokens[1].size()-1] == '\"')
    {
        return false;
    }
    for (int i = 0; i < tokens[1].size(); i++)
    {
        if(tokens[1][i] == '+' || tokens[1][i] == '-' || tokens[1][i] == '*' )
        {
            return true;
        }
    }
    return false;
};
bool Interpreter::isPrintStatement(const std::string &line)
{
    if(line == "PRINT")
    {
        return true;
    }

    return false;
};
bool Interpreter::quotesInString(const std::string &line)
{
    int countQuotes = 0;
    for (int i = 0; i < line.size(); i++)
    {
        if(line[i] == '#')
            break;
        if(line[i] == '\"')
            countQuotes++;
    }

    if (countQuotes > 2)
    {
        return true;
    }

    return false;
};



