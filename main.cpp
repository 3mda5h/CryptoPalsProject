/*
This program (maybe?) completes challenges 1-6 at https://cryptopals.com/sets/1
Emily MacPherson
Last Updated: 3/15/2023

Throughout this project I use strings as byte arrays because strings are easier to manipulate

This code is very messy and probably way longer than it needs to be :D

EXAMPLES OF ONES THAT WORK (or mostly work) WITH #6:
  B8NEBhFEBhFGEsIHBgWGAwA
  HHMxTUQxNCQBQDk/KEZAOzAkAVU6czVJRHU1LUBGdTwnAVU9NmFUTzwnJEUBJicgVUQmcy5HATQ+JFNINjJhQE8xczVOASE7JAEBJzYxVEM5OiIBRzohYVZJPDApAUghczJVQDs3Mg0BOj0kAU80JyhOT3lzNE9FMCFhRk4xf2FNSDc2M1VYdTIvRQE/JjJVSDY2YUdOJ3MgTU17
  EkZA1lIF0ERUAtEBQ0cCVlNCw8XUBREFxIRFxwBFw5QBBFAEEE5UBpACkEEFQpVRAwJUAlTCwYCERQBCw9QAhxACA0JUBVOCgZQHRxSFwAXFQoPRChQBBFICgpQOVlWDQ0cUA1ACApQERtOERVQHQABBwAEXllsHUETEQ0BDRJQBhxTHUETHxZNSkEkGBwBAQ8U
  MFBwcTBAEDx0SVR4EGAwTQk8ABlINCUkHDwwEEVIRBAxBCAYTHBwCTBoQGw4FWVIGAwUOAAoNVTMQHgwNBw4PGlInGQwPCgYAVQUEH0kVAU8TEB8AAQsEHE8VHRMRTA0IHRsAGwZFDQ8VCx0PGh0LTB4JCwFBHRsWTA8AGgcEB1IRAwYKTgcIGFIRA0kFBxwCGgQAHkkIDQpP
  KExFOEBwGR0EVHAxTCgQUWE4KHB4CDxNOHRxTHwkRTh0BFg5BAwYMARZLFRwLEFMAHxMBAA5TBhtBFU4EEh1LFRwLEFMAChhUGQEcUwYUBgoMAVMfCQYLDF1TGBUGDwcUFksVHAcHFABLAAYLSRsSGxERAAAdFEsPG04aBwEKDxMLG1MQBBQYCkkHGw4YVAwMXVMCB1QZDFMeDhUQQEkdGgwJAE4AHVMfCRFOARIdDAgaCUkHAQ4E
  DcJE0UkDwoWEgIXBAACQzMTEwINGxcIGQtNAAAHQR8RHkENDA8FABwUCw0CExZNCQ8VBFYHCAQAQwBWAQQSDxAVExdNBwERQQINCEEGFgwXC00TDwAE
  QJDE4DAgEBHw1JBwNBBwEHQQAAUA0GDRsFBhkeTUkCHwICHVxBBQcXCR0dXEEGGwRBBghQEgAJGBVHTgQJDE4DAgEBHw1JBwNBBwEHQQAAUA0GDRsFBhkeTUkCHwICHVxBBQcXCR0dXEEGGwRBBghQEgUHFwkd
*/
#include <iostream>
#include <cstring>
#include <math.h>
#include <vector>
#include <array>
#include <stdio.h>
#include <fstream>

using namespace std;

struct XORdString {
  string ciphertext = "";
  string plainText = "";
  char key;
  int score = 0;
  int line = 0; //line number in file
};

//more functions than stars in the universe
string hexStrToBase64(string hex);
string threeDigitHexToBase64(string threeDigitHex);
int shortHexStrToBase10(string hex);
int hexDigitToBase10(char c);
string HexXORCombo(string str1, string str2);
array<XORdString*, 5> findSingleByteXORKey(string str);
int scoreString(string str);
array<XORdString*, 5> detectSingleByteXOR(string fileName);
string byteXORCombo(string str, string key);
string hexToByte(string hex);
string byteToHex(string byte);
int hammingDistance (string str1, string str2);
string base64ToByte(string base64);
string fourDigitBase64ToByte(string base64);
int base64CharToBase10(char base64);
string base10ToByte(int base10);
array<string, 5> breakRepeatingXOR(string bytes);
vector<string> breakIntoBlocks(string bytes, int keySize);

