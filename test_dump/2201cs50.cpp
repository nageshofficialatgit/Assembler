// /*****************************************************************************
// TITLE: Assembler.cpp
// AUTHOR: Sahil Agrawal (2101CS69)
// Declaration of Authorship
// This file is part of the assignment of CS209 at the
// department of Computer Science and Engineering, IIT Patna.
// *****************************************************************************/

#include <bits/stdc++.h>
#include <fstream>
#include <cmath>
#include <chrono>
#include <numeric>

using namespace std;
#define fasterio ios_base::sync_with_stdio(0),cin.tie(0),cout.tie(0);
#define endl "\n"
#define ll long long
#define testcase int t; cin>>t; for(int i=0;i<t;i++)
#define INF 1e5

 string asmfile;
   string lgfile;
   string mcfile;
   string lsfile;

//IT IS ASSUMED THAT ALL FUNCTIONS ALLOW INTEGER and String of label.. but is not allowed for DATA

vector<pair<string,ll>> functions={{"ldc",0},{"adc",1}
,{"ldl",2},{"stl",3},{"ldnl",4},{"stnl",5},{"add",6},{"sub",7},
{"shl",8},{"shr",9},{"adj",10},{"a2sp",11},{"sp2a",12},{"call",13},
{"return",14},{"brz",15},{"brlz",16},{"br",17},{"HALT",18},{"data",19},{"SET",20}};

set<string> double_labels;
map<string,ll> label_with_address;
vector<string> pass2;
//THIS ONE IS BEST ONE
map<string,ll> address_of_label;

//ALL THE ERRORS
//  ALL THE ERRORS are written on the place where i have added errors
//
ll err_1=0;
vector<ll> line_err_1;
//
ll err_2=0;
vector<ll> line_err_2;
//
ll err_3=0;
vector<ll> line_err_3;
//
ll err_4=0;
vector<ll> line_err_4;
//
ll err_5=0;
vector<ll> line_err_5;
//
ll err_6=0;
vector<ll> line_err_6;
//
ll err_7=0;
vector<ll> line_err_7;
//
ll err_8=0;
vector<ll> line_err_8;
//
ll err_9=0;
vector<ll> line_err_9;
//
ll err_10=0;
vector<ll> line_err_10;
//
ll err_11=0;
vector<ll> line_err_11;
//Before Set there is no label
ll err_12=0;
vector<ll> line_err_12;
//NO HALT
ll warn_1=0;
//Multiple labels for same pc address
ll warn_2=0;
vector<ll>line_warn_2;

ll colon_finder(string s)
{
   ll length=s.size();
   ll find=0;
   for(ll i=0;i<length;i++)
   {
      if(s[i]==':')find=i;
   }
   return find;
}


