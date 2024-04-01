/*
********************************************************************************************
TITLE: asm																																
AUTHOR: 
ROLL NO.: 
#DECLARATION OF AUTHORSHIP
All contents of this file have been solely written by me for CS210 assignment at the Dept.
of Computer Science and Engineering IIT Patna.

#NOTE:
This file is compatible with g++ (V 11.2.0) compiler, file name has to be passed through
console which will be read using argc and argv.


#USAGE
1. To compile the asm.cpp file use the following command
g++ asm.cpp -o asm
2. To   run  the emulator use the following command
.\asm.exe bubble.asm

********************************************************************************************
*/


#include <bits/stdc++.h>
using namespace std;


//! .\asm_v4.exe ../asm/test8.asm

typedef long long int ll;
const string WHITESPACE = " \n\r\t\f\v";
int counter = 0;
string pc = "00000000";

vector<string> error;
// whole file
vector<string> lines;
vector<vector<string>> line_part;
vector<string> finalstr;

map<string, int> opCodes;

multimap<int, string> labelAddr;

vector<string> instructions;
vector<string> warning;
string warn;
bool errFlag;

void  Call_Error(int code, string label)
{
    string temp;
    switch (code)
    {
        case 0:
            temp = "ERROR: Unknown mnemonic \"" + label + "\" found with more than one argument\n";
            break;
        case 1:
            temp = "ERROR: Unknown mnemonic \"" + label + "\" found. With one argument\n";
            break;
        case 2:
            temp = "ERROR: Unknown mnemonic \"" + label + "\"  found. With no argument\n";
            break;
        case 3:
            temp = "ERROR: Bogus Label Name \"" + label + "\"\n";
            break;
        case 4:
            temp = "ERROR: Invalid Number Entered \"" + label + "\"\n";
            break;
        case 5:
            temp = "ERROR: No such label \"" + label + "\" exists\n";
            break;
        case 6:
            temp = "ERROR: Duplicate label \"" + label + "\" found\n";
            break;
        case 7:
            temp = "WARNING: Infinite loop detected\n";
            break;
        case 8:
            temp = "ERROR: Inappropriate arguments given: " + label + " \n";
            break;
        case 9:
            temp = "ERROR: Unknown mnemonic \"" + label + "\" found\n";
            break;
        default:
            temp = "Unknown error code";
    }
    error.push_back(temp);
}

void  padstring(string &s, int n, int cnt);
void pcounterinc(int cnt);
string  int_2_hex(int cnt);
void RemoveComment_token();
void EraseEmpty();

bool  num_check(string s);
string  colon_space(string s);
bool   Check_Label(string str);
bool   Hex_Check(string str);

void OpCode_Map()
{
    opCodes["ldc"] = 0;
    opCodes["adc"] = 1;
    opCodes["ldl"] = 2;
    opCodes["stl"] = 3;
    opCodes["ldnl"] = 4;
    opCodes["stnl"] = 5;
    opCodes["add"] = 6;
    opCodes["sub"] = 7;
    opCodes["shl"] = 8;
    opCodes["shr"] = 9;
    opCodes["adj"] = 10;
    opCodes["a2sp"] = 11;
    opCodes["sp2a"] = 12;
    opCodes["call"] = 13;
    opCodes["return"] = 14;
    opCodes["brz"] = 15;
    opCodes["brlz"] = 16;
    opCodes["br"] = 17;
    opCodes["HALT"] = 18;
    opCodes["data"] = 19;
    opCodes["SET"] = 20;
}
string left_erase(const string &str);
string right_erase(const string &str);
string erase(const string &s);
void console_test(vector<vector<string>> f);
int  String_Label(string s);
int  Opcode_Check(int opcode);
void prntmultimap(multimap<int, string> m);

void   Address_Final(string str, bool &pol);
void IntialPassAddress(ifstream &inputFile, ofstream &outputOFile);
void  pass_out(ofstream &outputOFile);
void  Instruction_Handler(ofstream &outputOFile, string str);
void   Handle_Branch(ofstream &outputOFile, string str, int opcode);
void opcode1_handler(ofstream &outputOFile, string str, int opcode, string iflabel);
void modify(string str, string labelname);
void valuewriter(ofstream &outputOFile, int opcode, int str);
int  string_2_int(string str);

