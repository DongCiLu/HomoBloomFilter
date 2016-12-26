#include "bloomfilter.h"

BloomFilter::BloomFilter()
{}

void BloomFilter::Set_BF(int ni, int mi, int ki)
{
    srand(time(NULL));

	n = ni;
	m = mi;
	k = ki;
	n_element = 0;

	//malloc the size for the filter
	filter = (int*)malloc(sizeof(int)*m);

	//initialize the filter bits to 0
	for (int i = 0; i < m; i++)
	{
		filter[i] = 0;
	}
}


BloomFilter::~BloomFilter()
{
    free(filter);
}

int BloomFilter::get_m()
{
	return m;
}
int BloomFilter::get_k()
{
	return k;
}

int BloomFilter::get_n()
{
	return n;
}

int* BloomFilter::get_filter()
{
	return filter;
}

void BloomFilter::insert(string s)
{
	int len = s.length();

	/* Check the Bloom filter, by hashing and checking bits. */
	for (int i = 0; i < k; i++)
	{
		unsigned int h_value = murmurhash(s.c_str(), len, i);
		unsigned int pos = h_value % m;
		//cout<<h_value<<"	"<<pos<<endl;
		filter[pos] = 1;
	}

	n_element++;
}

int BloomFilter::contain(string s)
{
	int len = s.length();

	/* Check the Bloom filter, by hashing and checking bits. */
	for (int i = 0; i < k; i++)
	{
		unsigned int h_value = murmurhash(s.c_str(), len, i);
		unsigned int pos = h_value % m;
		//cout<<h_value<<"	"<<pos<<endl;
		if (filter[pos] == 0)
		{
			return 0;
		}
	}

	return 1;
}


int BloomFilter::count_ones(string s)
{
	int count = 0;
	int len = s.length();

	/* Check the Bloom filter, by hashing and checking bits. */
	for (int i = 0; i < k; i++)
	{
		unsigned int h_value = murmurhash(s.c_str(), len, i);
		unsigned int pos = h_value % m;
		//cout<<h_value<<"	"<<pos<<endl;
		if (filter[pos] == 1)
		{
			count++;
		}
	}

	return count;
}


unsigned int BloomFilter::murmurhash(const void * key, int len, unsigned int seed)
{
	// 'm' and 'r' are mixing constants generated offline.
	// They're not really 'magic', they just happen to work well.

	const unsigned int m = 0x5bd1e995;
	const int r = 24;

	// Initialize the hash to a 'random' value

	unsigned int h = seed ^ len;

	// Mix 4 bytes at a time into the hash

	const unsigned char * data = (const unsigned char *)key;

	while (len >= 4)
	{
		unsigned int k = *(unsigned int *)data;

		k *= m;
		k ^= k >> r;
		k *= m;

		h *= m;
		h ^= k;

		data += 4;
		len -= 4;
	}

	// Handle the last few bytes of the input array

	switch (len)
	{
	case 3: h ^= data[2] << 16;
	case 2: h ^= data[1] << 8;
	case 1: h ^= data[0];
		h *= m;
	};

	// Do a few final mixes of the hash to ensure the last few
	// bytes are well-incorporated.

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
}

unsigned int BloomFilter::RSHash(const string str, unsigned int seed)
{
	unsigned int b = 378551;
	unsigned int a = 63689;
	unsigned int hash = 0;

	for (int i = 0; i < str.length(); i++)
	{
		hash = hash * a + str[i];
		a = a * b;
	}

	return (hash & seed);
}

unsigned int BloomFilter::JSHash(const string str, unsigned int seed)
{
	unsigned int hash = 1315423911;

	for (int i = 0; i < str.length(); i++)
	{
		hash ^= ((hash << 5) + str[i] + (hash >> 2));
	}

	return (hash & seed);
}

float BloomFilter::diff_ratio(BloomFilter& bf)
{
	int diff_count = 0;
	int* bf_filter = bf.get_filter();
	for (int i = 0; i < m; i++)
	{
		if ((1 - (bf_filter[i] ^ filter[i])) == 0)
			diff_count++;
	}
	cout << "different counts are : " << diff_count << endl;
	return float(diff_count) / m;
}


float BloomFilter::full_ratio()
{
	int full_count = 0;
	for (int i = 0; i < m; i++)
	{
		if (filter[i] == 1)
			full_count++;
	}
	cout << "ones counts are : " << full_count << endl;
	return float(full_count) / m;
}

int* BloomFilter::get_bitmap(){
    int int_len = sizeof(int) * BYTELEN;
    int n_int = (m + int_len - 1) / int_len; // ceiling
	int* bitmap = (int*)malloc(n_int * sizeof(int));
    int bitmask = 1; // which bit in a int to set to 1
    int offset = 0; // which int to set

    for (int i = 0; i < n_int; i++) {
        bitmap[i] = 0;
    }

    for (int i = 0; i < m; i++) {
        if (filter[i] == 1) {
            bitmap[offset] |= bitmask;
        }
        bitmask = bitmask << 1;
        if (bitmask == 0) {
            offset += 1;
            bitmask = 1;
        }
    }
    return bitmap;
}