void pass1(vector<string>&pass1str,vector<ll>&line_containing_colon,vector<ll>&line_containing_semicolon)
{


// all label ERRORS
   

   //err2:-    :xyz (no label name : direct)
   for(ll i=0;i<pass1str.size();i++)
   {
      for(ll j=0;j<pass1str[i].size();j++)
      {
         if(pass1str[i][j]==' ') continue;
         else {
            if (pass1str[i][j]==':')
            {
               err_2=1; line_err_2.push_back(i+1);
               break;
            }
            else break;
         }
      }
   }
   //err3:- number starting label
   for(ll i=0;i<pass1str.size();i++)
   {
      if(line_containing_colon[i]==0) continue;
      
      {
         for(ll j=0;j<pass1str[i].size();j++)
         {
            if(pass1str[i][j]==' ') continue;
            else {
               if(isalpha(pass1str[i][j]==0))
                  {err_3=1; line_err_3.push_back(i+1);break;
            }
            else break;
            

         }
      }
   }}
   //err4:- double colons in a string
   for(ll i=0;i<pass1str.size();i++)
   {
     if(line_containing_colon[i]==0) continue;
     ll count=0;
     for(ll j=0;j<pass1str[i].size();j++)
     {
      if(pass1str[i][j]==':') count+=1;

     }
     if(count==2) {err_4=1;line_err_4.push_back(i+1);}


   }

   //err5/6 :- wrong label name or double labels
   for(ll i=0;i<pass1str.size();i++)
   {
      string s;
      if(line_containing_colon[i]==0) continue;
      ll flag1=0;ll flag2=0;
      for(ll j=0;j<pass1str[i].size();j++)
      {
         if(pass1str[i][j]==' '&&flag1==0&&flag2==0) continue;
         else {
            if(flag2==0){
            flag1=1;
            
            
            if(isalnum(pass1str[i][j])!=0||pass1str[i][j]=='_') {s.push_back(pass1str[i][j]);continue;}
            else flag2=1;
         }

         if(flag2==1)
         {



            if(pass1str[i][j]==' ') continue;
            if(pass1str[i][j]==':')break;
            else {if(pass1str[i][j]!=':') {err_5=1;line_err_5.push_back(i+1);}

            else break;}

         }

      }
         


      }
      
      if(double_labels.find(s)==double_labels.end()) double_labels.insert(s);
      else {
         err_6=1;
         line_err_6.push_back(i+1);

      }
     
      

   }
   //LABEL WITH ; and basically : ke aage wala part ke liye.
   for(ll i=0;i<pass1str.size();i++)
   {
               ll flag1=0;ll flag2=0;ll flag3=0; ll flag4=0; string s;string number;

      ll alpha=colon_finder(pass1str[i]);
      if(line_containing_colon[i]==0) continue;
      for(ll j=alpha+1;j<pass1str[i].size();j++)
      {
         if(pass1str[i][j]==' '&&flag1==0) {continue;}
         else
         {
            if(flag2==0){
            flag1=1;
            if(pass1str[i][j]==' '||pass1str[i][j]==';') flag2=1;
            else s.push_back(pass1str[i][j]);
         }
         else {
            if(pass1str[i][j]==' '&&flag3==0) continue;
            else {
               flag3=1;
               if(flag4==0){
               if(pass1str[i][j]==' '||pass1str[i][j]==';') flag4=1;
               else number.push_back(pass1str[i][j]);
               if(pass1str[i][j]==';') break;
            }
            else {
               //if(pass1str[i][j]==';') break;
               if(pass1str[i][j]==' '||pass1str[i][j]==';'){if(pass1str[i][j]==';') break;continue;} 
               else {
                 // cout<<pass1str[i][j]<<endl;

                  err_8=1;line_err_8.push_back(i+1);
                  break;
               }
            }
         }
      }
   }
      }
     ll flag6=0;
      for(ll i=0;i<functions.size();i++)
      {
         if(s==functions[i].first) flag6=1;
      }
    
      if(flag6==0&&s.size()!=0) {err_7=1;line_err_7.push_back(i+1);}
      

ll flag5=0;
ll flag11=0;
for(ll i=0;i<functions.size();i++)
{
   if(i==7||i==8||i==9||i==10||i==12||i==13||i==15||i==19) continue;
   if(s==functions[i].first) flag5=1;

  
}


if(flag5==0)
{
   if(number.size()!=0) {err_9=1;line_err_9.push_back(i+1);}
}
ll flag7=0;
ll flag8=0;
ll flag9=0;
if(flag5==1)
{
   if(number.size()==0) flag9=1;
   if(number[0]==0&&number[1]=='x')
      for(ll i=2;i<number.size();i++)
      {
         if(isxdigit(number[i])==0||number[0]!='0'||number[1]!='x')
            flag7=1;
      }
      else
      {
         for(ll i=0;i<number.size();i++)
         {
            if(isdigit(number[i])==0)
               flag8=1;
         }
      }
      if(((flag8==1&&flag7==1)||flag9==1)&&(s!="br"&&s!="brz"&&s!="brlz"&&s!="ldc"&&s!="adc"&&s!="adj"&&s!="ldl"&&s!="stl"&&s!="ldnl"&&s!="stnl"&&s!="call")) {err_10=1; line_err_10.push_back(i+1);}
}


    if(s=="br"||s=="brz"||s=="brlz"&&s!="ldc"&&s=="adc"&&s=="adj"&&s=="ldl"&&s=="stl"&&s=="ldnl"&&s=="stnl"&&s=="call") {
      if(flag7==1&&flag8==1)
      {
         if(double_labels.find(number)==double_labels.end()) flag11=1; 
      }
      if(flag9==1||flag11==1) {err_1=1; line_err_1.push_back(i+1);}
}
      
   }

// all non label ERRORS
   
//err7:- wrong function names
//err8/err10:- extra operand/not a number(kuch aur) with the given function or giving no number when needed
   //err9:-/ no number needed still giving number
   //err 1 is br error
   //err 11 :- calling a label which is not defined
   for(ll i=0;i<pass1str.size();i++)
   {
      if(line_containing_colon[i]==0){
         ll flag1=0;ll flag2=0;ll flag3=0; ll flag4=0; string s;string number;
      for(ll j=0;j<pass1str[i].size();j++)
      {
         if(pass1str[i][j]==' '&&flag1==0) {continue;}
         else
         {
            if(flag2==0){
            flag1=1;
            if(pass1str[i][j]==' '||pass1str[i][j]==';') flag2=1;
            else s.push_back(pass1str[i][j]);
            if(pass1str[i][j]==';') break;
         }
         else {
            if(pass1str[i][j]==' '&&flag3==0) continue;
            else {
               flag3=1;
               if(flag4==0){
                  if(pass1str[i][j]==';') break;
               if(pass1str[i][j]==' '||pass1str[i][j]==';') flag4=1;
               else number.push_back(pass1str[i][j]);
               if(pass1str[i][j]==';') break;
            }
            else {
               //if(pass1str[i][j]==';') break;
               if(pass1str[i][j]==' '||pass1str[i][j]==';'){if(pass1str[i][j]==';') break;continue;} 
               else {
                 // cout<<pass1str[i][j]<<endl;

                  err_8=1;line_err_8.push_back(i+1);
                  break;
               }
            }
         }
      }
   }
}
//cout<<number<<endl;

if(double_labels.find(number)==double_labels.end()&&number.size()!=0) 
{
   err_11=1;
   line_err_11.push_back(i+1);
}
ll flag6=0;
      for(ll i=0;i<functions.size();i++)
      {
         if(s==functions[i].first) flag6=1;
      }
    
      if(flag6==0&&s.size()!=0) {err_7=1;line_err_7.push_back(i+1);}
      

ll flag5=0;
ll flag11=0;
for(ll i=0;i<functions.size();i++)
{
   if(i==7||i==8||i==9||i==10||i==12||i==13||i==15||i==19) continue;
   if(s==functions[i].first) flag5=1;

  
}


if(flag5==0)
{
   if(number.size()!=0) {err_9=1;line_err_9.push_back(i+1);}
   
}
ll flag7=0;
ll flag8=0;
ll flag9=0;
if(flag5==1)
{
   if(number.size()==0) flag9=1;
   if(number[0]==0&&number[1]=='x')
      for(ll i=2;i<number.size();i++)
      {
         if(isxdigit(number[i])==0||number[0]!='0'||number[1]!='x')
            flag7=1;
      }
      else
      {
         for(ll i=0;i<number.size();i++)
         {

            if(isdigit(number[i])==0)
               flag8=1;
         }
      }
      if(((flag8==1&&flag7==1)||flag9==1)&&(s!="br"&&s!="brz"&&s!="brlz"&&s!="ldc"&&s!="adc"&&s!="adj"&&s!="ldl"&&s!="stl"&&s!="ldnl"&&s!="stnl"&&s!="call")) {err_10=1; line_err_10.push_back(i+1);}
}


if(s=="br"||s=="brz"||s=="brlz"&&s!="ldc"&&s=="adc"&&s=="adj"&&s=="ldl"&&s=="stl"&&s=="ldnl"&&s=="stnl"&&s=="call") {
      if(flag7==1&&flag8==1)
      {
         if(double_labels.find(number)==double_labels.end()) flag11=1; 
      }
      if(flag9==1||flag11==1) {err_1=1; line_err_1.push_back(i+1);}
}

      
 }

}
}
// This is the initiations required for PASS 2
//Saving all the code lines in a compact without spaces and without empty line forms
void pass2_data_structure(vector<string>&pass1str,vector<ll>&line_containing_colon){
   for(ll i=0;i<pass1str.size();i++)
    {
      ll flag1=0;ll flag2=0;ll flag3=0;ll flag4=0;string s;
      string number;
      //LINE NOT CONTAINING COLON 
      if(line_containing_colon[i]==0)
      {
         for(ll j=0;j<pass1str[i].size();j++)
         {
            if(pass1str[i][j]==' '&&flag1==0) continue;
            else
            {
               flag1=1;
               
               if(pass1str[i][j]==';')break;
               else
               {
                  if(pass1str[i][j]!=' '&&flag2==0) s+=pass1str[i][j];
                  else
                  {
                     flag2=1;
                     if(pass1str[i][j]==' '&&flag3==0) continue;
                     else
                     {
                        flag3=1;
                        if(pass1str[i][j]==';')break;
                      else
                      {
                        if(pass1str[i][j]!=' '&&flag4==0) number+=pass1str[i][j];
                        else{
                           flag4=1;
                           if(pass1str[i][j]==' ') continue;
                           if(pass1str[i][j]==';') break;
                        }
                     }

                     }
                  }
               }
           
         }

         }
         if(s.size()!=0)
         pass2.push_back(s+' '+number);

      }
      
      //LINE CONTAINING COLON :
      else
      {
         ll alpha=colon_finder(pass1str[i]);
         for(ll j=alpha+1;j<pass1str[i].size();j++ )
         {

            if(pass1str[i][j]==' '&&flag1==0) continue;
            else
            {
               flag1=1;
               
               if(pass1str[i][j]==';')break;
               else
               {
                  if(pass1str[i][j]!=' '&&flag2==0) s+=pass1str[i][j];
                  else
                  {
                     flag2=1;
                     if(pass1str[i][j]==' '&&flag3==0) continue;
                     else
                     {
                        flag3=1;
                        if(pass1str[i][j]==';')break;
                      else
                      {
                        if(pass1str[i][j]!=' '&&flag4==0) number+=pass1str[i][j];
                        else{
                           flag4=1;
                           if(pass1str[i][j]==' ') continue;
                           if(pass1str[i][j]==';') break;
                        }
                     }

                     }
                  }
               }
           
         }


         }
         string label;
         for(ll j=0;j<alpha;j++)
            if(pass1str[i][j]!=' ') label+=pass1str[i][j];
         pass2.push_back(label+':');
         if(s.size()!=0)
         pass2.push_back(s+' '+number);



      }



    }
    ll fla=0;

    for(ll i=1;i<pass2.size();i++)
    {
      string checker;
      for(ll j=0;j<pass2[i].size();j++)
         {if(pass2[i][j]==' ') break;
      else checker+=pass2[i][j];
   }
   if(checker=="HALT") fla=1;
   
    
      string checker2;
      for(ll j=0;j<pass2[i-1].size();j++)
         {if(pass2[i-1][j]==' ') break;
      else checker2+=pass2[i-1][j];
   }
  // cout<<checker<<checker2<<endl;
   if(checker[checker.size()-1]==checker2[checker2.size()-1])
      {warn_2=1; line_warn_2.push_back(i+1);}

    }

    // if(fla==0) warn_1=1;
    // cout<<warn_1<<warn_2<<endl;
}


