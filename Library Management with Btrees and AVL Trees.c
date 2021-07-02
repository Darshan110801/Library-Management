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
#define max(a,b) a>b?a:b
#define MAX 3
#define MIN 2
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
     int i,ret_val;
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
    if(n2 > n1)
      ret_val = n2-n1;
    else 
      ret_val = 0;
    return ret_val;
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
    int cur_fine;

}book;

typedef struct book_node {
	book b;
	struct book_node* left;
	struct book_node* right;
	int height;
}book_node;

typedef struct member{
     char name[NAME_SIZE];
     char dept[NAME_SIZE];
     Designation design;
     int id;
     int num_books_issued;
     book_node* borrowed_books[4];
     float fine;
     int times_paid_fine;
}member;


typedef struct member_node{
    member m;
    struct member_node* left;
    struct member_node* right;
    int height;
}member_node;

struct btreeNode {
  member_node item[MAX + 1];
  int count;
  struct btreeNode *link[MAX + 1];
};

struct btreeNode *root = NULL;
book_node* book_root = NULL;
member_node* faculty_root = NULL;
member_node* student_root = NULL;

//function comparing two books which is key for avl tree insertion
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
//function comparing two members which is key for avl tree insertion
int member_comp1(member_node *m1 ,member_node*m2)
{
      int ret_val = 0;
     
      if(strcmp((m1->m).dept,(m2->m).dept) > 0)
      {
          ret_val = 1;
      }
      else if(strcmp((m1->m).dept,(m2->m).dept) > 0)
      {
          ret_val = -1;
      }
      else
      {
          if((m1->m).id > (m2->m).id)
          {
              ret_val = 1;
          }
          else if((m1->m).id < (m2->m).id)
          {
              ret_val = -1;
          }
          else
          {
              ret_val = 0;
          }
      }
      return ret_val;
}
//Comparing members as per attributes required in question 6
int member_comp2(member_node* root,member_node* ins) // for subquestion 6
{
    int ret_val;
    if(root->m.num_books_issued > ins->m.num_books_issued)
    {
        ret_val = -1;
    }
    else if(root->m.num_books_issued < ins->m.num_books_issued)
    {
        ret_val = 1;
    }
    else
    {
        if(root->m.design == FACULTY && ins->m.design == STUDENT)
        {
            ret_val = -1;
        }
        else if(root->m.design == STUDENT && ins->m.design == FACULTY)
        {
            ret_val = 1;
        }
        else
        {
            if(strcmp(root->m.name,ins->m.name) > 0)
            {
                ret_val = 1;
            }
            else if(strcmp(root->m.name,ins->m.name) < 0)
            {
                ret_val = -1;
            }
            else
            {
                ret_val = 0;
            }
        }
    }
    return ret_val;
}
/***********************************************************
 Functions for AVL tree insertion of a book***************/
int height_of_book(book_node* root)
{
    int ret_val;
    if(root == NULL)
    {
        ret_val = 0;
    }
    else
    {
        ret_val = root->height;
    }
    return ret_val;
}
int calculate_imbalance_book(book_node* root)
{
    int ret_val = 0;
    if(root != NULL)
    {
        ret_val = height_of_book(root->left) - height_of_book(root->right);
    }
    return ret_val;
}
book_node* left_rotate_book(book_node* root)
{
    book_node* x = root->right;
    book_node* T2 = x->left;

    x->left = root;
    root->right = T2;

    root->height = max(height_of_book(root->left), height_of_book(root->right))+1;
    x->height = max(height_of_book(x->left), height_of_book(x->right))+1;
 
    
    return x;
}
book_node* right_rotate_book(book_node* root)
{
    book_node *x = root->left;
    book_node *T2 = x->right;
 
    x->right = root;
    root->left = T2;
 

    root->height = max(height_of_book(root->left), height_of_book(root->right))+1;
    x->height = max(height_of_book(x->left), height_of_book(x->right))+1;
 
    
    return x;
}
book_node* avl_insert_book(book_node* root,book_node* book_ins)
{
      book_node* ret_val;
      int imbalance_index;
      if(root == NULL)
      {
          ret_val = book_ins;
      }
      else if(book_comp(root,book_ins) == 0)
      {
          ret_val = root;
      }
      else 
      {
          ret_val = root;
          if(book_comp(root,book_ins)>1)
          {
              root->left = avl_insert_book(root->left,book_ins);
          }
          else if(book_comp(root,book_ins) == -1)
          {
              root->right = avl_insert_book(root->right,book_ins);
          }

          root->height = 1 + max(height_of_book(root->left),height_of_book(root->right));
          imbalance_index = calculate_imbalance_book(root);
          
          if(imbalance_index > 1 && book_comp(root->left,book_ins) == 1)
          {
              ret_val = right_rotate_book(root);
          }
          else if(imbalance_index < -1 && book_comp(book_ins,root->right) == 1)
          {
              ret_val = left_rotate_book(root);
          }
          else if(imbalance_index > 1 && book_comp(root->left,book_ins) == -1)
          {
              root->left = left_rotate_book(root->left);
              ret_val = right_rotate_book(root);
          }
          else if(imbalance_index < -1 && book_comp(book_ins,root->right) == -1)
          {
              root->right = right_rotate_book(root->right);
              ret_val = left_rotate_book(root);
          }
        

      }
      return ret_val;
}
/***********************************************************
 Functions for AVL tree insertion of a member***************/
