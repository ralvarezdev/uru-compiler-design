#include<iostream>
#include<filesystem>
#include<string>
#include<fstream>

#include "lib/lexical-analyzer/lexical_analyzer.hpp"
#include "lib/syntax-analyzer/syntax_analyzer.hpp"

using std::getline;
using std::ifstream;
using std::string;
using std::cout;
namespace fs = std::filesystem;

// Constants
const string root_path = "interpreter";
const string file_to_interpret = "supercode.ralvarezdev";

// Function prototypes
void move_to_root();
void move_to_data();

int main()
{
    string line;
    int line_number = 1, column_number;

    // Move to the data directory of the project
    move_to_data();

    // Initalize analyzers
    auto* lexical = new lexical_analyzer();
    auto* syntax = new syntax_analyzer();

    // Open the file to interpret
    auto file_to_interpret_stream = ifstream(file_to_interpret);

    // Check if the file was opened successfully
    if(!file_to_interpret_stream.is_open())
    {
        cout << "Error: Could not open file " << file_to_interpret << "\n";
        return 1;
    }

    // Read file line by line
    while(file_to_interpret_stream.good())
    {
        // Set current column to 1
        column_number = 1;

        // Read line
        getline(file_to_interpret_stream,line);
        cout << line << "\n";

        // Increase current line
        line_number++;
    }

    return 0;
}

// Move to the root directory of the project
void move_to_root()
{
    fs::path root = fs::current_path();
    while(root.filename() != root_path)
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