const char BASE_64_CHARS[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};
const char BASE_16_CHARS[] = {"0123456789abcdef"};

int main() 
{
  string input; 
  while(input != "quit")
  {
    cout << "Enter one of the following numbers: " << endl;
    cout << "[1] to convert hexidecimal to base64" << endl;
    cout << "[2] to get the XOR combo of two hexidecimal numbers" << endl;
    cout << "[3] to break single byte XOR" << endl;
    cout << "[4] to detect single byte XOR" << endl;
    cout << "[5] to encrypt using a repeating key XOR " << endl;
    cout << "[6] to break repeating key XOR" << endl;
    cout << "[7] to get hamming distance" << endl;
    cout << "[8] to score a string" << endl;
    cout << "-> ";
    getline(cin, input);
    if(input == "1") 
    {
      string hexStr;
      cout << "Enter a number in Hex: ";
      getline(cin, hexStr);
      cout << "This number in base 64 is: ";
      string base64 = hexStrToBase64(hexStr);
      cout << base64; 
    }
    else if(input == "2")
    {
      string hex1;
      string hex2;
      cout << "Enter first hexidecimal number: ";
      getline(cin, hex1);
      cout << "Enter second hexidecimal number: ";
      getline(cin, hex2);
      string combo = HexXORCombo(hex1, hex2);
      cout << "XOR Combination: " << combo << endl;
    }
    else if(input == "3") 
    {
      string encodedStr;
      cout << "Enter hexidecimal-encoded XOR'd string: ";
      getline(cin, encodedStr);
      array<XORdString*, 5> top5 = findSingleByteXORKey(encodedStr);
      cout << endl << "Most likely to be plain text: " << endl << endl;
      for(int i = 0; i < 5; i++) 
      {
        cout << "Key: '" << top5[i]->key << "' (" << int(top5[i]->key) << "), " << " Score: " << top5[i]->score << endl;
        cout << "Plaintext: ";
        for(int j = 0; j < top5[i]->plainText.length(); j++) cout << top5[i]->plainText[j];
        cout << endl << endl;
      }
    }
    else if (input == "4")
    {
      string fileName;
      cout << "Enter file name containing hex strings: ";
      getline(cin, fileName);
      array<XORdString*, 5> top5 = detectSingleByteXOR(fileName);
      cout << endl << "Hex strings that have most likely been XOR'd against a single characer: " << endl << endl;
      for(int i = 0; i < 5; i++) 
      {
        cout << "Key: '" << top5[i]->key << "' (" << int(top5[i]->key) << "), " << " Score: " << top5[i]->score;
        cout << ", line in file: " << top5[i]->line << endl;
        cout << "Hex string: ";
        for(int j = 0; j < top5[i]->ciphertext.length(); j++) cout << top5[i]->ciphertext[j];
        cout << endl << "Plaintext: ";
        for(int j = 0; j < top5[i]->plainText.length(); j++) cout << top5[i]->plainText[j];
        cout << endl << endl;
      }
    }
    else if (input == "5")
    {
      string plainText;
      string key;
      cout << "Enter plaintext: ";
      getline(cin, plainText);
      cout << "Enter key: ";
      getline(cin, key);
      string XORcombo = byteXORCombo(plainText, key);
      string XORcomboHex = byteToHex(XORcombo);
      cout << "XOR in bytes (decimal) is: ";
      for(int i = 0; i < XORcombo.length(); i++) cout << int(XORcombo[i]) << " ";
      cout << endl << "The hex-encoded XOR is: " << XORcomboHex << endl;
    }
    else if (input == "6")
    { 
      string input;
      string base64;
      string bytes;
      array<string, 5> keys;
      cout << "would you like to input from file (f) or manually (m)?" << endl;
      getline(cin, input);
      if(input == "f")
      {
        ifstream file("6.txt");
        string line;
        while(getline(file, line)) base64+=line;
        file.close();
        bytes = base64ToByte(base64);
        keys = breakRepeatingXOR(bytes);
      }
      else
      {
        cout << "enter base 64: ";
        getline(cin, base64);
        bytes = base64ToByte(base64);
        keys = breakRepeatingXOR(bytes);
      }
      cout << endl << endl << "Your potential keys are: " << endl; 
      for(int i = 0; i < 5; i++)
      {
        cout << i + 1 << ". key: " << keys[i] << endl;
        if(input != "f") cout << "plaintext: " << byteXORCombo(bytes, keys[i]) << endl << endl;
      }
      if(input == "f")
      {
        cout << endl << "Enter which number key you would like to see the plaintext for (Enter 1 - 5): ";
        getline(cin, input);
        int index = stoi(input) - 1;
        cout << endl << "plaintext for key \"" << keys[index] << "\": " << endl << byteXORCombo(bytes, keys[index]) << endl << endl;
      }
    }
    //for testing:
    else if (input == "7")
    {
      string str1;
      string str2;
      cout << "enter first string: " << endl;
      getline(cin, str1);
      cout << "enter second string: " << endl;
      getline(cin, str2);
      cout << "Hamming distance is: " << hammingDistance(str1, str2);
     }
    else if(input == "8")
    {
      string str;
      cout << "enter string: " << endl;
      getline(cin, str);
      int score = scoreString(str);
      cout << "score is: " << score;
    }
    cout << endl;
    cout << "-------------------------------------------------" << endl << endl;
  }
}

