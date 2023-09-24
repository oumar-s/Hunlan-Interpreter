#include <iostream>
#include <fstream>
#include <iostream>
#include <cctype>
#include "interpreter.h"

int main(int argc, char** argv)
{
    
    Interpreter interpreter;
    long long lineNumber = 0;
    bool isValidInstruction;
    std::string line;
    std::ifstream file(argv[1]);
    while(getline(file, line))
    {
        lineNumber++;
        if(line.empty())
        {
            continue;
        }

        else
        {
            isValidInstruction = interpreter.interpret(line);
        }    

        
        if (isValidInstruction == false)
        {
            std::cout << "\nERROR: Bad instruction in Line ";
            std::cout << lineNumber << std::endl;
            abort();
        }
            
        
    }

    file.close();
    return 0;
};