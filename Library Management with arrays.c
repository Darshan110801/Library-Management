#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define NAME_SIZE 100
#define CAPACITY 10000
#define MBR_SIZE 100
#define MIF 4
#define MIS 2
#define TWO_WEEKS 1209600
#define DAY 86400

int books_count = 0;
int students_count = 0;
int profs_count = 0;

typedef struct book{
	char name[NAME_SIZE];
	char subject[NAME_SIZE];
	char author[NAME_SIZE];
	int acc_no;// acc_no. = books_count+1 at any point for newly added book
	int issued;
	int yr; //year of publish or print....
	int hall_n;
	int rack_n;
	int row_n;
	int borrowed_by;//id of the memeber who borrowed the book
	int times_renewed;
	long int renew_time;
}book;

typedef struct member{
	char name[NAME_SIZE];
    char dept[NAME_SIZE];
    char status[10];
    int id;//the member id is equal to the number in chronological order of joining of members...
    int books_issued;
    float fine;
}member;

typedef struct library{
	book books[CAPACITY];
	member students[MBR_SIZE];//students id can be between 100 and 200 excluding 200
	member profs[MBR_SIZE];//profs id can be between 200 and 300 excluding 300
}library;

library my_lib; // my library 
int compare_books(book book1,book book2)
{
	int ret_val,sub_comp,title_comp,auth_comp;
	sub_comp = strcmp(book1.subject,book2.subject);
	if(sub_comp>0)
      ret_val = 1;
    else if(sub_comp<0)
      ret_val = -1;
    else
      {
      	title_comp = strcmp(book1.name,book2.name);
      	if(title_comp>0)
      	  ret_val = 1;
   	    else if(title_comp<0)
   	      ret_val = -1;
        else
          {
          	auth_comp = strcmp(book1.author,book2.author);
         	if(auth_comp>0)
      	      ret_val = 1;
   	        else if(auth_comp<0)
   	          ret_val = -1;
            else
              {
              	if(book1.yr>book2.yr)
              	  ret_val = 1;
           	    else if(book1.yr<book2.yr)
           	      ret_val = -1;
       	        else
       	          ret_val = 0;
			  }
            
		  }
      	
	  }
    return ret_val;
}
int compare_borrowers(member m1,member m2)
{
	int ret_val;
	if(m1.books_issued>m2.books_issued)
      ret_val =1;
    else if(m1.books_issued<m2.books_issued)
      ret_val =-1;
    else
      {
      	if(m1.id>=200 && m2.id<200)
      	  ret_val =1;
   	    else if(m1.id<200 && m2.id>=200)
          ret_val = -1;
        else
          {
          	int comp=strcmp(m1.name,m2.name);
          	if(comp>0)
        	  ret_val =1;
       	    else if (comp<0)
              ret_val =-1;
            else
              ret_val = 0;
		  }
	  }
    return ret_val;
}
void merge(member members[],int l,int mid,int h)
{
	member a[mid-l+1];
	member b[h-mid];
	int i,j,k;
	k = l;
	for(i=0;i<mid-l+1;i++)
	{
		a[i] = members[k++];
	}
	for(i=0;i<h-mid;i++)
	{
		b[i] = members[k++];
	}
	i=0;
	j=0;
	k=l;
	while(i<mid-l+1 && j<h-mid)
	{
		int comp = compare_borrowers(a[i],b[j]);
		if(comp==1)
          members[k++]=a[i++];
        else 
          members[k++]=b[j++];
	}
	while(i<mid-l+1)
	    members[k++] = a[i++];
    while(j<h-mid)
        members[k++] = b[j++];
}
void sort_helper(member members[],int l,int h)
{
	if(l<h)
	{
		int mid = (l+h)/2;
		sort_helper(members,l,mid);
		sort_helper(members,mid+1,h);
		merge(members,l,mid,h);
	}
}
void sort_borrowers()
{
	member members[2*MBR_SIZE];
	int count = 0,i;
	for(i=0;i<profs_count;i++)
	   members[count++] = my_lib.profs[i];
    for(i=0;i<students_count;i++)
       members[count++] = my_lib.students[i];
    sort_helper(members,0,count-1);
	printf("Sorted list of borrowers in descending order of borrows:\n\n");  
	for(i=0;i<count;i++)
       printf("%s\n",members[i].name);
}
void book_insertion()
{
	int i;
	book b;
	char temp[10];
	printf("Enter information about book:\n\n");
    printf("\n\nTitle:\n");
    scanf("%s",temp);
    fgets(b.name,100,stdin);
    printf("\n\nSubject:\n");
    fgets(b.subject,100,stdin);
    printf("\n\nAuthor name (surname first)\n");
    fgets(b.author,100,stdin);
    printf("\n\nYear of publish:\n");
    scanf("%d",&b.yr);
    printf("\n\nHall Number:\n");
    scanf("%d",&b.hall_n);
    printf("\n\nRack Number:\n");
    scanf("%d",&b.rack_n);
    printf("\n\nRow Number:\n");
    scanf("%d",b.row_n);
	b.acc_no = books_count+1;
	b.issued = 0;
	b.borrowed_by=0;
	b.times_renewed =0;
	b.renew_time = 0;
	i=books_count-1;
	if(i==(-1))
	  my_lib.books[i+1] = b;
	while(i>=0 && compare_books(my_lib.books[i],b)== 1)
	{
		my_lib.books[i+1]=my_lib.books[i];
		i--;
	}
	my_lib.books[i+1]=b;
	books_count = books_count+1;
	
	printf("\n\nBook Inserted successfully\n\n");
}

