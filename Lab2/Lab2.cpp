//aa aa 2000 5 5 5 5 5 5 4
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const int g_NUMBER_OF_MARKS = 7;
float g_AverGroupMark = 0;
int g_nStudentCnt = 0;
struct Sstudents {
    char cName[20];
    char cSurname[20];
    int nYear;
    int nMarks[8];
    float fMarkAver;
    struct Sstudents* pNext;
}table;

struct Sstudents* first = NULL;

//Sorting the Structure by the field cSurname in descending order
void sortStruct()
{
    struct Sstudents* root = first;
    struct Sstudents* new_first = NULL;
    while (root != NULL)
    {
        struct Sstudents* new_element = root;
        root = root->pNext;
        //if new_element should be first element
        if (new_first == NULL || strcmp(new_element->cSurname, new_first->cSurname) < 0)
        {
            new_element->pNext = new_first;
            new_first = new_element;
        }
        else
        {
            struct Sstudents* current = new_first;
            //looking for a place to insert new_element
            while (current->pNext != NULL && strcmp(current->pNext->cSurname, new_element->cSurname) < 0)
            {
                current = current->pNext;
            }
            new_element->pNext = current->pNext;
            current->pNext = new_element;
        }
    }
    first = new_first;
}
//printing the Structure as a spreadsheet
void printStruct()
{
    struct Sstudents* currentStudent = first;
    while (currentStudent != NULL)
    {
        printf("%s %s %d ", currentStudent->cName, currentStudent->cSurname, currentStudent->nYear);
        for (int j = 0; j < g_NUMBER_OF_MARKS; j++)
        {
            printf("%d ", currentStudent->nMarks[j]);
        }printf("\n");
        currentStudent = currentStudent->pNext;
    }
    printf("\n");
}
//erasing students whosе average mark is worse than the average group mark
void erasingBadStudents()
{
    struct Sstudents* currentStudent = first;
    while (1)
    {
        struct Sstudents* pPrevStudent = currentStudent;
        currentStudent = currentStudent->pNext;
        if (!currentStudent) break;
        if (currentStudent->fMarkAver < g_AverGroupMark)
        {
            if (currentStudent->pNext == NULL) { pPrevStudent->pNext = NULL; break; }
            pPrevStudent->pNext = currentStudent->pNext;
            free(currentStudent);
            currentStudent = pPrevStudent;
        }
    }
    currentStudent = first;
    if (currentStudent->fMarkAver < g_AverGroupMark) {
        if (currentStudent->pNext) first = currentStudent->pNext;
        else first = NULL;
        free(currentStudent);
    }
}
//adding a new student as a new first element
//then in main() sorting the structure
void addingStudent()
{
    int x;
    float nMarksCnt = 0;
    scanf("%s", &table.cName);
    scanf("%s", &table.cSurname);
    scanf("%d", &table.nYear);
    printf("\n");
    struct Sstudents* new_student = (struct Sstudents*)malloc(sizeof(struct Sstudents));

    for (int j = 0; j < g_NUMBER_OF_MARKS; j++)
    {
        scanf("%d", &x);
        nMarksCnt += x;
        new_student->nMarks[j] = x;
    }

    strcpy(new_student->cSurname, table.cSurname);
    strcpy(new_student->cName, table.cName);
    new_student->nYear = table.nYear;
    new_student->fMarkAver = nMarksCnt / g_NUMBER_OF_MARKS;
    new_student->pNext = first;
    first = new_student; 
}

int main()
{
    // getting data from file info.txt
    FILE* fp = fopen("info.txt", "r");

    struct Sstudents* pCurrentStudent = (struct Sstudents*)malloc(sizeof(struct Sstudents));
    int nVal;
    struct Sstudents *pPrevStudent=NULL;
    while (!feof(fp))
    {
        float sumMarksCurrent = 0;
        fscanf(fp, "%s %s %d", &table.cName, &table.cSurname, &table.nYear);
        strcpy(pCurrentStudent->cName, table.cName);
        strcpy(pCurrentStudent->cSurname, table.cSurname);
        pCurrentStudent->nYear = table.nYear;
        for (int j = 0; j < g_NUMBER_OF_MARKS; j++)
        {
            fscanf(fp, "%d", &nVal);
            
            sumMarksCurrent += nVal;
            pCurrentStudent->nMarks[j] = nVal;
        }
        pCurrentStudent->fMarkAver = sumMarksCurrent / g_NUMBER_OF_MARKS;
        g_AverGroupMark += pCurrentStudent->fMarkAver;
        
        g_nStudentCnt++;
        if (!first) { first = pCurrentStudent; }
        struct Sstudents* nextStudent = (struct Sstudents*)malloc(sizeof(struct Sstudents));
        pPrevStudent = pCurrentStudent;
        pCurrentStudent->pNext = nextStudent;
        pCurrentStudent = nextStudent;
    }
    //Last pointer is NULL
    pPrevStudent->pNext = NULL;

    g_AverGroupMark /= g_nStudentCnt ;

    printf("FIRST LIST:\n");
    printStruct();
    
    printf("SORTED LIST:\n");
    sortStruct();
    printStruct();

    printf("Add a new student:\n");
    addingStudent();
    sortStruct();

    printf("LIST WITH A NEW STUDENT:\n");
    printStruct();


    printf("LIST WITHOUT BAD STUDENTS:\n");
    erasingBadStudents();
    printStruct();

}
