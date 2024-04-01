#include <bits/stdc++.h>

using namespace std;
vector<pair<int , int >> opcode ;
vector<string> input ; 
map<int, string> OPTABLE = {{0, "ldc"}, {1, "adc"}, {2, "ldl"}, {3, "stl"}, {4, "ldnl"}, {5, "stnl"}, {6, "add"}, {7, "sub"}, {8, "shl"}, {9, "shr"}, {10, "adj"}, {11, "a2sp"}, {12, "sp2a"}, {13, "call"}, {14, "return"}, {15, "brz"}, {16, "brlz"}, {17, "br"}, {18, "halt"}, {1000, "data"}, {100, "set"}};
int A , B , SP ; // 32 BIT REGISTER 
int PC ;
int PC_CALL ;  //PROGRAM COUNTER
int mem_limit = 10000 ; // max memory limit 
int memory[10000] ={};//memory segment 

int main(int argc, char *argv[])
{

    
}