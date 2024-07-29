#include "lexical_analyzer.hpp"

#include <format>
#include<cstring>

#include "../tokens/tokens.hpp"
#include"../../../../exceptions/expression_exception.hpp"
#include"../../../../dfa-regex-validator/src/lib/validators/validators.hpp"
#include"../../../../dfa-regex-validator/src/lib/validators/digits/digits.hpp"
#include"../../../../dfa-regex-validator/src/lib/validators/integers/integers.hpp"
#include"../../../../dfa-regex-validator/src/lib/validators/floats/floats.hpp"
#include"../../../../dfa-regex-validator/src/lib/validators/identifiers/identifiers.hpp"
#include"../../../../dfa-regex-validator/src/lib/validators/letters/letters.hpp"
#include"../../../../dfa-regex-validator/src/lib/validators/operators/operators.hpp"
#include"../../../../dfa-regex-validator/src/lib/validators/words/words.hpp"

using std::format;
using std::cerr;
using std::isspace;
using std::pair;
using std::to_string;

// - Lexical analyzer class

// Constructor
lexical_analyzer::lexical_analyzer(const bool debug)
{
    // Initialize symbols table
    this->symbols_table_ = map<string, token_info*>();

    // Initialize validators
    this->validators_=map<tokens::t_type, validators*>();

    // Set debug mode
    this->debug_=debug;

    // Add validators_
    this->validators_[tokens::t_digits]=new digits();
    this->validators_[tokens::t_floats]=new floats();
    this->validators_[tokens::t_identifiers]=new identifiers();
    this->validators_[tokens::t_integers]=new integers();
    this->validators_[tokens::t_letters]=new letters();
    this->validators_[tokens::t_operators]=new operators();
    this->validators_[tokens::t_words]=new words();
}

// Add token to symbols table
void lexical_analyzer::add_token(const string& key, token_info* info)
{
    this->symbols_table_[key]= info;
}

// Get token from symbols table
token_info* lexical_analyzer::get_token(const string& key)
{
    return this->symbols_table_.at(key);
}

// Update token at symbols table
void lexical_analyzer::update_token(const string& key, token_info* info)
{
    this->add_token(key, info);
}

// Remove token from symbols table
void lexical_analyzer::remove_token(const string& key)
{
    this->symbols_table_.erase(key);
}

// Print tokens
void lexical_analyzer::print_tokens()
{
    for (auto const& x : this->symbols_table_)
        cout << x.first << " : " << this->symbols_table_.at(x.first)->to_string() << "\n";
}

// Read line
deque<token*>* lexical_analyzer::read_line(const string& line, int line_number)
{
    auto* tokens = new deque<token*>();
    string t_key;
    token_info* t_info;
    map<tokens::t_type,bool>* token_types;
    bool is_last,is_space, has_operator=false,has_alphanum=false,is_valid=false;
    int column_number = 1, token_start = 0;

    // Read line
    if (this->debug_)
        cout << "Reading line: " << line << "\n\n";

    // Tokenize line
    for (int i = 0; i < line.size(); i++)
    {
        column_number++;
        is_last=i+1==line.size();

        if(this->debug_)
            cout<<"Reading character: '"<<line[i]<<"'\n\n";

        // Check if it's not a separator
        if (isalpha(line[i]) || isdigit(line[i]) || line[i] == '.' || line[i] == '_')
        {
            has_alphanum=true;

            if(!is_last)
                continue;
        }

        // Check if it's an space or an operator
        is_space = isspace(line[i]);
        if (!is_space)
        {
            t_key=string(1,line[i]);
            has_operator = this->validators_.at(tokens::t_operators)->validate(&t_key, false);
        }

        if (is_space || has_operator||is_last)
        {
            if(this->debug_)
                cout<<"Space found: "<<(is_space?"Y":"N")<<"\n"
                    <<"Operator found: "<<(has_operator?"Y":"N")<<"\n"
                    <<"Alphanum found: "<<(has_alphanum?"Y":"N")<<"\n"
                    <<"Token start: "<<token_start<<"\n\n";

            if(!has_alphanum&&!has_operator)
            {
                token_start++;
                continue;
            }

            // If it has an alphanumeric character, add its token
            if(has_alphanum)
            {
                // Get token
                if(!is_last||has_operator)
                    t_key = line.substr(token_start, i - token_start);
                else
                    t_key = line.substr(token_start, i - token_start + 1);

                if (this->debug_)
                    cout << "Reading token: '" << t_key << "'\n\n";

                // Validate tokens
                token_types=this->validate_token(t_key);

                // Check if it's valid
                for (int j= 0; j < tokens::t_end; j++)
                    if (token_types->at(static_cast<tokens::t_type>(j)))
                    {
                        is_valid = true;
                        break;
                    }

                // If it's not valid, throw an exception
                if(!is_valid)
                    throw expression_exception(
                        format("Lexical Error: Invalid token '{}' found at column {} in line {}",
                        t_key, token_start+1, line_number));

                // Add token info
                t_info = new token_info(token_types, token_start);

                if(this->debug_)
                    cout<<t_info->to_string()<<"\n\n";

                // If it's an identifier but not a reserved word, store it
                if(token_types->at(tokens::t_identifiers))
                    if(t_key!=reserved_words::print&&t_key!=reserved_words::root)
                        this->add_token(t_key, t_info);

                // Add token
                tokens->emplace_back(new token(t_key, t_info));
            }

            // If it has an operator, add its token
            if(has_operator)
            {
                t_key =string(1, line[i]);
                token_types = this->validate_token(t_key);

                // Add token info
                t_info = new token_info(token_types, i);

                if(this->debug_)
                    cout<<t_info->to_string()<<"\n\n";

                tokens->emplace_back(new token(t_key, t_info));
            }

            // Reset separator flags
            has_operator=has_alphanum=is_valid=false;
            token_start = i + 1;

            continue;
        }

        // Invalid character
        throw expression_exception(
            format("Lexical Error: Invalid character '{}' found at column {} in line {}",
            line[i], column_number-1, line_number));
    }

    return tokens;
}

// Validate token
map<tokens::t_type, bool>* lexical_analyzer::validate_token(const string& token)
{
    const auto validator_result = new map<tokens::t_type, bool>();
    tokens::t_type type;

    if(this->debug_)
        cout<<"Validating token: ";

    for (int i = 0; i < tokens::t_end; i++)
    {
        type=static_cast<tokens::t_type>(i);

        if(this->debug_)
            cout<<type<<"";

        (*validator_result)[type]= this->validators_[type]->validate(&token, false);
    }

    if(this->debug_)
        cout<<"\n\n";

    return validator_result;
}
