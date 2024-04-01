// TITLE: emulator.cpp
// AUTHOR: Sahil Agrawal(2101CS69)
// Declaration of Authorship
// This file is part of the assignment of CS209 at the
// department of Computer Science and Engineering, IIT Patna. 

#include <bits/stdc++.h>
using namespace std;
#define endl "\n"

int mainMemory[1 << 24];
int total = 0;
string file_name;
fstream _file;
int limiting_factor = 100000;
int SL = 1 << 23;
int PC = 0, SP = 0, RegisterA = 0, registerB = 0;
vector<int> OF;
vector<string> mnemonics={"ldc", "adc", "ldl", "stl", "ldnl", "stnl", "add", "sub", "shl", "shr", "adj", "a2sp", "sp2a", "call", "return", "brz", "brlz", "br", "HALT"};

string converting_to_hex_again(int value, int len)
{
    stringstream object;
    object << hex << value;
    string r(object.str());
    reverse(r.begin(), r.end());
    while ((int)r.size() > len)
    {
        r.pop_back();
    }
    while ((int)r.size() < len)
    {
        r.push_back('0');
    }
    reverse(r.begin(), r.end());
    return r;
}



void executeOpcode(int opc, int Value)
{
    if(opc==0)
    {
        registerB = RegisterA;
        RegisterA = Value;

    }
    if(opc==1)
    {
        RegisterA += Value;

    }
    if(opc==2)
    {
        registerB = RegisterA;
        RegisterA = mainMemory[SP + Value];

    }
    if(opc==3)
    {
        mainMemory[SP + Value] = RegisterA;
        RegisterA = registerB;

    }
    if(opc==4)
    {
       RegisterA = mainMemory[RegisterA + Value];

    }
   
    if(opc==5)
    {
       mainMemory[RegisterA + Value] = registerB;  
    }
    if(opc==6)
    {
      RegisterA = registerB + RegisterA;  
    }
    if(opc==7)
    {
       RegisterA = registerB - RegisterA; 
    }
     if(opc==8)
    {
       RegisterA = registerB << RegisterA; 
    }
    if(opc==9)
    {
       RegisterA = registerB >> RegisterA;
    }
     if(opc==10)
    {
       SP = SP + Value;
    }
     if(opc==11)
    {SP = RegisterA;
        RegisterA = registerB;
    }
     if(opc==12)
    {
        registerB = RegisterA;
        RegisterA = SP;
    }
     if(opc==13)
    {
       registerB = RegisterA;
        RegisterA = PC;
        PC = Value +PC;
    }
     if(opc==14)
    {
      PC = RegisterA;
        RegisterA = registerB;
    }
     if(opc==15)
    {
       if (RegisterA == 0)
        {
            PC = PC + Value;
        }
    }
     if(opc==16)
    {
      if (RegisterA < 0)
        {
            PC = PC + Value;
        }
    }
     if(opc==17)
    {
       PC = PC + Value;
    }
     if(opc==18)
    {
      printf("A = %08X, B = %08X, PC = %08X, SP = %08X\n", RegisterA, registerB, PC, SP);
    _file << "A = " << converting_to_hex_again(RegisterA, 8) << ", B = " << converting_to_hex_again(registerB, 8) << ", PC = " << converting_to_hex_again(PC, 8) << ", SP = " << converting_to_hex_again(SP, 8) << endl;
                
                 cout << "Number of instructions passed: " << total << endl;
                 exit(0);
    }
    if(opc>18)
    {
       cout << "Invalid opcode.Please input correct machine codes!."; 
        exit(0);
    }
   
    }


int ArgRun()
{
    // if (PC >= (int)OF.size())
    // {
    //     cout << "Segmentation fault!";
    //     exit(0);
    // }

    int opc = 0xFF & OF[PC]; // last 8 bits
    int Value = OF[PC] >> 8;  // first 24 bits

    if (opc == 18) // HALT
    {
        total++;
        return 0;
    }

    cout << mnemonics[opc] << "\t";
    printf("%08X\n", Value);

    executeOpcode(opc, Value);

    printf("A = %08X, B = %08X, PC = %08X, SP = %08X\n", RegisterA, registerB, PC, SP);
    _file << "A = " << converting_to_hex_again(RegisterA, 8) << ", B = " << converting_to_hex_again(registerB, 8) << ", PC = " << converting_to_hex_again(PC, 8) << ", SP = " << converting_to_hex_again(SP, 8) << endl;

    total++;
    PC++;

    if (total > limiting_factor)
    {
        cout << "Infinite loop detected!";
        exit(0);
    }

    if (SP > SL)
    {
        cout << "Stack overflow!";
        exit(0);
    }

    return 1;
}

pair<long long, bool> read_operand(string &operand)
{
    int len = (int)operand.size();

    if (len == 0)
    {
        return {0, 0};
    }

    char *str = (char *)malloc(len * sizeof(char));

    for (int i = 0; i < len; i++)
    {
        str[i] = operand[i];
    }
    for (int i = len; i < strlen(str); i++)
    {
        str[i] = '\0';
    }

    char *end;
    long long num;

    num = strtol(str, &end, 10); // conversion from decimal to decimal
    if (!*end)
    {
        return {num, 1};
    }

    num = strtol(str, &end, 16); // conversion from hexadecimal to decimal
    if (!*end)
    {
        return {num, 1};
    }

    return {-1, 0};
}

int function1()
{
    cout << "Please input in Emulator input: ";
    string hola;
    cin >> hola;

    for (int i = 0; i < hola.size(); i++)
        hola[i] = tolower(hola[i]);

    if (hola == "-t")
    {
        if (ArgRun() == 0)
            cout << "HALTing instructions!.\n";
    }
    else if (hola == "-all")
    {
        if (ArgRun() == 0)
            cout << "HALT instruction!.\n";
        else
        {
            while (ArgRun() == 1)
            {
            }
        }
    }
    else if (hola == "-dump")
    {
        string operand, offset;

    cout << "Base address: ";
    cin >> operand;

    int BA = read_operand(operand).first;

    cout << "No. of values: ";
    cin >> offset;

    auto value = read_operand(offset);

    int i = BA;
    while (i < BA + value.first && i < (1 << 24))
    {
        printf("%08X \t %08x\n", i, mainMemory[i]);
        i++;
    }
    }
    else if (hola == "-exit")
    {
        return 0;
    }
    else
    {
        cout << "Invalid emulator input." << endl;
    }

    return 1;
}



int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Error: Input valid file name" << endl;
        return 0;
    }

    int random;

    string __file = argv[1];

    ifstream file(__file, ios::in | ios::binary);
    while (file.read((char *)&random, sizeof(int)))
    {
        OF.push_back(random);
    }

    for (int i = 0; i < __file.length(); i++)
    {
        if (__file[i] == '.')
            break;
        file_name.push_back(__file[i]);
    }

    _file.open(file_name + ".txt", ios::out);

    int i = 0;
    for (auto x : OF)
    {
        mainMemory[i] = x;
        i++;
    }

    cout<<"Only for one instruction output: -t"<<endl;
    cout<<"All instruction output at once: -all"<<endl;
    cout<<"See memory dump -dump\n";
    cout<<"Exit: -exit\n";

    while (true)
    {
        if (function1() == 1)
        {
            continue;
        }
        break;
    }

    _file.close();

    cout << "Number of instructions passed: " << total << endl;
    return 0;
}