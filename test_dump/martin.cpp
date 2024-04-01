/*
Author@ - PATEL MARTIN KAMALNAYAN
ROLL - 2101CS43

Declaration of Authorship
This cpp file, asm.cpp, is part of the miniproject of CS209/CS210 at the 
department of Computer Science and Engg, IIT Patna 
*/

#include <bits/stdc++.h>

using namespace std;

typedef vector<string> vs;
typedef vector<pair<int, string>> vpis;

// Structure For a Mnemonic
struct Mnemonic
{

    string opcode; // Opcode in Binary
    int mnemonic_type;
    /*  Type:
        Case 0 - No Operands
        Case 1 - Argument is Given
        Case 2 - Offset is Given
    */
};

// Structure For Labels And Symbols
struct Symbol
{

    int address; // 32- Bit Address

    int symbol_type;

    /*
        Type:
        Case 0: Data or Variable is Declared
        Case 1: Label or Block name is Declared

    */
};
map<string, Symbol> Declared_Label; // For Storing Declared Labels
map<string, Symbol> Used_Label;     // For Storing Used Labels
// Class For Storing OPCODES
class OPTABLE
{

public:
    map<string, Mnemonic> Optable; // Map for storing opcodes
    // Function For Filling OPTABLE
    void Populate_OPMAP()
    {

        // Filling the OPMAP by inserting Mnemonics, OPcode and Type From the Instruction Manual
        // Opcode is 8-bit which is 2- Byte HEXADECIMAL

        // For Type with 1 Argument As Value i.e Type=1

        Optable["adj"] = {"0A", 1};
        Optable["set"] = {"", 1};
        Optable["data"] = {"", 1};
        Optable["ldc"] = {"00", 1};
        Optable["adc"] = {"01", 1};

        // For Type with Offset i.e Type=2
        Optable["ldl"] = {"02", 2};
        Optable["stl"] = {"03", 2};
        Optable["ldnl"] = {"04", 2};
        Optable["stnl"] = {"05", 2};
        Optable["call"] = {"0D", 2};
        Optable["brz"] = {"0F", 2};
        Optable["brlz"] = {"10", 2};
        Optable["br"] = {"11", 2};

        // For Type with No Operand i.e Type=0

        Optable["shr"] = {"09", 0};
        Optable["a2sp"] = {"0B", 0};
        Optable["sp2a"] = {"0C", 0};
        Optable["return"] = {"0E", 0};
        Optable["halt"] = {"12", 0};
        Optable["add"] = {"06", 0};
        Optable["sub"] = {"07", 0};
        Optable["shl"] = {"08", 0};
    }
};

OPTABLE OPtable;
// Structure for extracting Label Instruction Operand

struct LIO
{

    string Label, Instruction, Operand;

    LIO(string &LABEL, string &INSTRUCTION, string &OPERAND)
    {

        Label = LABEL;
        Instruction = INSTRUCTION;
        Operand = OPERAND;
    }
};

vector<pair<int, LIO>> Data_Segment; // Pair of 32- Bit addres and Label, Instruction, Operand For Data Segment
// Declaring Variables For All required Data Variables

int pc = 0; // Program Counter

bool Halt_Declared = false; // Flag to check if Halt Function is called

// Declaring The ouput Files Data Types

vs MachineCode; // Final Machine Code
vs ListingCode; // Final Listing Code

// Warnings and Errors

vector<pair<int, string>> Warnings;
vpis Errors;

// Clean Output for 1st- Pass

vector<pair<int, LIO>> Clean_Instructions; // 32-Bit address with Label,Instruction,Operand for 1st pass without comments and spaces

// input For 2nd- Pass

vector<pair<int, LIO>> Input_Instructions; // 32-Bit address with Label,Instruction,Operand for 2nd Pass after error Checking

// Input Filenmae,LogFile,MachineCode,ListingCode

string InputName;
string MachineCodeFileName;
string ListingFileName;
string LogFileName;

set<string> Declared_Data; // Labels with Data
set<string> Set_Data;      // Labels with Set