//The listing file maker code.. one part of pass 2
void pass2_check_and_load()
{

     std::ofstream out(lsfile);
    std::streambuf *coutbuf = std::cout.rdbuf(); 
    std::cout.rdbuf(out.rdbuf()); 

    vector<ll> pass2_line_with_colon(pass2.size(),0);
    ll pc_for_address=0;
    ll pc=0;
   
//SAVING THE LINE WITH COLONS
for(ll i=0;i<pass2.size();i++)
{
  
   for(ll j=0;j<pass2[i].size();j++)
      if(pass2[i][j]==':') pass2_line_with_colon[i]=1; 
}


//SAVING ADDRESS FOR THE LABELS IN A MAP (address_of_label)
for(ll i=0;i<pass2.size();i++)
{
   if(pass2_line_with_colon[i]==1)
   {
      string str;
      for(ll j=0;j<pass2[i].size();j++)
      {
         if(pass2[i][j]==':') break;
         else str+=pass2[i][j];
      }
      address_of_label.insert({str,pc_for_address});
   }
   else {pc_for_address++;
   string str;
   int flagx=0;
   for(int j=0;j<pass2[i].size();j++)
   {
      if(pass2[i][j]==' '){flagx=j;break;} 
         else str+=pass2[i][j];
   }
   string number;
   for(int j=flagx+1;j<pass2[i].size();j++)
      number+=pass2[i][j];
   string number2;
   for(int j=flagx+3;j<pass2[i].size();j++)
      number2+=pass2[i][j];

   if(isdigit(number[0])==1&&isdigit(number[1])==1){
unsigned ll x;   
                  std::stringstream ss;
                  ss  << number;
                  ss >> x;
if(str=="SET") 
           {      string past_label;
                  for(ll j=0;j<pass2[i-1].size();j++)
                  {
                     if(pass2[i-1][j]==':') break;
                     else past_label+=pass2[i-1][j];
                  }
                  address_of_label[past_label]=x;}}

                  if(number[0]=='0'&&number[1]=='x')
                  {
                     unsigned ll x;   
                  std::stringstream ss;
                  ss  <<hex<< number2;
                  ss >> x;
if(str=="SET") 
           {      string past_label;
                  for(ll j=0;j<pass2[i-1].size();j++)
                  {
                     if(pass2[i-1][j]==':') break;
                     else past_label+=pass2[i-1][j];
                  }
                  address_of_label[past_label]=x;}

                  }
                  }
}
vector<string> list_file;


//Couting in the file one by one
for(ll i=0;i<pass2.size();i++)
{
   // COUTING THE PC ADDRESS 
    if(pass2_line_with_colon[i]==0){
      cout<<setw(8) << setfill('0')<<hex<<pc<<" ";
      string function;
      string number;
      ll x=0;

      for(ll j=0;j<pass2[i].size();j++)
         {
            x=j;
            if(pass2[i][j]==' ') break;
            else function+=pass2[i][j];
         }
         //cout<<x<<endl;

         for(ll j=x+1;j<pass2[i].size();j++)
         {
            number+=pass2[i][j];
         }
//IF THE FUNCTION IS DATA OR SET 
            if(function=="data"||function=="SET")
            { //IF THE NUMBER IS OF THE FORM 0X234 OR -0X234

                 if((number[0]=='0'&&number[1]=='x')||(number[0]=='-'&&number[1]=='0'&&number[2]=='x'))
               {
                  if(number[0]=='0'&&number[1]=='x'){
                  for(ll i=0;i<(8-number.size());i++)
                     cout<<0;
                  string number2;
                  for(ll i=2;i<number.size();i++)
                     {cout<<number[i];number2+=number[i];}

                  //    cout<<" ";
                  unsigned ll x;   
                  std::stringstream ss;
                  ss << std::hex << number2;
                  ss >> x;
                  if(function=="SET")
               {if(pass2_line_with_colon[i-1]==0){err_12=1;line_err_12.push_back(i+1);}
                  string past_label;
                  for(ll j=0;j<pass2[i-1].size();j++)
                  {
                     if(pass2[i-1][j]==':') break;
                     else past_label+=pass2[i-1][j];
                  }
                  address_of_label[past_label]=x;

               }

               }
               if(number[0]=='-'&&number[1]=='0'&&number[2]=='x')
               {
                  ll x=0;
                  string number2;
                  for(ll i=3;i<number.size();i++)
                  {
                     number2+=number[i]; 
                   }

                   for(ll i=0;i<number2.size();i++)
                   {

                     if(number2[i]>=48&&number2[i]<=57)
                       {x+=pow(16,(number2.size()-i-1))*(number2[i]-'0');}
                     else 
                     {x+=pow(16,number2.size()-i-1)*(number2[i]-'a');}
                   }

                   x-=2*x;
                   // IF THE FUNCTIUON IS SET I AM REFRESHING THE MAP CONTAINING THE ADDRESS OF LABEL AND CHANGING THE ADDRESS (everywhere you see a set)
                   if(function=="SET")
               {if(pass2_line_with_colon[i-1]==0){err_12=1;line_err_12.push_back(i+1);}
                  string past_label;
                  for(ll j=0;j<pass2[i-1].size();j++)
                  {
                     if(pass2[i-1][j]==':') break;
                     else past_label+=pass2[i-1][j];
                  }
                  address_of_label[past_label]=x;

               }
                   stringstream stream;
                   stream<<hex<<x;
                   string str;
                   stream>>str;

                  // cout<<str<<endl;
                   
                   for(ll i=10;i<16;i++) cout<<str[i];


               }
               }

               //IF THE NUMBER IS OF THE FORM 123 OR -123
               else 
               {
                  
                unsigned ll x;
               stringstream ss;
                ss  << number;
                ss >> x;
               // cout<<x<<endl;
               stringstream stream;
                  stream <<hex<< x;
                string str;
               stream >> str;
              if(number[0]=='-') 
             {for(ll i=10;i<16;i++) cout<<str[i];}
               else
               cout<<setw(6) << setfill('0') << str;

                if(function=="SET")
               {
                  if(pass2_line_with_colon[i-1]==0){err_12=1;line_err_12.push_back(i+1);}
                  string past_label;
                  for(ll j=0;j<pass2[i-1].size();j++)
                  {
                     if(pass2[i-1][j]==':') break;
                     else past_label+=pass2[i-1][j];
                  }
                  address_of_label[past_label]=x;

               }
   
               }
               for(ll i=0;i<functions.size();i++)
            {
               if(function==functions[i].first)
                  cout<<setw(2)<<setfill('0')<<functions[i].second<<" ";
            }

            }
            else 
            {
               if((number[0]=='0'&&number[1]=='x')||(number[0]=='-'&&number[1]=='0'&&number[2]=='x'))
               {
                  if(number[0]=='0'&&number[1]=='x'){
                  for(ll i=0;i<(8-number.size());i++)
                     cout<<0;
                  for(ll i=2;i<number.size();i++)
                     cout<<number[i];
                  //cout<<" ";
               }
               if(number[0]=='-'&&number[1]=='0'&&number[2]=='x')
               {
                  ll x=0;
                  string number2;
                  for(ll i=3;i<number.size();i++)
                  {
                     number2+=number[i]; 
                   }

                   for(ll i=0;i<number2.size();i++)
                   {

                     if(number2[i]>=48&&number2[i]<=57)
                       {x+=pow(16,(number2.size()-i-1))*(number2[i]-'0');}
                     else 
                     {x+=pow(16,number2.size()-i-1)*(number2[i]-'a');cout<<x<<endl;}
                   }

                   x-=2*x;
                   stringstream stream;
                   stream<<hex<<x;
                   string str;
                   stream>>str;
                   
                   for(ll i=10;i<16;i++) cout<<str[i];cout<<" ";


               }

               }
               // FOR THE NEGATIVE DECIMAL NUMBERA
               
               else
               {
                  if(isdigit(number[0])==1||number[0]=='-'){
                  unsigned ll x;
                  stringstream ss;
                  ss  << number;
                  ss >> x;
                  // cout<<x<<endl;
                  stringstream stream;
                  stream <<hex<< x;
                  string str;
                  stream >> str;
                  if(number[0]=='-') 
                 {for(ll i=10;i<16;i++) cout<<str[i];}
                  else
                 cout<<setw(6) << setfill('0') << str;

               }
               else 
               {
                  if(number.size()!=0)
                  {ll n; 
                   if(function=="ldc"||function=="adc"||function=="adj")
                      n=address_of_label[number];
                     else
                        n=address_of_label[number]-pc-1;
                stringstream stream;
                   stream<<hex<<n;
                   string str;
                   stream>>str;

                   if(n<0)
                  for(int i=10;i<16;i++)  cout<<str[i];
                     else
                     {
                        for(int i=0;i<6-str.size();i++)
                           cout<<'0';
                        cout<<str;
                        
                     }
            }

               if(number.size()==0) for(ll i=0;i<6;i++) cout<<'0';

               }

            }
             for(ll i=0;i<functions.size();i++)
            {
               if(function==functions[i].first)
                  cout<<setw(2)<<setfill('0')<<functions[i].second<<" ";
            }


            }
            cout<<pass2[i]<<endl;
            pc++;
         }
else{
   cout<<setw(8)<<setfill('0')<<hex<<pc<<" ";
   for(ll i=0;i<8;i++)
      cout<<" ";
   cout<<" ";
   cout<<pass2[i]<<endl;

}
      }
      out.close();
}

