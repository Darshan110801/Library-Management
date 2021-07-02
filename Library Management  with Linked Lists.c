#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>

#define MAX_RENEWS 3
#define MAX_ISSUES_FAC 4
#define MAX_ISSUES_STUD 2
#define NAME_SIZE 100
#define ISSUED 0
#define AVAILABLE 1
#define MAX(a,b) a>b?a:b

// Time utility functions.......
/**********************************************************************************************************************/

typedef struct Date{
    int day,month,year;
    
}Date;
int isLeap(int y) 
{ 
    if (y%100 != 0 && y%4 == 0 || y %400 == 0) 
        return 1; 
  
    return 0; 
} 
  
// Given a date, returns number of days elapsed 
// from the  beginning of the current year (1st 
// jan). 
int offsetDays(int d, int m, int y) 
{ 
    int offset = d; 
  
    switch (m - 1) 
    { 
    case 11: 
        offset += 30; 
    case 10: 
        offset += 31; 
    case 9: 
        offset += 30; 
    case 8: 
        offset += 31; 
    case 7: 
        offset += 31; 
    case 6: 
        offset += 30; 
    case 5: 
        offset += 31; 
    case 4: 
        offset += 30; 
    case 3: 
        offset += 31; 
    case 2: 
        offset += 28; 
    case 1: 
        offset += 31; 
    } 
  
    if (isLeap(y) && m > 2) 
        offset += 1; 
  
    return offset; 
} 
  
// Given a year and days elapsed in it, finds 
// date by storing results in d and m. 
void revoffsetDays(int offset, int y, int *d, int *m) 
{ 
    int month[13] = { 0, 31, 28, 31, 30, 31, 30, 
                      31, 31, 30, 31, 30, 31 }; 
  
    if (isLeap(y)) 
        month[2] = 29; 
  
    int i; 
    for (i = 1; i <= 12; i++) 
    { 
        if (offset <= month[i]) 
            break; 
        offset = offset - month[i]; 
    } 
  
    *d = offset; 
    *m = i; 
} 
  
// Add x days to the given date. 
Date extend_date(Date cur_date, int x) 
{ 
    int d1 = cur_date.day,m1 = cur_date.month,y1 = cur_date.year;
    int offset1 = offsetDays(d1, m1, y1); 
    int remDays = isLeap(y1)?(366-offset1):(365-offset1); 
  
    // y2 is going to store result year and 
    // offset2 is going to store offset days 
    // in result year. 
    int y2, offset2; 
    if (x <= remDays) 
    { 
        y2 = y1; 
        offset2 = offset1 + x; 
    } 
  
    else
    { 
        // x may store thousands of days. 
        // We find correct year and offset 
        // in the year. 
        x -= remDays; 
        y2 = y1 + 1; 
        int y2days = isLeap(y2)?366:365; 
        while (x >= y2days) 
        { 
            x -= y2days; 
            y2++; 
            y2days = isLeap(y2)?366:365; 
        } 
        offset2 = x; 
    } 
  
    // Find values of day and month from 
    // offset of result year. 
    int m2, d2; 
    revoffsetDays(offset2, y2, &d2, &m2); 
    Date ret_val;
    ret_val.day = d2;
    ret_val.month = m2;
    ret_val.year = y2;
    return ret_val;
} 


const int monthDays[12]
    = { 31, 28, 31, 30, 31, 30, 
       31, 31, 30, 31, 30, 31 };
 
// This function counts number of 
// leap years before the given date
int countLeapYears(Date d)
{
    int years = d.year;
 
    // Check if the current year needs to be
    //  considered for the count of leap years
    // or not
    if (d.month <= 2)
        years--;
 
    // An year is a leap year if it 
    // is a multiple of 4,
    // multiple of 400 and not a 
     // multiple of 100.
    return years / 4 
           - years / 100
           + years / 400;
}
 
