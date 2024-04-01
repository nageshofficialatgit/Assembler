/*

Author@ - NAGESH DESAI
ROLL - 2201CS50

Declaration of Authorship
This cpp file, emu.cpp, is part of the miniproject of CS209/CS210 at the 
department of Computer Science and Engg, IIT Patna . 

*/

#include <bits/stdc++.h>
#include <iomanip>
using namespace std;

// Declaring 32 - bit register
int A, B, SP;
// Program Counter
int PROGRAM_COUNTER  , Current_PC;
int Cnt{};
const int N = 10000; // MAX MEMORY
ofstream Trcout;
int PrintSt = 0;
int Memory[N];
vector<pair<int,int>> mem(N);
vector<pair<int,int>> Instruction;
string TraceFileName = "";
int total = 0;

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
    const size_t width = 8; // Assuming 32-bit integer

    std::string hexStr;
    hexStr.reserve(width);

    for (size_t j = 0; j < width; ++j) {
        int nibble = (i >> ((width - j - 1) * 4)) & 0xF;
        char hexChar = nibble < 10 ? '0' + nibble : 'A' + (nibble - 10);
        hexStr.push_back(hexChar);
    }

    return hexStr;
}

void Memory_Dump(bool ifBefore) {
    std::cout << "---- Memory Dump -----" << std::endl;
    int temp = Current_PC - 1;
    if (!ifBefore)
        temp = total + 2;

    std::cout << "+--------+--------+" << std::endl;
    std::cout << "| Address|  Value |" << std::endl;
    std::cout << "+--------+--------+" << std::endl;

    for (int i = 0; i < temp - 1; i++) {
        if ((i % 4) == 0) {
            std::cout << "| " << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << i << " | ";
            std::cout << std::setw(8) << std::hex << std::uppercase << std::setfill('0') << Memory[i] << " | ";
        }
        else if (i % 4 == 3) {
            std::cout << std::setw(8) << std::hex << std::uppercase << std::setfill('0') << Memory[i] << " |" << std::endl;
            cout <<endl ; 
        } 
        else {
            std::cout << std::setw(8) << std::hex << std::uppercase << std::setfill('0') << Memory[i] << " | ";
        }
    }
      cout << endl ; 
    std::cout << "--------+--------+--------+--------+--------+--------+--------+" << std::endl << std::endl;
}



