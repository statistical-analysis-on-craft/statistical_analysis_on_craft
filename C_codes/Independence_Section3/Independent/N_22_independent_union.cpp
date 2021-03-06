// Independence.cpp : 定义控制台应用程序的入口点。
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <random>
#include <fstream>
#include <iostream>
using namespace std;

/* S-box */
const int S[16] =
{0Xc, 0xa, 0xd, 0x3, 0xe, 0xb, 0xf, 0x7, 0x8, 0x9, 0x1, 0x5, 0x0, 0x2, 0x4, 0x6};

/* Cell Shuffle in the linear layer */
const int P[16] =
{0xf, 0xc, 0xd, 0xe, 0xa, 0x9, 0x8, 0xb, 0x6, 0x5, 0x4, 0x7, 0x1, 0x2, 0x3, 0x0};

/* Cell Shuffle in the tk schedule */
const int Q[16] =
{0xc, 0xa, 0xf, 0x5, 0xe, 0x8, 0x9, 0x2, 0xb, 0x3, 0x7, 0x4, 0x6, 0x0, 0x1, 0xd};

/* Round constants generated by 3-bit LFSR, XOR-ed at state[5] */
const int RC3[32] =
{0x1, 0x4, 0x2, 0x5, 0x6, 0x7, 0x3, 0x1, 0x4, 0x2, 0x5, 0x6, 0x7, 0x3, 0x1, 0x4,
 0x2, 0x5, 0x6, 0x7, 0x3, 0x1, 0x4, 0x2, 0x5, 0x6, 0x7, 0x3, 0x1, 0x4, 0x2, 0x5};

/* Round constants generated by 4-bit LFSR, XOR-ed at state[4] */
const int RC4 [32] =
{0x1, 0x8, 0x4, 0x2, 0x9, 0xc, 0x6, 0xb, 0x5, 0xa, 0xd, 0xe, 0xf, 0x7, 0x3, 0x1,
 0x8, 0x4, 0x2, 0x9, 0xc, 0x6, 0xb, 0x5, 0xa, 0xd, 0xe, 0xf, 0x7, 0x3, 0x1, 0x8};


int TK[4][16]; /* TK[0][], TK[1][], TK[2][], TK[3][] */
int Diff_Out[16];

void Initialize_key(int Key[2][16], int Tweak[16]){

    for (int i = 0; i < 16; i++){
        TK[0][i] = Key[0][i] ^ Tweak[i];
        TK[1][i] = Key[1][i] ^ Tweak[i];
        TK[2][i] = Key[0][i] ^ Tweak[Q[i]];
        TK[3][i] = Key[1][i] ^ Tweak[Q[i]];
    }
}


int Round(int Stt[16], int r)
{
    /* MC */
    for (int i = 0; i < 4; i++){
        Stt[i] ^= (Stt[i + 8] ^ Stt[i + 12]);
        Stt[i + 4] ^= Stt[i + 12];
    }

    /* Add constants */
    Stt[4] ^= RC4[r];
    Stt[5] ^= RC3[r];

    /* Add tweakey */
    for (int i = 0; i < 16; i++)
        Stt[i] ^= TK[r%4][i];


    int Temp[16];

    /* PN */
    for (int i = 0; i < 16; i++)
        Temp[P[i]] = Stt[i];

    /* SB */
    for (int i = 0; i < 16; i++)
        Stt[i] = S[Temp[i]];

	return 0;

}

