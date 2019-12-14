//Rakshak Shrestha 5375496 rs720 CSCI361 A3 Part 2

#include <iostream>
#include <istream>
#include <fstream>
#include <math.h>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <random>
#include <chrono>
#include <cctype>
#include <bitset>

using namespace std;

bool checkPrime(unsigned long long int p);
unsigned long long int binToDeci(string bin);
string getBinary(int l);
int gcd(int a, int b);
void signFunction();
void verifyFunction();
unsigned long long int modInverse(unsigned long long int n1, unsigned long long int n2);
unsigned long long int sqMul( unsigned long long x, unsigned long long H, unsigned long long n);

int main(){
  unsigned long long int l, p, q, n, t, e[3], d[3], finalE, finalD;
  string inp = "a";
  while (inp != "q"){
    cout << "Generate keys (k) / Execute sign function (s)  / Execute verify function (v) / Quit program (q): ";
    cin >> inp;
  if (inp == "k"){
    cout << endl << "Please enter the bit-length of p & q: ";
    cin >> l;
    cout << endl << "This might take a few seconds...";
    string nBin, nBin2;
    unsigned long long int nDeci, nDeci2;
    srand(time(NULL));
    bool cond = false;
    while (!cond){     //generates required binary and checks if the decimal equivalent is prime
      nBin = getBinary(l);
      nDeci = binToDeci(nBin);
        if (checkPrime(nDeci) == 1) cond = true;
    }
    p = nDeci;    //assigning the first prime number
    bool cond2 = false;
    while (!cond2){
      nBin2 = getBinary(l);
      nDeci = binToDeci(nBin2);
        if (checkPrime(nDeci) == 1) cond2 = true;
      if (nDeci == p) cond2 = false;
      //cout << "try " << endl;
    }
    q = nDeci;    //assigning the second prime number
    n = p * q;   //product of prime
    cout << "prime numbers: " << p << " " << q << endl;
    t = (p - 1) * (q - 1);  //Euler's totient function
    int c = 0;
    long long tmpD;
    for (int i = 2; i < t; i++){   //checks of 3 valid values for e and d
      if (gcd(i, t) != 1) continue;
      if (checkPrime(i) == 1 && i != p && i != q){
        tmpD = modInverse(t, i);
        if (i*tmpD % t == 1 && i != tmpD){
          e[c] = i;      //sets the possible values for e
          d[c] = tmpD;   //sets the possible values for d
          c++;
        }
        if (c > 2) break;
      }
    }
    int randomE = rand() % 2;
    finalE = e[randomE];   //assigns a random value to e from the possible values of e
    finalD = d[randomE];   //assigns a random value to d from the possible values of d
    ofstream of;
    of.open("pk.txt");    //writes the public key into pk.txt
    of << n << " ";
    of << finalE;
    of.close();
    of.open("sk.txt");    //writes the private key into sk.txt
    of << n << " " << p << " " << q << " " << finalD;
    of.close();
    cout << endl << "Keys generated and stored in pk.txt and sk.txt" << endl;
    cout << endl << "KEYS ARE " << finalD << " " << finalE << endl;
  }
  else if (inp == "s"){
    signFunction();
  }
  else if (inp == "v"){
    verifyFunction();
  }
  else if (inp != "q"){
    cout << endl << "Invalid input.. Try again " << endl;
  }
  cout << endl;
  }
  return 0;
}

void signFunction(){     //function to sign
  unsigned long long int N, P, Q, D, M, S, K, j;
  ifstream fin;
  fin.open("sk.txt");
  fin >> N;
  fin >> P;
  fin >> Q;
  fin >> D;
  fin.close();
  fin.open("mssg.txt");
  fin >> M;
  K = 1;
  cout << endl << "Executing sign function..." << endl;
  /*for (j = 0; j < D; j++){
    K = (K * M) % N;
  }*/
  K = sqMul(M, D, N);
  ofstream of;
  of.open("sig.txt");
  of << K;
  of.close();
  cout << endl << "Sign function completed. Cipher text written in sig.txt." << endl;
}

void verifyFunction(){   //function to verify the keys
  ifstream fin;
  unsigned long long int CT, E, N, j, K;
  fin.open("sig.txt");
  fin >> CT;
  fin.close();
  fin.open("pk.txt");
  fin >> N;
  fin >> E;
  fin.close();
  K = 1;
  cout << endl << "Executing verifying function..." << endl;
  /*for (j = 0; j < E; j++){
    K = K * CT;
    K = K % N;
  }*/
  K = sqMul(CT, E, N);
  long long msg;
  cout << endl << "Please enter the message for verification: ";
  cin >> msg;
  if (K == msg) cout << endl << "TRUE" << endl;
  else  cout << endl << "FALSE" << endl;
}

unsigned long long int sqMul( unsigned long long x, unsigned long long H, unsigned long long n){  //square and multiply method for fast modular exponentian

	unsigned long long int h;
	unsigned long long int r;
	int bin[64];
	int i = 0;
  r = x;
	while( H > 0 ){  //converting to binary
    if (H % 2 == 0){
			bin[i] = 0;
		}
    else{
			bin[i] = 1;
		}
    H = H/2;
		i++;
  }
  i--;
  while(i>0){
    r = (r * r) % n;
    i--;
    if( bin[i] == 1 ){
			r = (r * x)  % n;
		}
  }
  return r;
}

unsigned long long int modInverse(unsigned long long int n1, unsigned long long int n2)   //calculates modular inverse of n2 mod n1 using euclidean algorithm
{
  long long a1 = 1, b1 = 0, a2 = 0, b2 = 1, tmp;
  while (1){
  long long q = n1/n2;
  long long  r = n1 % n2;
  if (r == 0){
    return b2;
  }
  n1 = n2;
  n2 = r;
  tmp = a2;
  a2 = a1 - q * a2;
  a1 = tmp;
  tmp = b2;
  b2 = b1 - q * b2;
  b1 = tmp;
 }
}

int gcd(int a, int b) {   //calculates gcd of two integers
	if (b == 0)
		return a;
	return gcd(b, a % b);
}

string getBinary(int l){   //generates random binary number of given bit length
  int len = l;
  string newBin;
  default_random_engine dre (chrono::steady_clock::now().time_since_epoch().count());
  uniform_int_distribution<int> dis(0,1);
  newBin.append("1");
  for (int i = 1; i < len - 1; i++){
    int j = dis(dre);
    string cj = to_string(j);
    newBin.append(cj);
  }
  newBin.append("1");
  return newBin;

}

bool checkPrime(unsigned long long int p){   //checks if decimal number is prime
  unsigned long long int num;
  num = p;
  for (unsigned long long int i = 3; i * i < num; i+=2){
    //cout << "I " << i;
    if (num % i == 0) return false;
  }
  //if (count > 0) return false;
  //else
  return true;
}

unsigned long long int binToDeci(string bin){   //transforms binary to decimal
  string num = bin;
  unsigned long long int decValue = 0;
  unsigned long long int base = 1;
  unsigned long long int len = num.length();
  for (int i = len-1; i >= 0; i--){
    if (num[i] == '1')
      decValue += base;
      base = base * 2;
  }
  return decValue;
  /*unsigned long long decValue = std::bitset<32>(bin).to_ullong();
  return decValue;*/
}
