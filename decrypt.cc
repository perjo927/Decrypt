#include <cmath>
#include <typeinfo>
#include <map>
#include <iostream>
#include <chrono>
#include <vector>
#include <set>
#include "Key.h"

using namespace std;

typedef map<Key,vector<Key>> keymap;
typedef set<Key> keyset;


void
looper(int start,int stop,Key candidate,const Key& encrypted,const Key table[],keymap& keys,keyset& deckeys)
{
  if (start == stop)
    return;
  for (int i{0}; i < R; ++i)
  {
    Key candenc = KEYsubsetsum(candidate, table);

    if (candenc == encrypted)
    {
      deckeys.insert(candidate);
    } else {
      if (keys.find(encrypted-candenc) != keys.end()) {
        for (auto& decrypted : keys[encrypted-candenc])
        {
          deckeys.insert(decrypted+candidate);
        }
      }
    }

    looper(start+1,stop,candidate,encrypted,table,keys,deckeys);
    ++candidate.digit[start];
  }
}

int
main(int argc, char* argv[])
{
  unsigned char buffer[C+1];     // temporary string buffer
  Key candidate = {{0}};         // a password candidate
  Key encrypted;                 // the encrypted password
  Key candenc;                   // the encrypted password candidate
  Key T[N];                      // the table T
  int subset = 4;                // number of blocks in subset
  keymap keys;                   // <Encrypted key, [Uncrypted keys]>
  keyset deckeys;                // Decrypted keys
  
  if (argc != 2)
  {
    cout << "Usage:" << endl << argv[0] << " password < rand8.txt" << endl;

    return 1;
  }

  // read in table T
  for (int i{0}; i < N; ++i)
  {   
    scanf("%s", buffer);
    T[i] = KEYinit(buffer);
  }

  auto begin = chrono::high_resolution_clock::now();

  // Create keys from first subset and store in map
  // aaa4z/2vy0h = encrypted ajtiz (in subset 1)
  int stop = pow(pow(2,B),subset); // subset blocks of rows

  for (int i{1}; i <= stop; ++i)
  {
    candenc = KEYsubsetsum(candidate, T);
    keys[candenc].push_back(candidate);
    ++candidate;
  }

  encrypted = KEYinit((unsigned char *) argv[1]);

  // Find encrypted password in map
  if (keys.find(encrypted) != keys.end()) {
    for (auto& decrypted : keys[encrypted])
    {
      deckeys.insert(decrypted);
    }
  }

  candidate = {{0}};
  // Create keys from second subset and calculate decrypted password
  // uuuaa = encrypted uyx2i (in subset 2)
  // chdks = encrypted vylid (between subset 1 and 2)
  looper(0,C-subset,candidate,encrypted,T,keys,deckeys);

  cout << "Decrypted password(s): " << endl;
  for (auto& decrypted : deckeys)
  {
    cout << decrypted << endl;
  }
  
  auto end = chrono::high_resolution_clock::now();
  cout << "Decryption took a total of "
       << std::chrono::duration_cast<chrono::seconds>(end - begin).count()
       << " seconds." << endl << endl;

  return 0;
}