bool acceptlabel(string str);

void   Final_Routine(string filename)
{
    ifstream inputFile(filename);
    cout << filename << endl;
    filename = filename.substr(0, filename.length() - 4);
    ofstream outputOFile(filename + ".o");
    ofstream outputLogFile(filename + ".log");

   OpCode_Map();
    string line;
    while (getline(inputFile, line))
    {
        lines.push_back(line);
    }

    for (int i = 0; i < lines.size(); i++)
    {
        lines[i] =  colon_space(lines[i]);
    }

    // comment removal
    RemoveComment_token();
   EraseEmpty();

    //console_test(line_part);
    IntialPassAddress(inputFile, outputOFile);
     pass_out(outputOFile);
    prntmultimap(labelAddr);

    for (auto val : error)
    {
        outputLogFile << val << endl;
    }
    outputOFile.close();
    ifstream inputOFile(filename + ".o");
    ofstream outputLFile(filename + ".lst");
    string linenew;
    int i = 0;

    while (i < lines.size())
    {
        getline(inputOFile, linenew);
        outputLFile << linenew<< "    " << finalstr[i] << endl;
        i++;
    }
    outputLFile.close();
    inputFile.close();
    outputOFile.close();
    outputLogFile.close();
}

int main(int argc, char **argv)
{
    string filename;
    if (argc == 2)
    {
        filename = argv[1];
    }
    else
    {
        cout << "ASSEMBLY FAILED: Invalid number of arguments passed (" << argc - 1 << ") Only the file name is to be entered." << endl;
        exit(1);
    }
    cout << "FILE OPENED SUCCESSFULLY" << endl;
      Final_Routine(filename);
    return 0;
}
//used to remove prefix whitespace such as tab , null , space etc  
string left_erase(const string &str)
{
 size_t start = 0;
    while (start < str.length() && WHITESPACE.find(str[start]) != string::npos) {
        start++;
    }
    return (start == str.length()) ? "" : str.substr(start);
}
//used to remove suffix whitespace for given string ;
string right_erase(const string &str)
{
     int end = str.length() - 1;
    while (end >= 0 && WHITESPACE.find(str[end]) != string::npos) {
        end--;
    }
    return (end < 0) ? "" : str.substr(0, end + 1);
}
//self evident 
string erase(const string &s)
{
    return right_erase(left_erase(s));
}
//test file are read here using ifstream and ofstream used to know output file , where strings are sended to address decider for futher work 
void IntialPassAddress(ifstream &inputFile, ofstream &outputOFile)
{string fl = "";
    vector<string> lines;
    bool prelabel = false;
    string line;
    while (getline(inputFile, line)) {
        stringstream ss(line);
        string word, temp = "";
        while (ss >> word) {
            fl += word + " ";
            temp += word + " ";
        }
        lines.push_back(temp);
        fl += "\n";
    }
    int i = 0;
    while (i < lines.size()) {
          Address_Final(lines[i], prelabel);
        i++;
    }
}
//used to increase value of pc variable which is for pc coounter 
void pcounterinc(int cnt, string &pc)
{
    counter = cnt;
    pc =  int_2_hex(counter);
     padstring(pc, 8, counter);
}
//given function take sign of cnt and pads s with length less than n with 0 of F and if s greater than n it is truncated 
void  padstring(string &s, int n, int cnt) {
    char fillChar = (cnt >= 0) ? '0' : 'F';

    if (s.length() < n) {
        s = string(n - s.length(), fillChar) + s;
    } else if (s.length() > n) {
        s = s.substr(s.length() - n);
    }
}
//simple int 2 hex conversion using stream itself 
string  int_2_hex(int cnt)
{
    stringstream stream;
    stream << std::hex << cnt;
    return stream.str();
}
//function removes coment tokenize the lines and remove comment in between them 
void RemoveComment_token()
{
   // This function removes any comments from the given code.
   // It iterates through each line of the code, searching for
   // the ';' character which denotes the start of a comment.
   // If a comment is found, it is removed from the line.

   for (int i = 0; i < lines.size(); i++)
   {
       // Search for the start of a comment on each line.
       int pos = lines[i].find(';');
       if (pos != -1)
       {  
           // If a comment is found, remove it from the line.
           lines[i].erase(lines[i].begin() + pos, lines[i].end()); 
       }
   }

   // The code is then split into smaller parts based on the
   // presence of whitespace characters, such as spaces and tabs.
   for (auto val : lines)
   {
       vector<string> t;
       string t1;
       for (int i = 0; i < val.length(); i++)
       {
           if (val[i] == ' ' || val[i] == '\t')
           {
               // If a whitespace character is found, split the
               // code into separate parts.
               if (t1 != "")
               {
                   t.push_back(t1);
                   t1 = "";
               }
           }
           else
           {
               t1 += val[i];
           }
       }
       if (t1 != "")
       {
           t.push_back(t1);
       }
       line_part.push_back(t);
   }

   // Finally, any remaining comments are removed from the smaller
   // parts of the code.
   for (int i = 0; i < line_part.size(); i++)
   {
       for (int j = 0; j < line_part[i].size(); j++)
       {
           // Search for the start of a comment in each part.
           int pos = line_part[i][j].find(';');
           if (pos != -1)
           {
               // If a comment is found, remove it from the part.
               line_part[i].erase(line_part[i].begin() + pos, line_part[i].end());
           }
       }
   }
}

