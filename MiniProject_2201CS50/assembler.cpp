
/*
Author@ - NAGESH RAJENDRABHAI DESAI
ROLL - 2201CS50

Declaration of Authorship
This cpp file, assembler.cpp, is part of the miniproject of CS209/CS210 at the 
department of Computer Science and Engg, IIT Patna 
*/
#include <bits/stdc++.h>
using namespace std ;
vector<string> branch_nature ;
vector<string> lines ; //TRIMED AND COMMENT END ASM LINES HERE 
vector<map<int,string >> reduced_lines_pass1 ;
 // datastructure that would handle lines as label , intstruction , operand 
vector<map<int, pair<int,string>>> reduced_lines_pass2 ;
map<string,int > label_found ; // will store all the unique label encounter and their addres till now during code ; 
set<string>label_used ; // used to store label that are used ;
set<string>set_labed ; // label that used set ;
//listing code 
vector <string> listing ; 
//machine code 
int dope = 0 ;
vector <string > machine ; 
// optable 
int err_1=0;
vector<int> line_err_1;
//
int err_2=0;
vector<int> line_err_2;
//
int err_3=0;
vector<int> line_err_3;
//
int err_4=0;
vector<int> line_err_4;
//
int err_5=0;
vector<int> line_err_5;
//
int err_6=0;
vector<int> line_err_6;
//
int err_7=0;
vector<int> line_err_7;
//
int err_8=0;
vector<int> line_err_8;
//
int err_9=0;
vector<int> line_err_9;
//
int err_10=0;
vector<int> line_err_10;
//
int err_11=0;
vector<int> line_err_11;
//Before Set there is no label
int err_12=0;
vector<int> line_err_12;
//NO HALT
int warn_1=0;
//Multiple labels for same pc address
int warn_2=0;
vector<int>line_warn_2;
map<string, pair<string, int>> Optable = {
    // For Type with 1 Argument As Value i.e Type=1
    {"adj", {"0A", 1}},
    {"set", {"14", 1}},
    {"data", {"13", 1}},
    {"ldc", {"00", 1}},
    {"adc", {"01", 1}},

    // For Type with Offset i.e Type=2
    {"ldl", {"02", 2}},
    {"stl", {"03", 2}},
    {"ldnl", {"04", 2}},
    {"stnl", {"05", 2}},
    {"call", {"0D", 2}},
    {"brz", {"0F", 2}},
    {"brlz", {"10", 2}},
    {"br", {"11", 2}},

    // For Type with No Operand i.e Type=0
    {"shr", {"09", 0}},
    {"a2sp", {"0B", 0}},
    {"sp2a", {"0C", 0}},
    {"return", {"0E", 0}},
    {"halt", {"12", 0}},
    {"add", {"06", 0}},
    {"sub", {"07", 0}},
    {"shl", {"08", 0}}
};
// instruction and the type 
//pc count
int pc  =0 ; 
vector<pair<int , string >> err_warn ; 
vector<pair<int, string>> warnings;

