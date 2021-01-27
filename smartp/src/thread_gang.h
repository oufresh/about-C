/**
 * @file thread_gang.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-01-27
 * 
 * @copyright Copyright (c) 2021
 * 
 * Shared pointer risolve il problema della condivisione della ownership di un oggetto tra N scopes,
 * il cui controllo può essere distribuito su M <= N threads.
 * In altre parole: gli scopes possono essere distribuiti tutti su threads differenti,
 * oppure, per esempio, anche essere tutti sotto il controllo di un unico thread.
 * Attenzione! Shared pointer non entra nel merito di ciò che si vuole condividere,
 * quello è esclusivamente un nostro affare!
 * Non aspettiamoci, ad esempio, che shared pointer fornisca un meccanismo implicito di sincronizzazione
 * tra i threads per l’oggetto che si va a condividere.
 * Tutto ciò che è lecito aspettarsi, è che shared pointer si preoccupi per noi di capire
 * quando l’oggetto condiviso non sia più accessibile da nessuno scope prendendosi carico di rilasciare le risorse quando serve.
 * 
 */

#ifndef _THG
#define _THG

void thread_gang_test();
#endif