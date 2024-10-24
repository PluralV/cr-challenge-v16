#include <stdio.h>

#ifndef PASSWORD_FILE_PATH
#define PASSWORD_FILE_PATH "./passwd.txt"
#endif
#define USAGE "USING PWGR: ./pwgr [-h|-g [string]]\n-h: displays usage \n-g: retrieves password from file passwd.txt found with identifier string and outputs to stdout. If no string argument is given, pwgr returns the password corresponding to \"default\".\n\nPasswords in passwd.txt are stored as comma-separated pairs: \"identifier,password\", one per line.\nDO NOT PUT COMMAS IN PASSWORDS OR IDENTIFIERS."
#define RETURN '\n'


char cmp_buf[256];
static char defaultname[8] = "default";

int string_cmp(char* a, char*b){
    while (*a != 0) if (*a++ != *b++) return 0;
    return 1;
}

int clean_commas(char *a){
    while (*a != ',') if (*a++ == '\0') return 1;
    return 0;
}

int password_retrieve(char *cat){
    FILE *pw_file;
    if ((pw_file = fopen(PASSWORD_FILE_PATH,"r")) == NULL){
        fprintf(stderr,"WARNING: password file does not exist at %s",PASSWORD_FILE_PATH);
        return -1;
    } 
    char current_char;
    int i = 0;
    while ((current_char = fgetc(pw_file)) != EOF){
        if (current_char == ','){
            cmp_buf[i] = '\0';
            int cmp = string_cmp(cmp_buf,cat);
            while ((current_char = fgetc(pw_file))!='\n' && current_char != EOF) if (cmp) putchar(current_char);
            if (cmp) {
                fclose(pw_file);
                return 0;
            }
            i=0;
        }
        else cmp_buf[i++] = current_char;
    }
    fclose(pw_file);
    fprintf(stderr,"No password exists for identifier \"%s.\" Try checking your spelling.\n",cat);
    return -1;
}

int main(int argc, char **argv){
    if (argc <= 1 || **(argv+1)!='-'){
        fprintf(stderr,"ERROR: %s FIRST ARGUMENT: %s\nSee below for proper usage:\n%s",(argc > 1) ? "INVALID" : "MISSING",*(argv+1),USAGE);
        return -1;
    }
    switch(*(*(argv+1)+1)){
        case 'h':
            fprintf(stderr,USAGE);
            return 0;
        case 'g':
            if (argc <= 2 || clean_commas(*(argv+2)))
                return (argc > 2) ? password_retrieve(*(argv+2)) : password_retrieve(defaultname);
        default:
            fprintf(stderr,"ERROR: INVALID ARGUMENTS\nSee below for proper usage:\n%s",USAGE);
            return -1;
    }
}