//testing and debug purpose only 
void console_test(vector<vector<string>> f) {
    // Initialize index variables
    int i = 0;
    int j = 0;

    // Iterate over each vector in the 2D vector 'f'
    while (i < f.size()) {
        // Reset 'j' to 0 for each vector
        j = 0;

        // Iterate over each string in the current vector
        while (j < f[i].size()) {
            // Print the string, followed by a space
            cout << f[i][j] << " ";

            // Increment 'j'
            j++;
        }

        // Move to the next line after printing all strings in the current vector
        cout << endl;

        // Increment 'i'
        i++;
    }
}

// Function:EraseEmpty
// --------------------
// This function is designed to clean and remove any unnecessary empty lines and braces
// from a given 2D vector of strings (line_part).
// It iterates through the vector and checks each string for closing braces and empty strings.
// If a closing brace is found, it erases the brace and moves back one index in the inner loop.
// If an empty string is found, it erases the entire string and moves back one index in the outer loop.
void EraseEmpty()
{
   // Iterate through the 2D vector 'line_part'
   for (int i = 0; i < line_part.size(); i++)
   {
       for (int j = 0; j < line_part[i].size(); j++)
       {
           // If the current string contains a closing brace
           if (line_part[i][j] == "}")
           {
               // Erase the brace and move back one index in the inner loop
               line_part[i].erase(line_part[i].begin() + j);
               j--;
           }
       }

       // If the current string is empty
       if (line_part[i].size() == 0)
       {
           // Erase the empty string and move back one index in the outer loop
           line_part.erase(line_part.begin() + i);
           i--;
       }
   }
}

// Function name:  String_Label
int  String_Label(string s)
{
   // Iterate through the labelAddr map
   for (auto itx : labelAddr)
   {
       // Check if the current map value matches the input string 's'
       if (itx.second == s)
       {
           // If a match is found, return the corresponding map key
           return itx.first;
       }
   }
   // If no match is found, return -1 as a default value
   return -1;
}


