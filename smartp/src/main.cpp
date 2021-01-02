#include <cstdlib>
#include <iostream>
#include <fcntl.h>
#include <io.h>

#include "move.h"
#include "heapCorruption.h"
#include "smartp.h"
#include "traits.h"

ArrayWrapper arr = ArrayWrapper(10);

ArrayWrapper && getWrapper()
{
    return std::move(arr);
}

void printAddress (const ArrayWrapper& v)
{
    std::cout << reinterpret_cast<const void*>(&v) << std::endl;
}
void testMoveRvalue()
{
    std::cout << "Test move constructor... same address expected in the global and rvalue returned because of move, not copy" << std::endl;
    printAddress(arr);
    printAddress(getWrapper());

    std::cout << "Force move ctor"  << std::endl;
    ArrayWrapper a2(std::move(arr));
}

void testLamba()
{
    //lambda test
    int m = 0;
    int n = 0;
    [&, n](int a) mutable { m = ++n + a; }(4); //n by value, mm implicit by reference

    //type function lamda auto

    std::wcout << L"test lambda" << std::endl;
    auto myIncr = [&, n](int a) mutable { m = ++n + a; };
    std::cout << m << std::endl
              << n << std::endl;

    std::wcout << L"test ptr" << std::endl;
    int *test = new int(0);
    auto myChange = [=](int v) { *test = v; }; //can use by value = or by reference & becuse I chnge only the vlue pointed not the address,
    //mutable no used because doesn't change the
    //valu e of the poiinter but the pointed memory value
    std::wcout << L"value before: " << *test << std::endl;
    myChange(33);

    std::wcout << L"value changed: " << *test << std::endl;
    delete test;
}

void testUnicode()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    std::wstring str = L"Ciao è tempo di unicode per provare gli smart pointers! Peccato che su win non siriesce a far andare ... :(";
    std::wcout << str << std::endl;
    std::wcout << "Size = " << ((str).size() * sizeof(wchar_t)) << std::endl;
}

int printMenu()
{
    int choice;
    std::cout << std::endl << "Main Menu" << std::endl;
    std::cout << "Please make your selection\n";
    std::cout << "1 - Test lambda\n";
    std::cout << "2 - Test unicode\n";
    std::cout << "3 - Test move constructor\n";
    std::cout << "4 - Test heap corruption with raw pointers\n";
    std::cout << "5 - Test smart pointers\n";
    std::cout << "6 - Test traits\n";
    std::cout << "0 - Quit\n";
    std::cout << "Selection: ";
    std::cin >> choice;

    return choice;
}

int main(int argc, char **argv)
{

    int choice;

    do
    {
        choice = printMenu();

        switch (choice)
        {
        case 1:
            std::cout << "---- Test lambda ----\n";
            testLamba();
            break;
        case 2:
            std::cout << "---- Test unicode----\n";
            testUnicode();
            break;
        case 3:
            testMoveRvalue();
            break;
        case 4:
            std::cout << "---- Test heap corruption ----" << std::endl;
            testHeapCorruption();
            break;
        case 5:
            std::cout << "---- Test smart pointers ----" << std::endl;
            smartpMenu();
            break;
            case 6:
            std::cout << "---- Test traits ----" << std::endl;
            traitspMenu();
            break;
        default:
            break;
        }
    } while (choice != 0);

    system("PAUSE");
    return EXIT_SUCCESS;
}