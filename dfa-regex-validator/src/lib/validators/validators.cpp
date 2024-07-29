#include "validators.hpp"

#include<string>

using std::string;

validators::validators(const char* name)
{
    name_.assign(name);
}

string validators::get_name()
{
    return name_;
}

vector<char> validators::get_characters(const string* text)
{
    vector<char> characters;

    for (char c : *text)
        characters.push_back(c);

    return characters;
}
