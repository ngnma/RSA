#include <stdio.h>

// memory map
volatile unsigned long long int *gcd_inp1 = (unsigned long long int *) 0x80000000;
volatile unsigned long long int *gcd_inp2 = (unsigned long long int *) 0x80000008;
volatile unsigned long long int *gcd_out = (unsigned long long int *) 0x80000010;
volatile unsigned  int *gcd_load = (unsigned int *) 0x80000018;
volatile unsigned  int *gcd_done = (unsigned int *) 0x8000001c;
volatile unsigned long long int *power_x = (unsigned long long int *) 0x80000020;
volatile unsigned long long int *power_y = (unsigned long long int *) 0x80000028;
volatile unsigned long long int *power_m = (unsigned long long int *) 0x80000030;
volatile unsigned long long int *power_outp = (unsigned long long int *) 0x80000038;
volatile unsigned  int *power_load = (unsigned int *) 0x80000040;
volatile unsigned  int *power_done = (unsigned int *) 0x80000044;

// prototypes
unsigned long long int gcd(unsigned long long int, unsigned long long int);
unsigned long long power(unsigned long long, unsigned long long, unsigned long long);
unsigned int calculate_e(unsigned int z);
unsigned int calculate_d(unsigned int z, unsigned int e);
unsigned long long encryption(unsigned long long msg, unsigned int e, unsigned int n);
unsigned long long decryption(unsigned long long c, unsigned int d, unsigned int n);

int main(){

    // setup private and public keys
    unsigned int p = 23;
    unsigned int q = 2;
    unsigned int n = p*q;
    unsigned int z = (p-1)*(q-1);
    unsigned int e = calculate_e(z);
    unsigned int d = calculate_d(z, e);

    // Message to be encrypted
    unsigned long long msg = 26;
    printf("Message data = %lld\n", msg);

    unsigned long long cypher = encryption(msg,e,n);
    decryption(cypher,d,n);

    return 0;
}

unsigned long long int gcd(unsigned long long int a, unsigned long long int b){
    *gcd_inp1 = a;
    *gcd_inp2 = b;
    *gcd_load = 1;
    while (*gcd_done !=1 );
    long long int result = *gcd_out;
    *gcd_load = 0;
    return result;
}

unsigned long long power(unsigned long long x, unsigned long long y, unsigned long long m){
    *power_x = x;
    *power_y = y;
    *power_m = m;
    *power_load = 1;
    while (*power_done !=1 );
    long long int result = *power_outp;
    *power_load = 0;
    return result;
}

unsigned int calculate_e(unsigned int z){
    unsigned int e = 2;
    while (e < z) {
        // e must be co-prime to z and smaller than z
        if (gcd(e, z)==1)
            break;
        else
            e++;
    }
    return e;
}
unsigned int calculate_d(unsigned int z, unsigned int e){
    unsigned int d = 2;
    while (d < z) {
        if ((d*e)%z == 1)
            break;
        else
            d++;
    }
    return d;
}

unsigned long long encryption(unsigned long long msg, unsigned int e, unsigned int n){
    unsigned long long cypher  = power(msg, e, n);
    printf("Encrypted data = %lld\n", cypher);
    return cypher;
}

unsigned long long decryption(unsigned long long c, unsigned int d, unsigned int n){
    unsigned long long msg = power(c, d, n);
    printf("Original Message Sent = %lld\n", msg);
    return msg;
}

