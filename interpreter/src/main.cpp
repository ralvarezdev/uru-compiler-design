#include<iostream>
#include<filesystem>
#include<string>
#include<fstream>
#include<deque>

#include "../../exceptions/expression_exception.hpp"
#include "lib/interpreter/syntax_tree_interpreter.hpp"
#include "lib/tokens/tokens.hpp"
#include "lib/tree/parse_tree_node.hpp"
#include "lib/tree/syntax_tree_node.hpp"
#include "lib/lexical-analyzer/lexical_analyzer.hpp"
#include "lib/syntax-analyzer/syntax_analyzer.hpp"
#include "lib/interpreter/syntax_tree_interpreter.hpp"

using std::deque;
using std::getline;
using std::ifstream;
using std::string;
using std::cout;
namespace fs = std::filesystem;

// Constants
const bool DEBUG_TOKENS=false;
const bool DEBUG_LEXICAL_ANALYZER = false;
const bool DEBUG_SYNTAX_ANALYZER = false;
const bool DEBUG_SYNTAX_TREE = false;
const bool DEBUG_INTERPRETER = false;
const bool DEBUG_SYMBOLS_TABLE=false;
const string ROOT_PATH = "interpreter";
const string FILE_TO_INTERPRET = "supercode.ralvarezdev";

// Function prototypes
void move_to_root();
void move_to_data();

int main()
{
    string line;
    deque<token*>* tokens=nullptr;
    int line_number = 1;
    parse_tree_node* parse_root=nullptr;
    syntax_tree_node* syntax_root=nullptr;

    // Move to the data directory of the project
    move_to_data();

    // Initalize analyzers
    auto lexical = lexical_analyzer(DEBUG_LEXICAL_ANALYZER);
    auto syntax = syntax_analyzer(DEBUG_SYNTAX_ANALYZER);
    auto interpreter = syntax_tree_interpreter(&lexical, DEBUG_INTERPRETER);

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
            if(tokens->empty())
            {
                line_number++;
                continue;
            }

            // Parse line and get parse tree root node
            parse_root=syntax.parse_line(line_number,tokens);

            // Increase current line
            line_number++;

            // Get syntax tree root node
            syntax_root=syntax_tree_node::get_syntax_tree_node(parse_root);

            if(DEBUG_SYNTAX_TREE)
                cout<<syntax_root->to_string();

            // Interpret
            interpreter.interpret(line_number,syntax_root);

            // Check symbols table
            if(DEBUG_SYMBOLS_TABLE)
                cout<<lexical.to_string();

            // Free memory
            delete parse_root;
            delete syntax_root;
        }
    }
    catch (expression_exception& e)
    {
        // Free memory
        delete parse_root;
        delete syntax_root;

        for(auto t: *tokens)
        {
            // If it's not a identifier, delete the token info
            if(t->get_info()->get_type()->at(tokens::t_identifiers))
                delete t->get_info();

            delete t;
        }

        cout << e.what() << "\n";
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