//converts a hexidecimal of up to 500 digits to base 64
string hexStrToBase64(string hex)
{
  string base64;
  string threeDigitHex = "000";
  //3 digits in hexidecimal (4 bits * 3 = 12 bits) translate to 2 digits in base64 (8 bits * 2 = 12 bits)
  //split hex string into chuncks of 3 chars and send them to ThreeDigitHexToBase64
  //if number of chars in hex is not divisible by 3, add extra 0s to front to make it so
  for(int i = 0; i < hex.length() % 3; i++) hex.insert(hex.begin(), '0');
  for(int i = 0; i < hex.length(); i += 3) 
  {
    threeDigitHex = hex.substr(i, 3);
    string twoDigitBase64 = threeDigitHexToBase64(threeDigitHex);
    base64 += twoDigitBase64;
  }
  if(base64[0] == 'A') base64.erase(0, 1); //remove first A or first 2 A's
  if(base64[0] == 'A') base64.erase(0, 1);
  return base64;
}

//converts a 3 digit hexidecimal string into a 2 digit base 64 string
string threeDigitHexToBase64(string hex)
{
  string twoDigitBase64 = "AA";
  int base10 = shortHexStrToBase10(hex);
  //convert base 10 to base 64
  twoDigitBase64[1] = BASE_64_CHARS[base10 % 64];
  twoDigitBase64[0] = BASE_64_CHARS[int(base10/64)]; //int always rounds down
  return twoDigitBase64;
}

//will not work with very large hex inputs since an int can only hold so many digits
int shortHexStrToBase10(string hex) 
{
  int currentPlace = 1; //value of the current numercal place (like 10s place, 100s place--except it's base 16)
  int base10 = 0;
  int exponent = hex.length() - 1;
  for(int i = 0; i < hex.length(); i++) 
  {
    currentPlace = pow(16, exponent);
    exponent--;
    base10 += hexDigitToBase10(hex[i]) * currentPlace;
  }
  return base10;
}

//converts single hex charcter (A, F, 3, etc...) to a base 10 int of the same value
int hexDigitToBase10(char c)
{
  c = tolower(c);
  for(int i = 0; i < 16; i++) if(BASE_16_CHARS[i] == c) return i;
  return 0;
}

//returns XOR Combo (in hexidecimal format) of two equal-length hexidecimal inputs
string HexXORCombo(string hex1, string hex2)
{
  string XORcombo;
  for(int i = 0; i < hex1.length(); i++)
  {  
    unsigned int XORdDigit = hexDigitToBase10(hex1[i]) ^  hexDigitToBase10(hex2[i]); 
    XORcombo += BASE_16_CHARS[XORdDigit];
  }
  return XORcombo;
}

//takes in a hex array and converts it to a byte array (/string/text/base 256/ whatever its called) 
string hexToByte(string hex)
{
  string bytes;
  if(hex.length() % 2 != 0) hex.insert(hex.begin(), '0');
  string twoDigitHex = "00"; //two hex digits = 1 byte = 1 ascii character 
  for(int i = 0; i < hex.length(); i+= 2)
  {
    twoDigitHex = hex.substr(i, 2);
    int byte = shortHexStrToBase10(twoDigitHex);
    bytes.push_back(byte);
  }
  return bytes;
}

