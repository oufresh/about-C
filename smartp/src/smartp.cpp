
#include <iostream>
#include <memory>
#include <random>

#include "user.h"

/**
 * @brief Gli smart pointers Forniscono un modello di controllo della memoria dinamica efficace,
 * senza dover pagare il costo di un garbage collector in esecuzione nel nostro programma.
 * L’idea vincente di uno smart pointer rispetto al classico raw pointer è l’aggiunta dell’informazione
 * necessaria per capire quando la risorsa può essere rilasciata.
 * A prima vista può sembrare poco, ma in realtà la differenza è sostanziale.
 * **/

/**
 * @brief una risorsa acquisita, in questo caso memoria dinamica (mediante l’operatore new<int>),
 * è costruito come una variabile automatica direttamente nello scope. Solo così il compilatore
 * ci garantisce che ad ogni possibile uscita dallo scope stesso, il distruttore dell’oggetto  quindi il wrapper) sia effettivamente chiamato.
 * L’uscita forzata dallo scope mediante returncopre anch’essa la chiamata al distruttore di uiptre sarebbe lo stesso anche se l’uscita dallo
 * scope fosse determinata da una throw.
 * 
 * Un tipo move-only, guarda caso, è proprio uno smart pointer: std::unique_ptr è stato pensato per risolvere il problema dell’exclusive ownership di 
 * puntatore a memoria dinamicamente acquisita.
 */
void testUnique()
{
    //scope begin
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 generator(rd()); // seed the generator
    std::uniform_int_distribution<int> distribution(1,100);
    std::unique_ptr<int> uiptr(new int(0));

    *uiptr = distribution(generator);

    if (*uiptr < 50)
    {
        std::cout << "Exit before function end: " << *uiptr << ", scope out anyway" << std::endl;
        return;
    }

    std::cout << "End of function and scope: " << *uiptr << std::endl;

    std::unique_ptr<int> uiptr_2 = std::move(uiptr);  //now it's ok, we want to move it!
    //copy not permited , only move on smartpointers: uiptr is a lvalue temp copy is created but get a compile error 
    //std::unique_ptr<int> uiptr_2 = uiptr;

    //unique_ptr defs cannot permit copy, only move
    //move assignment    
    //unique_ptr& operator= (unique_ptr&& x) noexcept;

    //copy assignment (deleted!)
    //unique_ptr& operator= (const unique_ptr&) = delete; 

    //scope end
}

void testUserClass()
{
    User user = User("Pippo");
    user.setSalary(1000);
    std::cout << "Salary is" << user.getSalary() << std::endl;

}

int printSmartPMenu()
{
    int choice;
    std::cout << std::endl
              << "Smart pointers Menu" << std::endl;
    std::cout << "Please make your selection\n";
    std::cout << "1 - Test unique_ptr\n";
    std::cout << "2 - Test User class with internal unique_ptr\n";
    std::cout << "0 - Bacck to main menu\n";
    std::cout << "Selection: ";
    std::cin >> choice;

    return choice;
}

void smartpMenu()
{
    int choice;

    do
    {
        choice = printSmartPMenu();

        switch (choice)
        {
        case 1:
            std::cout << "---- Test unique_ptr ----\n";
            testUnique();
            break;
        case 2:
            std::cout << "---- Test unique_ptr ----\n";
            testUserClass();
            break;
        default:
            break;
        }
    } while (choice != 0);
}