// Function:  pass_out
// Description: This function performs the second pass of processing the data.
void  pass_out(ofstream &outputOFile)
{
   // Initialize an empty string 'fl' and a vector 'lines' to store the processed data.
   string fl = "";
   vector<string> lines;

   // Iterate over the 'line_part' vector.
   auto it = line_part.begin();
   while (it != line_part.end())
   {
       string t = "";

       // Concatenate the words in the current line with a space separator.
       auto it1 = it->begin();
       while (it1 != it->end())
       {
           fl += *it1 + " ";
           t += *it1 + " ";
           it1++;
       }

       // Store the processed line in the 'lines' vector.
       lines.push_back(t);
       fl += "\n";
       it++;
   }

   // Reset the 'finalstr' variable to the processed data.
   finalstr = lines;
   counter = 0;
   int i = 0;

   // Iterate over the processed lines and apply ' colon_space' and 'decide' functions.
   while (i < lines.size())
   {
       lines[i] =  colon_space(lines[i]);
        Instruction_Handler(outputOFile, lines[i]);
       i++;
   }
}
//check whether such opcode exist 
int  Opcode_Check(int opcode)
{
   // Define the set of valid opcodes.
   const std::set<int> validOpcodes = {0, 1, 2, 3, 4, 5, 10, 13, 15, 16, 17, 19, 20};

   // Check if the provided opcode is present in the set of valid opcodes.
   return validOpcodes.count(opcode) > 0;
}


bool  num_check(string s)
{
   // This function checks if the given string 's' is a valid number.
   // It supports both decimal and hexadecimal numbers.

   // hexnumber
   if (  Hex_Check(s))
   {
       // If 's' is a hexadecimal number, the function returns true.
       return true;
   }

   // Check for empty string or strings starting with '+' or '-' sign.
   if (s.empty() || (s.size() > 1 && (s[0] == '-' || s[0] == '+')))
   {
       // If the string is empty or starts with '+' or '-' (except for the first character),
       // the function returns false.
       return false;
   }

   // Iterate through each character in the string 's'.
   for (char c : s)
   {
       // If a non-digit character is found, the function returns false.
       if (!isdigit(c))
       {
           return false;
       }
   }

   // If the string contains only digits, the function returns true.
   return true;
}

void  Instruction_Handler(ofstream &outputOFile, string str) {
    stringstream ss(str);
    string word;
    vector<string> words;

    while (ss >> word)
        words.push_back(word);

    // label
    if (  Check_Label(words[0]) && words.size() == 1) {
        // no pc increment passon
        outputOFile << pc << endl;
        return;
    }

    outputOFile << pc << " ";
    counter++;
    pcounterinc(counter, pc);

    // max label + op + arg
    if (words.size() > 3) {
           Call_Error(8, " counter-->  " + to_string(counter));
        outputOFile << endl;
        return;
    }

    int opcode;
    if (words.size() == 1) {
        opcode = opCodes[words[0]];
    } else {
        opcode = opCodes[  Check_Label(words[0]) ? words[1] : words[0]];
    }

    switch (words.size()) {
        case 1:
            if (opcode == -1) {
                   Call_Error(9, words[0]);
                outputOFile << endl;
            } else {
                switch (Opcode_Check(opcode)) {
                    case 0:
                        valuewriter(outputOFile, opcode, 0);
                        break;
                    default:
                           Call_Error(8, words[0] + " counter-->  " + to_string(counter));
                        outputOFile << endl;
                        break;
                }
            }
            break;

        case 2:
            if (opcode == -1) {
                   Call_Error(9, words[0]);
                outputOFile << endl;
            } else {
                switch (Opcode_Check(opcode)) {
                    case 0:
                        valuewriter(outputOFile, opcode, 0);
                        break;
                    case 1:
                        if (opcode == 15 || opcode == 16 || opcode == 17) {
                              Handle_Branch(outputOFile, words[1], opcode);
                        } else {
                            opcode1_handler(outputOFile, words[1], opcode, words[1]);
                        }
                        break;
                    default:
                           Call_Error(8, words[1] + " counter-->  " + to_string(counter));
                        outputOFile << endl;
                        break;
                }
            }
            break;

        case 3:
            if (  Check_Label(words[0])) {
                if (opcode == -1) {
                       Call_Error(9, words[1]);
                    outputOFile << endl;
                } else {
                    switch (Opcode_Check(opcode)) {
                        case 1:
                            if (opcode == 15 || opcode == 16 || opcode == 17)
                                  Handle_Branch(outputOFile, words[2], opcode);
                            else {
                                opcode1_handler(outputOFile, words[2], opcode, words[0]);
                                string labelname = words[0];
                            }
                            break;
                        default:
                               Call_Error(8, words[1] + " counter-->  " + to_string(counter));
                            outputOFile << endl;
                            break;
                    }
                }
            } else {
                   Call_Error(3, words[0] + " counter-->  " + to_string(counter));
                outputOFile << endl;
            }
            break;
    }
}



