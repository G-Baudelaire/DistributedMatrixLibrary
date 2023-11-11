#include <stdio.h>
#include <pthread.h>

void hello(int *n) {
    printf("hello %i", *n);
}


//int main(void) {
//    int m1, m2;
//    pthread_t thr1, thr2;
//
//    m1 = 1;
//    pthread_create(&thr1, NULL, (void *(*)(void *)) hello, (void *) &m1);
//    m2 = 2;
//    pthread_create(&thr2, NULL, (void *(*)(void *)) hello, (void *) &m2);
//    pthread_join(thr1, NULL);
//    pthread_join(thr2, NULL);
//    return 0;
//}