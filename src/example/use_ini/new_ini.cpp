#include <iostream>
#include <stdio.h>

#include "IniFile.h"
#include "String.h"

using namespace std;
using namespace mb::helpers;

int main(void) {
    string path = "mb.ini";
    IniFile ini(path);

    if (ini.isOpen()) cout << "Is open" << endl;
    
    for (const auto& section : ini.sections()) {
        std::cout << "SECTION: " << section << std::endl;
        std::cout << "PARAMS" << std::endl;
        for (auto& param : ini[section]) {     
            std::string p = ini.get(section,param).toString();       
            std::cout << trim(param) << "=" <<trim(p) << std::endl;
        }
    }

    // cout << "GGWP" << endl;

	return 0;
}