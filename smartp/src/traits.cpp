/**
 * @file traits.cpp
 * @author oufresh
 * @brief 
 * @version 0.1
 * @date 2021-01-02
 * 
 * @copyright Copyright (c) 2021
 * 
 * Think of a trait as a small object whose main purpose is to carry information used by another object
 * or algorithm to determine "policy" or "implementation details".
 * 
 * Traits allow you to get information about a type during compilation.
 * The fact that traits must work with built-in types means that things like nesting information
 * inside types won't do, because there's no way to nest information inside pointers.
 * The traits information for a type, then must be external to the type.
 * The standard technique is to put it into a template and cone or more specializations
 * of that template.
 * By convention, traits are always implemented as structs.
 * Another convention is that the structs used to implement traits are knows
 * as trait classes.
 */

#include <iostream>
#include <vector>
#include <memory>
#include <limits>

void numericLimits()
{

    std::cout << "max values\n";
    std::cout << "int: " << std::numeric_limits<int>::max() << std::endl;
    std::cout << "double: " << std::numeric_limits<double>::max() << std::endl;
    std::cout << "float: " << std::numeric_limits<float>::max() << std::endl;
    std::cout << "unsigned int: " << std::numeric_limits<unsigned int>::max() << std::endl;
}

template <class T>
T findMax(std::vector<T> &data,
          const size_t numItems)
{
    // Obtain the minimum value for type T
    T largest =
        std::numeric_limits<T>::min();
    for (unsigned int i = 0; i < numItems; ++i)
        if (data[i] > largest)
            largest = data[i];
    return largest;
}

void testMax()
{
    std::vector<int> data(50);
      // assign some values:
    for (unsigned i=0; i<50; i++) data[i]=i;

    int max = findMax(data, 50);

    std::cout << "Found max "  << max << std::endl;
}

int printTraitsMenu()
{
    int choice;
    std::cout << std::endl
              << "Traits Menu" << std::endl;
    std::cout << "Please make your selection\n";
    std::cout << "1 - Test numeric limits traits\n";
    std::cout << "2 - Test find max traits\n";
    std::cout << "0 - Bacck to main menu\n";
    std::cout << "Selection: ";
    std::cin >> choice;

    return choice;
}

void traitspMenu()
{
    int choice;

    do
    {
        choice = printTraitsMenu();

        switch (choice)
        {
        case 1:
            std::cout << "---- Test numeric limits traits ----\n";
            numericLimits();
            break;
            case 2:
            std::cout << "---- Test find maxtraits ----\n";
            testMax();
            break;
        default:
            break;
        }
    } while (choice != 0);
}