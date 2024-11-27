// Čudars Jēkabs 231RDB342

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <locale>
#include <algorithm>
#include <cwctype>
#include <unordered_map>
#include <set>


using namespace std;

void writeToTSV(string path, unordered_map<wchar_t, int> charCount) {
    wofstream tsvFile(path);
    tsvFile.imbue(locale("en_US.UTF-8"));

    vector<pair<wchar_t, int>> sortedVec(charCount.begin(), charCount.end());

    // Asked chatGPT "How do I sort a vector with pair elements in cpp?"
    sort(sortedVec.begin(), sortedVec.end(), [](const auto &a, const auto &b) {
        return a.second > b.second;
    });

    // writes the values on .tsv file
    for (int i = 0; i < sortedVec.size(); i++) {
        tsvFile << sortedVec[i].first << L"\t" << sortedVec[i].second << L"\n";
    }
}

unordered_map<wchar_t, int> countSymbols(wstring content) {
    unordered_map<wchar_t, int> count;
    for (int i =0; i<content.length(); i++) {
        if (count[content[i]]) {
            count[content[i]]++;
        }else {
            count[content[i]] = 1;
        }
    }

    return count;
}

//Asked chatGPT how do I lowercase a wstring?
wstring toLower(wstring content) {
    transform(content.begin(), content.end(), content.begin(), [](wchar_t c) {
        return std::towlower(c);
    });
    return content;
}

//Asked chatGPT "How do I read in a wstring from .txt file into a wstring?"
wstring getFileContents(string path) {
    wifstream file(path); // Use wide string for the file name if needed
    if (!file.is_open()) {
        cout << "Failed to open file." << endl;
        return L"";
    }

    // Set the locale to handle wide characters
    file.imbue(locale("en_US.UTF-8"));

    // Read the file into a wstring
    wstring content;
    wstring line;
    while (getline(file, line)) {
        content += line + L"\n"; // Add lines to the wstring, preserving newlines
    }

    file.close();

    return content;
}

// Mainly intended to shorten the main function
// This is still lengthy and not so readable, but I believe that it is fine for now
// It essentially validates all of the flags, and sets the provided parameter values
int parseFlags(int argc, char* argv[], bool &lowercaseFlag, string &inputFile, string &outputFile) {
    set<string> flags = {"-l", "-i", "-o"};

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (flags.find(arg) == flags.end()) {
            cerr << "Unknown argument: " << arg << std::endl;
            return 1;
        }

        if (arg == "-l") {
            lowercaseFlag = true;
        } else if (arg == "-i") {
            if (i + 1 > argc) {
                cerr << "After -i flag a file name must follow" << std::endl;
                return 1;
            }

            inputFile = argv[i + 1];
            i++; // incrementing i to skip the value of the flag
        }else if (arg == "-o") {
            if (i + 1 > argc) {
                cerr << "After -i flag a file name must follow" << std::endl;
                return 1;
            }

            outputFile = argv[i + 1];
            i++;
        }
    }

    if (inputFile.empty()) {
        cerr << "Must provide an inputfile -i inputFileName.txt " << std::endl;
        return 1;
    }

    if (outputFile.empty()) {
        size_t pos = inputFile.find_last_of('.');
        if (pos != string::npos) {
            outputFile = inputFile.substr(0, pos) + ".tsv";
        } else {
            outputFile = inputFile + ".tsv"; // If there's no extension, append ".tsv"
        }
    }else if (outputFile.size() < 4 || outputFile.substr(outputFile.size() - 4) != ".tsv") {
        cerr << "Output file must have a .tsv extension" << std::endl;
        return 1;
    }

    return 0;
}

int main(int argc, char* argv[]) {
    bool lowecaseFlag = false;
    string inputFile;
    string outputFile;

    // This is a very golangish way to write things, but in my opinion this makes it very readable
    int err = parseFlags(argc, argv, lowecaseFlag, inputFile, outputFile);
    if (err != 0) {
        return 1;
    }

    wstring content = getFileContents(inputFile);
    if (content.empty()) {
        cerr << "File is empty or not readable." << endl;
        return 1;
    }
    if (lowecaseFlag) {
        content = toLower(content);
    }

    unordered_map<wchar_t, int> charCount = countSymbols(content);
    writeToTSV(outputFile, charCount);
    return 0;
}