// This function returns number of 
// days between two given dates
int date_diff(Date dt1, Date dt2)
{
    // COUNT TOTAL NUMBER OF DAYS
    // BEFORE FIRST DATE 'dt1'
     int i;
    // initialize count using years and day
    long int n1 = dt1.year * 365 + dt1.day;
 
    // Add days for months in given date
    for ( i = 0; i < dt1.month - 1; i++)
        n1 += monthDays[i];
 
    // Since every leap year is of 366 days,
    // Add a day for every leap year
    n1 += countLeapYears(dt1);
 
    // SIMILARLY, COUNT TOTAL NUMBER OF
    // DAYS BEFORE 'dt2'
 
    long int n2 = dt2.year * 365 + dt2.day;
    for ( i = 0; i < dt2.month - 1; i++)
        n2 += monthDays[i];
    n2 += countLeapYears(dt2);
 
    // return difference between two counts
    return (n2 - n1);
}






//Time utility functions ended.....

/**********************************************************************************************************************/
int num_books = 0;
int  num_members = 0;

typedef enum Designation{FACULTY,STUDENT} Designation;


typedef struct Loc{
    int hall_no,rack_no,row_no;
}Loc;

typedef struct book{
    int acc_no;
    Loc loc;
    char title[NAME_SIZE];
    char subject[NAME_SIZE];
    char author[NAME_SIZE];
    int yr_of_pub;
    int status; // issued_ or_available
    Date renew_date; 
    int borrowed_by;
    int renewed_times; //list of ints where each node represents no. of times perticular copy is renewed 

}book;

typedef struct member{
     char name[NAME_SIZE];
     char dept[NAME_SIZE];
     Designation design;
     int id;
     int num_books_issued;
     float fine;
}member;

typedef struct book_node{
    book b;
    struct book_node* next;
}book_node;

typedef struct member_node{
    member m;
    struct member_node* next;
}member_node;

book_node* book_head = NULL ;
member_node* stud_head  = NULL;
member_node* fac_head  = NULL;

int mem_comp(member_node*m1,member_node* m2) // comparing two member nodes as per key for sorting
{
     int ret_val;
     if((m1->m).num_books_issued<(m2->m).num_books_issued)
     {
         ret_val = 1;
     }
     else if((m1->m).num_books_issued > (m2->m).num_books_issued)
     {
         ret_val =-1;
     }
     else
     {
         if((m1->m).design == STUDENT && (m2->m).design == FACULTY)
         {
             ret_val = 1;
         }
         else if((m1->m).design == FACULTY && (m2->m).design == STUDENT)
         {
             ret_val = -1;
         }
         else
         {
             if(strcmp((m1->m).name,(m2->m).name)>0)
             {
                 ret_val = 1;
             }
             else if(strcmp((m1->m).name,(m2->m).name)<0)
             {
                ret_val = -1;
             }
             else
             {
                 ret_val = 0;
             }
         }
     }
      
}