//returns XOR Combo of byte array (string) XORd against a byte array (string) key
string byteXORCombo(string str, string key)
{
  string XORcombo;
  int k = 0; //keeps track of where we are in the key
  for(int i = 0; i < str.length(); i++)
  {
    XORcombo += char(str[i] ^ key[k]);
    if(k < key.length() - 1) k++;
    else k = 0;
  }
  return XORcombo;
}

string byteToHex(string bytes)
{
  string hex;
  int firstDigit;
  int secondDigit; //1 byte = 2 hex digits
  for(int i = 0; i < bytes.length(); i++)
  {
    firstDigit = floor(bytes[i] / 16);
    secondDigit = bytes[i] % 16;
    hex += BASE_16_CHARS[firstDigit];
    hex += BASE_16_CHARS[secondDigit];
  }
  return hex;
}

//find what single byte (character) a hex string has been XOR'd against
//returns array containing the 5 highest scored plaintexts as well as their corresponding key, score, and original encrypted hex string
array<XORdString*, 5> findSingleByteXORKey(string hex)
{
  array<XORdString*, 5> highestScoredCombos = {NULL}; //top 5 XOR combos most likely to be plain text
  int score;
  string bytes = hexToByte(hex);
  for(int i = 0; i <= 225; i++) //go thru all possible numbers within a byte
  { 
    string key; //convert int to string so I can pass into the function
    key += char(i);
    string XORcombo = byteXORCombo(bytes, key);
    score = scoreString(XORcombo);
    for(int j = 0; j < 5; j++)
    {
      if((highestScoredCombos[j] != NULL && score > highestScoredCombos[j]->score) || highestScoredCombos[j] == NULL) 
      {
        XORdString* higherScored = new XORdString();
        higherScored->ciphertext = hex;
        higherScored->plainText = XORcombo;
        higherScored->key = i;
        higherScored->score = score;
        highestScoredCombos[j] = higherScored;
        break;
      }
    }
  }
  return highestScoredCombos;
}

//returs an int score equal to the amount of letters + amount of spaces in a string
//more letters & spaces = more likely to be plain text
int scoreString(string str)
{
  int score = 0;
  float commonLetters = 0;
  float lowercase = 0;
  float totalSpaces = 0;
  float rareLetters = 0;
  float nonLetters = 0;
  for(int i = 0; i < str.length(); i++)
  {
    if(str[i] == ' ') totalSpaces++;
    if(islower(str[i])) lowercase++;
    //if(!isalpha(str[i]) && str[i] != ' ' && str[i] != '\n') nonLetters++;
    if(str[i] == 'e' || str[i] == 't' || str[i] == 'a' || str[i] == 'i' || str[i] == 'o') commonLetters++; //e, t, a, i, and o are most common letters in english alphabet
    if(str[i] == 'j' || str[i] == 'q' || str[i] == 'x' || str[i] == 'z' || str[i] == 'k') rareLetters++; //j, q, x, and z
    if((str[i] < 32 || str[i] > 126) && str[i] != '\n') score = -1000; //if it has even one of these weird characters we're going to assume it's not plaintext
  }
  //add percentage of letters, spaces, and common letters 
  score += (commonLetters/str.length()) * 50; 
  score += (lowercase/str.length()) * 100;
  score += (totalSpaces/str.length()) * 100;
  score -= (rareLetters/str.length()) * 50;
  //score -= (nonLetters/str.length()) * 200;
  return score;
}

/*returns array containing the 5 hex strings most likely to be plaintext XOR'd against a single byte, 
as well as their corresponding key, score, and plaintext */
array<XORdString*, 5> detectSingleByteXOR(string fileName)
{
  vector<string> hexStrings;
  array<XORdString*, 5> top5 = {NULL};
  ifstream file(fileName);
  string line;
  if(!file.is_open()) cout << "Could not open file :(" << endl;
  while(getline(file, line)) hexStrings.push_back(line);
  file.close();
  for(int i = 0; i < hexStrings.size(); i++)
  {
    XORdString* highestScored = findSingleByteXORKey(hexStrings[i])[0];
    for(int j = 0; j < 5; j++) 
    {
       if(top5[j] != NULL && highestScored->score > top5[j]->score || top5[j] == NULL)
       {
         top5[j] = highestScored;
         highestScored->line = i + 1;
         break;
       }
    }
  }
  return top5;
}

