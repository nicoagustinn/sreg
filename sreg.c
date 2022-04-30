#include <stdio.h>
#include <unistd.h>
#include <unistd.h>
#include <termios.h> 
#include <string.h>

#define SIZE 64

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
    char tgt[64], ch;
    FILE *fs; FILE *ft;
    fs = fopen("loginfo", "r");
    if(!fs){puts("Error while opening file!\n");return 1;}
    sprintf(tgt, "%s.ed", "loginfo");
    ft = fopen(tgt, "w");
    while((ch = fgetc(fs)) != EOF){fputc(~ch, ft);}
    fclose(fs);fclose(ft);
    remove("loginfo");rename(tgt, "loginfo");
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
    }
}

void login()
{
    char password[SIZE];
    FILE *fp;
    FILE *fp1;

    int cnt1 = 0;
    int cnt2 = 0;
    int flg = 0;

    fp = fopen("loginfo","w");
    if(!fp) {printf("File could not be opened.");}else{
        fputs("Enter username: ",stdout);
        fgets(username,SIZE,stdin);
        fputs("Enter password: ",stdout);
        getPassword(password);

        fflush(stdin);
        fputs(username, fp);
        fputs(password, fp);
        fclose(fp);
        enc();
        //remove("loginfo");

        //passwd & uname comp
        
        fseek(fp,0,SEEK_END);
        fseek(fp1,0,SEEK_END);
        cnt1 = ftell(fp);
        cnt2 = ftell(fp1);

        fseek(fp,0,SEEK_SET);
        fseek(fp1,0,SEEK_SET);

        if (cnt1 == cnt2) {puts("File contents are not the same!");}

        
    }
}


int main(void) 
{
    char loginfo[]="loginfo"; 
    if (access( loginfo, F_OK) == 0 ) {
        login();
    } else {registration();}
    return 0;
}