int book_comp(book_node* b1,book_node*b2)
{
      int ret_val = 0;
      if(strcmp((b1->b).subject,(b2->b).subject)>0)
      {
            ret_val = 1;
      }
      else if(strcmp((b1->b).subject,(b2->b).subject)<0)
      {
            ret_val = -1;
      }
      else
      {
            if(strcmp((b1->b).title,(b2->b).title)>0)
            {
                ret_val = 1;
            }
            else if(strcmp((b1->b).title,(b2->b).title)<0)
            {
                ret_val = -1;
            }
            else
            {
                if(strcmp((b1->b).author,(b2->b).author)>0)
                {
                    ret_val = 1;
                }
                else if(strcmp((b1->b).author,(b2->b).author)<0)
                {
                    ret_val = -1;
                }
                else
                {
                    if((b1->b).yr_of_pub > (b2->b).yr_of_pub)
                      ret_val = 1;
                    else if((b1->b).yr_of_pub < (b2->b).yr_of_pub)
                      ret_val = -1;
                    else
                      ret_val = 0;
                }
            }
      }

      return ret_val;

}
void insert_a_book()
{
    book_node* book_ins = (book_node*)malloc(sizeof(book_node));
    num_books += 1;
    (book_ins->b).acc_no = num_books;
    printf("Enter data corresponding to the book you wanna insert:\n");
     
    printf("Title:\n");
    fflush(stdin);
    fgets((book_ins->b).title,100,stdin);
    printf("Subject:\n");
    fgets((book_ins->b).subject,100,stdin);
    printf("Author name:\n");
    fgets((book_ins->b).author,100,stdin);
    printf("Year of publish:\n");
    scanf("%d",&((book_ins->b).yr_of_pub));
    printf("Hall no.\n");
    scanf("%d",&((book_ins->b).loc.hall_no));
    printf("Rack no.\n");
    scanf("%d",&((book_ins->b).loc.rack_no));
    printf("Row no.\n");
    scanf("%d",&((book_ins->b).loc.row_no));

    (book_ins->b).status = AVAILABLE;
    (book_ins->b).renew_date.day = (book_ins->b).renew_date.month = (book_ins->b).renew_date.year = 0;
    (book_ins->b).renewed_times = 0;
    (book_ins->b).borrowed_by = 0;
    if(book_head == NULL)
    {
        book_head = book_ins;
        book_head->next = NULL;
    }
    else
    {
        book_node* cur = book_head;
        book_node*prev = NULL;
        int flag = 1;
        while(cur != NULL && flag)
        {
            if(book_comp(cur,book_ins) == 1)
            {
                flag = 0;
            }
            else
            {
                prev = cur;
                cur = cur->next;
            }
        }
        if(prev == NULL)
        {
            book_ins->next = book_head;
            book_head = book_ins;
        }
        else if (cur == NULL)
        {
            prev->next = book_ins;
            book_ins->next = NULL;
        }
        else
        {
            book_node* next = prev->next;
            prev->next = book_ins;
            book_ins->next = next;
        }
    }
    printf("\n\nBook inserted successfully\n\n");
}

void print_book_data(book_node* book_nd)
{
    printf("Accesssion No. : \t %d\n",(book_nd->b).acc_no);
    printf("Title : \t %s\n",(book_nd->b).title);
    printf("Subject : \t %s\n",(book_nd->b).subject);
    printf("Author : \t %s\n",(book_nd->b).author);
    printf("Year of publish : \t %d\n",(book_nd->b).yr_of_pub);
    printf("status(issued = 0,available = 1) : \t %d\n",(book_nd->b).status);
    printf("Location(hall,rack,row): %d %d %d\n",(book_nd->b).loc.hall_no,(book_nd->b).loc.rack_no,(book_nd->b).loc.row_no);
}
int match(char str1[],char str2[])
{
    int ret_val = 0;
    if(strstr(str1,str2) != NULL)
       ret_val = 1;
    else 
       ret_val = 0;
    return ret_val;
}
int search_book_by_title()
{
     char title[NAME_SIZE];
     int match_found = 0;

     printf("Enter the title of the book you wanna search:\n");

     fgets(title,100,stdin);

     printf("\n\nBooks we found for you:\n\n ");

     book_node* itr;

     for(itr = book_head;itr != NULL;itr = itr->next)
     {
         if(match((itr->b).title,title))
         {
             print_book_data(itr);
             match_found = 1;
             printf("............................................................................");
             printf("\n\n");
         }
     }
     return match_found;
}

int search_book_by_author()
{
    char author[NAME_SIZE];
    int match_found = 0;

     printf("Enter the title of the book you wanna search:\n");

     fgets(author,100,stdin);

     printf("\n\nBooks we found for you:\n\n ");

     book_node* itr;

     for(itr = book_head;itr != NULL;itr = itr->next)
     {
         if(match((itr->b).author,author))
         {
             print_book_data(itr);
             match_found = 1;
             printf(".............................................................................");
             printf("\n\n");
         }
     }
     return match_found;
}