//https://www.geeksforgeeks.org/hamming-distance-between-two-integers/
//https://stackoverflow.com/questions/38922606/what-is-x-1-and-x-1
int hammingDistance (string str1, string str2)
{
  int differingBits = 0;
  for(int i = 0; i < str1.length(); i++)
  {
    int XOR = str1[i] ^ str2[i];
    while (XOR > 0)
    {
      differingBits += XOR & 1; //& is bitwise and. If the last digit of XOR is 1, then this returns 1. A 1 in an XOR means the two bits of the original strings were different.
      XOR >>= 1; //"set XOR to itself shifted by one bit to the right" - stack overflow. basically cutting off last bit I think?
    }
  }
  return differingBits;
}

//base 64 is 6 bit, byte is 8 bit
//4 * 6 bit = 24 bit, 3 * 8 bit = 24 bit
string base64ToByte(string base64)
{
  string bytes;
  string fourDigitBase64 = "AAAA";
  //if number of chars in base64 is not divisible by 4, add extra As to front to make it so
  for(int i = 0; i < base64.length() % 4; i++) base64.insert(base64.begin(), 'A'); 
  //cout << "base 64 length is: " << base64.length() << endl << endl;
  for(int i = 0; i < base64.length(); i += 4) 
  {
    fourDigitBase64 = base64.substr(i, 4);
    string threeDigitByte = fourDigitBase64ToByte(fourDigitBase64);
    bytes += threeDigitByte;
  }
  if(bytes[0] == 0) bytes.erase(0, 1); //get rid of empty bytes at the beginning 
  if(bytes[0] == 0) bytes.erase(0, 1);
  return bytes;
}

string fourDigitBase64ToByte(string base64)
{
  int place = 0;
  int exponent = 3;
  int base10 = 0;
  for(int i = 0; i < base64.length(); i++)
  {
    place = pow(64, exponent);
    exponent --;
    base10 += base64CharToBase10(base64[i]) * place;
  }
  string ThreeDigitByte = base10ToByte(base10);
  return ThreeDigitByte;
}

//3 bytes max
string base10ToByte(int base10)
{
  string bytes;
  int firstDigit = base10 / 65536; //256^2
  int secondDigit = (base10 - firstDigit * 65536) / 256;
  int thirdDigit = base10 % 256;
  /*if(firstDigit > 0) */bytes += char(firstDigit);
  /*if(secondDigit > 0) */bytes += char(secondDigit);
  bytes += char(thirdDigit);
  return bytes;
}

int base64CharToBase10(char base64)
{
  for(int i = 0; i < 64; i++) if(base64 == BASE_64_CHARS[i]) return i;
  return 0;
}

