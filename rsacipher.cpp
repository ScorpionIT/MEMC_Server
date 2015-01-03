#include "rsacipher.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

static int pickAPrimeNumber();
static bool isPrime( int n );
static bool areTheyCoprime( int a, int b );
static int computePivateKey( int publicKey2, int z );

int RSACipher::getPrivateKey() const
{
    return this->privateKey;
}

RSACipher::RSACipher()
{
    this->primeNumber1 = pickAPrimeNumber();
    this->primeNumber2 = pickAPrimeNumber();

    this->publicKey1 = primeNumber1 * primeNumber2;
    int z = ( primeNumber1 - 1 ) * ( primeNumber2 - 1 );

    do
    {
        this->publicKey2 = pickAPrimeNumber();
    }while( ! areTheyCoprime( publicKey2, z ) );

    this->privateKey = computePivateKey( publicKey2, z );



}

int RSACipher::getPublicKey1() const
{
    return this->publicKey1;
}

int RSACipher::getPublicKey2() const
{
    return this->publicKey2;
}

RSACipher::~RSACipher()
{

}

static int pickAPrimeNumber()
{
    srand (time(NULL));

    int number = rand() % 400 + 1;

    while( !isPrime( number ) )
        number++;

    return number;
}

static bool isPrime( int n )
{
    for( int i = 0; i < sqrt( n ); i++ )
        if( n % i == 0 )
            return false;

    return true;
}

static bool areTheyCoprime( int a, int b )
{
    int lowest;

    a < b ? lowest = a : lowest = b;

    for( int i = 3; i <= lowest; i++ )
        if( a % i == 0 && b % i == 0 )
            return false;

    return true;
}

static int computePivateKey( int publicKey2, int z )
{
    for( int i = 0; i < z; i++ )
        if( ( publicKey2 * i ) % z == 1 )
            return i;
}
