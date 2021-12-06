#include <stdio.h>
#include <unistd.h>
#include <unistd.h>
#include <termios.h> 
#include <string.h>

#define SIZE 32

char username[SIZE];

void getPassword(char password[])
{
    static struct termios oldt, newt;
    int i = 0;int c;
    tcgetattr( STDIN_FILENO, &oldt); // saving old settings of STDIN_FILENO & copy settings for resetting
    newt = oldt;
    newt.c_lflag &= ~(ECHO);        // set appropiate bit in termios struct   
    tcsetattr( STDIN_FILENO, TCSANOW, &newt); // sets new bits
    while ((c = getchar())!= '\n' && c != EOF && i < SIZE){password[i++] = c;} // reading password from console
    password[i] = '\0';
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // resetting STDIN_FILENO
}
int enc()   // encrypts original file
{      
    char src[7]="loginfo";
    char tgt[64], ch;
    FILE *fs; FILE *ft;
    fs = fopen(src, "r");
    if(!fs){puts("Error while opening file!\n");return 1;}
    sprintf(tgt, "%s.ed", src);
    ft = fopen(tgt, "w");
    while((ch = fgetc(fs)) != EOF){fputc(~ch, ft);}
    fclose(fs);fclose(ft);
    remove(src);rename(tgt, src);
}

int registration() 
{
    char password[SIZE];
    FILE *fp;
    fp = fopen("loginfo","w");
    if(!fp) {printf("File could not be opened.");}else{    
    fputs("Enter new username: ",stdout);fgets(username,SIZE,stdin);
    fputs("Enter new password: ",stdout);getPassword(password);
    puts("\nSuccesfully registered! Please login.");
    fflush(stdin);
    fputs(username,fp); fputs(password,fp);
    fclose(fp);
    enc();
    remove("loginfo");
    }
}
int main(void) 
{
    char loginfo[]="loginfo"; 
    if (access( loginfo, F_OK) == 0 ) {
        puts("File exists\n");login();
    } else {registration();}
    return 0;
}