//comments/instructions from cryptopals.com and https://www.educative.io/answers/how-to-break-a-repeating-key-xor-encryption
array<string, 5> breakRepeatingXOR(string encodedBytes)
{
  array<string, 5> keys;
  for(int i = 0; i < 5; i++) keys[i] = "";
  int indexInKeys = 0; //keeps track of where we are in keys array
  int keySize;
  float HD; //hamming distance
  int mostLikelySizes[5] = {0};
  float smallestHDs[5] = {1000, 1000, 1000, 1000, 1000}; //corresponding average normalized hamming distances
  //cout << "encoded bytes (decimal) are: ";
  //for(int i = 0; i < encodedBytes.length(); i++) cout << int(encodedBytes[i]) << " ";
  //cout << "encoded bytes (ascii) are: " << encodedBytes << endl;
  //cout << endl;
  //cout << endl << "number of bytes: " << encodedBytes.length() << endl;
  vector<string> keySizedBlocks;
  /* 
    Calculate the Hamming distance between the first two blocks.
    Normalize the distance by dividing it by the key size.
    Repeat the same for at least 4-5 blocks.
    Take the average of the calculated distances.
  */
  for(int i = 2; i < 40; i++)
  {
    if(i > encodedBytes.size()/2) break; 
    else keySize = i;
    keySizedBlocks = breakIntoBlocks(encodedBytes, keySize);
    /*cout << "key sized blocks: " << endl;
    for(int j = 0; j < keySizedBlocks.size(); j++) 
    {
      cout << endl << j << ". ";
      for(int k = 0; k < keySizedBlocks.at(j).length(); k++) cout << int(keySizedBlocks.at(j)[k]) << " ";
    }
    cout << endl; */
    string XORAgainstThis;
    string currentBlock;
    float averageHD = 0;
    vector<float>HDs;
    cout.precision(4);
    int stoppingPoint = 6;
    if(keySizedBlocks.size() < 6) stoppingPoint = keySizedBlocks.size();
    for(int j = 0; j < stoppingPoint; j++)
    {
      XORAgainstThis = keySizedBlocks[j];
      for(int k = 0; k < stoppingPoint; k++)
      {
        if(j != k)
        {
          currentBlock = keySizedBlocks.at(k);
          /*cout << "XORing block ";
          for(int a = 0; a < XORAgainstThis.length(); a++) cout << int(XORAgainstThis[a]) << " ";
          cout << " against block ";
          for(int a = 0; a < currentBlock.length(); a++) cout << int(currentBlock[a]) << " ";
          cout << endl; */
          HD = hammingDistance(XORAgainstThis, currentBlock);
          //cout << "hamming distance: " << HD << endl;
          HD = HD/keySize;
          //cout << "normalized hd: " << HD << endl;
          HDs.push_back(HD);
        }
      }
    }
    for(int j = 0; j < HDs.size(); j++) averageHD += HDs.at(j);
    averageHD = averageHD/HDs.size();
    HDs.clear();
    int largestHDIndex = 0;
    //cout << "found average hamming distance " << averageHD << " for keysize " << keySize << endl << endl;
    //finrd largest of 5 smallest hamming distances
    for(int j = 0; j < 5; j++)
    {
      if(smallestHDs[j] > smallestHDs[largestHDIndex]) 
      {
        largestHDIndex = j;
      }
    }
    //replace largest of 5 smallest HD with new smaller HD
    if(averageHD < smallestHDs[largestHDIndex])
    {
      mostLikelySizes[largestHDIndex] = keySize;
      smallestHDs[largestHDIndex] = averageHD;
    }
  }
  cout << "most likely key sizes are: " << endl;
  for(int i = 0; i < 5; i++) cout << "size: " << mostLikelySizes[i] << ", average HD: " << smallestHDs[i] << endl;
  
  //procede with the 3 keysizes with the smallest average hamming distance
  for(int i = 0; i < 5; i++)
  {
    //break the ciphertext into blocks of KEYSIZE length.
    keySize = mostLikelySizes[i];
    keySizedBlocks = breakIntoBlocks(encodedBytes, keySize);
    /*Now transpose the blocks: make a block that is the first byte of every block, 
and a block that is the second byte of every block, and so on. */
    vector<string> transposedBlocks;
    for(int j = 0; j < keySize; j++)
    {
      string transposedBlock;
      for(int k = 0; k < keySizedBlocks.size(); k++)
      {
        transposedBlock += keySizedBlocks.at(k)[j];
      }
      transposedBlocks.push_back(transposedBlock);
    }

     //cout << "transposed blocks: " << endl;
    //for(int k = 0; k < transposedBlocks.size(); k++) cout << transposedBlocks.at(k) << endl;
    
    //Solve each block as if it was single-character XOR
    //cout << endl << endl << "key size: " << transposedBlocks.size() << endl;
    for(int j = 0; j < transposedBlocks.size(); j++)
    {
      string hex = byteToHex(transposedBlocks.at(j)); //converting to hex because my function takes in hex
      array<XORdString*, 5> singleByteKeys = findSingleByteXORKey(hex); //returns this transposed block's most likely single byte key
      /*cout << endl << "this transposed block in bytes is: ";
      for(int k = 0; k < transposedBlocks.at(j).length(); k++) cout << int(transposedBlocks.at(j)[k]) << " ";
      cout << endl << "most likely " << j << "th chars:" << endl;
      for(int k = 0; k < singleByteKeys.size(); k++)
      {
        cout << "char: "<< singleByteKeys.at(k)->key << " (" << int(singleByteKeys.at(k)->key) << endl;
        cout << "plaintext: " << singleByteKeys.at(k)->plainText << " (score: " << singleByteKeys.at(k)->score << ") " << endl << endl;
      } */
      keys[i] += singleByteKeys[0]->key;
    }
    indexInKeys++;
    keySizedBlocks.clear();
    transposedBlocks.clear();
  }
  return keys;
}

vector<string> breakIntoBlocks(string bytes, int keySize)
{
  vector<string> keySizedBlocks;
  for(int j = 0; j < bytes.size(); j += keySize)
  {
    if((bytes.size() - j) < keySize) break; //if we don't have enough of encodedBytes left to make another keysized block
    else 
    {
      string block = bytes.substr(j, keySize);
      keySizedBlocks.push_back(block);
    }
  }
  return keySizedBlocks;
}