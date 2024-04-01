/*

Author@ - PATEL MARTIN KAMALNAYAN
ROLL - 2101CS43

Declaration of Authorship
This cpp file, emu.cpp, is part of the miniproject of CS209/CS210 at the 
department of Computer Science and Engg, IIT Patna . 

*/

#include <bits/stdc++.h>

using namespace std;

// Declaring 32 - bit register
int A, B, SP;
// Program Counter
int PC, Current_PC;
int Cnt{};
const int N = 10000; // MAX MEMORY
ofstream Trcout;
vector<int> Instructions;
int PrintSt = 0;
int Memory[N];
string TraceFileName = "test2.trace ";
int Max_N = 0;
set<int> type = {6, 7, 8, 9, 11, 12, 14, 18};
map<int, string> OPTABLE = {{0, "ldc"}, {1, "adc"}, {2, "ldl"}, {3, "stl"}, {4, "ldnl"}, {5, "stnl"}, {6, "add"}, {7, "sub"}, {8, "shl"}, {9, "shr"}, {10, "adj"}, {11, "a2sp"}, {12, "sp2a"}, {13, "call"}, {14, "return"}, {15, "brz"}, {16, "brlz"}, {17, "br"}, {18, "halt"}, {1000, "data"}, {100, "set"}};

void ISA()
{
    cout << "Opcode Mnemonic Operand\n";
    cout << "       data     value\n";
    cout << "0      ldc      value\n";
    cout << "1      adc      value\n";
    cout << "2      ldl      value\n";
    cout << "3      stl      value\n";
    cout << "4      ldnl     value\n";
    cout << "5      stnl     value\n";
    cout << "6      add\n";
    cout << "7      sub\n";
    cout << "8      shl\n";
    cout << "9      shr\n";
    cout << "10     adj      value\n";
    cout << "11     a2sp\n";
    cout << "12     sp2a\n";
    cout << "13     call     offset\n";
    cout << "14     return\n";
    cout << "15     brz      offset\n";
    cout << "16     brlz     offset\n";
    cout << "17     br       offset\n";
    cout << "18     HALT\n";
    cout << "       SET      value\n";
}
string int_to_hex(int i)
{
    stringstream stream;
    stream << setfill('0') << setw(8)
           << hex << i;
    return stream.str();
}

void prompt()
{
    cout << "Usage: ./emu.exe [option] file.o" << endl;
    cout << "Options:" << endl;
    cout << "\t-trace\tshow instruction trace" << endl;
    cout << "\t-before\tshow memory dump before execution" << endl;
    cout << "\t-after\tshow memory dump after execution" << endl;
    cout << "\t-isa\tdisplay ISA" << endl;
}