// adding space at label after colon 
string  colon_space(string s) {
    string result = s ;
    size_t pos = result.find(':');
    
    if (pos != std::string::npos) {
        // Insert space after colon if necessary
        if (pos + 1 < result.length() && result[pos + 1] != ' ') {
            result.insert(pos + 1, 1, ' ');
            pos++; // Update position after insertion
        }
        // Insert space before colon if necessary
        if (pos > 0 && result[pos - 1] != ' ') {
            result.insert(pos, 1, ' ');
        }
    }
    return result;
}

bool Check_Label(string str) {
    // Find the position of the colon in the string
    size_t colonPos = str.find(':');

    // Check if a colon is found in the string
    bool flag = (colonPos != string::npos);

    // If a colon is found, proceed with further processing
    if (flag) {
        // Extract the label part of the string before the colon
        string label = str.substr(0, colonPos);

        // Validate the label using the acceptlabel function
        if (!acceptlabel(label)) {
            // If the label is not accepted, raise an error and return false
               Call_Error(3, label);
            return false;
        }
    }

    // Return true if a colon is found, otherwise return false
    return flag;
}



//function to check for the branching mechanism and d Instruction_Handler for the branches twhat to do when so called for
void   Handle_Branch(ofstream &outputOFile, string str, int opcode) {
    // Retrieve the address of the target label in the program
    int labeladdr = String_Label(str);
    
    // Calculate the offset between the current instruction and the target label
    int offset = (labeladdr != -1) ? labeladdr - counter : 0;

    // Check for infinite loop
    if (offset == -1)
           Call_Error(7, "INFINITE LOOP");

    // Write the opcode and offset to the output file
    valuewriter(outputOFile, opcode, (labeladdr != -1) ? offset : 0);

    // If the label is not found, report an error
    if (labeladdr == -1)
           Call_Error(5, str);
}


// when an opcode is encountered then this function is called for making further decisions
void opcode1_handler(ofstream &outputOFile, string str, int opcode, string iflabel) {
    switch (opcode) {
        // Handling opcodes that expect only number values
        case 20:
        case 0:
        case 1:
        case 10:
        case 19: {
            // Check if the string contains only a number
            if (num_check(str)) {
                // If opcode is 20, modify the value based on the iflabel
                if (opcode == 20)
                    modify(str, iflabel);
                
                // Write the opcode and the converted string to integer to the output file
                valuewriter(outputOFile, opcode,  string_2_int(str));
            } else if ((opcode == 0 || opcode == 1) && (num_check(str) || String_Label(iflabel) != -1)) {
                // Write the opcode and the label address to the output file
                valuewriter(outputOFile, opcode, String_Label(iflabel));
            } else {
                // Raise an error if the string does not meet the requirements
                   Call_Error(8, str);
                outputOFile << endl;
            }
            break;
        }
        // Handling opcodes that expect label or offset values
        case 2:
        case 3:
        case 4:
        case 5:
        case 13: {
            // Retrieve the label address
            int labeladdr = String_Label(str);
            
            // If label address is found, write the opcode and label address to the output file
            if (labeladdr != -1) {
                valuewriter(outputOFile, opcode, labeladdr);
            } else {
                // If the string is a number, write it as the offset, otherwise raise an error
                int offset = num_check(str) ?  string_2_int(str) : 0;
                valuewriter(outputOFile, opcode, offset);
                
                // Raise an error if the string does not meet the requirements
                if (!num_check(str)) {
                       Call_Error(8, str);
                    outputOFile << endl;
                }
            }
            break;
        }
        default:
            // Handle default case if opcode is not recognized
               Call_Error(8, str);
            outputOFile << endl;
            break;
    }
}


