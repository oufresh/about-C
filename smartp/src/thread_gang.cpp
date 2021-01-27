
/**
 * @file thread_gang.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-01-27
 * 
 * @copyright Copyright (c) 2021
 * 
 * Bene, vediamo prima a grandi linee quello che dovrebbe fare il codice.
 * L’idea è quella di condividere un oggetto di tipo monitortra ben 500 threads
 * ognuno dei quali vuole effettuare una operazione di qualche tipo su quest’unica istanza.
 * L’operazione effettuata sull’oggetto condiviso è banale: ogni thread tira a caso una moneta e se viene,
 * mettiamo testa, il membro count_di monitorviene incrementato di 1, altrimenti si decrementa di 1.
 * Il main thread attende l’esecuzione di tutti quanti i threads figli per poi stampare a video il valore finale di count_.
 * 
 * Supponendo che il thread che condivide un oggetto allocato sullo heap con altri threads non effettui
 * la join perchè ha bisogno di fare altro, a chi spetta l’onere di rilasciare l’oggetto precedentemente allocato e poi condiviso?
 * Bella domanda! Chi lo sà? Nessun thread che riceve l’oggetto condiviso può sapere se qualche altro attore
 * non deve o non dovrà usare quell’oggetto.

 * Perchè non usare per esempio, il membro count_dell’oggetto token?
 * Magari riusciamo a estrapolare qualche logica su questo campo per capire quando l’istanza condivisa non sarà più usata da nessuno!
 * Purtroppo però, ci accorgiamo ben presto che dal membro count_difficilmente riusciremo a ideare una strategia valida per decidere
 * che è il momento di sbarazzarci di token, quel campo viene aggiornato con una politica random!
 * Niente da fare, nessun campo applicativo di tokenpuò essere usato per capire l’oggetto può essere rilasciato.
 *
 * POTREMMO METTERE UN CAMPO NON APPLICATIVO APPOSTA PER QUESTO SCOPO!
 * Si, potremmo e sicuramente con le dovute accortezze il meccanismo funzionerebbe.
 * C’è però un problema, anzi più di uno:

 * Dobbiamo sporcare la nostra implementazione di tokencon del codice accessorio, tecnico, non applicativo,
 * per gestire un aspetto che nulla ha a che vedere con il business del tipo.
 * Dobbiamo farlo per tutti i tipi le cui istanze intendiamo condividere con N threads.
 * Nel mondo reale il software è scritto da N programmatori e ognuno, se siamo fortunati, implementa il suo meccanismo
 * per gestire la deallocazione delle istanze condivise.
 * Se siamo sfortunati, il problema non è stato nemmeno preso in considerazione.
 * Difficilmente i programmatori implementeranno una soluzione ingegnerizzata per il problema e il codice avrà una miriade di bugs
 * subdoli e difficili da individuare.
 * Non è una questione di bravura, il programmatore più bravo del mondo quando deve risolvere un problema in ambito industriale
 * ha poco tempo per scrivere bene il codice.
 * Le problematiche di contorno, extra business, sono spesso abbozzate per prediligere, come è giusto che sia, l’aspetto applicativo.
 */

#include <iostream>
#include <thread>
#include <random>
#include <memory>
#include <mutex>
#include <algorithm>
#include <vector>

//a single monitor object will be shared
//across all spawned threads
struct monitor
{
    monitor(int count) : count_(count) {}
    int count_;
    std::mutex mx_;
};

void thread_gang_test()
{
    //we create a shared pointer of a newly monitor object,
    //this call would be equivalent to: std::shared_ptr<monitor> token(new monitor(40));
    auto token = std::make_shared<monitor>(40);

    //we create a vector of 500 threads, none of them yet constructed!
    //we need a reference to each thread because later we want to join all of them.
    std::vector<std::unique_ptr<std::thread>> threads(500);

    //outer for_each lambda captures context by *reference* [&]
    std::for_each(threads.begin(), threads.end(), [&](auto &it) {
        //inner thread-ctor-lambda captures context by *value* [=] ,
        //this is crucial, because we want to *copy* the shared pointer: one for a thread.
        it.reset(new std::thread([=]() {
            std::random_device generator;
            std::uniform_real_distribution<double> distribution(0.0, 1.0);
            //a scoped lock used to access count_ in an exclusive manner.
            std::unique_lock<std::mutex> lk(token->mx_);

            //we increment count_ by 1 or decrement it by 1 with a random policy.
            token->count_ = distribution(generator) < 0.5 ? token->count_ - 1 : token->count_ + 1;

            std::cout << " thread:" << std::this_thread::get_id()
                      << " set count:" << token->count_
                      << " use_count is:" << token.use_count()
                      << std::endl;
        }));
    });

    //we wait for every thread to complete, no capture []
    std::for_each(threads.begin(), threads.end(), [](auto &it) {
        it->join();
    });

    //at this point we know that use_count will be equals to 1
    //because all threads have finished their execution.
    std::cout << "main thread reads count:" << token->count_
              << " use_count is:" << token.use_count()
              << std::endl;
}