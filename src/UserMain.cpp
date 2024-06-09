
//Created by kosta on 9.8.23.

#include "../test/printing.hpp"
#include "../h/syscall_cpp.hpp"
static volatile bool finishedA = false;
static volatile bool finishedB = false;
static uint64 fibonacci(uint64 n)

{

   if (n == 0 || n == 1) { return n; }

    if (n % 4 == 0) thread_dispatch();

   return fibonacci(n - 1) + fibonacci(n - 2);

}


void workerBodyA(void*)

{

   uint8 i = 0;

   for (; i < 3; i++)

    {

       printString("A: i=");

       printInt(i);

       printString("\n");

    }


    printString("A: yield\n");

    __asm__ ("li t1, 7");

    thread_dispatch();


    uint64 t1 = 0;

    __asm__ ("mv %[t1], t1" : [t1] "=r"(t1));


    printString("A: t1=");

    printInt(t1);

    printString("\n");


    uint64 result = fibonacci(20);

    printString("A: fibonaci=");

    printInt(result);

    printString("\n");


    for (; i < 6; i++)

    {

        printString("A: i=");

        printInt(i);

        printString("\n");

    }



    finishedA = true;
    thread_dispatch();

}


void workerBodyB(void*)

{

    uint8 i = 10;

    for (; i < 13; i++)

    {

        printString("B: i=");

        printInt(i);

        printString("\n");

    }


    printString("B: yield\n");

    __asm__ ("li t1, 5");

    thread_dispatch();


    uint64 result = fibonacci(23);

    printString("A: fibonaci=");

    printInt(result);

    printString("\n");


    for (; i < 16; i++)

    {

        printString("B: i=");

        printInt(i);

        printString("\n");

    }

    finishedB = true;

    thread_dispatch();

}
void usermain() {
        new Thread(workerBodyA,nullptr);

        printString("CoroutineA created\n");

        new Thread(workerBodyB,nullptr);

        printString("CoroutineB created\n");
    while (!(finishedA && finishedB)) {
        Thread::dispatch();
    }
        printString("Finished\n");

}