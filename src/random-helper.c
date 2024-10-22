#include <stdio.h>
#include <math.h>

//hex conversion macro for a byte, uses a short for -1 Just In Case
#define hex(ch) (((ch >= 48) ? ((ch <= 57 ) ? ch-'0' : ((ch >= 65) ? ((ch<=70) ? ch-55: ((ch>=97) ? ((ch<=102) ? ch-87 : -1) : -1)) : -1)) : -1))
//g_op: used for convenience with options to determine how to compare to mins array
#define g_op(sum,opt,mins) (opt&0x1) ? sum>mins[1] : sum<mins[1]

//histogram of all possible bytes
int hist[256] = {0,};
int opt = 0;

//Takes a list of hex strings, calculates entropy, and gives the lowest or highest depending on option set
//- flag is least, + flag is greatest; ignores whatever comes after (use spaces!1)
int main(int argc, char **argv){
    if (argc < 2) return 1;
    else switch(**(argv+1)){
        case('+'): opt = 1; break;
        case('-'): opt= -2; break;
        default: break;
    }
    double mins[2] = {-1,(opt&0x1) ? __DBL_MIN__ : __DBL_MAX__};//Used to store lowest/highest entropy string
    for (int i = (opt&0x80000001)? 2 : 1; i < argc; i++){//Moves through each string
        char *curr = *(argv+i);//curr: current string
        int len = 0;//Reads length for use later
        while (*curr!='\0') {//For this function we'll assume that if there is an odd number of bytes that the null terminator is used as padding (highest order bytes first)
            short byte = hex(*curr) << 4 | hex(*(curr+1));//convert the byte to hex; it will be -1 if an invalid hex code
            if (byte < 0) return 1;
            curr+=2;//increment pointer to look at next byte
            hist[byte]++;//increment histogram bar
            len++;//resultant string is one byte longer
        }
        double sum = 0.0;
        for (int j = 0; j<256; j++){//Shannon entropy formula - use len and histogram to find individual probabilities and sum them
            if (hist[j] > 0) sum-=(((double)hist[j])/((double)len)*(log(((double)hist[j])/((double)len))/log(2.0)));
            hist[j] = 0;
        }
        //prints probabilities
        fprintf(stdout,"%s: %f (length %d), efficiency %f\n",*(argv+i), sum, len, sum/256);
        //sets max/min depending on setting
        if (g_op(sum,opt,mins)){
            mins[0] = i;
            mins[1] = sum;
        }
    }
    //prints lowest/highest entropy string
    fprintf(stdout,"\nBest guess: %s (entropy %f)\n",*(argv+(int)mins[0]),mins[1]);
    return 0;
}