#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <iostream>
#include <functional>
#include <string>
#include <set>

#include <iostream>

using namespace std;

#define BYTELEN 8;

class BloomFilter
{
	int m, n, k;	//to save the value of m (bloom filter size), n (total number of elements), and k (the hash function number)
	float P; // the acceptable false negative rate
	int* filter; // the bloom filter
	short* Counting_filter; //for baseline counting bloom filter
	int n_element; //number of element inserted in the filter
	string hash_name; //the name of the hash function, it could be murmurhash, RShash, JShash
public:
	BloomFilter();
	void Set_BF(int ni, int mi, int ki);
	~BloomFilter();
	int get_m(); //output the number of m
	int get_n(); //output the number of n
	int get_k(); //output the number of k
	int* get_filter(); //output the filter
    void insert(string s); // insert an element
	int contain(string s); //check if an element is already in the filter

	int count_ones(string s); //count the number of 1s on the bits mapped for the element
	unsigned int murmurhash(const void * key, int len, unsigned int seed); //the hash function
	unsigned int RSHash(const string s, unsigned int seed);
	unsigned int JSHash(const string s, unsigned int seed);

	float diff_ratio(BloomFilter& bf);
	float full_ratio();

    int* get_bitmap();
};