class GetLIO
{

public:
    void getLIO(string &LabelName, string &Instruction, string &Operand, int &LineNumber, string FormatedInput)
    {

        int itr = 0;

        // Checking If a label is present

        for (; itr < FormatedInput.size(); itr++)
        {

            if (FormatedInput[itr] == ':')
            {

                for (int i = 0; i < itr; i++)
                {
                    LabelName += FormatedInput[i];
                }
                itr++;
                break;
            }
        }
        if (FormatedInput[itr - 1] != ':')
            itr = 0;

        while ((FormatedInput[itr] == ' ' || FormatedInput[itr] == '\t'))
        {

            if (itr == FormatedInput.size())
                break;
            itr++;
        }

        while (!(FormatedInput[itr] == ' ' || FormatedInput[itr] == '\t'))
        {

            if (itr == FormatedInput.size())
                break;
            Instruction += FormatedInput[itr];
            itr++;
        }

        while ((FormatedInput[itr] == ' ' || FormatedInput[itr] == '\t'))
        {

            if (itr == FormatedInput.size())
                break;
            itr++;
        }

        while (!(FormatedInput[itr] == ' ' || FormatedInput[itr] == '\t'))
        {

            if (itr == FormatedInput.size())
                break;
            Operand += FormatedInput[itr];

            itr++;
        }

        // Errors :

        while ((FormatedInput[itr] == ' ' || FormatedInput[itr] == '\t'))
        {

            if (itr == FormatedInput.size())
                break;
            itr++;
        }
        if (itr != FormatedInput.size())
        {
            string error = "There is a Extra Operand! ";
            Errors.push_back({LineNumber, error});
        }
    }
};
GetLIO GEtLIO;
class TypeCheck
{
public:
    bool ifNumber(char c)
    {
        bool ans = 1;
        bool temp = (c <= '9' && c >= '0');
        ans &= temp;

        return ans;
    }

    bool ifChar(char c)
    {
        bool ans = 1;

        bool temp = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
        ans &= temp;

        return ans;
    }

    bool ifHexaDecimal(string s)
    {
        return s.compare(0, 2, "0x") == 0 && s.size() > 2 && s.find_first_not_of("0123456789abcdefABCDEF", 2) == std::string::npos;
    }

    // Function to check if Octal
    bool ifOctal(string s)
    {

        return s.compare(0, 1, "0") == 0 && s.size() > 1 && s.find_first_not_of("01234567", 1) == std::string::npos;
    }

    int Hex_2_Dec(string hexVal)
    {

        int len = hexVal.size();

        // Initializing base value to 1, i.e 16^0
        int base = 1;

        int dec_val = 0;

        // Extracting characters as digits from last
        // character
        for (int i = len - 1; i >= 2; i--)
        {
            // if character lies in '0'-'9', converting
            // it to integral 0-9 by subtracting 48 from
            // ASCII value
            if (hexVal[i] >= '0' && hexVal[i] <= '9')
            {
                dec_val += (int(hexVal[i]) - 48) * base;

                // incrementing base by power
                base = base * 16;
            }

            // if character lies in 'A'-'F' , converting
            // it to integral 10 - 15 by subtracting 55
            // from ASCII value
            else if (hexVal[i] >= 'A' && hexVal[i] <= 'F')
            {
                dec_val += (int(hexVal[i]) - 87) * base;

                // incrementing base by power
                base = base * 16;
            }
        }
        return dec_val;
    }
    // Function to convert Octal to Decimal
    int Oct_2_Dec(string hexVal)
    {
        int len = hexVal.size();

        // Initializing base value to 1, i.e 16^0
        int base = 1;

        int dec_val = 0;

        // Extracting characters as digits from last
        // character
        for (int i = len - 1; i >= 1; i--)
        {
            // if character lies in '0'-'9', converting
            // it to integral 0-9 by subtracting 48 from
            // ASCII value
            if (hexVal[i] >= '0' && hexVal[i] <= '7')
            {
                dec_val += (int(hexVal[i]) - 48) * base;

                // incrementing base by power
                base = base * 8;
            }
        }
        return dec_val;
    }
    // Function To check If its a valid Label Name
    bool ifLabelName(string &Label_Name)
    {
        // If label Name is empty , then no a valid name
        if (Label_Name.empty())
            return false;
        bool ans = 1;
        // First should be Only Char
        ans &= (ifChar(Label_Name[0]));

        for (int i = 1; i < Label_Name.size(); i++)
        {

            bool temp = ifChar(Label_Name[i]);
            temp |= ifNumber(Label_Name[i]);
            temp |= Label_Name[i] == '_';
            ans &= temp;
        }
        return ans;
    }
};
string OperandProcess(string s, int *a, int LineNum)
{
    if (TypeCheck().ifLabelName(s))
    {
        Used_Label[s] = {LineNum, 5};
        return s;
    }

    int sign = 1;

    if (s[0] == '-')
        sign *= -1;

    bool ans = (s[0] == '+' || s[0] == '-');

    if (ans)
        s = s.substr(1);

    if (TypeCheck().ifOctal(s))
        return to_string(sign * (TypeCheck().Oct_2_Dec(s)));

    if (TypeCheck().ifHexaDecimal(s))
        return to_string(sign * (TypeCheck().Hex_2_Dec(s)));

    bool ok = 1;

    for (int i = 0; i < s.size(); i++)
    {
        ok &= TypeCheck().ifNumber(s[i]);
    }

    if (ok)
        return to_string(sign * stoi(s));

    *a = 1;
    return s;
}