string InputName ; 
string LogFileName ; 
string ListingFileName ; 
string MachineCodeFileName ; 
//make the string clean  of comment and prefix whitespace 
string CleanRawInput(string s)
{
    string processedString;
    bool commentFound = false;

    // Clear leading spaces
    size_t startPos = s.find_first_not_of(' ');
    if (startPos != string::npos) {
        s = s.substr(startPos);
    }

    // Iterate through each character in the input string
    for (char c : s)
    {
        // If a comment delimiter ';' is found, set the comment flag and exit the loop
        if (c == ';') {
            commentFound = true;
            break;
        }
        
        // Convert uppercase characters to lowercase
        if (isupper(c)) {
            c = tolower(c);
        }

        // Append the character to the processed string
        processedString += c;
    }

    // Remove trailing whitespace if a comment was found
    if (commentFound)
    {
        // Find the position of the last non-whitespace character
        size_t lastNonSpace = processedString.find_last_not_of(' ');
        if (lastNonSpace != string::npos)
        {
            // Erase any trailing whitespace characters
            processedString.erase(lastNonSpace + 1);
        }
    }
    else
    {
        // Remove trailing whitespace if no comment was found
        size_t lastNonSpace = processedString.find_last_not_of(' ');
        if (lastNonSpace != string::npos)
        {
            // Resize the string to remove trailing whitespace
            processedString.resize(lastNonSpace + 1);
        }
    }

    return processedString;
}
void GetFile(string &InputName)
{
    ifstream file;
    file.open(InputName);

    if (file.fail())
    {
        cout << "Error Occured - Can't Open Input File\n";
        exit(0);
    }

    int LineNumber = 0;
    string LineInput;
    // Taking Line By line Input From File and Removing trailing and extra Spaces
    while (getline(file, LineInput))
    {
        LineNumber++;
        string FormatedInput = CleanRawInput(LineInput);

        bool temp = FormatedInput.empty();
        if (temp)
            continue;

        lines.push_back(FormatedInput);
    }

    file.close(); // Close the file stream
}
void parseInstruction(const string& line, string& label, string& instruction, string& operand) {
    istringstream iss(line);

    // Read the label (if present)
    if (!line.empty() && line[0] == ':') {
        label = "--"; // Set label as "--" if the first character is ':'
    } else if (line.find(':') != string::npos) {
        getline(iss, label, ':');
        // Remove leading and trailing whitespaces
        label.erase(0, label.find_first_not_of(" \t\n\r\f\v"));
        label.erase(label.find_last_not_of(" \t\n\r\f\v") + 1);
    } else {
        label = ""; // No label found
    }

    // Read the instruction
    iss >> instruction;

    // Read the operand
    getline(iss >> ws, operand);
    // Remove leading and trailing whitespaces
    operand.erase(0, operand.find_first_not_of(" \t\n\r\f\v"));
    operand.erase(operand.find_last_not_of(" \t\n\r\f\v") + 1);
}
void printReducedLinesPass1(const vector<map<int, string>>& lines) {
    for (const auto& line : lines) {
        for (const auto& pair : line) {
            cout << "Key: " << pair.first << ", Value: " << pair.second << endl;
        }
        cout << "-----------------------" << endl;
    }
}
void reduce_push()
{
    // Iterate through each line in the global vector of strings 'lines'
    for(int x = 0 ; x < lines.size(); x++) {
        // Initialize strings to store label, instruction, and operand
        string label = "";
        string inst = ""; 
        string operand = ""; 
        
        // Parse the instruction into its components
        parseInstruction(lines[x], label, inst, operand);
        
        // Create a map to store the instruction components
        map<int, string> mp; 
        
        // Populate the map with label, instruction, and operand
        mp[0] = label; 
        mp[1] = inst; 
        mp[2] = operand; 
        
        // Push a pair (line number, map of instruction components) into the reduced_lines_pass1 vector
        reduced_lines_pass1.push_back(mp);
    }
}
void printErrorWarnings( vector<pair<int, string>>errors_warnings) {
    for (const auto& pair : errors_warnings) {
        cout << "Line " << pair.first << ": " << pair.second << endl;
    }
}
void printMap(const std::map<std::string, int>& label_found) {
    // Iterate over each key-value pair in the map
    for (const auto& pair : label_found) {
        std::cout << "Label: " << pair.first << ", Value: " << pair.second << std::endl;
    }
}
//print map ;
//check whether a valid label return true on empty which means no label was their 
bool isValidLabel(string label) {
    // Check if the label is empty
    if (label.empty()) {
        return true; // Empty label is considered valid
    }

    // Check if the first character is a digit
    if (std::isdigit(label[0])) {
        return false;
    }

    // Check each character in the label
    for (char ch : label) {
        // Allow uppercase letters
        if (std::isupper(ch)) {
            continue;
        }
        // Allow lowercase letters
        if (std::islower(ch)) {
            continue;
        }
        // Allow digits after the first character
        if (std::isdigit(ch)) {
            continue;
        }
        // Allow underscore character
        if (ch == '_') {
            continue;
        }
        // If any other character is encountered, the label is invalid
        return false;
    }

    // If all characters are valid, the label is valid
    return true;
}
//check whether a valid number 
std::string Commonstring(const std::string& text1, const std::string& text2) {
    int m = text1.size();
    int n = text2.size();

    // Create a 2D vector to store the lengths of LCS for substrings
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    // Fill the dp table using bottom-up approach
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (text1[i - 1] == text2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1; // Common character found, increase length of LCS
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]); // Choose the maximum of the two
            }
        }
    }

    // Now, reconstruct the LCS string using dp table
    std::string lcs;
    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (text1[i - 1] == text2[j - 1]) {
            lcs = text1[i - 1] + lcs; // Include the character in LCS
            --i;
            --j;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            --i;
        } else {
            --j;
        }
    }

    return lcs;
}
bool caseInsensitiveStringCompare(const char* str1, const char* str2) {
    while (*str1 && *str2) {
       
        str1++;
        str2++;
    }

    // Check if both strings have reached the end
    return *str1 == *str2;
}
void line_base_err(){
   line_err_10.push_back(1);
    line_err_1.push_back(1);
 line_err_2.push_back(1);
 line_err_3.push_back(1);
 line_err_4.push_back(1);
 line_err_5.push_back(1);
 line_err_6.push_back(1);
 line_err_7.push_back(1);
 line_err_8.push_back(1);
 line_err_9.push_back(1);
 line_err_11.push_back(1);
 line_err_12.push_back(1);
 line_err_10.push_back(2);


}
bool characterAfterWhitespace(string str) {
    bool foundWhitespace = false;

    for (char c : str) {
        if (std::isspace(c)) {
            foundWhitespace = true;
        } else if (foundWhitespace) {
            return true; // Found a character after whitespace
        }
    }

    return false; // No character found after whitespace
}

