#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include <vector>
#include <sstream>
#include <ctime>

#include "bloomfilter.h"
#include "fhe.h"

using namespace std;

#define NROUND 4
//#define NROUND 7

int main(int argc, char **argv) {
    // bf parameters: for 10% error, m=5n, 1% error, m=10n
    int max_number_of_elements[NROUND] = {50, 50, 100, 100};
    int size_of_bloomfilter[NROUND] = {250, 250, 500, 500};
    int number_of_hash[NROUND] = {4, 4, 4, 4};

    /*
    int max_number_of_elements[NROUND] = {20, 20, 20, 20, 20, 20, 20};
    int size_of_bloomfilter[NROUND] = {96, 125, 163, 192, 221, 259, 288};
    int number_of_hash[NROUND] = {4, 5, 6, 7, 8, 9, 10};
    */

    string ifn[NROUND] = {"datasets/processed/new_filtered_sessions_0_15.txt",
        "datasets/processed/new_filtered_sessions_15_30.txt",
        "datasets/processed/new_filtered_sessions_30_45.txt",
        "datasets/processed/new_filtered_sessions_45_above.txt"};

    //string ifn = "datasets/processed/new_filtered_sessions_30_45.txt";

    //int nexp = argc > 1 ? atoi(argv[1]): 1;
    //fhe_initialize(argc > 2 ? atoi(argv[2]): 2);
    int nexp = 20;
    fhe_initialize(4);

    for (size_t round = 0; round < NROUND; round ++) {
        cout << "----------------- ROUND " << round << " -----------------" << endl;
        // read inputs
        ifstream in(ifn[round].c_str());
        //ifstream in(ifn.c_str());
        vector< vector<string> > trajectories;
        string line;
        while(getline(in, line)) {
            vector<string> trajectory;
            istringstream iss(line);
            string coordinates;
            while (iss >> coordinates) {
                if (coordinates != "") {
                    trajectory.push_back(coordinates);
                }
            }
            trajectories.push_back(trajectory);
        }

        //srand(time(NULL));
        int correct_cnt = 0, in_cnt = 0;
        clock_t prep_time = 0, encryption_time = 0, band_time = 0, decryption_time = 0;

        for(int exp_i = 0; exp_i < nexp; exp_i++){
            cout << "." << flush;
            //int ix1 = rand() % trajectories.size();
            //int ix2 = rand() % trajectories.size();
            int ix1 = 0, ix2 = 0;
            if (exp_i < nexp / 2) {
                ix1 = exp_i;
                ix2 = exp_i;
            }
            else {
                ix1 = exp_i - (nexp / 2);
                ix2 = exp_i;
            }
            
            clock_t prep_start = clock();
            BloomFilter bf1;
            bf1.Set_BF(max_number_of_elements[round],
                    size_of_bloomfilter[round],
                    number_of_hash[round]);
            for (int i = 0; i < trajectories[ix1].size(); i++)
            //for (int i = 0; i < 20; i++)
                bf1.insert(trajectories[ix1][i]);
            int* bitmap1 = bf1.get_bitmap();

            BloomFilter bf2;
            bf2.Set_BF(max_number_of_elements[round],
                    size_of_bloomfilter[round],
                    number_of_hash[round]);
            bf2.insert(trajectories[ix2][0]); // only query the first
            int* bitmap2 = bf2.get_bitmap();
            prep_time += clock() - prep_start;

            int GroundTruth = bf1.contain(trajectories[ix2][0]);

            int int_len = sizeof(int) * BYTELEN;
            int bitmap_len = (bf1.get_m() + int_len - 1) / int_len; 

            fhe_integer result[bitmap_len];
            for (int i = 0; i < bitmap_len; i ++) {
                clock_t encryption_start = clock();
                mp_int **tmp_in1 = fhe_encrypt_integer(~bitmap1[i]);
                mp_int **tmp_in2 = fhe_encrypt_integer(bitmap2[i]);
                encryption_time += clock() - encryption_start;
                clock_t band_start = clock();
                mp_int **tmp = fhe_band_integers(tmp_in1, tmp_in2);
                band_time += clock() - band_start;
                clock_t decryption_start = clock();
                result[i] = fhe_decrypt_integer(tmp);
                decryption_time += clock() - decryption_start;
                DESTROY_ENCRYPTED_INTEGER(tmp);
            }

            int agg_result = 1;
            for (int i = 0; i < bitmap_len; i ++) {
                if (result[i] != 0) {
                    agg_result = 0;
                    break;
                }
            }

            if (agg_result == GroundTruth)
                correct_cnt += 1;
            else 
                cout << "Error in " << ix1 << " " << ix2 << endl;

            if (agg_result == 1)
                in_cnt += 1;

            free(bitmap1);
            free(bitmap2);
        }
        cout << "done!" << endl;
        cout << correct_cnt << " out of " << nexp << " is correct." << endl;
        cout << in_cnt << " out of " << nexp << " is in." << endl;
        cout << "Time consumption: " << endl;
        cout << "preparation: " << 1000 * double(prep_time) / CLOCKS_PER_SEC / nexp << "ms." << endl;
        cout << "encryption: " << 1000 * double(encryption_time) / CLOCKS_PER_SEC / nexp << "ms." << endl;
        cout << "band: " << 1000 * double(band_time) / CLOCKS_PER_SEC / nexp << "ms." << endl;
        cout << "decryption: " << 1000 * double(decryption_time) / CLOCKS_PER_SEC / nexp << "ms." << endl;  
    }
    return 0;
}