// Function to Generate Hexadecimal Code
string HexGen(int num, bool isAddress)
{
    map<int, char> m;

    char digit = '0';
    char c = 'A';

    for (int i = 0; i <= 15; i++)
    {
        if (i < 10)
        {
            m[i] = digit++;
        }
        else
        {
            m[i] = c++;
        }
    }

    // string to be returned
    string res = "";

    // check if num is 0 and directly return "0"
    if (!num)
    {
        res = "0";
    }
    // if num>0, use normal technique as
    // discussed in other post
    if (num > 0)
    {
        while (num)
        {
            res = m[num % 16] + res;
            num /= 16;
        }
    }
    // if num<0, we need to use the elaborated
    // trick above, lets see this
    else
    {
        // store num in a u_int, size of u_it is greater,
        // it will be positive since msb is 0
        unsigned int n = num;

        // use the same remainder technique.
        while (n)
        {
            res = m[n % 16] + res;
            n /= 16;
        }
    }

    int temp = res.size();
    for (int itr = 1; itr <= 8 - temp; itr++)
    {

        res = '0' + res;
    }

    if (isAddress)
    {

        return res.substr(2);
    }

    return res;
}
// Function To remove comments and trailing Spaces , Needed before 1st Pass
string CleanRawInput(string s)
{
    string Processed_String;
    int itr = 0;

    // Clearing Starting Spaces
    while (s[itr] == ' ')
    {
        if (itr == s.size())
            break;
        itr++;
    }

    int i = itr;
    /*Instruction Till Comment ';'*/

    for (; i < s.size(); i++)
    {
        char temp = s[i];

        if (temp == ';')
            break;
        /* Comment Case*/

        /* all lower Alphabets*/
        bool ans = 1;
        bool temp2 = (temp >= 'A' && temp <= 'Z');
        ans &= temp2;
        if (ans)
            temp = tolower(temp);

        Processed_String += temp;
    }

    return Processed_String;
}
// Function to Fill Label Names declared By Data And Set
void DeclaredBySet()
{
    vector<pair<int, LIO>> tempInstruction;

    for (auto &Line : Clean_Instructions)
    {
        bool temp = (Line.second.Instruction == "set");

        /* Checking For Set Label Name */

        if (temp)
        {
            Set_Data.insert(Line.second.Label);
        }
    }

    /* Now we will go through  Names Declared in Data */

    for (auto &Line : Data_Segment)
    {
        Declared_Data.insert(Line.second.Label);
    }
    /* For More than one Declaration Of same Label  */
    for (auto &Line : Clean_Instructions)
    {
        bool temp = (Line.second.Instruction == "set");
        bool inData = false;

        if (temp)
        {

            if (Declared_Data.count(Line.second.Label) == 0)
            {
                Line.second.Instruction = "data";
                Declared_Data.insert(Line.second.Label);
                inData = true;
                Data_Segment.push_back(Line);
            }
        }
        if (!inData)
        {
            /* Store in Temp Instruction */
            tempInstruction.push_back(Line);
        }
    }
    /* Since Tempory Instruction Stored we can clear The instructions for FInal Instructions */
    bool temp2 = 1;
    Clean_Instructions.clear();

    /* Reconstruction Instruction with data at the end of instruction */
    for (auto &Line : tempInstruction)
    {
        Clean_Instructions.push_back(Line);
    }
    temp2 = 0;
    for (auto &Line : Data_Segment)
    {
        Clean_Instructions.push_back(Line);
    }

    // Clearing Extra Memory
    tempInstruction.clear();
    temp2 = 1;
    Data_Segment.clear();

    // Final Cleaned Instructions for Pass 1 are ready
}

