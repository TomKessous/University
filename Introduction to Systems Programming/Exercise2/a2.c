#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// structures
typedef struct student {
    char *name;
    int  id;
    struct clist *courses;
} student;

typedef struct course {
    char *title;
    int  number;
    struct slist *students;
} course;

typedef struct slist {
    student      *info;
    struct slist *next;
} slist;

typedef struct clist {
    course       *info;
    struct clist *next;
} clist;


// prototypes
slist* add_student(slist *students, char *name, int id);
clist* add_course(clist *courses, char *title, int number);
void reg_student(slist *students, clist *courses, int id, int number);
void unreg_student(slist *students, int id, int number);
void print_students(slist *students);
void print_courses(clist *courses);
void free_all(slist *sl, clist *cl);
slist* sort_insert_std(slist* students, slist* add_std);
clist* sort_insert_course(clist* courses, clist* add_course);
slist* remove_std(int value, slist* head);
clist* remove_course(int value, clist* head);

//DO NOT TOUCH THIS FUNCTION
static void getstring(char *buf, int length) {
    int len;
    buf = fgets(buf, length, stdin);
    len = (int) strlen(buf);
    if (buf[len-1] == '\n')
        buf[len-1] = '\0';
}

//DO NOT TOUCH THIS FUNCTION 
int main() {
    slist* students = 0;
	clist* courses = 0;
    char  c;
    char  buf[100];
    int   id, num;
 
    do {
        printf("Choose:\n"
               "    add (s)tudent\n"
               "    add (c)ourse\n"
               "    (r)egister student\n"
               "    (u)nregister student\n"
			   "    (p)rint lists\n"
               "    (q)uit\n");
 
        while ((c = (char) getchar()) == '\n');
        getchar();
 
        switch (c) {
        case 's':
            printf("Adding new student.\n");
 
            printf("Student name: ");
            getstring(buf, 100);
 
            printf("Student ID: ");
            scanf("%d", &id);
 
            students = add_student(students, buf, id);
 
            break;
 
        case 'c':
            printf("Adding new course.\n");
 
            printf("Course name: ");
            getstring(buf, 100);
 
            printf("Course number: ");
            scanf("%d", &num);
 
            courses = add_course(courses, buf, num);
 
            break;
 
        case 'r':
            printf("Registering a student to a course.\n");
 
            printf("Student ID: ");
            scanf("%d", &id);
            
            printf("Course number: ");
            scanf("%d", &num);
 
            reg_student(students, courses, id, num);
 
            break;
 
        case 'u':
            printf("Unregistering a student from a course.\n");
 
            printf("Student ID: ");
            scanf("%d", &id);
            
            printf("Course number: ");
            scanf("%d", &num);
 
            unreg_student(students, id, num);
 
            break;
 
        case 'p':
            printf("Printing Information.\n");
 
			print_students(students);
			print_courses(courses);
 
            break;
 
        case 'q':
			printf("Quitting...\n");
            break;
        }
 
        if (c != 'q')
            printf("\n");
    } while (c != 'q');
	
	free_all(students,courses);
    return 0;
}
slist* sort_insert_std(slist* students, slist* add_std) {
    if (!students) return add_std;
    if (add_std->info->id < students->info->id) {
        add_std->next = students;
        return add_std;
    }
    students->next = sort_insert_std(students->next, add_std);
    return students;
}  
clist* sort_insert_course(clist* courses, clist* add_course) {
    if (!courses) return add_course;
    if (add_course->info->number < courses->info->number) {
        add_course->next = courses;
        return add_course;
    }
    courses->next = sort_insert_course(courses->next, add_course);
    return courses;
}
slist* add_student(slist* students, char* name, int id) {
    student std1, * pstd;
    pstd = &std1;
    pstd = (student*)malloc(sizeof(student));
    int n;
    if (!pstd) {
        //printf("memory allocation faild");
        exit(1);
    }
    pstd->name = (char*)malloc(strlen(name)+1);
    if (!(pstd->name)) {
        //printf("memory allocation faild");
        exit(1);
    }
    pstd->id = id;
    strcpy(pstd->name,name);
    pstd->courses = NULL;
    slist *add_std;                             //new item
    add_std = (slist*)malloc(sizeof(slist));    //allocating memory
    if (!add_std) {
        //printf("memory allocation faild");
        exit(1);
    }
    add_std->info = pstd;
    add_std->next = NULL;
    return sort_insert_std(students,add_std);         //addind the new item to the end of the list.
}
clist* add_course(clist* courses, char* title, int number) {
    course course1, * pcourse;
    pcourse = &course1;
    pcourse = (course*)malloc(sizeof(course));
    if (!pcourse) {
        //printf("memory allocation faild");
        exit(1);
    }
    pcourse->title = (char*)malloc(strlen(title) + 1);
    if (!(pcourse->title)) {
        //printf("memory allocation faild");
        exit(1);
    }
    pcourse->number = number;
    strcpy(pcourse->title, title);
    pcourse->students = NULL;
    clist* add_crs;                                     //*****this fuction is similar to add_student function******
    add_crs = (clist*)malloc(sizeof(clist));
    if (!add_crs) {
        //printf("memory allocation faild");
        exit(1);
    }
    add_crs->info = pcourse;
    add_crs->next = NULL;
    return sort_insert_course(courses, add_crs);
}
slist* remove_std(int value, slist* head) {
    slist* temp;
    if (!head) {
        printf("The record with key %d was not found\n", value);
        return NULL;
    }
    if (head->info->id == value) {
        temp = head;
        head = head->next;
        free(temp);
        return head;
    }
    head->next = remove_std(value, head->next);
    return head;
}
clist* remove_course(int value, clist* head) {
    clist* temp;
    if (!head) {
        printf("The record with key %d was not found\n", value);
        return NULL;
    }
    if (head->info->number == value) {
        temp = head;
        head = head->next;
        free(temp);
        return head;
    }
    head->next = remove_course(value, head->next);
    return head;
}
void reg_student(slist* students, clist* courses, int id, int number) {
    slist* curr_std;                        //current student in the list
    clist* curr_course;                     //current course in the list
    slist* new_std = NULL;
    clist* new_course = NULL;
    curr_std = students;                    //curr_std is the head of the list right now.
    while (!(curr_std->info->id == id)) {   //finding the item in students that contain the given id.
        curr_std = curr_std->next;
    }
    curr_course = courses;                  //curr_course is the head of the list right now.
    while (!(curr_course->info->number == number)) {        ////finding the item in courses that contain the given number.
        curr_course = curr_course->next;
    }
    new_std = (slist*)malloc(sizeof(slist));
    if (!new_std) {
        //printf("memory allocation faild");
        exit(1);
    }
    new_course = (clist*)malloc(sizeof(clist));
    if (!new_course) {
        //printf("memory allocation faild");
        exit(1);
    }
    new_std->info = curr_std->info;
    new_std->next = NULL;
    new_course->info = curr_course->info;
    new_course->next = NULL;
    curr_std->info->courses = sort_insert_course(curr_std->info->courses, new_course);
    curr_course->info->students = sort_insert_std(curr_course->info->students, new_std);
}
void unreg_student(slist* students, int id, int number) {
    slist* curr_std;
    clist* curr_course;
    student* selected_std;
    curr_std = students;
    if (curr_std == NULL) return;
    while (!(curr_std->info->id == id)) {               //findind the student with the given id
        curr_std = curr_std->next;
    }
    selected_std = curr_std->info;                     //this is the student
    curr_course = selected_std->courses;                //the list of course for this student
    if (curr_course == NULL) return;
    while (!(curr_course->info->number == number)) {               
        curr_course = curr_course->next;
    }
    curr_course->info->students = remove_std(id, curr_course->info->students);
    selected_std->courses = remove_course(number, selected_std->courses);
}  
void print_students(slist* students) {
    printf("STUDENT LIST:");
    slist* curr_std;
    clist* curr_course;
    curr_std = students;
    if (!(curr_std)) printf("EMPTY!\n");
    else printf("\n");
    while (curr_std) {
        printf("%d:", curr_std->info->id);
        printf("%s\n", curr_std->info->name);
        curr_course = curr_std->info->courses;
        if (!(curr_course)) {
            printf("student is not registered for courses.");
        }
        else printf("courses:");
        while (curr_course) {
            printf(" %d-", curr_course->info->number);
            printf("%s", curr_course->info->title);
            if (curr_course->next) printf(",");
            curr_course = curr_course->next;
        }
        printf("\n");
        curr_std = curr_std->next;
        
    }
}
void print_courses(clist* courses) {
    printf("COURSE LIST:");
    slist* curr_std;
    clist* curr_course;
    curr_course = courses;
    if (!(curr_course)) printf("EMPTY!\n");
    else printf("\n");
    while (curr_course) {
        printf("%d:", curr_course->info->number);
        printf("%s", curr_course->info->title);
        printf("\n");
        curr_std = curr_course->info->students;
        if (!(curr_std)) {
            printf("course has no students.");
        }
        if (curr_std) printf("students:");
        while (curr_std) {
            printf(" %d-", curr_std->info->id);
            printf("%s", curr_std->info->name);
            if (curr_std->next) printf(",");
            curr_std = curr_std->next;
        }
        printf("\n");
        curr_course = curr_course->next;
    }
}
void free_all(slist* sl, clist* cl) {
    slist* curr_std;
    clist* curr_course;
    slist* tmp_std;
    clist* tmp_course;
    curr_std = sl;
    while (curr_std) {
        curr_course = curr_std->info->courses;
        while (curr_course) {
            tmp_course = curr_course;
            curr_course = curr_course->next;
            free(tmp_course);
        }
        free(curr_std->info->name);
        free(curr_std->info);
        tmp_std = curr_std;
        curr_std = curr_std->next;
        free(tmp_std);
    }
    curr_course = cl;
    while (curr_course) {
        curr_std = curr_course->info->students;
        while (curr_std) {
            tmp_std = curr_std;
            curr_std = curr_std->next;
            free(tmp_std);
        }
        free(curr_course->info->title);
        free(curr_course->info);
        tmp_course = curr_course;
        curr_course = curr_course->next;
        free(tmp_course);
    }
}