int height_of_member(member_node* root)
{
    int ret_val;
    if(root == NULL)
    {
        ret_val = 0;
    }
    else
    {
        ret_val = root->height;
    }
    return ret_val;
}
int calculate_imbalance_member(member_node* root)
{
    int ret_val = 0;
    if(root != NULL)
    {
        ret_val = height_of_member(root->left) - height_of_member(root->right);
    }
    return ret_val;
}
member_node* left_rotate_member(member_node* root)
{
    member_node* x = root->right;
    member_node* T2 = x->left;

    x->left = root;
    root->right = T2;

    root->height = max(height_of_member(root->left), height_of_member(root->right))+1;
    x->height = max(height_of_member(x->left), height_of_member(x->right))+1;
 
    
    return x;
}
member_node* right_rotate_member(member_node* root)
{
    member_node *x = root->left;
    member_node *T2 = x->right;
 
    x->right = root;
    root->left = T2;
 

    root->height = max(height_of_member(root->left), height_of_member(root->right))+1;
    x->height = max(height_of_member(x->left), height_of_member(x->right))+1;
 
    
    return x;
}
member_node* avl_insert_member(member_node* root,member_node* member_ins,int (*member_comp)(member_node*,member_node*))

//third argument is like a key for comparing both nodes
//Similar to utility we have in languages like python....