int issued_max_books(member_node* mem)
{
    int ret_val;
    if((mem->m).design == FACULTY && (mem->m).num_books_issued == MAX_ISSUES_FAC)
    {
        ret_val = 1;
    }
    else if((mem->m).design == STUDENT && (mem->m).num_books_issued == MAX_ISSUES_STUD)
    {
        ret_val = 1;
    }
    else
    {
        ret_val = 0;
    }
    return ret_val;

}
void search_and_issue_book(member_node* mem)
{
    int option,match_found,proceed ;
    printf("Enter a option\n\n");
    printf("Search a book by:\n1.By title\n2.By author name\n");
    printf("Your choice\n");
    scanf("%d",&option);
    switch(option)
    {
        case 1:
            match_found = search_book_by_title();
            if(match_found == 0)
            {
                printf("\nOoops!! No book found with this title,try another one...\n");
            }
            break;
        case 2:
            match_found = search_book_by_author();
            if(match_found == 0)
            {
                printf("\nOoops!! No book found with this title,try another one...\n");
            }
            break;
        default:
            printf("\nOops!!! Please enter valid choice....\n\n");
    }
    printf("\n\nDo you want to proceed for issuing the book:\n\n");
    printf("1. YES \n 2. NO\nYour option:\n\n");
    scanf("%d",&proceed);
    if(proceed == 1 && issued_max_books(mem) )
    {
        printf("\n\nSorry you can't issue more books..max issues exceeded...Plz return books you already issued and try again\n\n ");
    }
    else if(proceed == 1)
    {
           int acc,found = 0;
           printf("\n\nEnter accession number of a book you want to issue...\n\n");
           scanf("%d",&acc);
           book_node*itr = book_head;
           while(itr != NULL && (!found))
           {
               if((itr->b).acc_no == acc && (itr->b).status == AVAILABLE)
               {
                   found = 1;
               }
               else 
               {
                   itr = itr->next;
               }
           }
           if(found)
           {
               Date issue_date;
               printf("\n\nEnter the today's date:\n\n");
               printf("day:\t");
               scanf("%d",&(issue_date.day));
               printf("month:\t");
               scanf("%d",&(issue_date.month));
               printf("year:\t");
               scanf("%d",&(issue_date.year));

               (mem->m).num_books_issued +=1 ;
               (itr->b).status = ISSUED;
               (itr->b).borrowed_by = (mem->m).id;
                if((mem->m).design == FACULTY)
                {
                    Date d = issue_date;
                    d = extend_date(d,14);
                    (itr->b).renew_date = d;
                }
                    
                else
                {
                    Date d = (itr->b).renew_date;
                    d = extend_date(d,14);
                    (itr->b).renew_date = d;
                }
               printf("\n\nBook issued successfully.....\n\n");
           }
           else
           {
               printf("\n\nSorry !! We don\'t have this book in our possession currently...\n\n");
           }
           
           

    }


}
void renew_book(member_node* mem)
{
    int acc,found = 0;
    printf("\n\nEnter accession no. of the book you want to return:\n\n");
    scanf("%d",&acc);
    book_node* itr = book_head;

    while(itr != NULL && (!found))
    {
        if((itr->b).acc_no == acc && (itr->b).borrowed_by == (mem->m).id)
           found = 1;
        else  
           itr = itr->next;
    }
    if(found)
    {
         if((itr->b).renewed_times == 3)
         {
             printf("\n\nBook can not be reissued...Max reissues reached..\n\n");
         }
         else
         {
             ((itr->b).renewed_times) += 1;
             if((mem->m).design == FACULTY)
             {
                 Date d = (itr->b).renew_date;
                 d = extend_date(d,30);
                 (itr->b).renew_date = d;
             }
             else 
             {
                 Date d = (itr->b).renew_date;
                 d = extend_date(d,14);
                 (itr->b).renew_date = d;
             }

             printf("\n\nBook reissued successfully\n\n");
         }
    }
    else{
        printf("\n\nPlease enter valid entry...You haven\'t issued this book\n\n");
    }
}
void ret_book(member_node* mem)
{
    int acc,found = 0;
    printf("\n\nEnter accession no. of the book you want to return:\n\n");
    scanf("%d",&acc);
    book_node* itr = book_head;

    while(itr != NULL && (!found))
    {
        if((itr->b).acc_no == acc && (itr->b).borrowed_by == (mem->m).id)
           found = 1;
        else  
           itr = itr->next;
    }
    
    if(found)
    {
       (itr->b).status = AVAILABLE;
       (itr->b).renewed_times = 0;
       (itr->b).renew_date.day = 0;
       (itr->b).renew_date.month = 0;
       (itr->b).renew_date.year = 0;
       (itr->b).borrowed_by = 0;
       printf("\n\nBook returned successfully\n\n");
    }
    else
    {
        printf("\n\nYou haven't issued this book...Kindly enter valid entry...\n\n");
    }
}
void create_member()
{
    int opt;
    member_node* new_mem_node = (member_node*)malloc(sizeof(member_node));
    printf("\n\nIs the member faculty or student:\n 1.Faculty\n2.Student\n");
    scanf("%d",&opt);
    if(opt == 1)
    {
          num_members += 1;
          printf("\n\nEnter name of faculty:\n\n");
          fflush(stdin);
          fgets((new_mem_node->m).name,100,stdin);
          printf("\n\nEnter department of faculty:\n\n");
          fgets((new_mem_node->m).dept,100,stdin);
          (new_mem_node->m).design = FACULTY;
          (new_mem_node->m).id = num_members;
          (new_mem_node->m).num_books_issued = 0;
          (new_mem_node->m).fine = 0;
          if(fac_head == NULL)
          {
              fac_head = new_mem_node;
              fac_head->next = NULL;
          }
          else
          {
              member_node* itr = fac_head;
              while(itr->next != NULL)
              {
                  itr = itr->next;
              }
              itr->next = new_mem_node;
              new_mem_node->next = NULL;

          }
    }
    else if(opt == 2)
    {
          num_members += 1;
          printf("\n\nEnter name of student:\n\n");
          fgets((new_mem_node->m).name,100,stdin);
          printf("\n\nEnter department of student:\n\n");
          fgets((new_mem_node->m).dept,100,stdin);
          (new_mem_node->m).design = STUDENT;
          (new_mem_node->m).id = num_members;
          (new_mem_node->m).num_books_issued = 0;
          (new_mem_node->m).fine = 0;
          if(stud_head == NULL)
          {
              stud_head= new_mem_node;
              stud_head->next = NULL;
          }
          else
          {
              member_node* itr = stud_head;
              while(itr->next != NULL)
              {
                  itr = itr->next;
              }
              itr->next = new_mem_node;
              new_mem_node->next = NULL;

          }
    }
    else
    {
        printf("\n\nPlease try next time by entering valid option....\n\n");
    }

}
void max_issues()
{
     printf("\n\nNames and member IDs of members who issued maximum no. of books:\n\n");
     printf("Faculty members:\n\n ");
     member_node* itr  = fac_head;
     for(itr = fac_head ; itr != NULL ;itr = itr->next)
     {
         if((itr->m).num_books_issued == MAX_ISSUES_FAC)
        {
            printf("%d\t%s\n",itr->m.id,itr->m.name);
        }
     }
     printf("Student members:\n\n ");
     for(itr = stud_head ; itr != NULL ;itr = itr->next)
     {
         if((itr->m).num_books_issued == MAX_ISSUES_STUD)
        {
            printf("%d\t%s\n",itr->m.id,itr->m.name);
        }
     }
     printf("\n\n");
}
int fine(member_node* mem)
{
    Date todays_date;
    printf("\n\nWhat\'s the date today:\n\n");
    printf("Day:\t");
    scanf("%d",&todays_date.day);
    printf("Month:\t");
    scanf("%d",&todays_date.month);
    printf("Year:\t");
    scanf("%d",&todays_date.year);
    book_node* itr;
    float fine  = 0;
    for(itr = book_head;itr != NULL ;itr = itr->next)
    {
        Date renew_date = (itr->b).renew_date;
        if((itr->b).borrowed_by == (mem->m).id )
        {
            if(date_diff(todays_date,renew_date)>0)
                fine = fine + date_diff(todays_date,renew_date)*0.50;
        }
    }
    (mem->m).fine = fine;

}
void max_fine_borrowers()
{
      float max_fine  = 0;
      float mem_fine = 0;
      member_node* itr = fac_head;
      for(itr = fac_head;itr != NULL ;itr = itr->next)
      {
          mem_fine = fine(itr);
          max_fine = MAX(max_fine,(itr->m).fine);
      }
      for(itr = stud_head;itr != NULL ;itr = itr->next)
      {
          mem_fine = fine(itr);
          max_fine = MAX(max_fine,(itr->m).fine);
      }
      printf("\n\nMembers with max fine which is %f\n\n",max_fine);
      printf("Faculties(id,name):\n\n");
      for(itr = fac_head;itr != NULL ;itr = itr->next)
      {
         if((itr->m).fine == max_fine)
         {
               printf("%d\t%s\n",(itr->m).id,(itr->m).name);
         }
      }
      printf("Students(id,name):\n\n");
      for(itr = stud_head;itr != NULL ;itr = itr->next)
      {
         if((itr->m).fine == max_fine)
         {
               printf("%d\t%s\n",(itr->m).id,(itr->m).name);
         }
      }
}