void Memory_Dump(bool ifBefore)
{

    // ofstream Trcout(TraceFileName);

    cout << "----Dumping From Memory!-------";
    Trcout << "Dumping From Memory!";
    int temp = Current_PC - 1;
    if (!ifBefore)
        temp = Max_N + 2;
    for (int i = 0; i < temp - 1; i++)
    {

        if ((i % 4) == 0)
        {

            printf("\n%08x \t %08x ", i, Memory[i]);
            Trcout << endl
                   << int_to_hex(i) << " \t " << int_to_hex(Memory[i]) << " ";
        }
        else
        {
            printf("%08x ", Memory[i]);
            Trcout << int_to_hex(Memory[i]) << " ";
        }
    }
    cout << endl
         << endl;
    Trcout << endl;
    // Trcout.close();
}
void ldc(int n)
{
    B = A;
    A = n;
}
void adc(int n)
{
    A = A + n;
}
void ldl(int n)
{
    B = A;
    if (SP + n >= N || SP + n < 0)
    {
        cerr << "ERROR, OUT OF RANGE MEMORY ACCESS\n";
        exit(0);
    }
    A = Memory[SP + n];
    Max_N = max(Max_N, SP + n);
}
void stl(int n)
{
    if (A + n >= N || A + n < 0)
    {
        cerr << "ERROR, OUT OF RANGE MEMORY ACCESS\n";
        exit(0);
    }

    Memory[SP + n] = A;
    Max_N = max(Max_N, SP + n);
    A = B;
}
void ldnl(int n)
{
    if (A + n >= N || A + n < 0)
    {
        cerr << "ERROR, OUT OF RANGE MEMORY ACCESS\n";
        exit(0);
    }
    A = Memory[A + n];
    Max_N = max(Max_N, A + n);
}
void stnl(int n)
{
    if (A + n >= N || A + n < 0)
    {
        cerr << "ERROR, OUT OF RANGE MEMORY ACCESS\n";
        exit(0);
    }
    Memory[A + n] = B;
    Max_N = max(Max_N, A + n);
}
void add(int n)
{
    A = B + A;
}
void sub(int n)
{
    A = B - A;
}
void shl(int n)
{
    A = B << A;
}
void shr(int n)
{
    A = B >> A;
}
void adj(int n)
{

    SP = SP + n;
}
void a2sp(int n)
{
    SP = A;
    A = B;
}
void sp2a(int n)
{
    B = A;
    A = SP;
}
void call(int n)
{
    B = A;
    A = PC;
    PC += n;
    if (n == -1)
    {
        cout << "Infinite Loop Detected!\n";
        exit(0);
    }
}
void return1(int n)
{
    PC = A;
    A = B;

    if (PC == A && A == B)
    {
        cout << "Infinite Loop Detected!\n";
        exit(0);
    }
}
void brz(int n)
{
    if (A == 0)
    {
        if (n == -1)
        {
            cout << "Infinite Loop Detected!\n";
            exit(0);
        }
        PC = PC + n;
    }
}
void brlz(int n)
{
    if (A < 0)
    {
        if (n == -1)
        {
            cout << "Infinite Loop Detected!\n";
            exit(0);
        }
        PC = PC + n;
    }
}
void br(int n)
{
    if (n == -1)
    {
        cout << "Infinite Loop Detected!\n";
        exit(0);
    }
    PC = PC + n;
}
void (*fun_arr[])(int) = {ldc, adc, ldl, stl, ldnl, stnl, add, sub, shl, shr, adj, a2sp, sp2a, call, return1, brz, brlz, br};
void Print_Ins(int n)
{
    unsigned int nn = n;
    // cout << n % 256 << endl;
    // ofstream Trcout(TraceFileName);
    Trcout << OPTABLE[nn % 256] << " ";
    cout << OPTABLE[nn % 256] << " ";
    unsigned int offset = nn / 256;
   

    if ((offset >> 23) & 1)
    {
        offset |= (255 << 24);
    }
    int ofs = (int)offset;
    if (type.find(nn % 256) != type.end())
    {
        Trcout << " " << int_to_hex(ofs);
        printf(" %08x", ofs);
    }
    else
    {
        Trcout << ofs;
        printf("%d", ofs);
    }

    Trcout << "\n\n";
    printf("\n");
    printf("\n");
    // Trcout.close();
}
void print_state()
{
    // ofstream Trcout(TraceFileName);
    Trcout << "PC=" << int_to_hex(PC) << " \t SP=" << int_to_hex(SP) << " \t A=" << int_to_hex(A) << " \t B=" << int_to_hex(B) << " ";
    printf("PC=%08X \t SP=%08X \t A=%08X \t B=%08X ", PC, SP, A, B);

    // cout << Cnt << endl;
    //  Trcout.close();
}
void Perform()
{
    Cnt++;
    unsigned int n = Instructions[PC];
    // cout << "n= " << n % 256 << endl;
    unsigned int opcode = n % 256;
    unsigned int offset = n / 256;

    if (OPTABLE.find(opcode) == OPTABLE.end())
    {
        // cout << "invalid op " << opcode << endl;
        cerr << "Error, Invalid Mnemonic Found !\n";
        exit(0);
    }

  

    if ((offset >> 23) & 1)
    {
        offset |= (255 << 24);
    }
    int ofs = (int)offset;
    if (opcode == 18)
    {
        Trcout << "PC=" << int_to_hex(PC) << " \t SP=" << int_to_hex(SP) << " \t A=" << int_to_hex(A) << " \t B=" << int_to_hex(B) << " ";
        printf("PC=%08X \t SP=%08X \t A=%08X \t B=%08X ", PC, SP, A, B);
        Trcout << "HALT ";
        cout << "HALT ";
        Trcout << "0";
        printf("%d", 0);
        Trcout << "\n\n";
        printf("\n");
        printf("\n");
        cout << Cnt << " number of instructions executed!\n\n";
        PC = Current_PC - 1;
        return;
    }
    (*fun_arr[opcode])(ofs);

    print_state();
    Print_Ins(Instructions[PC]);
    PC++;
    // cout << Cnt << endl;
}

int main(int argc, char *argv[])
{
    // Reseting registers
    A = 0;
    B = 0;
    PC = 0;
    SP = 9999;
    Current_PC = 0;

    if (argc < 3)
    {
        prompt();
        return (0);
    }

    // reading file

    ifstream infile;
    infile.open(argv[2], ios::in | ios::binary);

    int read;

    while (!infile.eof())
    {
        infile.read((char *)&read, sizeof(int));
        Memory[Current_PC] = read;
        Instructions.push_back(read);
        Current_PC++;
    }
    Max_N = max(Current_PC - 1, Max_N);
    string input;
    string word;
    int num = 0;
    int flag = 0;
    string in_file = argv[2];
    TraceFileName = "";
    int itr = 0;
    //cout << in_file << endl;
    while (in_file[itr] != '.')
    {

        TraceFileName += in_file[itr];
        itr++;
    }
    TraceFileName += ".trace";

    Trcout.open(TraceFileName);
    // cout << "Total Code " << Current_PC << endl;
    if ((string)argv[1] == "-trace")
    {

        while (PC < Current_PC - 1)
        {
           
            Perform();
            
        }
       ;
        Trcout.close();
    }
    else if ((string)argv[1] == "-before")
    {

        Memory_Dump(true);
        Trcout.close();
    }
    else if ((string)argv[1] == "-after")
    {
        while (PC < Current_PC - 1)
        {
            
            Perform();
           
        }
        Memory_Dump(false);
        Trcout.close();
    }
    else if ((string)argv[1] == "-isa")
    {
        ISA();
        Trcout.close();
    }
    else
    {
        Trcout.close();
        cout << "Invalid Argument!\n";
        prompt();
    }
    return 0;
}