{
      member_node* ret_val;
      int imbalance_index;
      if(root == NULL)
      {
          ret_val = member_ins;
      }
      else if(member_comp(root,member_ins) == 0)
      {
          ret_val = root;
      }
      else 
      {
          ret_val = root;
          if(member_comp(root,member_ins)>1)
          {
              root->left = avl_insert_member(root->left,member_ins,member_comp1);//passing function pointer for member_comp1
                                                                                 //function
          }
          else if(member_comp(root,member_ins) == -1)
          {
              root->right = avl_insert_member(root->right,member_ins,member_comp1);//passing function pointer for member_comp1
                                                                                 //function
          }

          root->height = 1 + max(height_of_member(root->left),height_of_member(root->right));
          imbalance_index = calculate_imbalance_member(root);
          
          if(imbalance_index > 1 && member_comp(root->left,member_ins) == 1)
          {
              ret_val = right_rotate_member(root);
          }
          else if(imbalance_index < -1 && member_comp(member_ins,root->right) == 1)
          {
              ret_val = left_rotate_member(root);
          }
          else if(imbalance_index > 1 && member_comp(root->left,member_ins) == -1)
          {
              root->left = left_rotate_member(root->left);
              ret_val = right_rotate_member(root);
          }
          else if(imbalance_index < -1 && member_comp(member_ins,root->right) == -1)
          {
              root->right = right_rotate_member(root->right);
              ret_val = left_rotate_member(root);
          }
        

      }
      return ret_val;
}
//Insertion of a book 
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
    book_ins->b.cur_fine = 0;
    book_ins->left = NULL;
    book_ins->right = NULL;
    
    book_ins->height = 1;

    book_root = avl_insert_book(book_root,book_ins);
    printf("\n\nBook inserted successfully\n\n");

}
//Insertion of a member
void create_member()
{
    int opt,i;
    member_node* new_mem_node = (member_node*)malloc(sizeof(member_node));
    new_mem_node->left = new_mem_node->right = NULL;
    new_mem_node->height = 0;
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
          (new_mem_node->m).times_paid_fine = 0;
          for(i=0;i<4;i++)
          {
              new_mem_node->m.borrowed_books[i] = NULL;
          }


          faculty_root = avl_insert_member(faculty_root,new_mem_node,member_comp1);
    }
    else if(opt == 2)
    {
          num_members += 1;
          printf("\n\nEnter name of student:\n\n");
          fflush(stdin);
          fgets((new_mem_node->m).name,100,stdin);
          printf("\n\nEnter department of student:\n\n");
          fgets((new_mem_node->m).dept,100,stdin);
          (new_mem_node->m).design = STUDENT;
          (new_mem_node->m).id = num_members;
          (new_mem_node->m).num_books_issued = 0;
          (new_mem_node->m).fine = 0;
          for(i=0;i<4;i++)
          {
              new_mem_node->m.borrowed_books[i] = NULL;
          }


          student_root = avl_insert_member(student_root,new_mem_node,member_comp1);
    }
    else
    {
        printf("\n\nPlease try next time by entering valid option....\n\n");
    }

}
//Printing data for the book
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
//checking if search string matche swith the database
int match(char str1[],char str2[])
{
    int ret_val = 0;
    if(strstr(str1,str2) != NULL)
       ret_val = 1;
    else 
       ret_val = 0;
    return ret_val;
}
//Searching book by title
void inorder_search_title(book_node* root,char title[],int * match_status)
{
    if(root != NULL)
    {
        inorder_search_title(root->left,title,match_status);
        if(match(root->b.title,title))
        {
          print_book_data(root);
          *match_status = 1;
        }
        inorder_search_title(root->right,title,match_status);

        
    }
}
int search_book_by_title()
{
     char title[NAME_SIZE];
     int match_found = 0;

     printf("Enter the title of the book you wanna search:\n");

     fgets(title,100,stdin);

     printf("\n\nBooks we found for you:\n\n ");

    inorder_search_title(book_root,title,&match_found);
    

     return match_found;
}
//Searching book by author
void inorder_search_author(book_node* root,char author[],int * match_status)
{
    if(root != NULL)
    {
        inorder_search_author(root->left,author,match_status);
        if(match(root->b.author,author))
        {
          print_book_data(root);
          *match_status = 1;
        }
        inorder_search_author(root->right,author,match_status);

        
    }
}
int search_book_by_author()
{
    char author[NAME_SIZE];

    int match_found = 0;

     printf("Enter the author's name for the book you wanna search:\n");

     fgets(author,100,stdin);

     printf("\n\nBooks we found for you:\n\n ");

     
     inorder_search_author(book_root,author,&match_found);

     
     return match_found;
}
//Checking if max numbers of books are issued for a member
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
//Searching and issuing books for a member
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
                printf("\nOoops!! No book found with this author - name,try another one...\n");
            }
            break;
        default:
            printf("\nOops!!! Please enter valid choice when trying again....\n\n");
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
           int found = 0;
           printf("\n\nNow as you know about details of the books you searched for plz enter about those of the book you wanna issue below.\n\n");
           book_node* book_tb_searched = (book_node*)malloc(sizeof(book_node));
           printf("\nSubject\n");
           fgets(book_tb_searched->b.subject,100,stdin);
           printf("\nTitle\n");
           fgets(book_tb_searched->b.title,100,stdin);
           printf("\nAuthor\n");
           fgets(book_tb_searched->b.author,100,stdin);
           printf("\nYear of publish:\n");
           scanf("%d",book_tb_searched->b.yr_of_pub);

           book_node*itr = book_root;

           while(itr != NULL && (!found))
           {
               if(book_comp(itr,book_tb_searched) == 0)
               {
                   found = 1;
               }
               else if(book_comp(itr,book_tb_searched) == 1)
               {
                   itr = itr->left;
               }
               else if(book_comp(itr,book_tb_searched) == -1)
               {
                   itr = itr->right;
               }

           }
           
           if(found == 1 && itr->b.status == AVAILABLE)
           {
               Date issue_date;
               printf("\n\nEnter the today's date:\n\n");
               printf("day:\t");
               scanf("%d",&(issue_date.day));
               printf("month:\t");
               scanf("%d",&(issue_date.month));
               printf("year:\t");
               scanf("%d",&(issue_date.year));
               (mem->m).borrowed_books[(mem)->m.num_books_issued] = itr;
               (mem->m).num_books_issued +=1 ;
               (itr->b).status = ISSUED;
               (itr->b).borrowed_by = (mem->m).id;
                if((mem->m).design == FACULTY)
                {
                    Date d = issue_date;
                    d = extend_date(d,30);
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
//reissuing book for a member
void reissue_book(member_node* mem)
{
    int found = 0,acc_no,i;
    printf("\n\nEnter accession number for the book you want to reissue:\n\n");
    scanf("%d",&acc_no);
    book_node* itr;
    i = 0;
    while(i<mem->m.num_books_issued && (!found))
    {
          if(mem->m.borrowed_books[i]->b.acc_no == acc_no)
          {
              found = 1;
              itr = mem->m.borrowed_books[i];
          }
          else
          {
              i++;
          }

    }
    if(found  && itr->b.borrowed_by == mem->m.id)
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
                 Date today;
                 printf("\nEnter todays date in space separated format day/month/year\n");
                 scanf("%d",&today.day,&today.month,&today.year);
                 if(date_diff(itr->b.renew_date,today) > 0)
                   mem->m.times_paid_fine += 1;
                 Date d = (itr->b).renew_date;
                 d = extend_date(d,30);
                 (itr->b).renew_date = d;
                 
                 itr->b.cur_fine = 0; // assumed that fine is recovered while reissuing
             }
             else 
             {
                 Date d = (itr->b).renew_date;
                 Date today;
                printf("\nEnter todays date in space separated format day/month/year\n");
                 scanf("%d",&today.day,&today.month,&today.year);
                 if(date_diff(itr->b.renew_date,today) > 0)
                   mem->m.times_paid_fine += 1;
                 d = extend_date(d,14);
                 (itr->b).renew_date = d;
                 itr->b.cur_fine = 0; // assumed that fine is recovered while reissuing
             }

        }

             printf("\n\nBook reissued successfully\n\n");
        

         
    }
    else{
        printf("\n\nPlease enter valid entry...You haven\'t issued this book\n\n");
    }
}
//return a book given member_node pointer
void ret_book(member_node* mem)
{
    int found = 0,acc_no,i;
    printf("\n\nEnter acc_no for the book you want to return:\n\n");

    scanf("%d",&acc_no);
    book_node* itr;
    i = 0;
    while(i<mem->m.num_books_issued && (!found))
    {
          if(mem->m.borrowed_books[i]->b.acc_no == acc_no)
          {
              found = 1;
              itr = mem->m.borrowed_books[i];
          }
          else
          {
              i++;
          }

    }


    
    if(found && itr->b.borrowed_by == mem->m.id)
    {
       (itr->b).status = AVAILABLE;
       (itr->b).renewed_times = 0;
       (itr->b).renew_date.day = 0;
       (itr->b).renew_date.month = 0;
       (itr->b).renew_date.year = 0;
       (itr->b).borrowed_by = 0;
       mem->m.num_books_issued -= 1;
       itr->b.cur_fine = 0; // assumed that fine is recovered while returning
       Date today;
        printf("\nEnter todays date in space separated format day/month/year\n");
        scanf("%d",&today.day,&today.month,&today.year);
        if(date_diff(itr->b.renew_date,today) > 0)
            mem->m.times_paid_fine += 1;

       printf("\n\nBook returned successfully\n\n");
    }
    else
    {
        printf("\n\nYou haven't issued this book...Kindly enter valid entry...\n\n");
    }
} 
//Functions to check whether max issue limit has reached for perticular member
void inorder_check_max_issues(member_node* root,int max_issues)
{
    if(root != NULL)
    {
        inorder_check_max_issues(root->left,max_issues);
        if(root->m.num_books_issued >= max_issues)
        {
            printf("%d\t%s\n",root->m.id,root->m.name);
        }
        inorder_check_max_issues(root->right,max_issues);
    }

}
void max_issues()
{
     printf("\n\nNames and member IDs of members who issued maximum no. of books:\n\n");
     printf("Faculty members:\n\n ");
     inorder_check_max_issues(faculty_root,MAX_ISSUES_FAC);
     
     printf("Student members:\n\n ");
     inorder_check_max_issues(student_root,MAX_ISSUES_STUD);
     
     printf("\n\n");
}
//Function To calculate fine against a member
int calc_fine(member_node* mem)
{
    Date todays_date;
    int i;
    printf("\n\nWhat\'s the date today:\n\n");
    printf("Day:\t");
    scanf("%d",&todays_date.day);
    printf("Month:\t");
    scanf("%d",&todays_date.month);
    printf("Year:\t");
    scanf("%d",&todays_date.year);
    float fine  = 0;
    for(i= 0;i < mem->m.num_books_issued ;i++)
    {
        Date renew_date = (mem->m.borrowed_books[i]->b).renew_date;
        
        fine = fine + date_diff(renew_date,todays_date)*0.50;
        (mem->m.borrowed_books[i]->b).cur_fine += date_diff(renew_date,todays_date)*0.50;
        
    }
    (mem->m).fine = fine;

}
//Function to print data of borrowers bearing maximum fine in their account
void visit_borrower(member_node*mem,int*max)
{
    calc_fine(mem);
    if(mem->m.fine > *max)
      *max = mem->m.fine;
}
void print_if_max_fined(member_node* mem,int * max)
{
    if(mem->m.fine == *max)
    {
        printf("\n%s\t%s\n",mem->m.id,mem->m.name);
    }
}
void inorder_trav_mem(member_node* root,int * max_fine_ptr,void (*visit)(member_node*,int*)) 
//Implemented function pointers
{
    if(root != NULL)
    {
        inorder_trav_mem(root->left,max_fine_ptr,visit);
        visit(root,max_fine_ptr);
        inorder_trav_mem(root->right,max_fine_ptr,visit);

    }
}
int max_fine()
{
    int max_f = 0;
    inorder_trav_mem(faculty_root,&max_f,visit_borrower);//passing  visit borrower function pointer
    inorder_trav_mem(student_root,&max_f,visit_borrower);
    return max_f;
}
void print_max_fine_borrowers()
{
    int max_f = max_fine();

    inorder_trav_mem(faculty_root,&max_f,print_if_max_fined);//passing print_if_max_fined function pointer
    inorder_trav_mem(student_root,&max_f,print_if_max_fined);
}
//Function to sort and display members' data according to condition given in Q6
void inorder_traverse_and_fill(member_node** n_root_ptr,member_node* root_ptr)
{
    if(root_ptr != NULL)
    {
        inorder_traverse_and_fill(n_root_ptr,root_ptr->left);

        *n_root_ptr = avl_insert_member(*n_root_ptr,root_ptr,member_comp2);

        inorder_traverse_and_fill(n_root_ptr,root_ptr->right);
    }
}

