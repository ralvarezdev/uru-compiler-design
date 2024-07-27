#include "lexical_analyzer.hpp"

using std::pair;
using std::to_string;

lexical_analyzer::lexical_analyzer()
{
    this->symbols_table = new map<string, tokens::token_info>();
}

// Add token to symbols table
void lexical_analyzer::add_token(const string& token_key, const tokens::token_info& token_info){
    this->symbols_table->insert(pair<string, tokens::token_info>(token_key, token_info));
}

// Get token from symbols table
tokens::token_info lexical_analyzer::get_token(const string& token_key){
    return this->symbols_table->at(token_key);
}

// Update token at symbols table
void lexical_analyzer::update_token(const string& token_key, const tokens::token_info& token_info)
{
    this->add_token(token_key, token_info);
}

// Remove token from symbols table
void lexical_analyzer::remove_token(const string& token_key)
{
    this->symbols_table->erase(token_key);
}

void lexical_analyzer::print_tokens(){
    for (auto const& x : *this->symbols_table)
        cout << x.first << " : " << this->symbols_table->at(x.first).to_string() << "\n";
}