void Print_Ins(pair<int,int> n)
{
    
   

    Trcout << OPTABLE[n.first] << " ";
    cout << OPTABLE[n.first] << " ";
    unsigned int offset = signExtend(n.second);
   int f = n.first ; 
    int ofs = (int)offset;
    if (f==6||f==7||f==8||f==9||f==11||f==12||f==14||f==18)
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
    
    
    Trcout << "PC=" << int_to_hex(PROGRAM_COUNTER) << " \t SP=" << int_to_hex(SP) << " \t A=" << int_to_hex(A) << " \t B=" << int_to_hex(B) << " ";
    printf("PC=%08X \t SP=%08X \t A=%08X \t B=%08X ",PROGRAM_COUNTER  , SP, A, B);

  
}
void Run()
{
    Cnt++;
    unsigned int opcode =Instruction[PROGRAM_COUNTER].first ;
    unsigned int offset = signExtend(Instruction[PROGRAM_COUNTER].second);
     cout << opcode << " "<< offset << endl ; 
    if (OPTABLE.find(opcode) == OPTABLE.end())
    {
        // cout << "invalid op " << opcode << endl;
        cerr << "Error, Invalid Mnemonic Found !:- "<< endl;
        exit(0);
    }

  

   
    int n = (int)offset;
    if (opcode == 18)
    {
        Trcout << "PC=" << int_to_hex(PROGRAM_COUNTER) << " \t SP=" << int_to_hex(SP) << " \t A=" << int_to_hex(A) << " \t B=" << int_to_hex(B) << " ";
        printf("PC=%08X \t SP=%08X \t A=%08X \t B=%08X ",PROGRAM_COUNTER  , SP, A, B);
        Trcout << "HALT ";
        cout << "HALT ";
        Trcout << "0";
        printf("%d", 0);
        Trcout << "\n\n";
        printf("\n");
        printf("\n");
        cout << Cnt << " number of instructions executed!\n\n";
       PROGRAM_COUNTER   = Current_PC - 1;
        return;
    }
    if(opcode!=19&&opcode!=20){
    switch (opcode) {
        case 0:
            B = A;
            A = n;
            break;
        case 1:
            A = A + n;
            break;
        case 2:
            B = A;
    if (SP + n >= N || SP + n < 0)
    {
        cerr << "ERROR, SEGMENTATION FAULT OUT OF REACH MEMORY ACCESSED \n";
        exit(0);
    }
    A = Memory[SP + n];
    total = max(total, SP + n);
            break;
        case 3:
            if (A + n >= N || A + n < 0)
    {
        cerr << "ERROR, SEGMENTATION FAULT OUT OF REACH MEMORY ACCESSED\n";
        exit(0);
    }

    Memory[SP + n] = A;
    total = max(total, SP + n);
    A = B;
            break;


        case 4:
           if (A + n >= N || A + n < 0)
    {
        cerr << "ERROR, SEGMENTATION FAULT OUT OF REACH MEMORY ACCESSED\n";
        exit(0);
    }
    A = Memory[A + n];
    total = max(total, A + n);
            break;



        case 5:
           
    if (A + n >= N || A + n < 0)
    {
        cerr << "ERROR, SEGMENTATION FAULT OUT OF REACH MEMORY ACCESSED\n";
        exit(0);
    }
    Memory[A + n] = B;
    total = max(total, A + n);
            break;
        case 6:
             A = B + A;
            break;
        case 7:
               A = B - A;
            break;
        case 8:
         
    A = B << A;
            break;
        case 9:
           
    A = B >> A;
            break;
        case 10:
         SP = SP + n;
            break;
        case 11:
            SP = A;
    A = B;
            break;
        case 12:
        
    B = A;
    A = SP;
            break;
        case 13:
           
    B = A;
    A = PROGRAM_COUNTER  ;
    PROGRAM_COUNTER   += n;
    if (n == -1)
    {
        cout << "INIFINITE LOOP RUNNING !\n";
        exit(0);
    }
            break;
        case 14:
         
    PROGRAM_COUNTER   = A;
    A = B;

    if (PROGRAM_COUNTER == A && A == B)
    {
        cout << "INIFINITE LOOP RUNNING! \n";
        exit(0);
    }
            break;
        case 15:
          if (A == 0)
    {
        if (n == -1)
        {
            cout <<  "INIFINITE LOOP RUNNING!\n";
            exit(0);
        }
        PROGRAM_COUNTER   = PROGRAM_COUNTER   + n;
    }
            break;
        case 16:
            if (A < 0)
    {
        if (n == -1)
        {
            cout << "INIFINITE LOOP RUNNING!\n";
            exit(0);
        }
        PROGRAM_COUNTER   = PROGRAM_COUNTER   + n;
    }
            break;
        case 17:
        
    if (n == -1)
    {
        cout << "INIFINITE LOOP RUNNING!\n";
        exit(0);
    }
    PROGRAM_COUNTER   = PROGRAM_COUNTER   + n;
            break;
    }
    }
    

    print_state();
  //  cout << endl ;
    Print_Ins(Instruction[PROGRAM_COUNTER]);
    PROGRAM_COUNTER  ++;
    // cout << Cnt << endl;
}



void ReadFile(const char* filename) {
    std::ifstream infile;
    infile.open(filename, std::ios::in | std::ios::binary);
    int read;

    while (!infile.eof()) {
        infile.read((char*)&read, sizeof(int));
        if (opecode(read) == 19) { 
            Memory[Current_PC] = offset(read);
        } else {
            Memory[Current_PC] = read;
        }
        Instruction.push_back({opecode(read), offset(read)});
        Current_PC++;
    }
}