void print_member_data(member_node* root,int* dummy)
{
    printf("\nName :- %s\nDepartment :- %s\nID :- %d\nNumber of  books issued :- %d\nFine :- %d\n",root->m.name,root->m.dept
          ,root->m.id,root->m.num_books_issued,root->m.fine);
}

void sort_acc_to_borrows()
{
    member_node* n_root = NULL;
     
    inorder_traverse_and_fill(&n_root,faculty_root);
    inorder_traverse_and_fill(&n_root,student_root);

    inorder_trav_mem(n_root,NULL,print_member_data); //passing second arg as NULL as it is not required over here....

}



//Using 3-ordered btree for keeping track of defaulters as said in Q.7


struct btreeNode *createNode(member_node item, struct btreeNode *child) {
  struct btreeNode *newNode;
  newNode = (struct btreeNode *)malloc(sizeof(struct btreeNode));
  newNode->item[1] = item;
  newNode->count = 1;
  newNode->link[0] = root;
  newNode->link[1] = child;
  return newNode;
}


void insertValue(member_node item, int pos, struct btreeNode *node,
          struct btreeNode *child) {
  int j = node->count;
  while (j > pos) {
    node->item[j + 1] = node->item[j];
    node->link[j + 1] = node->link[j];
    j--;
  }
  node->item[j + 1] = item;
  node->link[j + 1] = child;
  node->count++;
}