void create_member()
{
	int i;
    member m;
    char temp[100];
    printf("\n\nEnter Member Information\n\nEnter name:\n ");
    scanf("%s",temp);
    fgets(m.name,100,stdin);
    printf("\n\nEnter Department:\n");
    fgets(m.dept,100,stdin);
    printf("\n\nEnter status	(faculty/student):\n");
    fgets(m.status,10,stdin);
    m.id = 0;
    m.books_issued=0;
    m.fine = 0;
    
	
	if(strcmp(m.status,"faculty")==0)
	{
	 i = profs_count;
     my_lib.profs[i] = m;
	 my_lib.profs[i].id=200+profs_count;
	 profs_count += 1;
    } 
	else if(strcmp(m.status,"student")==0)
	{
	 i= students_count;
	 my_lib.students[i]=m;
	 my_lib.students[i].id=100+students_count;
	 students_count += 1;
	}
   	
}
void print_max_issues()/*As its not clear from the question i am printing names of members with 
issues equal to max_limit for the same as per there status...... */
{
	int i;
	printf("Faculties : \n\n");
	for(i=0;i<profs_count;i++)
	 {
	 	if(my_lib.profs[i].books_issued == MIF)
	 	  printf("  %s\n",my_lib.profs[i].name);
	 }
    i = 0;
    printf("Students : \n\n");
    for(i=0;i<students_count;i++)
     {
     	if(my_lib.students[i].books_issued == MIS)
  	      printf("  %s\n",my_lib.students[i].name);
     }
}
void book_search_issue()
{
	int option,entry,id,i;
	char key[100];
	printf("\n\nEnter member Id:\n\n");
	scanf("%d",&id);
	printf("\n\nPlease choose the option:\n\t1.  Search by name\n\t2.  Search by author\n");
	scanf("%d",&option);
	switch(option)
	{
		case 1:
			printf("Enter keywords for the book you want to search:\n");
			fgets(key,100,stdin);
			for(i=0;i<books_count;i++)
			{
				if(strstr(my_lib.books[i].name,key)!=NULL)
				{
					printf("acc_no. = %d\t%s\n",my_lib.books[i].acc_no,my_lib.books[i].name);
					
				}
			}
			
			break;
		case 2:
			printf("Enter name of the author:\n");
			fgets(key,100,stdin);
			for(i=0;i<books_count;i++)
			{
				if(strstr(my_lib.books[i].author,key)!=NULL)
				{
					printf("acc_no. = %d\t%s\n",my_lib.books[i].acc_no,my_lib.books[i].name);
					
				}
			}
			break;
	}
	printf("\n\nEnter acc_no of the book member wants to issue:\n\n");
	scanf("%d",&entry);
	if(my_lib.books[entry-1].issued==0)
	{
		if((id>=200 && my_lib.profs[id-200].books_issued == MIF)||(id>=100 && id<200 && my_lib.students[id-100].books_issued == MIS))
		 {
		 	printf("\n\nSorry the member has reached max. issues:\n\n");
		 }
	    else
	     {
	     	if(id>=200)
	     	{
			 	my_lib.books[entry-1].issued = 1;
	     		my_lib.books[entry-1].borrowed_by=id;
                my_lib.profs[id-200].books_issued = my_lib.profs[id-200].books_issued+1;
	     		time_t secs;
	     		time(&secs);
	     		my_lib.books[entry-1].renew_time = secs+TWO_WEEKS;
	     		secs = secs + TWO_WEEKS;
	     		struct tm* local = localtime(&secs);
  			   	printf("\n\nIssued the book successfully!!!\n\n");
	     		printf("\n\nDue date for Renew is %d/%d/%d\n\n",local->tm_mday,local->tm_mon,local->tm_year);
	        }
	        else
	        {
       		    my_lib.books[entry-1].issued = 1;
	     		my_lib.books[entry-1].borrowed_by=id;
                my_lib.students[id-100].books_issued = my_lib.students[id-100].books_issued+1;
	     		time_t secs;
	     		time(&secs);
	     		my_lib.books[entry-1].renew_time = secs+TWO_WEEKS;
	     		secs = secs + TWO_WEEKS;
	     		struct tm* local = localtime(&secs);
  			   	printf("\n\nIssued the book successfully!!!\n\n");
	     		printf("\n\nRenew date is %d/%d/%d\n\n",local->tm_mday,local->tm_mon,local->tm_year);
			}
   		 }
	}
	else
	{
		printf("\n\nSorry!!! The book is already issued to some other member\n\n");
	}
	
}
float members_fine(int id)
{
	int i;
	float fi = 0;
	for(i=0;i<books_count;i++)
	{
		if(my_lib.books[i].borrowed_by == id)
		{
			time_t secs;
			time(&secs);
			if(secs>my_lib.books[i].renew_time)
			  {
		  	     float fine_per_book = ((secs-my_lib.books[i].renew_time)/DAY)*0.50;
		  	     fi += fine_per_book;
			  }
		}
	}
	if(id>=200)
	{
		my_lib.profs[id-200].fine = fi;	
	}
	else
	{
		my_lib.students[id-100].fine = fi;
	}
	return fi;
}
void max_fine_mbrs()
{
	float max_fine = 0,f;
	int i;
	for(i=0;i<profs_count;i++)
	{
		f = members_fine(my_lib.profs[i].id);
		if(f>max_fine)
		  max_fine =f ;
	}
	for(i=0;i<students_count;i++)
	{
		f = members_fine(my_lib.students[i].id);
		if(f>max_fine)
          max_fine = f;
	}
	printf("\n\nMembers with max. fine:\n\nFaculties:\n\n");
	for(i=0;i<profs_count;i++)
	{
		if(max_fine == my_lib.profs[i].fine)
		  printf("%s\n",my_lib.profs[i].name);
	}
	printf("Students:\n\n");
	for(i=0;i<students_count;i++)
	{
		if(max_fine == my_lib.students[i].fine)
		   printf("%s\n",my_lib.students[i].name);
	}
}
void  renew()
{
	int acc;
	printf("\n\nEnter the accession no.of the book you want to reissue:\n\n");
	scanf("%d",&acc);
	if(acc>books_count)
     {
     	printf("\n\nSomething went wrong\n\n");
     	renew();
	 }
	if(my_lib.books[acc-1].times_renewed == 3)
	{
		printf("Sorry the book can not be reissued as you have reached maximum reissues\n\n");
	}
	else
	{
		my_lib.books[acc-1].times_renewed+=1;
		time_t now;
		time(&now);
		my_lib.books[acc-1].renew_time = now + TWO_WEEKS;
		struct tm *local = localtime(&now);
		printf("\n\nBook reissued Successfully:\n\n");
		printf("\n\n Due Date for further reissue/return is %d/%d/%d\n\n",local->tm_mday,local->tm_mon+1,local->tm_year+1900);
		
	}
}
void ret()
{
	int acc,id;
	printf("\n\nEnter your member id\n\n");
	scanf("%d",id);
	printf("\n\nEnter accession number of the book you want to return\n\n");
	scanf("%d",&acc);
	
	if(acc>books_count && !(id>=300 || id<100))
	{
		printf("\n\nSomething went wrong\n\n");
		ret();
	}
	else
	{
		my_lib.books[acc-1].issued = 0;
		my_lib.books[acc-1].times_renewed = 0;
		my_lib.books[acc-1].renew_time = 0;
		my_lib.books[acc-1].borrowed_by=0;
		if(id>=200)
           my_lib.profs[id-200].books_issued -= 1;
        else
           my_lib.students[id-100].books_issued -= 1;
        printf("\n\nThe book has been successfully returned\n\n");
	}
}

