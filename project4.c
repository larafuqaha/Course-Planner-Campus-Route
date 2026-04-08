// Name: Lara Foqaha  ID: 1220071  Section: 5

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXBUILDINGS 100 //max # of buildings is 100
int buildingsCounter = 0;
#define MAXCOURSES 100 //max # of courses is 100
int coursesCounter = 0;
#define INFINITY -1

// implementation of queue used in topological sort
struct QueueNode;
typedef struct QueueNode* ptrQ;
struct QueueNode
{
    char str[25];
    ptrQ next;
};
typedef ptrQ Queue;

//implementation of the adjacency list struct (normal linked list) for courses
struct courseNode;
typedef struct courseNode* ptrC;
struct courseNode
{
    char courseName[25];
    int indegree; 
    ptrC next;
};
typedef ptrC courseList;

//implementation of courses graph struct
struct CGraphBlock;
struct CGraphBlock
{
    courseList list[MAXCOURSES]; //array of lists, 12 is the number of courses
};
typedef struct CGraphBlock* CGraph;
CGraph coursesGraph;

//implementation of topological list struct
struct listNode;
typedef struct listNode* ptrL;
struct listNode 
{
    char courseName[25];
    ptrL next;
};
typedef ptrL topList; 
topList T; //declaring a global list for topological sort

struct buildingVertex;
struct buildingVertex
{
    char buildingName[25];
    int distance; //distance from the source
    int index; //the building index in the array
};
typedef struct buildingVertex* Bvertex;

struct minHeap; 
struct minHeap
{
    int size; //number of elements in the heap
    int capacity; //maximum number of elements allowed
    Bvertex *heapArray; //array of vertices
};
typedef struct minHeap* ptrMinHeap;

int buildingsGraph[MAXBUILDINGS][MAXBUILDINGS]; //declaring global buildings graph (adjacency matrix)
char buildings[MAXBUILDINGS][25];


void menu();
ptrQ createQueue();
void deleteQueue (Queue Q);
void enqueue(Queue Q, char word[]);
char* dequeue (Queue Q);
CGraph createCoursesGraph ();
void insertCourse (CGraph G, char CName[], int index);
void addCourseEdge (CGraph G, char course[], char preCourse[]);
void loadCoursesFile();
void printGraph(CGraph graph);
topList createTopList ();
void deleteTopList (topList T);
void insertTopList (topList T, char word[]);
void topSort (CGraph G);
void printTopList (topList T);
void loadBuildingFile ();
int calculateIndex(char biuldingName[], char Buildings[][25]);
ptrMinHeap createMinHeap(int Capacity);
void initializeDistances(int distance[], int sourceIndex, int numBuildings);
void dijkstra (char sourceBuilding[], char destBuilding[]);
int findMinDistanceIndex (int distance[], int isVisited[]);

int main ()
{
    coursesGraph = createCoursesGraph();
    T = createTopList();
    int option;
    do
    {
        menu();
        scanf("%d",&option);
        printf("\n");
        switch (option)
        {
        case 1:
            printf("Loading the input files...\n\n");
            loadCoursesFile();
            loadBuildingFile();
            printf("Input files have been loaded.\n\n");
            //printGraph(coursesGraph);
            break;
        case 2:
            printf("Enter the name of the first building.\n\n");
            char building1[25];
            scanf("%s",building1);
            printf("Enter the name of the seconf building.\n\n");
            char building2[25];
            scanf("%s" , building2);
            dijkstra(building1, building2);
            break;
        
        case 3:
            //
            break;

        case 4:
            printf("Sorting the courses using Topological sort...\n\n");
            topSort(coursesGraph);
            printf("Courses have been sorted.\n\n");
            break;
        case 5:
            printTopList(T);
            break;
        case 6:
            printf("Exited.\n\n");
            break;
        
        default:
            printf("No such option!\n\n");
            break;
        }

    } while (option != 6);

    deleteTopList(T);
    
    return 0;
}

void menu()
{
    printf("Choose an option from 1 to 6.\n");
    printf("1- Load the two files.\n");
    printf("2- Calculate the shortest distance between two buildings.\n");
    printf("3- Print the shortest route between two buildings and the total distance.\n");
    printf("4- Sort the courses using the topological sort.\n");
    printf("5- Print the sorted courses.\n");
    printf("6- Exit the application.\n");
}