// ALL THE ERRORS IN THE ERROR FIULE - you cAN SEE all the errors with the line numebrs initially stored in the vector
void errors_warns()
{
   std::ofstream out(lgfile);
    std::streambuf *coutbuf = std::cout.rdbuf(); 
    std::cout.rdbuf(out.rdbuf()); 

    if(err_1==1)
    {
      cout<<"SET or data should have number input only. No calling to label is allowed!"<<endl;
      cout<<"Line numbers containing this errors are:- ";
      for(ll i=0;i<line_err_1.size();i++)
         cout<<line_err_1[i]<<" ";
      cout<<endl<<endl;
    }
    if(err_2==1)
    {
      cout<<"Please input the label names!"<<endl;
      cout<<"Line numbers containing this errors are:- ";
      for(ll i=0;i<line_err_2.size();i++)
         cout<<line_err_2[i]<<" ";
      cout<<endl<<endl;
    }
    if(err_3==1)
    {
      cout<<"Label name cannot have a number!"<<endl;
      cout<<"Line numbers containing this errors are:- ";
      for(ll i=0;i<line_err_3.size();i++)
         cout<<line_err_3[i]<<" ";
          cout<<endl<<endl;    }
    if(err_4==1)
    {
      cout<<"Double label not allowed in the same line!"<<endl;
      cout<<"Line numbers containing this errors are:- ";
      for(ll i=0;i<line_err_4.size();i++)
         cout<<line_err_4[i]<<" ";
          cout<<endl<<endl;    }
    if(err_5==1)
    {
      cout<<"Wrong label name. Input a continuous label name!"<<endl;
      cout<<"Line numbers containing this errors are:- ";
      for(ll i=0;i<line_err_5.size();i++)
         cout<<line_err_5[i]<<" ";
         cout<<endl<<endl;    }
    if(err_6==1)
    {
      cout<<"Double labels with same name!"<<endl;
      cout<<"Line numbers containing this errors are:- ";
      for(ll i=0;i<line_err_6.size();i++)
         cout<<line_err_6[i]<<" ";
          cout<<endl<<endl;    }
    if(err_7==1)
    {
      cout<<"Wrong function name!"<<endl;
      cout<<"Line numbers containing this errors are:- ";
      for(ll i=0;i<line_err_7.size();i++)
         cout<<line_err_7[i]<<" ";
          cout<<endl<<endl;    }
    if(err_8==1)
    {
      cout<<"Extra operand in the line!"<<endl;
      cout<<"Line numbers containing this errors are:- ";
      for(ll i=0;i<line_err_8.size();i++)
         cout<<line_err_8[i]<<" ";
          cout<<endl<<endl;    }
    if(err_9==1)
    {
      cout<<"No operand needed for this function!"<<endl;
      cout<<"Line numbers containing this errors are:- ";
      for(ll i=0;i<line_err_9.size();i++)
         cout<<line_err_9[i]<<" ";
           cout<<endl<<endl;    }
    if(err_10==1)
    {
      cout<<"Operand not set for the given function!"<<endl;
      cout<<"Line numbers containing this errors are:- ";
      for(ll i=0;i<line_err_10.size();i++)
         cout<<line_err_10[i]<<" ";
            cout<<endl<<endl;    }
    if(err_11==1)
    {
      cout<<"Calling a label which is not defined in the given asm input!"<<endl;
      cout<<"Line numbers containing this errors are:- ";
      for(ll i=0;i<line_err_11.size();i++)
         cout<<line_err_11[i]<<" ";
          cout<<endl<<endl;    }
    if(err_12==1)
    {
      cout<<"There is no label for SET to SET it!"<<endl;
      cout<<"Line numbers containing this errors are:- ";
      for(ll i=0;i<line_err_12.size();i++)
         cout<<line_err_12[i]<<" ";
            cout<<endl<<endl;    }
    if(warn_1==1)
    {
      cout<<"WARNING:  ";
      cout<<"No HALT in the given instruction, may turn in infinite loop!"<<endl<<endl;
      
    }
    if(warn_2==1)
    {
      cout<<"WARNING:  ";
      cout<<"Multiple label for the given address!"<<endl;
      cout<<"Line numbers containing this errors are:- ";
      for(ll i=0;i<line_err_1.size();i++)
         cout<<line_err_1[i]<<" ";
       cout<<endl<<endl;    }
       out.close();
}

