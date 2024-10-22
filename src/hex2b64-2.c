#include <stdio.h>
#include <string.h>

#define HEX_TO_DEC(x) ()

const unsigned char BASE64[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";



int main(int argc, char **argv){
    const unsigned char hex[] = "\x54\x68\x69\x6E\x67\x73\x20\x74\x68\x61\x74\x20\x68\x61\x76\x65\x20\x6E\x65\x76\x65\x72\x20\x62\x65\x65\x6E\x20\x64\x6F\x6E\x65\x20\x62\x65\x66\x6F\x72\x65\x2E";
    int hexlen = strlen(hex);
    unsigned char b64[(hexlen/3)*4+hexlen%3+1];
    for (int i = 0, j = 0, rem_i=0; i < hexlen; i++, rem_i=i%3){
        b64[j++] = (rem_i) ? ((hex[i-1] << (8/(rem_i*2)) | hex[i] >> (8-(8/(rem_i*2)))) & 0x3f) : (hex[i]>>2);
        if (rem_i == 2 || i == hexlen - 1) b64[j++] = (hex[i]<<(4-(rem_i*2))) & 0x3f;
    }
    for (int k = 0; k < strlen(b64); k++) printf("%c",BASE64[b64[k]]);
}