void splitNode(member_node item, member_node *pval, int pos, struct btreeNode *node,
         struct btreeNode *child, struct btreeNode **newNode) {
  int median, j;

  if (pos > MIN)
    median = MIN + 1;
  else
    median = MIN;

  *newNode = (struct btreeNode *)malloc(sizeof(struct btreeNode));
  j = median + 1;
  while (j <= MAX) {
    (*newNode)->item[j - median] = node->item[j];
    (*newNode)->link[j - median] = node->link[j];
    j++;
  }
  node->count = median;
  (*newNode)->count = MAX - median;

  if (pos <= MIN) {
    insertValue(item, pos, node, child);
  } else {
    insertValue(item, pos - median, *newNode, child);
  }
  *pval = node->item[node->count];
  (*newNode)->link[0] = node->link[node->count];
  node->count--;
}


int setNodeValue(member_node item, member_node *pval,
           struct btreeNode *node, struct btreeNode** child) {
  int pos;
  if (!node) {
    *pval = item;
    *child = NULL;
    return 1;
  }

  if (item.m.times_paid_fine < node->item[1].m.times_paid_fine) {
    pos = 0;
  } else {
    for (pos = node->count;
       (item.m.times_paid_fine < node->item[pos].m.times_paid_fine && pos > 1); pos--)
      ;
    if (item.m.times_paid_fine == node->item[pos].m.times_paid_fine) {
      printf("Duplicates not allowed\n");
      return 0;
    }
  }
  if (setNodeValue(item, pval, node->link[pos], child)) {
    if (node->count < MAX) {
      insertValue(*pval, pos, node, *child);
    } else {
      splitNode(*pval, pval, pos, node, *child, child);
      return 1;
    }
  }
  return 0;
}