//Now this function writes the machine code.. it is very much similar to listing fucntion but the couting is removed from certain places like pc counter or etc.
//So similar comment scheme will be foloowed

void pass2_check_and_load_for_machine_code_16_bit()
{

    ofstream myfile;
  myfile.open ("output.txt");

  

    vector<ll> pass2_line_with_colon(pass2.size(),0);
    ll pc_for_address=0;
    ll pc=0;
   
//SAVING THE LINE WITH COLONS
for(ll i=0;i<pass2.size();i++)
{
  
   for(ll j=0;j<pass2[i].size();j++)
      if(pass2[i][j]==':') pass2_line_with_colon[i]=1; 
}


//SAVING ADDRESS FOR THE LABELS IN A MAP (address_of_label)
for(ll i=0;i<pass2.size();i++)
{
   if(pass2_line_with_colon[i]==1)
   {
      string str;
      for(ll j=0;j<pass2[i].size();j++)
      {
         if(pass2[i][j]==':') break;
         else str+=pass2[i][j];
      }
      address_of_label.insert({str,pc_for_address});
   }
   else pc_for_address++;
}
vector<string> list_file;


for(ll i=0;i<pass2.size();i++)
{

    if(pass2_line_with_colon[i]==0){
      //myfile<<setw(8) << setfill('0')<<hex<<pc<<" ";
      string function;
      string number;
      ll x=0;

      for(ll j=0;j<pass2[i].size();j++)
         {
            x=j;
            if(pass2[i][j]==' ') break;
            else function+=pass2[i][j];
         }
         //myfile<<x<<endl;

         for(ll j=x+1;j<pass2[i].size();j++)
         {
            number+=pass2[i][j];
         }

            if(function=="data"||function=="SET")
            {

                 if((number[0]=='0'&&number[1]=='x')||(number[0]=='-'&&number[1]=='0'&&number[2]=='x'))
               {
                  if(number[0]=='0'&&number[1]=='x'){
                  for(ll i=0;i<(8-number.size());i++)
                     myfile<<0;
                  string number2;
                  for(ll i=2;i<number.size();i++)
                     {myfile<<number[i];number2+=number[i];}

                 
                  unsigned ll x;   
                  std::stringstream ss;
                  ss << std::hex << number2;
                  ss >> x;
                  if(function=="SET")
               {if(pass2_line_with_colon[i-1]==0){err_12=1;line_err_12.push_back(i+1);}
                  string past_label;
                  for(ll j=0;j<pass2[i-1].size();j++)
                  {
                     if(pass2[i-1][j]==':') break;
                     else past_label+=pass2[i-1][j];
                  }
                  address_of_label[past_label]=x;

               }

               }
               if(number[0]=='-'&&number[1]=='0'&&number[2]=='x')
               {
                  ll x=0;
                  string number2;
                  for(ll i=3;i<number.size();i++)
                  {
                     number2+=number[i]; 
                   }

                   for(ll i=0;i<number2.size();i++)
                   {

                     if(number2[i]>=48&&number2[i]<=57)
                       {x+=pow(16,(number2.size()-i-1))*(number2[i]-'0');}
                     else 
                     {x+=pow(16,number2.size()-i-1)*(number2[i]-'a');
                     myfile<<x<<endl;}
                   }

                   x-=2*x;
                   if(function=="SET")
               {if(pass2_line_with_colon[i-1]==0){err_12=1;line_err_12.push_back(i+1);}
                  string past_label;
                  for(ll j=0;j<pass2[i-1].size();j++)
                  {
                     if(pass2[i-1][j]==':') break;
                     else past_label+=pass2[i-1][j];
                  }
                  address_of_label[past_label]=x;

               }
                   stringstream stream;
                   stream<<hex<<x;
                   string str;
                   stream>>str;
                   
                   for(ll i=10;i<16;i++) myfile<<str[i];


               }
               }
               else 
               {
                  
                unsigned ll x;
               stringstream ss;
                ss  << number;
                ss >> x;
               // myfile<<x<<endl;
               stringstream stream;
                  stream <<hex<< x;
                string str;
               stream >> str;
              if(number[0]=='-') 
             {for(ll i=10;i<16;i++) myfile<<str[i];}
               else
               myfile<<setw(6) << setfill('0') << str;

                if(function=="SET")
               {
                  if(pass2_line_with_colon[i-1]==0){err_12=1;line_err_12.push_back(i+1);}
                  string past_label;
                  for(ll j=0;j<pass2[i-1].size();j++)
                  {
                     if(pass2[i-1][j]==':') break;
                     else past_label+=pass2[i-1][j];
                  }
                  address_of_label[past_label]=x;

               }
   
               }
               for(ll i=0;i<functions.size();i++)
            {
               if(function==functions[i].first)
                  myfile<<hex<<setw(2)<<setfill('0')<<functions[i].second;
            }

            }
            else 
            {

               if((number[0]=='0'&&number[1]=='x')||(number[0]=='-'&&number[1]=='0'&&number[2]=='x'))
               {
                  if(number[0]=='0'&&number[1]=='x'){
                  for(ll i=0;i<(8-number.size());i++)
                     myfile<<0;
                  for(ll i=2;i<number.size();i++)
                     myfile<<number[i];
                  //myfile<<" ";
               }
               if(number[0]=='-'&&number[1]=='0'&&number[2]=='x')
               {
                  ll x=0;
                  string number2;
                  for(ll i=3;i<number.size();i++)
                  {
                     number2+=number[i]; 
                   }

                   for(ll i=0;i<number2.size();i++)
                   {

                     if(number2[i]>=48&&number2[i]<=57)
                       {x+=pow(16,(number2.size()-i-1))*(number2[i]-'0');}
                     else 
                     {x+=pow(16,number2.size()-i-1)*(number2[i]-'a');myfile<<x<<endl;}
                   }

                   x-=2*x;
                   stringstream stream;
                   stream<<hex<<x;
                   string str;
                   stream>>str;
                   
                   for(ll i=10;i<16;i++) myfile<<str[i];


               }

               }
               else
               {
                  if(isdigit(number[0])==1||number[0]=='-'){
                  unsigned ll x;
                  stringstream ss;
                  ss  << number;
                  ss >> x;
                  // myfile<<x<<endl;
                  stringstream stream;
                  stream <<hex<< x;
                  string str;
                  stream >> str;
                  if(number[0]=='-') 
                 {for(ll i=10;i<16;i++) myfile<<str[i];}
                  else
                 myfile<<setw(6) << setfill('0') << str;

               }
               else 
               {
                  if(number.size()!=0)
                  {ll n; 
                    //myfile<<number<<endl;
                   if(function=="ldc"||function=="adc"||function=="adj")
                      n=address_of_label[number];
                     else
                        n=address_of_label[number]-pc-1;
 //myfile<<hex<<setw(6)<<setfill('0')<<(n)<<endl;
                     //n=-1;

               //n=-1;
                     
                        stringstream stream;
                   stream<<hex<<n;
                   string str;
                   stream>>str;

                   if(n<0)
                  for(int i=10;i<16;i++)  myfile<<str[i];
                     else
                     {
                        for(int i=0;i<6-str.size();i++)
                           myfile<<'0';
                        myfile<<str;
                        
                     }
                     

                              }

               if(number.size()==0) for(ll i=0;i<6;i++) myfile<<'0';

               }

            }
             for(ll i=0;i<functions.size();i++)
            {
               if(function==functions[i].first)
                  myfile<<hex<<setw(2)<<setfill('0')<<functions[i].second;
            }


            }
          
            pc++;
         }
         myfile<<endl;

      }
      myfile.close();
}