bool setInstruction(string &LabelName, string &val, int LineNum, int to_add)
{

    /* Checking For Errors*/

    bool temp = LabelName.empty();
    if (temp)
    {
        string error = "No Label Name Given - Set Instruction! ";
        Errors.push_back({LineNum, error});
        return false;
    }
    bool temp2 = val.empty();
    if (temp2)
    {
        string error = "No Operand Given - Set Instruction! ";
        Errors.push_back({LineNum, error});
        return false;
    }
    int check = 0;
    /* Convertin Operand into int*/

    string nop = OperandProcess(val, &check, LineNum);

    temp = TypeCheck().ifChar(val[0]);
    temp |= (check == 1);
    if (temp)
    {
        string error = "Unexpected Number - Set Instruction! ";
        Errors.push_back({LineNum, error});
        return false;
    }
    temp = Declared_Data.count(LabelName);
    temp &= Declared_Label.count(LabelName);
    string ins;

    if (temp)
    {
        return true;
    }
    else
    {
        string op;
        string label;

        /* Increase Stack Size*/

        ins = "adj";
        op = "2";
        nop = OperandProcess(op, &check, LineNum);
        Input_Instructions.push_back({pc, LIO(label, ins, nop)});
        pc++;
        /* PUSH A*/

        ins = "stl";
        op = "-1";
        nop = OperandProcess(op, &check, LineNum);
        Input_Instructions.push_back({pc, LIO(label, ins, nop)});
        pc++;
        /* PUSH B */

        ins = "stl";
        op = "0";
        nop = OperandProcess(op, &check, LineNum);
        Input_Instructions.push_back({pc, LIO(label, ins, nop)});
        pc++;
        /* Load Value to Set */

        ins = "ldc";
        op = val;
        nop = OperandProcess(op, &check, LineNum);
        Input_Instructions.push_back({pc, LIO(label, ins, nop)});
        pc++;
        /* Load Location Of Pointer */

        ins = "ldc";
        op = LabelName;
        nop = OperandProcess(op, &check, LineNum);
        Input_Instructions.push_back({pc, LIO(label, ins, nop)});
        pc++;
        /* Set Value in Array */

        ins = "adc";
        op = to_string(to_add);
        nop = OperandProcess(op, &check, LineNum);
        Input_Instructions.push_back({pc, LIO(label, ins, nop)});
        pc++;
        /* Store value To Set */

        ins = "stnl";
        op = "0";
        nop = OperandProcess(op, &check, LineNum);
        Input_Instructions.push_back({pc, LIO(label, ins, nop)});
        pc++;
        /*  Load Back B */

        ins = "ldl";
        op = "0";
        nop = OperandProcess(op, &check, LineNum);
        Input_Instructions.push_back({pc, LIO(label, ins, nop)});
        pc++;

        /* Load Back A */

        ins = "ldl";
        op = "0";
        nop = OperandProcess(op, &check, LineNum);
        Input_Instructions.push_back({pc, LIO(label, ins, nop)});
        pc++;

        /* Decrease Stack Size */

        ins = "adj";
        op = "-2";
        nop = OperandProcess(op, &check, LineNum);
        Input_Instructions.push_back({pc, LIO(label, ins, nop)});
        pc++;
    }
    return false;
}
// Function To Get Code from File
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

        string LabelName;
        string Instruction;
        string Operand;

        /* Getting The Label Name ,Instruction ,Operand with Line and also checking For Errors */
        GEtLIO.getLIO(LabelName, Instruction, Operand, LineNumber, FormatedInput);

        temp = !Instruction.empty();
        temp &= Instruction == "data";

        /* Checking For Data Instruction */

        if (temp)
        {
            /* Storing Only Label name in Data Segment, Required For Branch Operation */

            bool temp2 = Clean_Instructions.size() > 0;
            if (temp2)
            {

                auto lastInstruction = Clean_Instructions.back();

                bool temp3 = LabelName.empty();
                temp3 &= lastInstruction.second.Instruction.empty();

                /*checking if last line of code just had Label Name */
                if (temp3)
                {
                    /* Label Name Should be stored In data for Branch operation, Needed in 2nd Pass*/

                    Data_Segment.push_back(lastInstruction);
                    Clean_Instructions.pop_back();
                }
            }
            /* Storing Current Line in data */
            Data_Segment.push_back({LineNumber, LIO{LabelName, Instruction, Operand}});
        }
        else
        {

            Clean_Instructions.push_back({LineNumber, LIO{LabelName, Instruction, Operand}});
        }
    }
}