Queue createQueue()
{
    Queue Q = (Queue)malloc(sizeof(struct QueueNode));
    if (Q == NULL)
    {
        printf("Out of memory.\n");
        return NULL;
    }
    Q->next = NULL;
    return Q;
}

void deleteQueue (Queue Q)
{
    if (Q == NULL)
    {
        printf("Queue not found.\n");
        return;
    }
    ptrQ temp = Q->next;
    ptrQ nextTemp = temp;
    while (temp != NULL)
    {
        nextTemp = nextTemp->next;
        free (temp);
        temp = nextTemp;
    }
    free (Q);
}

void enqueue(Queue Q, char word[]) //it inserts at the last of queue
{
    if (Q == NULL)
    {
        printf("Queue not found.\n");
        return;
    }

    //creating a new queue node
    ptrQ addedNode = (ptrQ)malloc(sizeof(struct QueueNode));
    if (addedNode == NULL)
    {
        printf("Out of memory.\n");
        return;
    }
    strcpy(addedNode->str, word);

    if (Q->next == NULL) //if the list is empty, add the new node after the head
    {
        addedNode->next = NULL;
        Q->next = addedNode;
    }
    else //trverse throught the list and add the new node after the last one
    {
        ptrQ last = Q;
        while (last->next != NULL) //to find the last node to enqueue after it
            last = last->next;
        addedNode->next = NULL;
        last->next = addedNode;
    }
}

char* dequeue (Queue Q) //returns the word at the first node of the queue
{
    if (Q == NULL)
    {
        printf("Queue not found.\n");
        return NULL;
    }

    if (Q->next == NULL) //if the list is empty
    {
        printf("The queue is empty.\n");
        return NULL;
    }

    ptrQ front = Q->next;
    //copying the string in front node into word 
    char* word = strdup(front->str); // same thing as allocating memory (malloc) for word to have the same size as front->st,
                                           // then using strcpy to copy contents into word

    if (front->next == NULL) //if there is only one node
        {
            Q->next = NULL;
            free(front);
        }
    else 
        {
            Q->next = front->next;
            free(front);
        }
    
    return word; 
}

CGraph createCoursesGraph ()
{
    CGraph G = (CGraph)malloc(sizeof(struct CGraphBlock));
    if (G == NULL)
    {
        printf("Out of memory.\n");
        return NULL;
    }

    //initializing the graph
    for (int i=0; i<MAXCOURSES; i++)
        G->list[i] = NULL;
    return G;
}

void insertCourse (CGraph G, char CName[], int index) //adds the courses to the array of lists
{
    if (G == NULL)
    {
        printf("Courses Graph not found.\n");
        return;
    }
    ptrC addedCourse = (ptrC)malloc(sizeof(struct courseNode));
    if (addedCourse == NULL)
    {
        printf("Out of memory.\n");
        return;
    }

    strcpy(addedCourse->courseName, CName);
    addedCourse->indegree = 0;
    addedCourse->next = NULL;
    G->list[index] = addedCourse;
}

void addCourseEdge (CGraph G, char course[], char preCourse[]) //adds courses to their prerequsite courses lists and increments theit indegree
{
    if (G == NULL)
    {
        printf("Graph not found.\n");
        return;
    }

    int courseIndex = -1, preCourseIndex = -1;
    for (int i = 0; i<MAXCOURSES; i++)
    {
        if (G->list[i] != NULL && strcmp(G->list[i]->courseName, course) == 0) //finding the index of the course to increment it
            courseIndex = i;
        if (G->list[i] != NULL && strcmp(G->list[i]->courseName, preCourse) == 0) //finding the index of the prerequisite course to add to it
            preCourseIndex = i;
    }

    if (courseIndex == -1)
    {
        printf("Course '%s' not found.\n", course);
        return;
    }
    if (preCourseIndex == -1)
    {
        printf("Course '%s' not found.\n", preCourse);
        return;
    }

    // when the courses are found, add the course to its preCourse list
    ptrC addedCourse = (ptrC)malloc(sizeof(struct courseNode));
    if (addedCourse == NULL)
    {
        printf("Out of memory.\n");
        return;
    }
    strcpy(addedCourse->courseName, course);
    addedCourse->next = NULL;
    ptrC last = G->list[preCourseIndex];
    while (last->next != NULL)
        last = last->next;
    last->next = addedCourse;

    G->list[courseIndex]->indegree++;
}

