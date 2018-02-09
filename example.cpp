#include "ModelChecker.h"
#include "checker.hpp"
//#include "../../Instrument.h"
//#include "../../Executor.h"

#include <vector>
#include <iostream>
#include <thread>
#include <atomic>
#include <cassert>
 
using namespace checker;

int data1, data2, data3, data4;
std::atomic<int> x, y, z;
 
void f1()
{
    checker_thread_begin("f1");
   
    //data1 = z;
    if (data1 > 0) {
        x = data4;
        data2 = data4;
    }

    //if (data2 > 0)
    //    data2++;

    data3 = data2;
    data1 = x;
/*    y = data2-1;
    y = 2;*/
    
    checker_thread_end();
}

void f2()
{
    checker_thread_begin("f2");

    if (z > 2)
        x = 3;
    z = 1;
    data3 = y;

    checker_thread_end();
}
 
/*int main() {
    f1();
    if (data1 > 0) //{
        if (data2 > 0)
            x = 1;
    //} else
    //    x = 2;

    f2();
    y = 2;
    data1 = x;

    return 0;
}*/
 
int user_main()
{
    checker_generateExecutor();
    checker_thread_begin("main");

    checker_shared(&data1);
    checker_shared(&data2);
    checker_shared(&data3);
    x = 0, y = 0, z = 0;

    std::thread b(f2);
    std::thread a(f1);
    
    checker_thread_create(a.get_id());
    checker_thread_create(b.get_id());

    //f1();
    //f2();

    checker_thread_join(a.get_id());
    checker_thread_join(b.get_id());
    
    a.join(); b.join();
    printf("data: %d, %d, %d\n", data1, data2, data3);
    checker_thread_end();
    checker_solver();
    return 0;
}

int main() {
    modelChecker = new ModelChecker();
    user_main();
    
    while (modelChecker->getSchList().size()) 
        user_main();

    delete modelChecker;

    return 0;
}

/*#include <assert.h>
#include <stdio.h>

long int fact(int x)
{
    long int r = x;
    while (--x >=2)
        r *= x;

    return r;
}

int main(void)
{
    int a, b, c = 7;

    while (scanf("%d", &a) > 0) {
        assert(a > 0);
        printf("fact: %lu\n", fact(a));
    }

    return 0;
}*/