//this  Instruction_Handlers for the label's address what to put as the address of the labels
void   Address_Final(string str, bool &pol) {
    stringstream ss(str);
    string word;
    vector<string> words;

    // Tokenize the input string
    while (ss >> word)
        words.push_back(word);

    // pol indicates whether the previous word was a label
    if (pol) {
        // If the previous word was a label, decide the future based on the current word
        switch (words.size()) {
            case 1:
                // Check if the current word is a valid label and if it's already defined
                if (Check_Label(words[0]) && String_Label(words[0].substr(0, words[0].size() - 1)) == -1) {
                    // Insert the label and its address into the map
                    labelAddr.insert(pair<int, string>(counter, words[0].substr(0, words[0].size() - 1)));
                    pol = true; // Update the state indicating the previous word was a label
                } else {
                    pol = false; // Update the state indicating the previous word was not a label
                }
                break;
            default:
                // Similar logic for cases where the number of words is different
                if (Check_Label(words[0]) && String_Label(words[0].substr(0, words[0].size() - 1)) == -1) {
                    labelAddr.insert(pair<int, string>(counter, words[0].substr(0, words[0].size() - 1)));
                    pol = false;
                } else {
                    pol = false;
                }
                break;
        }
    } else {
        // If the previous word was not a label, decide the future based on the current word
        switch (words.size()) {
            case 1:
                if (Check_Label(words[0]) && String_Label(words[0].substr(0, words[0].size() - 1)) == -1) {
                    labelAddr.insert(pair<int, string>(counter, words[0].substr(0, words[0].size() - 1)));
                    pol = true;
                } else {
                    pol = false;
                }
                break;
            default:
                if (Check_Label(words[0]) && String_Label(words[0].substr(0, words[0].size() - 1)) == -1) {
                    labelAddr.insert(pair<int, string>(counter, words[0].substr(0, words[0].size() - 1)));
                    pol = false;
                } else {
                    pol = false;
                }
                break;
        }
        // Increment the counter after processing
        counter++;
    }
}
//! set operation
void modify(string str, string labelname) {
    // Convert the string representation of the address to an integer
    int address = stoi(str);

    // Remove the label with the given name from labelAddr map, if it exists
    labelAddr.erase(String_Label(labelname.substr(0, labelname.size() - 1)));

    // Insert the new address and labelname pair into the labelAddr map
    labelAddr.insert({address, labelname.substr(0, labelname.size() - 1)});
}


void valuewriter(ofstream &outputOFile, int opcode, int str)
{
    string op =  int_2_hex(opcode);
     padstring(op, 2, opcode);
    string val =  int_2_hex(str);
     padstring(val, 6, str);
    outputOFile << val << op << endl;
}

int  string_2_int(string str) {
    int num = 0;
    int t = (str[0] == '-') ? -1 : 1; // Check if the string starts with '-' sign

    // Remove '-' or '+' sign from the string if present
    str = (str[0] == '-' || str[0] == '+') ? str.substr(1) : str;

    // Check if the string is in hexadecimal format
    if (  Hex_Check(str)) {
        const char *c = str.c_str();
        return (int)strtol(c, NULL, 0);
    }

    // Convert the string to integer in decimal format using while loop
    int i = 0;
    while (i < str.size()) {
        num = num * 10 + (str[i++] - '0');
    }

    // Return the final integer value, multiplied by t to handle negative numbers
    return num * t;
}


bool Hex_Check(string  str) {
    // Check if the string starts with "0x" and has at least 3 characters
    bool startsWith0x = (str.length() >= 3 && str.substr(0, 2) == "0x");

    // Check if all characters after "0x" are hexadecimal digits
    bool allHexDigits = true;
    size_t i = 2; // Start checking from index 2
    while (i < str.length()) {
        allHexDigits = (isxdigit(str[i])) ? allHexDigits : false;
        i++;
    }

    // Return true only if the string meets both conditions
    return startsWith0x && allHexDigits;
}


bool acceptlabel(string str) {
    // Check if the first character is a digit
    bool isDigit = isdigit(str[0]);

    // If the first character is a digit, return false
    return isDigit ? false : std::all_of(str.begin(), str.end(), [](char c) {
        // Check if all characters are alphanumeric
        return isalnum(c);
    });
}

void prntmultimap(multimap<int, string> m)
{
    for (auto itx : m)
    {
        cout << itx.first << " " << itx.second << endl;
    }
}