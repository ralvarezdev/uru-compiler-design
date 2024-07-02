#include<iostream>
#include<vector>
#include<sstream>
#include<map>
#include<iomanip>

#include "lib/validators/digits/digits.hpp"
#include"lib/validators/floats/floats.hpp"
#include"lib/validators/identifiers/identifiers.hpp"
#include"lib/validators/integers/integers.hpp"
#include "lib/validators/letters/letters.hpp"
#include "lib/validators/operators/operators.hpp"
#include "lib/validators/words/words.hpp"

using std::left;
using std::setfill;
using std::setw;
using std::stringstream;
using std::cin;
using std::cout;
using std::getline;
using std::map;

int main()
{
    // Columns size
    constexpr int name_size = 20;
    constexpr int status_size = 10;
    constexpr int delimiter_size = name_size + status_size;

    // Validators enum
    enum v
    {
        v_digits, v_integers, v_floats, v_identifiers, v_letters, v_operators, v_words, v_end
    };

    vector<validators*> validators;

    validators.insert(validators.begin() + v_digits, new digits());
    validators.insert(validators.begin() + v_integers, new integers());
    validators.insert(validators.begin() + v_floats, new floats());
    validators.insert(validators.begin() + v_identifiers, new identifiers());
    validators.insert(validators.begin() + v_letters, new letters());
    validators.insert(validators.begin() + v_operators, new operators());
    validators.insert(validators.begin() + v_words, new words());

    string input, token;
    cout << "Welcome to DFA Regex Validator\n"
        << "Enter 'exit' to quit the program\n";

    while (true)
    {
        map<string, map<v, bool>> tokens_validators;

        // Get the input from the user
        cout << "\nEnter expression to be validated: ";
        getline(cin, input);

        stringstream ss(input);

        vector<string> tokens;

        // Tokenize the input
        while (getline(ss, token, ' '))
        {
            tokens.push_back(token);
        }

        if (input == "exit")
            break;

        for (auto const& t : tokens)
        {
            map<v, bool> validator_result;

            for (int i = v_digits; i < v_end; i++)
                validator_result[static_cast<v>(i)] = validators.at(i)->validate(&t, false);

            tokens_validators[t] = validator_result;
        }

        for (auto const& [t, v] : tokens_validators)
        {
            cout << "\nToken: " << t << "\n"
                << string(delimiter_size, '-') << "\n";
            bool is_valid=false;

            for (auto const& [validator, result] : v)
            {
                if(!is_valid && result)
                    is_valid=true;

                cout << left << setw(name_size) << setfill(' ') << validators.at(validator)->get_name()
                    << left << setw(status_size) << setfill(' ') << (result ? "Y" : "N") << "\n";
            }

            cout<< string(delimiter_size, '-') << "\n"
            <<left<<setw(name_size)<<setfill(' ')<<"Is valid? "
                <<left<<setw(status_size)<<setfill(' ')<<(is_valid?"Y":"N")<<"\n";
        }
    }
}
