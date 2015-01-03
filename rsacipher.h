#ifndef RSACIPHER_H
#define RSACIPHER_H

#include "connection.h"

class RSACipher
{
private:
    int primeNumber1, primeNumber2, publicKey1, publicKey2, privateKey;

    int getPrivateKey() const;

public:
    RSACipher();
    int getPublicKey1() const;
    int getPublicKey2() const;
    ~RSACipher();

    friend class Connection;
};

#endif // RSACIPHER_H