bool isHexadecimal(string str) {
    if (str.empty()) {
        return false; // Empty string is not a hexadecimal number
    }

    size_t start = 0;

    // Check for optional "0x" or "0X" prefix
    if (str.size() >= 2 && str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
        start += 2;
    }

    // Check for hexadecimal digits
    for (size_t i = start; i < str.size(); ++i) {
        if (!std::isxdigit(str[i])) {
            return false; // Non-hexadecimal character found
        }
    }

    return true; // All characters are hexadecimal
}
bool isDecimal(string str) {
    if (str.empty()) {
        return false; // Empty string is not a decimal number
    }

    size_t start = 0;

    // Check for optional sign
    if (str[start] == '+' || str[start] == '-') {
        ++start;
    }

    // Check for digits
    bool hasDigits = false;
    for (size_t i = start; i < str.size(); ++i) {
        if (!std::isdigit(str[i])) {
            return false; // Non-digit character found
        }
        hasDigits = true;
    }

    return hasDigits; // Return true if at least one digit is found
}
bool isBinary(string str) {
    if (str.empty()) {
        return false; // Empty string is not a binary number
    }

    for (char c : str) {
        if (c != '0' && c != '1') {
            return false; // Non-binary character found
        }
    }

    return true; // All characters are binary
}
void all_check(string s){


   int a = characterAfterWhitespace(s);
    int b = isHexadecimal(s);
    int c =isBinary(s);
    int d = isDecimal(s);
    if(a||b||c||d)dope++;
}
std::string numberToHexString(int number) {
    std::stringstream stream;
    stream << std::hex << number; // Convert the number to hexadecimal format
    return stream.str(); // Return the hexadecimal string
}
string intToHex(int value) {
    // Create a stringstream to hold the hexadecimal value
    stringstream ss;
    // Output the value as a hexadecimal number with 8 digits (32 bits)
    ss << setw(8) << setfill('0') << hex << value;
    // Return the hexadecimal string
    return ss.str();
}
string to24BitHex(int number) {
    stringstream ss;
    
    // Convert the number to hexadecimal with 24-bit width
    ss << setw(6) << setfill('0') << hex << uppercase << (number & 0xFFFFFF);
    
    return ss.str(); // Return the hexadecimal string
}
bool isValidNumber(string str) {
    // Regular expression patterns for integer, binary, and 32-bit hexadecimal numbers
    regex intPattern("^[+-]?\\d+$");
    regex binPattern("^0[bB][01]+$");
    regex hexPattern("^0x[0-9A-Fa-f]{1,8}$");

    // Check if the string matches any of the patterns
    if (regex_match(str, intPattern) || regex_match(str, binPattern) || regex_match(str, hexPattern)) {
        return true;
    }
    return false;
}
// Function to convert a string of integer, binary, or hex to their respective number
int stringToNumber(const std::string& str) {
    // Check if the string starts with "0x" indicating a hexadecimal number
    if (str.substr(0, 2) == "0x") {
        // Create a stringstream to convert the hexadecimal string to integer
        std::stringstream ss;
        ss << std::hex << str; // Set the stringstream to hexadecimal mode
        int num;
        ss >> num; // Extract the integer value
        return num;
    }

    // Check if the string starts with "0b" indicating a binary number
    if (str.substr(0, 2) == "0b") {
        // Convert the binary string to integer using stoi with base 2
        return std::stoi(str.substr(2), nullptr, 2);
    }

    // Otherwise, treat the string as a decimal integer
    return std::stoi(str);
}
//string to intger 
int stringToInt(const std::string& str) {
    try {
        return std::stoi(str);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "Out of range error: " << e.what() << std::endl;
    }

    // Return a default value in case of conversion failure
    return 0;
}
//to check valid ness of instruction and operand pair 
int checkInstructionOperand(string instruction, string operand) {
    // Check if both instruction and operand are empty
    if (instruction.empty() && operand.empty()) {
        return 0; // Both instruction and operand are empty
    }

    // Check if the instruction is present in the Optable
    if (Optable.find(instruction) == Optable.end()) {
        return 1; // Instruction not found in Optable
    }

    // Check if operand contains whitespace between characters
    for (char c : operand) {
        if (isspace(c)) {
            return 2; // Operand contains whitespace between characters
        }
    }

    // Get the type of the instruction from Optable
    int type = Optable[instruction].second;

    // Check if the type is 0 (no operand expected)
    if (type == 0 && operand != "") {
        return 2; // Type and nature of operand match
    }

    // Other cases can be added here as per requirement

    // If the type is not 0, it means an operand is expected
    return 0; // Type and nature of operand match
}
void renum(){
    for(int x  =0 ; x < lines.size(); x++){
        if(lines[x]==""){
            for(int y = 0  ; y< err_warn.size() ;y++){
                if(err_warn[y].first>=x){err_warn[y].first++;}
            }
        }
    }
    
}
void printVector(const std::vector<std::string>& vec) {
    for (const auto& str : vec) {
        std::cout << str << std::endl;
    }
}
uint32_t hex_to_32bit_hex(const std::string& hex_string) {
    // Remove any '0x' prefix if present
    std::string hex = hex_string;
    if (hex.substr(0, 2) == "0x") {
        hex = hex.substr(2);
    }

    // Pad the hexadecimal string with zeros if necessary
    while (hex.length() < 8) {
        hex = "0" + hex;
    }

    // Convert the hexadecimal string to a 32-bit hexadecimal number
    std::stringstream ss;
    ss << std::hex << hex;
    uint32_t hex_number;
    ss >> hex_number;

    return hex_number;
}
void operand_nature(string label , string operand ){
    if(label =="br"||label=="brlz"||label=="brz"||label=="call"){
        if(label_found.find(operand)!=label_found.end()){
            branch_nature.push_back("1");
        }
        else { branch_nature.push_back("0");}
    }
}
std::string toLowercase(const std::string& str) {
    std::string result;
    for (char ch : str) {
        if (std::isalpha(ch)) {
            result += std::tolower(ch);
        } else {
            result += ch;
        }
    }
    return result;
}
void first_pass() { int list =  0 ;
    // Loop through instructions for single error and pushing the label
    for (int x = 0; x < reduced_lines_pass1.size(); x++) {
       
        string label = reduced_lines_pass1[x][0];
        string instruction = reduced_lines_pass1[x][1];
        string operand = reduced_lines_pass1[x][2];
         all_check(operand) ;

        if (reduced_lines_pass1[x][0] != "") {
            if (label_found.find(label) != label_found.end()) {
                err_warn.push_back({x + 1, "Duplicate Label"});
            }
             else {
                if (instruction == "set") {
                    label_found[label] = stringToNumber(operand); list++;
                } else {
                    label_found[label] = list ;
                    if(instruction!=""){list++;}
                }
            }
        }
        else {list++;
        if(instruction=="set"){
            err_warn.push_back({x+1,"set called without any label"});
        }
        }

        if (!isValidLabel(label)) {
            err_warn.push_back({x + 1, "Error: Invalid Label"});
        }

        // Here comes check for operand and instruction
        int t = checkInstructionOperand(instruction, operand);
        if (t == 1) {
            err_warn.push_back({x + 1, "Error: Instruction Not Found"});
        } else if (t == 2) {
            err_warn.push_back({x + 1, "Error: Unexpected amount of operand"});
        }
    }

    for (int x = 0; x < reduced_lines_pass1.size(); x++) {
        string label = reduced_lines_pass1[x][0];
        string instruction = reduced_lines_pass1[x][1];
        string operand = reduced_lines_pass1[x][2];

        if (Optable[instruction].second != 0) {
            // Then check whether given operand exists in label map and is either a valid number or not
            if (label_found.find(operand) != label_found.end()) {
                label_used.insert(operand);
            } else if (!isValidNumber(operand)) {
                err_warn.push_back({x + 1, "Error: Operand is not a number or label is not present"});
            }
        }
    }
    int flag = 0 ; 
    for( int x =0 ; x<reduced_lines_pass1.size(); x++)
    {
        if(reduced_lines_pass1[x][1] == "halt"){
         flag = 1 ; break ; 
        }
        
    }
    if(!flag){warnings.push_back({reduced_lines_pass1.size()+1,"Warning ! no Halt instruction found"});}
    for(auto it = label_found.begin(); it!=label_found.end();it++)
    {
        string l = it->first ;
        if(label_used.find(l) == label_used.end()){
            warnings.push_back({it->second+1,"Warning ! given label is declared but never used :- "+l}) ;  
        }
    }
    
    
    line_base_err();
   //renum();
    sort(err_warn.begin(), err_warn.end());
    sort(warnings.begin(),warnings.end());

}
//all warning and error considered here now listing would be done 
void second_pass()
{   
    // Initialize list counter
    int list = 0 ;

    // Iterate over reduced lines from pass 1
    for(int x = 0; x < reduced_lines_pass1.size(); x++ ){
        // Extract label, instruction, and operand
        string label = reduced_lines_pass1[x][0] ; 
        string instruction = reduced_lines_pass1[x][1];
        string operand = reduced_lines_pass1[x][2];
        
        // Convert list to hexadecimal
        string list_HEX = intToHex(list) ; 

        // Increment list if instruction is not empty
        if(instruction!="")
        {
            list++;
        }

        // Initialize opcode
        string opcode ="        ";

        // Determine operand nature
        operand_nature(instruction, operand); // check whether branch type is offset or label to jump ;

        // Determine opcode based on instruction and operand
        if(Optable[instruction].second != 0){
            if(label_found.find(operand) != label_found.end()){
                if(instruction == "br" || instruction == "brlz" || instruction == "brz" || instruction == "call"){
                    opcode= to24BitHex(label_found[operand]-list) ;
                }
                else { 
                    opcode= to24BitHex(label_found[operand]) ;
                }
            }
            else{
                opcode = to24BitHex(stringToNumber(operand));
            }
        }
        else{
            if(instruction != "")  opcode="000000" ;
        }
        
        // Prepare dil
        string dil = opcode ;
        if(instruction == "data" || instruction == "set"){
            dil += "00" ;
        }
        else {
            dil += Optable[instruction].first ;
        }

        // Append opcode based on instruction
       
        
            opcode += Optable[instruction].first ;
        

        // Prepare label and instruction
        if(label != ""){
            label += ":" ;
        }
        else{
            label = "    ";
        }
        if(instruction == ""){
            instruction = "   ";
        }

        // Generate final line
        string final = list_HEX +" "+ toLowercase(dil) +"  "+ label +" "+ instruction +" "+ operand;
        listing.push_back(final);

        // Add opcode to machine if not empty
        if(!opcode.empty() && opcode != "        "){
            machine.push_back(toLowercase(opcode));
        }
    }
}

