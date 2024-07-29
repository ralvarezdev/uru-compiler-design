#include<iostream>
#include<filesystem>
#include<string>
#include<fstream>
#include<vector>

#include "lib/lexical-analyzer/lexical_analyzer.hpp"
#include "lib/syntax-analyzer/syntax_analyzer.hpp"

using std::vector;
using std::getline;
using std::ifstream;
using std::string;
using std::cout;
namespace fs = std::filesystem;

// Constants
const bool DEBUG_TOKENS=false;
const bool DEBUG_LEXICAL_ANALYZER = false;
const string ROOT_PATH = "interpreter";
const string FILE_TO_INTERPRET = "supercode.ralvarezdev";

// Function prototypes
void move_to_root();
void move_to_data();

int main()
{
    string line;
    vector<token*>* tokens;
    int line_number = 1;

    // Move to the data directory of the project
    move_to_data();

    // Initalize analyzers
    auto lexical = lexical_analyzer(DEBUG_LEXICAL_ANALYZER);
    auto syntax = syntax_analyzer();

    // Open the file to interpret
    auto file_to_interpret_stream = ifstream(FILE_TO_INTERPRET);

    // Check if the file was opened successfully
    if (!file_to_interpret_stream.is_open())
    {
        cout << "Error: Could not open file " << FILE_TO_INTERPRET << "\n";
        return 1;
    }

    // Read file line by line
    try
    {
        while (file_to_interpret_stream.good())
        {
            // Read line
            getline(file_to_interpret_stream, line);

            // Parse line
            tokens = lexical.read_line(line, line_number);

            // Print tokens
            if(DEBUG_TOKENS)
                for(auto t: *tokens)
                    cout<<t->to_string()<<"\n";

            // Analyze syntax

            // Increase current line
            line_number++;
        }
    }
    catch (const std::exception& e)
    {
        cout << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}

// Move to the root directory of the project
void move_to_root()
{
    fs::path root = fs::current_path();
    while (root.filename() != ROOT_PATH)
    {
        fs::current_path(root.parent_path());
        root = fs::current_path();
    }
}

// Move to the data directory of the project
void move_to_data()
{
    move_to_root();
    fs::path root = fs::current_path();
    root.append("src");
    root.append("data");
    fs::current_path(root);
}
