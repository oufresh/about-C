#ifndef _MOVE
#define _MOVE


#include <iostream>

class ArrayWrapper
{
public:

    // il costruttore di default crea un array dale dimensioni moderate
  ArrayWrapper()
    : _p_vals(new int[ 64 ])
    ,_size(64)
    {}

    ArrayWrapper(int n)
        : _p_vals(new int[n]), _size(n)
    {
    }

    // copy constructor
    ArrayWrapper(const ArrayWrapper &other)
        : _p_vals(new int[other._size]), _size(other._size)
    {
        for (int i = 0; i < _size; ++i)
        {
            _p_vals[i] = other._p_vals[i];
        }
    }

    //move constructor: prendo un rvlue(oggetto temporaneo e mi sposto i dati con maggiore efficenza) parametro non costante perchè da modificare
    ArrayWrapper(ArrayWrapper&& other)
        : _p_vals(other._p_vals)
        , _size(other._size)
    {
        other._p_vals = nullptr;  //must be null l'oggetto tmporaneo verrà poi distrutto, dvo assicurarmi di nulllare per no distruggere la memoria spostata
        ///If expression evaluates to a null pointer value, no destructors are called, and the deallocation function is not called.
        other._size = 0;
        std::cout << "Move ctor called" << std::endl;
    }


    ~ArrayWrapper()
    {
        delete[] _p_vals;
    }

private:
    int *_p_vals;
    int _size;
};

#endif