int Last_round(int Stt[16], int r)
{
    /* MC */
    for (int i = 0; i < 4; i++){
        Stt[i] ^= (Stt[i + 8] ^ Stt[i + 12]);
        Stt[i + 4] ^= Stt[i + 12];
    }

    /* Add constants */
    Stt[4] ^= RC4[r];
    Stt[5] ^= RC3[r];

    /* Add tweakey */
    for (int i = 0; i < 16; i++)
        Stt[i] ^= TK[r%4][i];

	return 0;
}
int Test_Condition_ciphertext(int C0[16], int C1[16])
{
    for (int i = 0; i < 16; i++)
        Diff_Out[i] = C0[i] ^ C1[i];

     if (Diff_Out[ 0] == 0x0 && Diff_Out[ 1] == 0xa && Diff_Out[ 2] == 0x0 && Diff_Out[ 3] == 0x0 &&
         Diff_Out[ 4] == 0x0 && Diff_Out[ 5] == 0x0 && Diff_Out[ 7] == 0x0 && Diff_Out[ 8] == 0x0 && 
	 Diff_Out[ 9] == 0x0 && Diff_Out[11] == 0x0 && Diff_Out[12] == 0x0 && Diff_Out[13] == 0x0 && 
	 Diff_Out[15] == 0x0)
     {
          return 1;
     }
     else {
		 return 0;
	 }
}
int main()
{
    ofstream outfile;
    outfile.open("N_22_independent_union.txt");
    int Key[2][16];
    int Tweak[16];
    int P1[16];
    int P0[16];
    double counter[16];
    int M,N;
    int T[12] = {9,10,11,12,13,14,15,16,17,18,19,20};
    int sum[2][12] = {0};
    int sum_union[12][12] = {0};
    double PS[2][12] = {0};
    double PS_union[12][12] = {0};
    double PS_inde_union[12][12] = {0};
    int Diff[16] = {
	    0x0, 0xa, 0x0, 0x0,
	    0x0, 0x0, 0x0, 0x0,
	    0x0, 0x0, 0xa, 0x0,
	    0x0, 0x0, 0xa, 0x0};
	
    int R0 = 6;              /* Number of rounds */
    double d = pow(2, 22);  /* Number of pairs */
    mt19937_64 mt_rand(time(0));
    for (int ex = 0; ex < 10000; ex++)
    {
	outfile << "This is the " << ex+1 << " experiment" << endl;
	cout << "This is the " << ex+1 << " experiment" << endl;
        for (int i = 0; i < 16; i++)
            counter[i] = 0;
        for (int i = 0; i < 16; i++)
        {
            Key[0][i] = mt_rand()%16;
        }
        for (int i = 0; i < 16; i++)
        {
            Key[1][i] = mt_rand()%16;
        }
        outfile << endl;
        outfile << endl;
        for(int guess = 0; guess < 16; guess++)
        {
            for (double data = 0; data < d; data++)
            {
                for (int i = 0; i < 16; i++)
                     P0[i] = mt_rand()%16;
                for (int i = 0; i < 16; i++)
                     P1[i] = P0[i] ^ Diff[i];
                for (int i = 0; i < 16; i++)
                     Tweak[i] = mt_rand()%16;
                Tweak[6] = guess; 
		Tweak[12] = guess;
                Initialize_key(Key, Tweak);
                for (int r = 0; r < R0; r++)
                     Round(P0, r);
                for (int r = 0; r < R0; r++)
                     Round(P1, r);
                if (Test_Condition_ciphertext(P0, P1))
                {
                     counter[guess] = counter[guess] + 1;
                }
	    }
	}
	    M = counter[Key[1][12]];
	    N = counter[Key[1][12]^0xa];
	    outfile << "The counter corresponding to the guessed key value - " << Key[1][12] << " : "
                    << "Get " <<M<< " correct pairs"  <<  endl;
	    outfile << "The counter corresponding to the guessed key value - " << (Key[1][12]^0xa) <<" : "
                    << "Get " <<N<< " correct pairs"  <<  endl;
	    cout << "The counter corresponding to the guessed key value - " << Key[1][12] << " : "
                        << "Get " <<M<< " correct pairs"  <<  endl;
	    cout << "The counter corresponding to the guessed key value - " << (Key[1][12]^0xa) <<" : "
                 << "Get " <<N<< " correct pairs"  <<  endl;
	    cout << endl;
	    for(int i = 0; i < 12; i++)
	    {
		    if(M == T[i])
	            sum[0][i] = sum[0][i] + 1;
		    if(N == T[i])
	            sum[1][i] = sum[1][i] + 1;
	    }
	    for(int i = 0; i < 12; i++)
	    {
		    for(int j = 0; j < 12; j++)
		    {
			    if(M == T[i] && N == T[j])
				    sum_union[i][j] = sum_union[i][j] + 1;
		    }
	    }
    }
   outfile << "T1  ";
   for(int i = 0; i < 12; i++)
   {
	   outfile << T[i] << " ";
   }
   outfile << endl;
   outfile << "sum2  ";
   for(int i = 0; i < 12; i++)
   {
	   outfile << sum[0][i] <<"   ";
   }
   outfile << endl;
   outfile << "---------------------------------------------------------------" << endl;
   outfile << endl;
   outfile << "T2  ";
   for(int i = 0; i < 12; i++)
   {
       outfile << T[i] << " ";
   }
   outfile << endl;
   outfile << "sum2  ";
   for(int i = 0; i < 12; i++)
   {
       outfile << sum[1][i] << "   ";

   }
   outfile << endl;
   outfile << "---------------------------------------------------------------" << endl;
   outfile << endl;
   for(int i = 0; i < 2; i++)
  {
	   for(int j = 0; j < 12; j++)
	{
		   PS[i][j] = float(sum[i][j]) / float(10000);
	}
}
   int log_d = log(d)/log(2);
   outfile << "T1  ";
   for(int i = 0; i < 12; i++)
   {
	   outfile << T[i] << " ";
   }
   outfile << endl;
   outfile << "PS1  ";
   for(int i = 0; i < 12; i++)
   {
	   outfile << PS[0][i] << "   ";
   }
   outfile << endl;
   outfile << "---------------------------------------------------------------"<<endl;
   outfile << endl;
   outfile << "T2  ";
   for(int i = 0; i < 12; i++)
   {
	   outfile << T[i] << " ";
   }
   outfile << endl;
   outfile << "PS2  ";
   for(int i = 0; i < 12; i++)
   {
	   outfile << PS[1][i] << "   ";
   }
   outfile << endl;
   outfile << endl;
   for(int i = 0; i < 12; i++)
   {
	   for(int j = 0; j < 12; j++)
	   {
		   PS_inde_union[i][j] = PS[0][i] * PS[1][j];
	   }
   }
   outfile << "T ";
   for(int i = 0; i < 12; i++)
   {
	  outfile << T[i] << " ";
   }
   outfile << endl;
   for(int i = 0; i < 12; i++)
   {
	   outfile << T[i] << " ";
	   for(int j = 0; j < 12; j++)
	   {
		outfile << sum_union[i][j] << "  ";
	   }
	   outfile << endl;
   }
	outfile << "---------------------------------------------------------------" << endl;
	for(int i = 0;i < 12; i++)
	{
		for(int j = 0; j < 12; j++)
		{
			PS_union[i][j] = float(sum_union[i][j]) / float(10000);
		}
	}
	outfile << "T ";
	for(int i = 0; i < 12; i++)
	{
		outfile << T[i] << " ";
	}
	outfile << endl;
	for(int i = 0; i < 12;i++)
	{
		outfile << T[i] << " ";
		for(int j = 0; j < 12; j++)
		{
			outfile << PS_union[i][j] << "  ";
		}
		outfile << endl;
	}
	outfile << "---------------------------------------------------------------" << endl;
        outfile << endl;
        outfile << "T ";
	for(int i = 0; i < 12; i++)
	{
		outfile<< T[i] << " ";
	}
	outfile << endl;
	for(int i=0;i < 12; i++)
	{
		outfile << T[i] << " ";
		for(int j = 0; j < 12; j++)
		{
			outfile << PS_inde_union[i][j] << "  ";
		}
	        outfile << endl;
	}
	system("pause");
	return 0;
}




