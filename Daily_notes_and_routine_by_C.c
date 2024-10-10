#include<stdio.h>
#include <string.h>

#define USER_NAME_MAX_LEN 30
#define EMAIL_MAX_LEN 200
#define PASSWORD_MAX_LEN 18
#define PASSWORD_MIN_LEN 6
#define MAX_USER 100

int activeuser=-1; //global variable


//structure for login system
struct users
{
    char user_name[USER_NAME_MAX_LEN];
    char email[EMAIL_MAX_LEN];
    char password[PASSWORD_MAX_LEN];

};


struct dates
{
    int day;
    int month;
    int year;
};

struct times
{
    int hour;
    int minute;
    int second;
};
struct Routine
{
    char Title[500];
    struct dates date;
    struct times time;
};

//structure for notes
struct notes
{
    char title[5000];
    char content[100000];
    struct dates date;
    struct times time;
};


void printCal(int year);
int daynum(int day, int month, int year);
int numofdays(int monthnum, int year);
char *month_name(int monthnum);
int login_menu(int act);

void create_account();
void load_user_list(); //global veriable
void routine();



void show_menu(int is_loggedin, char *username, struct users user_list[3]);
int login(struct users user_list[3]);
int checkuser(char *name, char *password,struct users user_list[3]);

struct users create_user(char *name, char *password, char *email);
struct users user_list[MAX_USER];


void show_notes();
void create_notes();


int main()

{
    char username[USER_NAME_MAX_LEN];
    char password[PASSWORD_MAX_LEN];
    int is_loggedin=0;
    int act;

    load_user_list();

    show_menu( is_loggedin, username, user_list); //calling menu function


}


struct users create_user(char *user_name, char *password, char *email)
{
    struct users user;
    strcpy(user.user_name,user_name);
    strcpy(user.password,password);
    strcpy(user.email,email);

    return user;

};

void create_account()
{
    struct users user;

    // char namee[100];
    char password[48];
    char email[200];
    char user_name[50];

    printf("Please provide the informations below to create an account:\n");

    printf("\nUser name(user name should be less than 20 characters):");
    scanf("%s",&user_name);
    if(strlen(user_name)>20)
    {
        printf("User name must be less than 20 characters.");
        return;
    }
    printf("\nPassword (password should be less than 20 characters and at least 4 characters):");
    scanf("%s",&password);

    if(strlen(password)>20 || strlen(password)<4 )
    {
        printf("User name must be between 4 to 20 characters.");
        return;
    }

    user = create_user(user_name, password, email);
    FILE *f;

    printf("%s", user.user_name);
    f=fopen("users.dat", "ab");

    fwrite(&user, sizeof (struct users), 1, f);

    printf("\nAccount created successfully.\n");
    fclose(f);
}

void load_user_list()
{

    struct users temp_user;

    FILE *f;
    f=fopen("users.dat", "rb");

    if(f == NULL) //if file does not exist, return
    {
        return;
    }

    int user_counter = 0;
    while(fread(&temp_user, sizeof(temp_user), 1, f)==1)
    {
        user_list[user_counter] = temp_user;
        user_counter++;
    }
    //printf("%d user account already exists\n", user_counter);
    fclose(f);

}

//check username and pass
int checkuser(char *name, char *password,struct users user_list[3])
{
    for(int i=0; i<3; i++)
    {


        if(strcmp(user_list[i].user_name, name)==0)
        {
            //printf("name matched\n");
            if(strcmp(user_list[i].password, password)==0)
            {
                //printf("matched");
                activeuser=i;
                return 1;

            }
        }

    }

    return 0;

}

//after log in
void show_menu(int is_loggedin, char *username, struct users user_list[3])

{
    //system("cls");

    printf("                                Welcome to Daily Notes and Routine.                      \n\n\n\nPlease select an option.\n");

    printf("\nEnter 1 to login.\n \nEnter 2 to create an account.\n");
    printf("Your command:");

    int user_input,act;

    scanf("%d", &user_input);

    switch (user_input)

    {
    case 1:
        is_loggedin=login(user_list);
        break;
    case 2:
        create_account();
        load_user_list();
        show_menu( is_loggedin, username, user_list);

    default:
        break;
    }

}

//during log in
int login(struct users user_list[3])
{
    int act;
    printf("Login\n");
    char username[USER_NAME_MAX_LEN];
    char password[PASSWORD_MAX_LEN];
    char *email;
    printf("Enter your user name:");
    scanf("%s", &username);


    printf("\nEnter your user password:");
    scanf("%s", &password);

    if(checkuser(username, password,  user_list))
    {
        printf("\n%s\n","Login success\n");
        login_menu(act);
    }

    else
    {
        printf("Failed\n");

        return 0;
    }
}