void insertion(member_node item) {
  int flag;
  member_node i;
  struct btreeNode *child;

  flag = setNodeValue(item, &i, root, &child);
  if (flag)
    root = createNode(i, child);
}


void copySuccessor(struct btreeNode *myNode, int pos) {
  struct btreeNode *dummy;
  dummy = myNode->link[pos];

  for (; dummy->link[0] != NULL;)
    dummy = dummy->link[0];
  myNode->item[pos] = dummy->item[1];
}


void rightShift(struct btreeNode *myNode, int pos) {
  struct btreeNode *x = myNode->link[pos];
  int j = x->count;

  while (j > 0) {
    x->item[j + 1] = x->item[j];
    x->link[j + 1] = x->link[j];
  }
  x->item[1] = myNode->item[pos];
  x->link[1] = x->link[0];
  x->count++;

  x = myNode->link[pos - 1];
  myNode->item[pos] = x->item[x->count];
  myNode->link[pos] = x->link[x->count];
  x->count--;
  return;
}


void leftShift(struct btreeNode *myNode, int pos) {
  int j = 1;
  struct btreeNode *x = myNode->link[pos - 1];

  x->count++;
  x->item[x->count] = myNode->item[pos];
  x->link[x->count] = myNode->link[pos]->link[0];

  x = myNode->link[pos];
  myNode->item[pos] = x->item[1];
  x->link[0] = x->link[1];
  x->count--;

  while (j <= x->count) {
    x->item[j] = x->item[j + 1];
    x->link[j] = x->link[j + 1];
    j++;
  }
  return;
}


void mergeNodes(struct btreeNode *myNode, int pos) {
  int j = 1;
  struct btreeNode *x1 = myNode->link[pos], *x2 = myNode->link[pos - 1];

  x2->count++;
  x2->item[x2->count] = myNode->item[pos];
  x2->link[x2->count] = myNode->link[0];

  while (j <= x1->count) {
    x2->count++;
    x2->item[x2->count] = x1->item[j];
    x2->link[x2->count] = x1->link[j];
    j++;
  }

  j = pos;
  while (j < myNode->count) {
    myNode->item[j] = myNode->item[j + 1];
    myNode->link[j] = myNode->link[j + 1];
    j++;
  }
  myNode->count--;
  free(x1);
}