void machine_label_address(){
    for(int x = 0 ; x < branch_nature.size(); x++){
        machine.push_back(branch_nature[x]); 
    }
}

signed main (int argc, char *argv[])
{
    // Check if the correct number of command-line arguments is provided
if (argc != 2)
{
    // Print an error message and exit if only one argument is not provided
    cout << "ERROR: PASS ONLY *.asm File ";
    return 0;
}

// Extract the filename from the command-line argument
char *Filename = argv[1];

// Check if the file has the correct extension (.asm)
if (strlen(Filename) <= 3 || strcmp("asm", Filename + strlen(Filename) - 3))
{
    // Print an error message and exit if the file extension is incorrect
    cout << "ERROR: INCORRECT FILE TYPE\n";
    cout << "ERROR: ENTER FILE TYPE OF *.asm ";
    return 0;
}

// Extract the filename without the .asm extension
string FilenameWithoutASM;
for (int i = 0; argv[1][i] != '.'; i++)
{
    FilenameWithoutASM += argv[1][i];
}

// Store the input filename
InputName = argv[1];

// Set up filenames for output files based on the input filename
MachineCodeFileName = FilenameWithoutASM + ".o";
ListingFileName = FilenameWithoutASM + ".l";
LogFileName = FilenameWithoutASM + ".log";
GetFile(InputName);
reduce_push();
first_pass();
//(reduced_lines_pass1);
//printMap(label_found);

//printErrorWarnings(err_warn);
//printErrorWarnings(warnings);
 if (err_warn.size() > 0)
    {

        /* Reading From Errors and priting */
        cout << "-----------------------------Process Failed!---------------------------\n";
        cout << "-------------------------------Errors Found!---------------------------\n";
        sort(err_warn.begin(), err_warn.end());

        ofstream Lgout(LogFileName);

        Lgout << "Failed To Assemble\n";
        Lgout << "ERRORS :- \n";
        for (auto &i : err_warn)
        {
            cout << "Error at Line " << i.first << " : " << i.second << endl;
        }
        for (auto &i : err_warn)
        {
            Lgout << "Error at Line " << i.first << " : " << i.second << endl;
        }

        Lgout.close();
         exit(0);
    }
if (warnings.size() > 0)
    {
        /* Reading From Warning and priting */

        sort(warnings.begin(), warnings.end());

        ofstream Lgout(LogFileName);
        cout << "---------------------------Warnings--------------------------------- \n";
        Lgout << "Warnings -- \n";
        for (auto &i : warnings)
        {
            cout << "Warning at Line " << i.first << " : " << i.second << endl;
        }
        for (auto &i : warnings)
        {
            Lgout << "Warning at Line " << i.first << " : " << i.second << endl;
        }

        Lgout.close();
    }
if(err_warn.size()==0){cout << "No Error :- assembled successfully ! "<< endl ; 
second_pass();

//printVector(listing); 
//printVector(machine);
 ofstream Lout(ListingFileName);
    for (auto i : listing)
    {
        Lout << i << endl;
    }
    Lout.close();

    ofstream Objout;

    Objout.open(MachineCodeFileName, ios::binary | ios::out);
    for (auto &i : machine)
    {
        unsigned int y;
        stringstream ss;
        ss << hex << i;
        ss >> y;
        static_cast<int>(y);
        Objout.write((const char *)&y, sizeof(unsigned));
    }
    Objout.close();


}




    
}