//In the above function cout is given to the output file, which is aagain used to remove the hexadecimal information and then converted to the binary fil.o
void machine_code_binary()
{
   string line;
   ofstream bin_mac;
  bin_mac.open (mcfile,ios::binary|ios::out);

  

   std::ifstream myfile("output.txt");
   while(std::getline(myfile, line))
   {
      if(line.size()==0) continue;
      for(ll i=0;i<line.size();i++)
         line[i]=toupper(line[i]);
     // bin_mac<<line<<endl;
      unsigned ll y;


      auto &s =line;
    stringstream ss;
    ss << hex << s;
    
    ss >> y;
     static_cast<ll>(y);
     // CONVERTIN G HEXA TO BINARY
    bin_mac.write((const char*)&y , sizeof(unsigned )) ;
        

   }
   bin_mac.close(); 
 
}




int main(int argc,char*argv[])
{
	
	vector<string> pass1str;
	string st;

   string n_file=(string)argv[1];
   string in_file="";

   for(auto i:n_file){
      if(i=='.')
         break;
      in_file+=i;
   }

   asmfile=in_file+".asm";
   lgfile=in_file+".log";
   mcfile=in_file+".o";
   lsfile=in_file+".lst";
	 ll number_of_lines_in_asm_file = 0;
    std::string line;
    std::ifstream myfile(asmfile);

    while (std::getline(myfile, line))
    {
    	pass1str.push_back(line);
        number_of_lines_in_asm_file++;
    }
    vector<ll> line_containing_colon(pass1str.size());
    vector<ll> line_containing_semicolon(pass1str.size());


    for(ll i=0;i<pass1str.size();i++)
    {
      for(ll j=0;j<pass1str[i].size();j++)
      {
         if(pass1str[i][j]==':') line_containing_colon[i]=1;

      }
    }

    for(ll i=0;i<pass1str.size();i++)
    {
      for(ll j=0;j<pass1str[i].size();j++)
      {
         if(pass1str[i][j]==';') line_containing_semicolon[i]=1;

      }
    }
    
    pass1(pass1str,line_containing_colon,line_containing_semicolon);
    pass2_data_structure(pass1str,line_containing_colon);
    pass2_check_and_load();
    errors_warns();
    pass2_check_and_load_for_machine_code_16_bit();
    machine_code_binary();

return 0;

}




