#pragma once
#include <iostream>
#include <vector>
/*instructions: 
done - empty instruction  
done - comment 
done- variable declaration
    *cannot declare and assign on same line
done- variable assignment (condition parser)
- mathematic expressions (condition parser)
    *you can use a stack (or a tree)
- Print command

* make a fuction for each instruction
*/

enum class DataType {
    NUMBER,
    STRING,
    
};

class Data {
public:
    Data(const DataType &type, const std::string &name);
    std::string getName() const;
    DataType getDataType() const;
    virtual int getNumberValue() const = 0;
    virtual std::string getStringValue() const = 0;
    virtual void setVariableValue(std::string value) = 0;
    virtual ~Data();
protected:
    std::string mName = "";
    DataType mType;
};
class Number : public Data {
public:
    Number(const DataType &type, const std::string &name);
    ~Number();
    int getNum();
    virtual int getNumberValue() const;
    virtual std::string getStringValue() const;
    virtual void setVariableValue(std::string value);
private:
    long long mNumber = 0;
};

class String : public Data {
public:
    String(const DataType &type, const std::string &name);
    ~String();
    std::string getStr();
    virtual int getNumberValue() const;
    virtual std::string getStringValue() const;
    virtual void setVariableValue(std::string value);

private:
    std::string mString = "";
};
class Parser {
public:
    Parser();
    std::vector<std::string> parse(const std::string &line);
    std::vector<std::string> parseAssignment(const std::string &line);
    std::vector<std::string> parseExpression(const std::string &expression);
    bool isAssignment(const std::string &line);
    std::string trim(const std::string &str, char *seperators);
    bool isAllSpace(const std::string &line);
    bool isQuote(char c);

private:
    char mSeparators[2] = {' ', '\t'};
    bool isQuoteMark = false;
};

class Interpreter {
public:
    Interpreter();
    ~Interpreter();
    bool interpret(const std::string &line); 
    bool declareVar(const std::vector<std::string> &tokens); 
    bool AssignVar(const std::vector<std::string> &tokens); 
    long long evaluateExpression(std::vector<std::string> expression);
    void assignVarWithArithmetic(const std::vector<std::string> &tokens, std::vector<std::string> expression);
    bool PRINT(const std::string &str);
    bool isComment(const std::string &line);
    bool isDeclaration(const std::string &type, const std::string &name);  
    bool isNumberVariable(const std::string &line);
    bool isStringVariable(const std::string &line);
    bool isAssignment(const std::vector<std::string> &tokens);  
    bool isArithmetic(const std::vector<std::string> &tokens);  
    bool isPrintStatement(const std::string &line);
    bool quotesInString(const std::string &line);

private:
    std::vector<Data*> variables;
};

