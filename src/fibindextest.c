#include <stdio.h>
#include <math.h>

int FIBONACCI_INDEX(int x){
    return ((int)(log(((double)x+0.5)*sqrt(5))/log(1.618)))-2;
}
/*
Given a number, returns the index in the Fibonacci sequence at which it occurs (next-lowest for intermediate numbers)
Takes any sequence of decimal numbers as input
*/
int main(char argc, char **argv){
    for (int i = 1; i < argc; i++){
        char * currstr = *(argv+i);
        int tot = 0;
        while (*currstr){
            tot = tot*10 + (*currstr - '0');
            currstr++;
        }
        fprintf(stderr,"%f\n",FIBONACCI_INDEX(tot));
    }
    return 0;
}