void loadCoursesFile()
{
    FILE* f = fopen("input_courses.txt","r");
    if (f == NULL)
    {
        printf("Can't open the courses file.\n");
        return;
    }

    char line[50];
    int index = 0; 
    while (fgets(line,50,f) != NULL)
    {
        char* course;
        course = strtok(line,"#\n");
        insertCourse(coursesGraph, course, index);
        coursesCounter++;

        char* preCourse; 
        preCourse = strtok(NULL, "#\n");
        while (preCourse != NULL)
        {
            addCourseEdge(coursesGraph, course, preCourse);
            preCourse = strtok(NULL, "#\n");
        }
        index++;
    }
    fclose(f);
}

void printGraph(CGraph graph)
{
    for (int i = 0; i < MAXCOURSES; i++)
    {
        if (graph->list[i] != NULL)
        {
            ptrC temp = graph->list[i];
            printf("%s with indegree %d: ", temp->courseName, temp->indegree);
            ptrC nextTemp = temp->next;
            while (nextTemp != NULL)
            {
                printf("%s -> ", nextTemp->courseName);
                nextTemp = nextTemp->next;
            }
            printf("\n");
        }
    }
}

topList createTopList () //creates an empty list for topological sort
{
    topList T = (topList)malloc(sizeof(struct listNode));
    if (T == NULL)
    {
        printf("Out of memoty.\n");
        return NULL;
    }
    T->next = NULL;
    return T;
}

void deleteTopList (topList T)
{
    if (T == NULL)
    {
        printf("List not found.\n");
        return;
    }
    ptrL P, temp;
    P = T->next;
    T->next = NULL;

    while(P != NULL)
    {
        temp = P->next;
        free(P);
        P=temp;
    }
}

void insertTopList (topList T, char word[]) //inserts the word at the end of the topological list
{
    if (T == NULL)
    {
        printf("List not found.\n");
        return;
    }
    ptrL addedWord = (ptrL)malloc(sizeof(struct listNode));
    if (addedWord == NULL)
    {
        printf("Out of memory.\n");
        return;
    }

    strcpy(addedWord->courseName, word);
    addedWord->next = NULL;
    ptrL last = T;
    while (last->next != NULL)
        last = last->next;
    last->next = addedWord;
}

void topSort (CGraph G)
{
    Queue Q = createQueue();
    int inDegree[MAXCOURSES];
    int topNum[MAXCOURSES];

    for (int i =0; i<MAXCOURSES; i++)
    {
        if (G->list[i] != NULL)
        {
            inDegree[i] = G->list[i]->indegree; //initializing the indegree array
            if (inDegree[i] == 0)
                enqueue(Q, G->list[i]->courseName); //adding the courses with zero indegree to the queue
        }
    }

    while (Q->next != NULL) //while the queue is not empty
    {
        char* course = dequeue(Q); // u is the course
        insertTopList(T, course); //adding u to T

        //removing all outgoing edges form u
        for (int i = 0; i<MAXCOURSES; i++)
        {
            if (G->list[i] != NULL && strcmp(G->list[i]->courseName, course) == 0)
            {
                ptrC temp = G->list[i]->next;
                while (temp != NULL)
                {
                    for (int j=0; j<MAXCOURSES; j++)
                    {
                        if (G->list[j] != NULL && strcmp(G->list[j]->courseName, temp->courseName) == 0)
                        {
                            G->list[j]->indegree --;
                            if (G->list[j]->indegree == 0) //if w is adjacent to u and its indegree becomes 0, add it to queue
                                enqueue(Q, G->list[j]->courseName);
                            break;
                        }
                    }
                    temp = temp->next;
                }
                break;
            }
        }
    }

    deleteQueue (Q);
}

void printTopList (topList T)
{
    if (T == NULL)
    {
        printf("List not found.\n");
        return;
    }
    if (T->next == NULL)
    {
        printf("The list is empty.\n");
        return;
    }

    ptrL temp = T->next;
    while (temp != NULL)
    {
        printf("%s, ", temp->courseName);
        temp = temp->next;
    }
    printf("\n\n");
}