/* -------------------------ASSEMBLER PASS NO 1 ----------------------------------*/

void Assembler_Pass_1()
{
    /* First We Have To GEt Input File */

    GetFile(InputName);
    /* Checking if declared by set or data */

    DeclaredBySet();

    string prev;
    int count = 0;
    /* Iterating through cleaned pass 1 instruction */

    for (auto &Line : Clean_Instructions)
    {
        // cout << Declared_Label.size() << " hi\n";
        int LineNum = Line.first;
        string Operand = Line.second.Operand;
        string LabelName = Line.second.Label;
        string Instruction = Line.second.Instruction;

        bool toDeclare = false;
        bool temp1 = LabelName.empty();
        if (!temp1)
        {
            /* Checking For A Valid Label Name */

            bool check = TypeCheck().ifLabelName(LabelName);
            if (!check)
            {
                string error = "Incorrect Label Name!";
                Errors.push_back({LineNum, error});
            }
            else
            {
                /* Declare the Label Name */

                count = 0;
                toDeclare = 1;
                prev = LabelName;
            }
        }
        else
        {
            /* Already in label name or No Label Name */

            count++;
        }

        bool needOperand = 0;
        bool flag = 0;
        int use{};
        int minus = 1;
        int toIncrease = 0;

        // Checking For Instruction

        if (!Instruction.empty())
        {

            /* Cheking for valid Instruction name */
            bool check = (OPtable.Optable.count(Instruction) > 0);
            if (!check)
            {
                // cout << Instruction << endl;
                string error = "Invalid Mnemonic! ";
                Errors.push_back({LineNum, error});
            }
            else
            {
                /* Need Operand for Mnemic Type>0 */
                bool temp2 = OPtable.Optable[Instruction].mnemonic_type > 0;
                needOperand |= temp2;

                /* IF instruction is set */

                bool temp3 = (Instruction == "set");
                if (temp3)
                {
                    flag = setInstruction(prev, Operand, LineNum, count);
                }
                if (flag)
                {
                    Instruction = "data";
                }
                toIncrease++;

                if (toDeclare && Instruction == "data")
                {
                    use = 1;
                }
            }
        }
        bool temp4 = (Instruction == "set");
        if (!flag && temp4)
        {
            continue;
        }
        if (toDeclare)
        {
            // cout << LineNum << " declare check" << endl;
            bool temp5 = Declared_Label.count(LabelName) > 0;
            if (temp5)
            {
                string error = "Duplicate Label Found - \"" + LabelName + "\"";
                Errors.push_back({LineNum, error});
            }
            
            else
            { if(Instruction == "set") 
                     { Declared_Label[Line.second.Label] = {stoi(Line.second.Operand),1}; }
               else { Declared_Label[LabelName] = {pc, use};}
            }
        }
        string nop;
        bool temp5 = !Operand.empty();
        if (temp5)
        {
            bool temp6 = !needOperand;
            if (temp6)
            {
                string error = "Unexpected Operand Found ! ";
                Errors.push_back({LineNum, error});
            }
            else
            {
                /* Checking iF it is Label Name or Number */
                int check = 0;
                nop = OperandProcess(Operand, &check, LineNum);
                if (check == 1)
                {

                    string error = "Valid Number or label Name not Found! ";
                    Errors.push_back({LineNum, error});
                }
            }
        }
        else
        {
            bool temp6 = needOperand;
            if (temp6)
            {
                string error = "Operand Missing! ";
                Errors.push_back({LineNum, error});
            }
        }
        Input_Instructions.push_back({pc, LIO(LabelName, Instruction, nop)});
        pc += toIncrease;
    }
    for (auto it : Used_Label)
    {
        string Labelname = it.first;

        int addr = it.second.address;
        bool check1 = Declared_Label.count(Labelname) == 0;
        if (check1)
        {
            string error = "No  Label as \"" + Labelname + "\" has been Declared! ";
            Errors.push_back({addr, error});
        }
    }
    for (auto it : Declared_Label)
    {
        string Labelname = it.first;
        bool check1 = Used_Label.count(Labelname) == 0;
        if (check1)
        {
            string error = "Label with Name \"" + Labelname + "\" not used but Declared! ";
            Warnings.push_back({it.second.address, error});
        }
    }
}
// Pass 2
void Assembler_Pass_2()
{
    for (auto &Line : Input_Instructions)
    {
        bool isDataValue = false;
        string Instruction = Line.second.Instruction;
        int OperandValue;
        string InstructionValue;
        string Operand = Line.second.Operand;
        bool needOffset = false;
        string LabelName = Line.second.Label;
        int Location = Line.first;

        bool temp1 = !Instruction.empty();
        if (temp1)
        {

            bool checkt = OPtable.Optable[Instruction].mnemonic_type == 2;
            InstructionValue = OPtable.Optable[Instruction].opcode;

            /* Special Case For Data and Halt */

            if (Instruction == "halt")
                Halt_Declared = true;

            if (checkt)
            {
                needOffset = 1;
            }

            if (Instruction == "data")
                isDataValue = true;
        }

        bool temp2 = !Operand.empty();
        string Final;
        if (temp2)
        {
            bool temp3 = Declared_Label.count(Operand) == 0;
            if (temp3)
            {
                /* Convert The operand to Int*/
                // cout << Operand << endl;
                OperandValue = stoi(Operand);
            }
            else
            {
                OperandValue = Declared_Label[Operand].address;
            }
            bool temp4 = needOffset;
            temp4 &= Declared_Label.count(Operand) != 0;
            if (temp4)
            {
                OperandValue -= Location + 1;

                // Take care when operand should be 0 but it comes to -1
            }
            bool temp5 = isDataValue;
            if (temp5)
            {
                /* Need to Convert to 32-Bit Address */
                string tempk = HexGen(OperandValue, false);
                Final = tempk;
            }
            else
            {
                /* Convert to 24 Bit Address i.e 6 place hexadecimal */
                string tempk = HexGen(OperandValue, true);
                tempk += InstructionValue;
                Final = tempk;
            }
        }
        else
        {
            string tempk = "000000";
            Final = tempk + InstructionValue;
        }

        string ProgramCounter = HexGen(Location, false);

        string Mac_Code;

        string Lis_Code = ProgramCounter;
        bool temp6 = !Instruction.empty();
        if (temp6)
        {
            Mac_Code += Final;
            Lis_Code += " " + Final;
        }
        else
        {
            Lis_Code += "         ";
        }
        bool temp7 = !LabelName.empty();
        if (temp7)
        {
            Lis_Code += " " + LabelName + ":";
        }
        bool temp8 = !Instruction.empty();
        if (temp8)
        {
            Lis_Code += " " + Instruction;
        }
        bool temp9 = !Operand.empty();
        if (temp9)
        {
            Lis_Code += " " + Operand;
        }

        if (!Mac_Code.empty())
            MachineCode.push_back(Mac_Code);

        if (!Lis_Code.empty())
            ListingCode.push_back(Lis_Code);
    }
    if (!Halt_Declared)
    {

        string error = "HALT Instruction Not Found! ";
        Warnings.push_back({-1, error});
    }
}
void printVector(const std::vector<std::string>& vec) {
    for (const auto& str : vec) {
        std::cout << str << std::endl;
    }
}
// Function To write Listing File

