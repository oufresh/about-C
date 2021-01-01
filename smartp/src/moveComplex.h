#ifndef _MOVEC
#define _MOVEC

#include <string>
#include <utility>

class MetaData
{
public:
    MetaData(int size, const std::string &name)
        : _name(name), _size(size)
    {
    }

    // copy constructor
    MetaData(const MetaData &other)
        : _name(other._name), _size(other._size)
    {
    }

    // move constructor
    MetaData(MetaData &&other)
        : _name(other._name), _size(other._size)
    {
    }

    MetaData &operator=(MetaData &&other)
    {
        // non eseguiamo alcuna operazione se se si tenta di assegnare l'oggetto a se stesso
        if (this != &other)
        {
            // preleviamo le risorse
            _name = std::move(other._name);
            _size = other._size;
        }
        return *this;
    }

    std::string getName() const
    {
        return _name;
    }

    int getSize() const
    {
        return _size;
    }

private:
    std::string _name;
    int _size;
};

class ArrayWrapperComplex
{

public:
    ArrayWrapperComplex()
        : _p_vals(new int[64]), _metadata(64, "ArrayWrapperComplex")
    {
    }

    ArrayWrapperComplex(int n)
        : _p_vals(new int[n]), _metadata(n, "ArrayWrapperComplex")
    {
    }

    // move constructor
    ArrayWrapperComplex(ArrayWrapperComplex &&other)
        : _p_vals(other._p_vals), _metadata(std::move(other._metadata))
    //chiama il copy constructor perchè nell'oggetto temporaneo ha un locazione precisa ed è un lvalue
    //è possiibile indirizzarlo e vive nello scope di questa funzione, se venisse già chiamato il suo move torveremo dentro roba già spostata, come fare?
    // se _metadata( other._metadata ) chiamasse il move constructor, usare
        // other._metadata d’ora in poi sarebbe estremamente pericoloso, mi interessa però solo farne l a move e non usarlo
    // poter trasformare un lvalue in un rvalue, in modo che il move constructor venga chiamato.
    //La funzione std::move esegue proprio questa operazione, non spostando nulla in realtà, ma semplicemente eseguendo un cast
        //Essendo senza nome, questo rvalue reference è temporaneo e “attiva” il move constructor
    {
        
        other._p_vals = nullptr;
    }

    // copy constructor
    ArrayWrapperComplex(const ArrayWrapperComplex &other)
        : _p_vals(new int[other._metadata.getSize()]), _metadata(other._metadata)
    {
        for (int i = 0; i < _metadata.getSize(); ++i)
        {
            _p_vals[i] = other._p_vals[i];
        }
    }

    ArrayWrapperComplex &operator=(ArrayWrapperComplex &&other)
    {
        // non eseguiamo alcuna operazione se se si tenta di assegnare l'oggetto a se stesso
        if (this != &other)
        {

            // liberiamo la memoria occupata dall’oggetto corrente
            delete[] _p_vals;
            // preleviamo le risorse
            _p_vals = other._p_vals;
            _metadata = std::move(other._metadata);
            // impostiamo l’oggetto ricevuto come parametro in uno stato innocuo
            other._p_vals = NULL;
        }
        return *this;
    }
    ~ArrayWrapperComplex()
    {
        delete[] _p_vals;
    }

private:
    int *_p_vals;
    MetaData _metadata;
};

#endif