void PrintISA() {
    std::cout << "+-------+---------+--------+" << std::endl;
    std::cout << "| Opcode| Mnemonic| Operand|" << std::endl;
    std::cout << "+-------+---------+--------+" << std::endl;
    std::cout << "|   0   |   ldc   | value  |" << std::endl;
    std::cout << "|   1   |   adc   | value  |" << std::endl;
    std::cout << "|   2   |   ldl   | value  |" << std::endl;
    std::cout << "|   3   |   stl   | value  |" << std::endl;
    std::cout << "|   4   |   ldnl  | value  |" << std::endl;
    std::cout << "|   5   |   stnl  | value  |" << std::endl;
    std::cout << "|   6   |   add   |        |" << std::endl;
    std::cout << "|   7   |   sub   |        |" << std::endl;
    std::cout << "|   8   |   shl   |        |" << std::endl;
    std::cout << "|   9   |   shr   |        |" << std::endl;
    std::cout << "|  10   |   adj   | value  |" << std::endl;
    std::cout << "|  11   |   a2sp  |        |" << std::endl;
    std::cout << "|  12   |   sp2a  |        |" << std::endl;
    std::cout << "|  13   |   call  | offset |" << std::endl;
    std::cout << "|  14   |  return |        |" << std::endl;
    std::cout << "|  15   |   brz   | offset |" << std::endl;
    std::cout << "|  16   |   brlz  | offset |" << std::endl;
    std::cout << "|  17   |   br    | offset |" << std::endl;
    std::cout << "|  18   |   HALT  |        |" << std::endl;
    std::cout << "|       |   SET   | value  |" << std::endl;
     std::cout << "|       |   data   | value  |" << std::endl;
    std::cout << "+-------+---------+--------+" << std::endl;
        Trcout.close();
}

void HandleArguments(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: ./emu.exe [option] file.o" << std::endl;
        std::cout << "Options:" << std::endl;
        std::cout << "\t-trace\tshow instruction trace" << std::endl;
        std::cout << "\t-before\tshow memory dump before execution" << std::endl;
        std::cout << "\t-after\tshow memory dump after execution" << std::endl;
        std::cout << "\t-isa\tdisplay ISA" << std::endl;
        exit(0);
    }

    std::string in_file = argv[2];
    int itr = 0;
    while (in_file[itr] != '.') {
        TraceFileName += in_file[itr];
        itr++;
    }
    TraceFileName += ".trace";

    Trcout.open(TraceFileName);
    std::string option = argv[1];
    if (option == "-trace") {
        while (PROGRAM_COUNTER < Current_PC - 1) {
            Run();
        }
    } else if (option == "-before") {
        Memory_Dump(true);
    } else if (option == "-after") {
        while (PROGRAM_COUNTER < Current_PC - 1) {
            Run();
        }
        Memory_Dump(false);
    } else if (option == "-isa") {
        PrintISA();
    } else {
        std::cout << "Invalid Argument!\n";
        std::cout << "Usage: ./emu.exe [option] file.o" << std::endl;
        std::cout << "Options:" << std::endl;
        std::cout << "\t-trace\tshow instruction trace" << std::endl;
        std::cout << "\t-before\tshow memory dump before execution" << std::endl;
        std::cout << "\t-after\tshow memory dump after execution" << std::endl;
        std::cout << "\t-isa\tdisplay ISA" << std::endl;
    }
}

int main(int argc, char* argv[]) {
        A = 0;
    B = 0;
    PROGRAM_COUNTER   = 0;
    SP = 9999;
    Current_PC = 0;
    ReadFile(argv[2]);
    HandleArguments(argc, argv);
    Trcout.close(); // Close Trcout once all operations are done
    return 0;
}