int main()
{
	int i,option,running,id;
	
	//initializing contents of library
	for(i=0;i<CAPACITY;i++)
	{
		strcpy(my_lib.books[i].name,"\0");
		strcpy(my_lib.books[i].subject,"\0");
		strcpy(my_lib.books[i].author,"\0");
		my_lib.books[i].acc_no = 0;
		my_lib.books[i].hall_n = 0;
		my_lib.books[i].rack_n = 0;
		my_lib.books[i].row_n = 0;
		my_lib.books[i].issued = 0;
		my_lib.books[i].yr=0;
		my_lib.books[i].borrowed_by=0;
		my_lib.books[i].times_renewed=0;
		my_lib.books[i].renew_time = 0;
	}
	for(i=0;i<MBR_SIZE;i++)
	{
		strcpy(my_lib.students[i].name,"\0");
		strcpy(my_lib.students[i].dept,"\0");
		strcpy(my_lib.students[i].status,"\0");
		my_lib.students[i].id=0;
		my_lib.students[i].books_issued=0;
		my_lib.students[i].fine=0;
		strcpy(my_lib.profs[i].name,"\0");
		strcpy(my_lib.profs[i].dept,"\0");
		strcpy(my_lib.profs[i].status,"\0");
		my_lib.profs[i].id=0;
		my_lib.profs[i].books_issued=0;
		my_lib.profs[i].fine=0;
	}
	running = 1;
	while(running)
	{
	
        printf("\n\n\t\tWelcome to my library\t\t\n\n");
   		printf("Choose any option:\n\n");
		printf("1.\tInsert new book\n\n");
  		printf("2.\tAdd new member\n\n");
		printf("3.\tSearch and issue the book\n\n");
		printf("4.\tRenew the book's due date'\n\n");
		printf("5.\treturn the book\n\n");
		printf("6.\tNames of faculty members who issued maximum number of books\n\n");
  		printf("7.\tFine against a member\n\n");
        printf("8.\tMembers having maximum fines\n\n");
  		printf("9.\tDescending order of borrowers as per books in their possession:\n\n");
  		printf("10.\tExit\n\n");
  		
	    printf("\n\nEnter your option:\n\n");
	    scanf("%d",&option);
	    switch(option)
	    {
	    	case 1:
	    		system("cls");
	    		book_insertion();
	    		printf("\n\nPress any key to go back to main menu..\n");
	    	    getch();
	    		system("cls");
	    		break;
    		case 2:
    			system("cls");
    			create_member();
    			printf("\n\nPress any key to go back to main menu..\n");
	    		getch();
	    		system("cls");
	    		break;
    		case 3:
    			system("cls");
    			book_search_issue();
    			printf("\n\nPress any key to go back to main menu..\n");
	    		getch();
	    		system("cls");
	    		break;
    		case 4:
    			system("cls");
    			renew();
    			printf("\n\nPress any key to go back to main menu..\n");
	    		getch();
	    		system("cls");
	    		break;
    		case 5:
    			system("cls");
    			ret();
    			printf("\n\nPress any key to go back to main menu..\n");
	    		getch();
	    		system("cls");
	    		break;
    		case 6:
    			system("cls");
    			print_max_issues();
   				printf("\n\nPress any key to go back to main menu..\n");
	    		getch();
	    		system("cls");
	    		break;
    		case 7:
    			system("cls");
    			printf("Enter member id:\n\n");
    			scanf("%d",&id);
    			printf("\nFine against member is %d\n\n",members_fine(id));
    			printf("\n\nPress any key to go back to main menu..\n");
	    		getch();
	    		system("cls");
	    		break;
    		case 8:
    			system("cls");
    			max_fine_mbrs();
   				printf("\n\nPress any key to go back to main menu..\n");
	    		getch();
	    		system("cls");
	    		break;
    		case 9:
    			system("cls");
    			sort_borrowers();
    			printf("\n\nPress any key to go back to main menu..\n");
	    		getch();
	    		system("cls");
	    		break;
    		case 10:
    			running = 0;
    			break;
   			default:
   				break;
    			
    			
    		
    			
		}
	    
	
	
    }
	return 0;
}
