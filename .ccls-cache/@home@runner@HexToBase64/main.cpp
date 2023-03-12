/*
Converts hexidecimal (base 16) to base64 & finds XOR combo of two equal legnth hexidecimal numbers
Emily MacPherson
Last Updated: 2/27/2023
*/
#include <iostream>
#include <cstring>
#include <math.h>
#include <vector>
#include <array>
#include <stdio.h>

using namespace std;

vector<char> hexToBase64(char hexString[]);
array<char, 2> threeDigitHexToBase64(char threeDigitHex[]);
int hexToBase10(char hex[]);
char base10toBase64Digit(int base10);
int hexDigitToBase10(char c);
vector<char> XORCombo(char string1[], char string2[]);
char* findXORKey(char hexString[]);

int main() 
{
  char input[500]; 
  while(strcmp(input, "quit") != 0)
  {
    cout << "Enter 1 for hex to base 64 conversion or 2 for XOR combo: ";
    cin >> input;
    if(strcmp(input, "1") == 0) 
    {
      char hexInput[500];
      cout << "Enter a number in Hex: ";
      cin.ignore();
      cin.getline(hexInput, 500);
      cout << "This number in base 64 is: ";
      vector<char> base64 = hexToBase64(hexInput);
      if(base64.at(base64.size() - 1) == 'A') base64.at(base64.size() - 1) = '\0'; //don't print A at beginning
      for(int i = base64.size() - 1; i >= 0; i--) cout << base64.at(i); //print out backwards bc values were put in backwards
    }
    else if(strcmp(input, "2") == 0)
    {
      char hexString1[500];
      char hexString2[500];
      cout << "Enter first hexidecimal number: ";
      cin.ignore();
      cin.getline(hexString1, 500);
      cout << "Enter second hexidecimal number: ";
      cin.getline(hexString2, 500);
      vector<char> combo = XORCombo(hexString1, hexString2);
      cout << "XOR Combination: ";
      for(int i = 0; i < combo.size(); i++) cout << combo.at(i);
    }
    cout << endl;
    cout << "------------------------------------" << endl;
  }
}

/*
1111 = 15 in binary, 1 hexidecimal digit can represent any number 0-15
therefore 1 hexidecimal digit is represented with 4 binary digits (4 bits)
111111 = 63 in binary, 1 base64 digit can represent any number 0-63
therefore 1 base64 digit is represented with 6 bits
3 digits in hexidecimal (12 bits) translate to 2 digits in base64 (12 bits)
*/

//converts a hexidecimal of up to 500 digits to base 64
//outputs vecctor instead of char array cause c++ is a cry baby and doesn't like returning char arrays
vector<char> hexToBase64(char hexString[])
{
  vector<char> base64;
  char threeDigitHex[3] = {'0', '0', '0'};
  //split hexString into chuncks of 3 chars and send them to ThreeDigitHexToBase64
  int j = 2;
  for(int i = strlen(hexString) - 1; i >= 0; i--)
  {
    threeDigitHex[j] = hexString[i];
    if(j != 0) j--;
    else 
    {
      j = 2;
      array<char, 2> twoDigitBase64 = threeDigitHexToBase64(threeDigitHex);
      base64.push_back(twoDigitBase64[1]);
      base64.push_back(twoDigitBase64[0]);
      for(int k = 0; k < 3; k++) threeDigitHex[k] = '0';
    }
  }
  if(strlen(hexString) % 3 != 0)
  {
    array<char, 2> twoDigitBase64 = threeDigitHexToBase64(threeDigitHex);
    base64.push_back(twoDigitBase64[1]);
    base64.push_back(twoDigitBase64[0]);
  }
  return base64;
}

//converts a 3 digit hexidecimal number into a 2 digit base 64 number
array<char, 2> threeDigitHexToBase64(char hex[])
{
  array<char, 2> twoDigitBase64 = {'A', 'A'};
  int base10 = hexToBase10(hex);
  //convert base 10 to base 64
  twoDigitBase64[1] = base10toBase64Digit(base10 % 64);
  if(base10 > 63) twoDigitBase64[0] = base10toBase64Digit(floor(base10/64));
  return twoDigitBase64;
}

//will not work with very large hex inputs since an int can only hold so many digits
int hexToBase10(char hexString[])
{
  //convert hex to base 10
  int currentPlace = 1; //value of the current numercal place (like 10ths place, 100ths place--except it's base 16)
  int base10 = 0;
  int exponent = strlen(hexString) - 1;
  for(int i = 0; i < strlen(hexString); i++) 
  {
    currentPlace = pow(16, exponent);
    exponent--; 
    base10 += hexDigitToBase10(hexString[i]) * currentPlace; //hex[] stores numbers as ascii. -48 to get to the correct number
  }
  return base10;
}

int hexDigitToBase10(char c)
{
  int hexDigitToBase10;
  if(isalpha(c)) //if hex[i] is a letter
    {
      c = toupper(c);
      hexDigitToBase10 = int(c) - 55;
      return hexDigitToBase10;
    }
  else hexDigitToBase10 = c - 48;
  return hexDigitToBase10;
}

//converts a base 10 number 0-63 into the corresponding base 64 character 
//https://keyholesoftware.com/wp-content/uploads/Screen-Shot-2019-08-02-at-12.54.20-PM.png
char base10toBase64Digit(int base10)
{
  char base64Digit;
  //char(intx) converts ascii value intx into its corresponding character
  if(0 <= base10 && base10 <= 25) base64Digit = char(base10 + 65); //base10 corresponds with a capital letter
  else if(25 < base10 && base10 <= 51) base64Digit = char(base10 + 71); //base10 corresponds with a lowercase letter
  else if(51 < base10 && base10 <= 61) base64Digit = char(base10 - 4); //base10 corresponds with a number
  else if(base10 == 62) base64Digit = '+';
  else if(base10 == 63) base64Digit = '/';
  return base64Digit;
}


/*
 example: 
XOR Combo of hexidecimal number F and hexidecimal number A
Hexidecimal F represents 15, which in binary is 1111
Hexidecimal A represents 10, which in binary is 1010

1 = true, 0 = false
XOR is true if only one of the conditions is true

        F: 1 1 1 1
        A: 1 0 1 0
XOR Combo: 0 1 0 1  (which is 5)

*/
vector<char> XORCombo(char str1[], char str2[])
{
  vector<char> combo;
  for(int i = 0; i < strlen(str1); i++)
  {
    unsigned int XORdDigit = hexDigitToBase10(str1[i]) ^ hexDigitToBase10(str2[i]); //unsigned int to not allow negatives
    //cout << "base 10:" << XORdDigit << endl;
    char XORdHexDigit = char(XORdDigit + 48); //convert integer to char of the same symbol
    if(XORdDigit > 9) XORdHexDigit = char(XORdDigit + 55); //convert base 10 to hex digit
    //cout << "hex: " << XORdHexDigit << endl;
    combo.push_back(XORdHexDigit);
  }
  return combo;
}

//find what repeating character a hex string has been XOR'd against
char* findXORKey(char hexString[])
{
  char key;
  
  return key;
}

//returs an int score representing how likely a string is to be plain text
//higher int = more likely to be plain text
int scoreString()