int main(int argc, char *argv[])
{

    /* Checking If only *.asm is passed */

    if (argc != 2)
    {

        cout << "ERROR: PASS ONLY *.asm File ";
        return 0;
    }

    char *Filename = argv[1];

    // Checking if File is of .asm extension
    if (strlen(Filename) <= 3 or strcmp("asm", Filename + strlen(Filename) - 3))
    {
        cout << "ERROR: INCORRECT FILE TYPE\n";
        cout << "ERROR: ENTER FILE TYPE OF *.asm ";
        return 0;
    }

    string FilenameWithoutASM;

    for (int i = 0; argv[1][i] != '.'; i++)
    {
        FilenameWithoutASM += argv[1][i];
    }

    InputName = argv[1];

    MachineCodeFileName = FilenameWithoutASM + ".o";
    ListingFileName = FilenameWithoutASM + ".l";
    LogFileName = FilenameWithoutASM + ".log";

    // Populating OPTABLE
    OPtable.Populate_OPMAP();

    // cout << OPtable.Optable.size();

    // Calling Of Functions For Passes And Warnings , Errors Generation
    // cout << OPtable.Optable.count("halt");
    // cout << OPtable.Optable.count("data");
    Assembler_Pass_1();

    if (Errors.size() > 0)
    {

        /* Reading From Errors and priting */
        cout << "-----------------------------Process Failed!---------------------------\n";
        cout << "-------------------------------Errors Found!---------------------------\n";
        sort(Errors.begin(), Errors.end());

        ofstream Lgout(LogFileName);

        Lgout << "Failed To Assemble\n";
        Lgout << "ERRORS :- \n";
        for (auto &i : Errors)
        {
            cout << "Error at Line " << i.first << " : " << i.second << endl;
        }
        for (auto &i : Errors)
        {
            Lgout << "Error at Line " << i.first << " : " << i.second << endl;
        }

        Lgout.close();
         exit(0);
    }

    Assembler_Pass_2();
     for(auto it : Declared_Label){
        cout << it.first << " "<< it.second.address<< endl ; 
       }
    if (Warnings.size() > 0)
    {
        /* Reading From Warning and priting */

        sort(Warnings.begin(), Warnings.end());

        ofstream Lgout(LogFileName);
        cout << "---------------------------Warnings--------------------------------- \n";
        Lgout << "Warnings -- \n";
        for (auto &i : Warnings)
        {
            cout << "Warning at Line " << i.first << " : " << i.second << endl;
        }
        for (auto &i : Warnings)
        {
            Lgout << "Warning at Line " << i.first << " : " << i.second << endl;
        }

        Lgout.close();
    }
printVector(MachineCode);
    ofstream Lout(ListingFileName);
    for (auto i : ListingCode)
    {
        Lout << i << endl;
    }
    Lout.close();

    ofstream Objout;

    Objout.open(MachineCodeFileName, ios::binary | ios::out);
    for (auto &i : MachineCode)
    {
        unsigned int y;
        stringstream ss;
        ss << hex << i;
        ss >> y;
        static_cast<int>(y);
        Objout.write((const char *)&y, sizeof(unsigned));
    }
    Objout.close();
    // WrtieListing(ListingFileName);
    // WriteObj(MachineCodeFileName);

    return 0;
}