void loadBuildingFile ()
{
    FILE* f = fopen("input_buildings.txt","r");
    if (f == NULL)
    {
        printf("Can't open the buildings file.\n");
        return;
    }

    char line[50];

    while (fgets(line,50,f) != NULL)
    {
        char *sourceBuilding, *destBuilding;
        int destination;

        sourceBuilding = strtok(line,"#");
        destBuilding = strtok(NULL, "#");
        destination = atoi(strtok(NULL, "\n")); //converting string to int

        int sourceIndex = calculateIndex(sourceBuilding, buildings);
        if (sourceIndex == -1) //if the building doesn't already exist, add it to the buildings array
        {
            strcpy(buildings[buildingsCounter], sourceBuilding);
            sourceIndex = buildingsCounter++; //update source index to insert to the graph and increment the # of buildings
        }

        int destIndex = calculateIndex(destBuilding, buildings);
        if (destIndex == -1)
        {
            strcpy(buildings[buildingsCounter], destBuilding);
            destIndex = buildingsCounter++;
        }

        buildingsGraph[sourceIndex][destIndex] = destination;
    }
    
    fclose(f);
}

int calculateIndex(char biuldingName[], char buildings[][25])
{
    for (int i = 0; i< buildingsCounter; i++)
    {
        if (strcmp(biuldingName, buildings[i]) == 0)
            return i;
    }
    return -1; //the building does not exist 
}   

ptrMinHeap createMinHeap(int Capacity)
{
    ptrMinHeap minHeap = (ptrMinHeap)malloc(sizeof(struct minHeap));
    if (minHeap == NULL)
    {
        printf("Out of space.\n");
        return NULL;
    }

    minHeap->size = 0; //initializing it to 0 at first
    minHeap->capacity = Capacity;
    minHeap->heapArray = (Bvertex*)malloc(sizeof(Bvertex) * Capacity);
    if (minHeap->heapArray == NULL)
    {
        printf("Out of space.\n");
        return NULL;
    }
    return minHeap;
}

void initializeDistances(int distance[], int sourceIndex, int numBuildings) //sets the distance from the source to the buildings to infinity at first
{
    for (int i = 0; i < numBuildings; i++)
    {
        if (i == sourceIndex)
            distance[i] = 0;
        else 
            distance[i] = INFINITY;
    }
}

void dijkstra (char sourceBuilding[], char destBuilding[])
{
    int sourceIndex = calculateIndex(sourceBuilding, buildings); 
    if (sourceIndex == -1)
    {
        printf("Building '%s' does not exist.\n\n", sourceBuilding);
        return;
    }
    int destIndex = calculateIndex(destBuilding, buildings);
    if (destIndex == -1)
    {
        printf("Building '%s' does not exist.\n\n", destBuilding);
        return;
    }

    int distance[MAXBUILDINGS]; //stores the distance between a building and the source building
    initializeDistances(distance, sourceIndex, buildingsCounter); 
    int isVisited[MAXBUILDINGS]; //stores if each building has been visited (1) or not (0)
    for (int i=0;i<MAXBUILDINGS; i++)
        isVisited[i] = 0;

    for (int i = 0; i <buildingsCounter; i++)
    {
        int minIndex = findMinDistanceIndex(distance, isVisited); //finding the index of the building with min distance
        isVisited[minIndex] = 1; //updating it to 1 after it was visited
        for (int j = 0; j<buildingsCounter; j++)
            if (isVisited[j] == 0) 
                if (buildingsGraph[minIndex][j] != INFINITY && distance[minIndex] != INFINITY) 
                    if (distance[minIndex] + buildingsGraph[minIndex][j] < distance[j]) 
                        distance[j] = distance[minIndex] + buildingsGraph[minIndex][j];
        
    }      
    printf("shortest : %d", distance[destIndex]);         

}

int findMinDistanceIndex (int distance[], int isVisited[]) //returns the index of the unvisited building that has the min distance from source
{
    int min = distance[0], minIndex = 0; //initializing min to compare it to others
    for (int i = 1; i< buildingsCounter; i++)
        if (distance[i] < min && isVisited[i] == 0)
        {
            min = distance[i];
            minIndex = i;
        }
    return minIndex;
}