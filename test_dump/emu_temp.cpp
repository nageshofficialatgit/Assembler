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
int PrintSt = 0;
int Memory[N];
vector<pair<int,int>> mem(N);
vector<pair<int,int>> Instruction;
string TraceFileName = "test.trace";
int Max_N = 0;
set<int> type = {6, 7, 8, 9, 11, 12, 14, 18};
map<int, string> OPTABLE = {{0, "ldc"}, {1, "adc"}, {2, "ldl"}, {3, "stl"}, {4, "ldnl"}, {5, "stnl"}, {6, "add"}, {7, "sub"}, {8, "shl"}, {9, "shr"}, {10, "adj"}, {11, "a2sp"}, {12, "sp2a"}, {13, "call"}, {14, "return"}, {15, "brz"}, {16, "brlz"}, {17, "br"}, {18, "halt"}, {19, "data"}, {20, "set"}};

unsigned int opecode(unsigned int n){
    return n % 256 ;
}
unsigned int offset(unsigned int n){
    return n / 256;
}

int32_t signExtend(int32_t offset) {
    // Check if the sign bit (23rd bit after shifting) is set
    if ((offset >> 23) & 1) {
        // If sign bit is set, Run sign extension by setting bits from 24th to 31st
        offset |= (0xFF << 24); // 0xFF is hexadecimal for 255
    }
    return offset;
}

string int_to_hex(int i)
{
    stringstream stream;
    stream << setfill('0') << setw(8)
           << hex << i;
    return stream.str();
}

void Memory_Dump(bool ifBefore) {
    std::cout << "---- Dumping From Memory! -----" << std::endl;
    int temp = Current_PC - 1;
    if (!ifBefore)
        temp = Max_N + 2;

    for (int i = 0; i < temp - 1; i++) {
        if ((i % 4) == 0) {
            std::cout << std::endl << std::setw(8) << std::setfill('0') << std::hex << i << " \t " << std::setw(8) << std::setfill('0') << std::hex << Memory[i] << " ";
            // Trcout << std::endl << int_to_hex(i) << " \t " << int_to_hex(Memory[i]) << " ";
        }
        else if(i%4==3){ std::cout << std::setw(8) << std::setfill('0') << std::hex << Memory[i] << " "<<endl ;

        } else {
            std::cout << std::setw(8) << std::setfill('0') << std::hex << Memory[i] << " ";
            // Trcout << int_to_hex(Memory[i]) << " ";
        }
    }
    std::cout << std::endl << std::endl;
    
    // Trcout << std::endl;
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

void Print_Ins(pair<int,int> n)
{
    
   

    Trcout << OPTABLE[n.first] << " ";
    cout << OPTABLE[n.first] << " ";
    unsigned int offset = signExtend(n.second);
   
    int ofs = (int)offset;
    if (type.find(n.first) != type.end())
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
    
    
    Trcout << "PC=" << int_to_hex(PC) << " \t SP=" << int_to_hex(SP) << " \t A=" << int_to_hex(A) << " \t B=" << int_to_hex(B) << " ";
    printf("PC=%08X \t SP=%08X \t A=%08X \t B=%08X ", PC, SP, A, B);

  
}
void Run()
{
    Cnt++;
    unsigned int opcode =Instruction[PC].first ;
    unsigned int offset = signExtend(Instruction[PC].second);

    if (OPTABLE.find(opcode) == OPTABLE.end())
    {
         cout << "invalid op " << opcode << endl;
        cerr << "Error, Invalid Mnemonic Found !:- \n";
        exit(0);
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
    if(opcode!=19&&opcode!=20){
    switch (opcode) {
        case 0:
            ldc(ofs);
            break;
        case 1:
            adc(ofs);
            break;
        case 2:
            ldl(ofs);
            break;
        case 3:
            stl(ofs);
            break;
        case 4:
            ldnl(ofs);
            break;
        case 5:
            stnl(ofs);
            break;
        case 6:
            add(ofs);
            break;
        case 7:
            sub(ofs);
            break;
        case 8:
            shl(ofs);
            break;
        case 9:
            shr(ofs);
            break;
        case 10:
            adj(ofs);
            break;
        case 11:
            a2sp(ofs);
            break;
        case 12:
            sp2a(ofs);
            break;
        case 13:
            call(ofs);
            break;
        case 14:
            return1(ofs);
            break;
        case 15:
            brz(ofs);
            break;
        case 16:
            brlz(ofs);
            break;
        case 17:
            br(ofs);
            break;
    }
    }
    

    print_state();
  //  cout << endl ;
    Print_Ins(Instruction[PC]);
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
          cout << "Usage: ./emu.exe [option] file.o" << endl;
    cout << "Options:" << endl;
    cout << "\t-trace\tshow instruction trace" << endl;
    cout << "\t-before\tshow memory dump before execution" << endl;
    cout << "\t-after\tshow memory dump after execution" << endl;
    cout << "\t-isa\tdisplay ISA" << endl;
        return (0);
    }

    // reading file

    ifstream infile;
    infile.open(argv[2], ios::in | ios::binary);

    int read;

    while (!infile.eof())
    {
        infile.read((char *)&read, sizeof(int));
        if(opecode(read)==19){Memory[Current_PC]=offset(read);}
        else
        Memory[Current_PC] = read;
        Instruction.push_back({opecode(read),offset(read)});
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
           
            Run();
            
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
            
            Run();
           
        }
        Memory_Dump(false);
        Trcout.close();
    }
    else if ((string)argv[1] == "-isa")
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
        Trcout.close();
    }
    else
    {
        Trcout.close();
        cout << "Invalid Argument!\n";
          cout << "Usage: ./emu.exe [option] file.o" << endl;
    cout << "Options:" << endl;
    cout << "\t-trace\tshow instruction trace" << endl;
    cout << "\t-before\tshow memory dump before execution" << endl;
    cout << "\t-after\tshow memory dump after execution" << endl;
    cout << "\t-isa\tdisplay ISA" << endl;
    }
  
    return 0;
}