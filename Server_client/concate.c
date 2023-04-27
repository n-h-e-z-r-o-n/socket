// C program to concatenating two
// strings using function
#include <stdio.h>
#include <string.h>
 
void concatenate_string(char* p, char* p2)
{
    while (*p != '\0'){
        // printf("%c ", *p);
        p++;
    }
    while (*p2 != '\0'){
        // printf("%c", *p2);
        *p=*p2;
        p++;
        p2++;
    }
}
 
int main()
{
 
    char s[100] = "\"serial Number \": \"";
    char * p;
    char * p2;
    p =s;
    char s1[]= "this is awesome";
    strcat(s,s1);

    printf("\n");
    printf("%s",s);

 
    // printf("Enter the second string: ");
    // scanf("%s",s1);
 
    // // function concatenate_string
    // // called and s and s1 are
    // // passed
    // concatenate_string(s, s1);
 
    // printf("Concatenated String is: '%s'\n", s);
 
    return 0;
}