void adjustNode(struct btreeNode *myNode, int pos) {
  if (!pos) {
    if (myNode->link[1]->count > MIN) {
      leftShift(myNode, 1);
    } else {
      mergeNodes(myNode, 1);
    }
  } else {
    if (myNode->count != pos) {
      if (myNode->link[pos - 1]->count > MIN) {
        rightShift(myNode, pos);
      } else {
        if (myNode->link[pos + 1]->count > MIN) {
          leftShift(myNode, pos + 1);
        } else {
          mergeNodes(myNode, pos);
        }
      }
    } else {
      if (myNode->link[pos - 1]->count > MIN)
        rightShift(myNode, pos);
      else
        mergeNodes(myNode, pos);
    }
  }
}




void insert_into_btree_design(member_node* root) // insert the data of defaulters into btree
{
    if(root != NULL)
    {
        insert_into_btree_design(root->left);
        if(root->m.times_paid_fine > 0)
           insertion(*root);
        insert_into_btree_design(root->right);
    }
}

void insert_in_btree()
{
    insert_into_btree_design(faculty_root);
    insert_into_btree_design(student_root);
}
void removeVal(struct btreeNode *myNode, int pos) {
  int i = pos + 1;
  while (i <= myNode->count) {
    myNode->item[i - 1] = myNode->item[i];
    myNode->link[i - 1] = myNode->link[i];
    i++;
  }
  myNode->count--;
}

int delValFromNode(member_node item, struct btreeNode *myNode) {
  int pos, flag = 0;
  if (myNode) {
    if (item.m.times_paid_fine < myNode->item[1].m.times_paid_fine) {
      pos = 0;
      flag = 0;
    } else {
      for (pos = myNode->count; (item.m.times_paid_fine < myNode->item[pos].m.times_paid_fine && pos > 1); pos--)
        ;
      if (item.m.times_paid_fine == myNode->item[pos].m.times_paid_fine) {
        flag = 1;
      } else {
        flag = 0;
      }
    }
    if (flag) {
      if (myNode->link[pos - 1]) {
        copySuccessor(myNode, pos);
        flag = delValFromNode(myNode->item[pos], myNode->link[pos]);
        if (flag == 0) {
          printf("Given data is not present in B-Tree\n");
        }
      } else {
        removeVal(myNode, pos);
      }
    } else {
      flag = delValFromNode(item, myNode->link[pos]);
    }
    if (myNode->link[pos]) {
      if (myNode->link[pos]->count < MIN)
        adjustNode(myNode, pos);
    }
  }
  return flag;
}

// Delete operaiton
void delete (member_node item, struct btreeNode *myNode) {
  struct btreeNode *tmp;
  if (!delValFromNode(item, myNode)) {
    printf("Not present\n");
    return;
  } else {
    if (myNode->count == 0) {
      tmp = myNode;
      myNode = myNode->link[0];
      free(tmp);
    }
  }
  root = myNode;
  return;
}

void searching(member_node item, int *pos, struct btreeNode *myNode) {
  if (!myNode) {
    return;
  }

  if (item.m.times_paid_fine < myNode->item[1].m.times_paid_fine) {
    *pos = 0;
  } else {
    for (*pos = myNode->count;
       (item.m.times_paid_fine < myNode->item[*pos].m.times_paid_fine && *pos > 1); (*pos)--)
      ;
    if (item.m.times_paid_fine == myNode->item[*pos].m.times_paid_fine) {
      printf("%d present in B-tree", item);
      return;
    }
  }
  searching(item, pos, myNode->link[*pos]);
  return;
}
void traversal(struct btreeNode *myNode) {
  int i;
  if (myNode) {
    for (i = 0; i < myNode->count; i++) {
      traversal(myNode->link[i]);
      if(myNode->item[i + 1].m.times_paid_fine >= 3)
      {
        print_member_data(&myNode->item[i+1],NULL);
      }
    }
    traversal(myNode->link[i]);
  }
}

int main()
{
	
}
