#include "stdafx.h"
#include "header.h"

class ParseFile {
 private:
  string inputFilePath, outputFilePath;

 public:
  ParseFile(const string i, const string o)
      : inputFilePath(i), outputFilePath(o){};
  void parseFile() {
    string line;
    ifstream localInputFile(prj::cwd +
                            "/Meeting-7/teach-1-maxima.txt");
    ofstream localOutputFile(
           + "/Meeting-7/teach-1-maxima-parsed-cpp.txt",
        ofstream::binary);

    std::vector<string> arr;

    if (localInputFile.is_open()) {
      for (size_t loopVar = 0; getline(localInputFile, line); loopVar++) {
        arr.push_back(line.substr(0, line.find("*")));
      }
      for (size_t loopVar = 0; loopVar < arr.size(); ++loopVar) {
        localOutputFile << arr[loopVar] << endl;
      }
      localInputFile.close();
    } else {
      cout << "Unable to open file";
    }
  };
};
