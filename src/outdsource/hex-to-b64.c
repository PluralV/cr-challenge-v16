#include <stdio.h>
#include <string.h>

int main(int argc, char **argv){
    const unsigned char BASE64[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    unsigned char hex[] = "\x54\x68\x69\x6E\x67\x73\x20\x74\x68\x61\x74\x20\x68\x61\x76\x65\x20\x6E\x65\x76\x65\x72\x20\x62\x65\x65\x6E\x20\x64\x6F\x6E\x65\x20\x62\x65\x66\x6F\x72\x65\x2E";
    int hexlen = strlen(hex);
    int b64len = (hexlen/3)*4+(hexlen%3 == 0 ? 0 : 4);
    unsigned char b64[b64len];
    int i;
    int j = 0;
    for (i = 0; i < hexlen; i++){
        int rem_i = i % 3;
        switch(rem_i){
            case 0: 
                b64[j++] = BASE64[hex[i]>>2];
                if (i == hexlen-1) {
                    b64[j++] = BASE64[hex[i]<<4 & 0x3f];
                }
                break;
            case 1:
                b64[j++] = BASE64[(hex[i-1] << 4 | hex[i] >> 4) & 0x3f];
                if (i == hexlen - 1){
                    b64[j++] = BASE64[hex[i] << 2 & 0x3f];
                }
                break;
            case 2: 
                b64[j++] = BASE64[(hex[i-1] << 2 | hex[i] >> 6) & 0x3f];
                b64[j++] = BASE64[hex[i] & 0x3f];
                break;

        }
    }
    while (j < b64len){
        b64[j++] = '=';
    }
    b64[j] = '\0';
    printf("b64: %s\n",b64);
}