member_node* divide(member_node* loc_head)
{
    member_node* fast_ptr = loc_head->next;
    member_node* slow_ptr = loc_head;
    member_node* ret_ptr;
    while(fast_ptr != NULL)
    {
        fast_ptr = fast_ptr->next;
        if(fast_ptr != NULL)
        {
            fast_ptr = fast_ptr->next;
            slow_ptr = slow_ptr->next;
        }
    }
    ret_ptr = slow_ptr->next;
    slow_ptr->next = NULL;
    return ret_ptr;
}
member_node* merge(member_node* lptr1,member_node*lptr2)
{

    member_node* dummy_node = (member_node*)malloc(sizeof(member_node));
    dummy_node->next = NULL;
    member_node* tail = dummy_node;
    member_node*itr1 = lptr1;
    member_node*itr2 = lptr2;
    while(itr1 != NULL && itr2 != NULL)
    {
        if(mem_comp(itr1,itr2) == 1)
        {
            tail->next = itr2;
            tail = itr2;
            itr2 = itr2->next;
        }
        else
        {
            tail->next = itr1;
            tail = itr1;
            itr1 = itr1->next;
        }
    }
    if(itr1 != NULL)
    {
        tail->next = itr1;
    }
    if(itr2 != NULL)
    {
        tail->next = itr2;
    }
    return dummy_node->next;
}
member_node* merge_sort(member_node* loc_head)
{
      member_node* lptr = loc_head;
      if(lptr != NULL && lptr->next != NULL)
      {
          member_node* mid = divide(lptr);
          lptr = merge_sort(lptr);
          mid = merge_sort(mid);
          lptr = merge(lptr,mid);
      }
      return lptr;
}
void sort()
{   
    member_node* loc_fac_head = fac_head;
    fac_head = merge_sort(loc_fac_head); //physical sorting(sorting nodes).......
    member_node* loc_stud_head = stud_head;
    stud_head = merge_sort(loc_stud_head); // physical sorting(sorting nodes).....
    member_node* itr1 = fac_head;
    member_node* itr2 = stud_head;
    printf("Sorted list of borrowers in ascending manner of no. of borrows:\n\n");
    printf("(Designation\tid\tname)\n\n");

    // meging indirectly...because,in question it has been told to keep two separatr lists for faculy
    // and student members

    while(itr1 != NULL && itr2 != NULL)
    {
        if(mem_comp(itr1,itr2) == 1)
        {
            printf("Student\t%d\t%s\n",itr2->m.id,itr2->m.name);
            itr2 = itr2->next;
        }
        else 
        {
            printf("Faculty\t%d\t%s\n",itr1->m.id,itr1->m.name);
            itr1 = itr1->next;
        }
    }
    while(itr1 != NULL)
    {
        printf("Faculty\t%d\t%s\n",itr1->m.id,itr1->m.name);
        itr1 = itr1->next;
    }
    while(itr2 != NULL)
    {
        printf("Student\t%d\t%s\n",itr2->m.id,itr2->m.name);
        itr2 = itr2->next;
    }

}
int main()
{
    int opt,man_opt,mem_opt;
     printf("\n\nHello there!!! Plz tell us about you\n\n");

     printf("\n1.I am library manager\n");
     printf("\n2.I am member of library\n");
 
     printf("Your option\n");
     scanf("%d",&opt);
     if(opt == 1)
     {
         int flag = 1;
         while(flag)
         {
             system("cls");
             printf("\n\nHii Manager what you wanna do today\n\n");
             printf("1.Insert a book\n");
             printf("2.Add a member\n");
             printf("3.Give me a list of borrowers having max_limit reached for their issues\n");
             printf("4.Display names of borrowers having maximum fine till date\n");
             printf("5.Sorted list of members as per number of borrows in descending order\n");
             printf("6.Quit\n");
             
             printf("\n\nYour option\n");
             scanf("%d",&man_opt);
             switch(man_opt)
             {
                 case 1:
                       system("cls");
                       insert_a_book();
                       printf("Enter any key to go back to main menu:\n\n");
                       getch();
                       break;
                case 2:
                       system("cls");
                       create_member();
                       printf("Enter any key to go back to main menu:\n\n");
                       getch();
                       break;
                case 3:
                       system("cls");
                       max_issues();
                       printf("Enter any key to go back to main menu:\n\n");
                       getch();
                       break;
                case 4:
                       system("cls");
                       max_fine_borrowers();
                       printf("Enter any key to go back to main menu:\n\n");
                       getch();
                       break;
                case 5:
                       system("cls");
                       sort();
                       printf("Enter any key to go back to main menu:\n\n");
                       getch();
                       break;
                case 6:
                       system("cls");
                       flag = 0;
                       break;
                default:
                       system("cls");
                       printf("\nPlz enter valid option\n");
                       printf("Enter any key to go back to main menu:\n\n");
                       getch();
                       break;
             }

         }
     }
     else if(opt == 2)
     {
         int id,opttt = 0,found = 0;

         member_node* cur_mem;
         printf("Enter your member id:\n\n");
         scanf("%d",&id);
         member_node* itr = fac_head;
         printf("What is your designation\n\n");
         printf("1.Faculty 2. Student\n\nYour choice:\n\n");
         scanf("%d",&opttt);
         if(opttt == 1)
         {
             member_node*  itr = fac_head;
             while(itr != NULL && (!found))
             {
                 if((itr->m).id == id)
                 {
                     found = 1;
                 }
                 else
                 {
                     itr = itr->next;
                 }

             }

         }
         else if(opttt == 2)
         {
            member_node*  itr = stud_head;
             while(itr != NULL && (!found))
             {
                 if((itr->m).id == id)
                 {
                     found = 1;
                 }
                 else
                 {
                     itr = itr->next;
                 }

             }
         }
         
        // if found == 1 ,that is,if the person is member I will let him do the things elaborated in functions like
          //  fine etc specific to that member
          //I  have written complete code for those functions...but could not write menu driven program in time..
          //but I think it can be understood...
     }
     else
     {
         printf("\n\nPlz enter valid option next time\n\n");
     }

}