int login_menu(int act)
{
    printf("\nMenu--\n");
    printf("\nEnter\n 3 to show notes.\n Enter 4 to create new notes.\n Enter 5 to see calendar.\n Enter 6 to see routine.\n Enter 7 to logout.\n:");
    printf("Your command:");
    int user_input;
    scanf("%d", &user_input);

    if (user_input==3||user_input==4||user_input==5 ||user_input==6 ||user_input==7 )
    {
        switch (user_input)
        {
        case 3:
        {
            printf("\nWelcome %s\n",user_list[activeuser].user_name);
            show_notes();
            login_menu(act);

        }
        break;
        case 4:

        {
            create_notes();
            login_menu(act);

        }

        break;
        case 5: //calendar
        {
            int year ;
            printf("Enter year:");
            scanf("%d",&year);
            printCal(year);

        }
        break;
        case 6:

        {
            routine();

        }

        break;
        case 7:
        {
            char username[USER_NAME_MAX_LEN];
            int is_loggedin=0;
            activeuser=-1;
            printf("Successfully logged out");
            show_menu(is_loggedin, username, user_list);
        }
        break;
        }
        login_menu(act);
    }


    else
    {
        char username[USER_NAME_MAX_LEN];
        char password[PASSWORD_MAX_LEN];
        int is_loggedin=0;
        show_menu( is_loggedin, username, user_list);
    }


    return 0;
}


//showing notes
void show_notes()
{
    struct notes note;

    FILE *f;
    f=fopen(user_list[activeuser].user_name, "rb");

    while(fread(&note, sizeof ( note), 1, f)==1)
    {
        printf("Title:%s\n ", note.title);
        printf("Content:%s\n ", note.content);

        printf("%d %d %d", note.date.day, note.date.month, note.date.year);


    }
    fclose(f);

}

//creating notes
void create_notes()
{
    struct notes note;

    char titles[5000];

    char body[100000];
    struct dates date;

    printf("Title\n");
    scanf("%s",&titles);

    printf("Body content\n");
    scanf("%s",&body);

    printf("Enter Day,Month,Year\n");
    scanf("%d %d %d",&date.day, &date.month, &date.year);
    note.date=date;

    strcpy(note.title,titles);
    strcpy(note.content,body);
    FILE *f;

    printf("%s", note.title);
    f=fopen(user_list[activeuser].user_name, "ab");

    int size;

    size=fwrite(&note, sizeof (struct notes), 1, f);
    printf("%d",sizeof (struct notes));

    fclose(f);
}

//calendar part
//print calendar
void printCal(int year)
{

    printf("Calendar - %d\n", year);

    int days;
    int i,j,k;

    int current = daynum(1, 0, year);

    // i=month
    //j=days
    //k =week

    for ( i = 0; i < 12; i++)
    {

        days = numofdays(i, year);

        printf("\n ------------%s-------------\n",month_name(i));
        printf("  Sat Sun  Mon  Tue  Wed  Thu  Fri\n");

        //print space
        for (k = 0; k < current; k++)

            printf("     ");


        for (j = 1; j <= days; j++)
        {

            printf("%5d", j);


            if (k++ > 5) //column of days
            {

                k = 0;

                printf("\n");

            }

        }


        if (k)

            printf("\n");


        current = k;

    }


    return;
}

int daynum(int day, int month, int year)
{
    year=year - month < 3;
    return (year + year / 4  - year / 100  + year / 400  + (month-1) + day) % 7;
}

int numofdays(int monthnum, int year)
{

    // January

    if (monthnum == 0)

        return (31);


    // February

    if (monthnum == 1)
    {
        if (year % 400 == 0 || (year % 4 == 0  && year % 100 != 0))

            return (29);

        else

            return (28);

    }


    // March

    if (monthnum == 2)

        return (31);


    // April

    if (monthnum == 3)

        return (30);


    // May

    if (monthnum == 4)

        return (31);


    // June

    if (monthnum == 5)

        return (30);


    // July

    if (monthnum == 6)

        return (31);


    // August

    if (monthnum == 7)

        return (31);


    // September

    if (monthnum== 8)

        return (30);


    // October

    if (monthnum == 9)

        return (31);


    // November

    if (monthnum == 10)

        return (30);


    // December

    if (monthnum == 11)

        return (31);
}

char *month_name(int monthnum)
{

    char *month;


    switch (monthnum)
    {

    case 0:

        month = "January";

        break;

    case 1:

        month = "February";

        break;

    case 2:

        month = "March";

        break;

    case 3:

        month = "April";

        break;

    case 4:

        month = "May";

        break;

    case 5:

        month = "June";

        break;

    case 6:

        month = "July";

        break;

    case 7:

        month = "August";

        break;

    case 8:

        month = "September";

        break;

    case 9:

        month = "October";

        break;

    case 10:

        month = "November";

        break;

    case 11:

        month = "December";

        break;

    }

    return month;
}


void routine()
{
    struct Routine rt;

    printf("Enter Title: ");
    scanf(" %[^\n]s", rt.Title);
    printf("Enter Date DD/MM/YYYy :");
    scanf(" %d%d%d", &rt.date.day, &rt.date.month, &rt.date.year);
    printf("Enter Time HH:MM:SS : ");
    scanf(" %d%d%d", &rt.time.hour, &rt.time.minute, &rt.time.second);

    printf("You have %s on:\n", rt.Title);
    printf("%d/%d/%d at %d:%d:%d", rt.date.day, rt.date.month, rt.date.year, rt.time.hour, rt.time.minute, rt.time.second);
}

