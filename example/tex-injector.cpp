#include "../parser.hpp"

#include <iostream>
#include <fstream>

using namespace std;

/*
 *  The interface of this program is 
 *  stdin -> tex-injector [sourceFile] -> stdout
 */

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "\nFormat\n\tstdin -> tex-injector [sourceFile] -> stdout\n\n";
        return 1;
    }

    ifstream fileExtractor(argv[1]);
    string fileData = "", 
           inputData = Parser().parse(cin);
    
    const string replStr = "%-- Contents --";
    /*
     *  We need complete, unmodified strings from both file and stdin.
     *  Instead of using a while loop to get those, we can use '\0' character as it always will be at the end.
     */

    getline(fileExtractor, fileData, '\0');
    
    cout << fileData.replace(fileData.find(replStr), replStr.size() , inputData.c_str());
    
    return 0;
}
