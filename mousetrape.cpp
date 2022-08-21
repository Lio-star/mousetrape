




#include <iostream>
#include <stdlib.h>
#define EX 2 //exit point co-ordinate (x)
#define EY 9 //exit point co-ordinate (y)
#define N 10 //dimension of the matrix; NxN

using namespace std;

struct node
{
    int x, y;
    struct node* next;
};

struct Head
{
    int count;
    struct node* top;
}*headVst, *headAlt;

struct Head* init(struct Head* head)
{
    head = new Head;
    head->top = NULL;
    head->count = 0;
    return head;
}

void push(int x, int y, struct Head* head)
{
    if(head->top == NULL)
    {
        struct node *temp = new node;
        temp->x = x;
        temp->y = y;
        temp->next = NULL;
        head->top = temp;
    }
    else
    {
        struct node *temp = new node;
        temp->x = x;
        temp->y = y;
        temp->next = head->top;
        head->top = temp;
    }
    head->count++;
}

struct node* pop(struct Head* head)
{
    struct node *temp1, *temp2;
    temp1 = head->top;
    if(temp1 == NULL)
    {
        cout<<"\n(!) the mouse is trapped; there are no possible exits."<<endl; //stack underflow
        exit(1);
    }
    temp2 = new node;
    temp2->x = temp1->x;
    temp2->y = temp1->y;

    head->top = head->top->next,
    delete temp1;
    head->count--;
    return temp2;
}

struct node* top(struct Head* head)
{
    struct node* temp;
    temp = head->top;
    return temp;
}

int maze[10][10] =
{
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 1, 0, 0, 1, 1, 1, 1, 1,
    1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 1, 0, 1, 1, 1, 1,
    1, 0, 1, 1, 0, 0, 0, 1, 0, 1,
    1, 0, 1, 1, 0, 1, 0, 1, 0, 1,
    1, 0, 1, 1, 0, 1, 0, 1, 0, 1,
    1, 0, 1, 0, 0, 1, 0, 0, 0, 1,
    1, 0, 0, 0, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

/*
//for test; this second maze has no exits, the mouse will be trapped
int maze[10][10] =
{
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 1, 0, 0, 1, 1, 1, 1, 1,
    1, 0, 1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 1, 0, 1, 1, 1, 1,
    1, 0, 1, 1, 0, 0, 0, 1, 0, 1,
    1, 0, 1, 1, 0, 1, 0, 1, 0, 1,
    1, 0, 1, 1, 0, 1, 0, 1, 0, 1,
    1, 0, 1, 0, 0, 1, 0, 0, 0, 1,
    1, 0, 0, 0, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};
*/


void runMaze(int x, int y)
{
    cout<<"path:"<<endl;

    push(x, y, headVst);
    cout<<"\n"<<x<<","<<y<<"\n";

    int prex = -1, prey = -1; //used to store previous co-ordinates

    if(x == EX && y == EY)
    {
        cout<<"\n(!) the mouse has exited the maze.\n";
        return;
    }
    else
    {
        struct node* temp1 = new node;
        while(!maze[x][y])
        {
            int c = 0;
            if(!maze[x][y+1]  &&  y+1 != prey  &&  y<N) //checking left
            {
                    temp1->x = x;
                    temp1->y = y+1;
                    c++;
            }
            if(!maze[x][y-1]  &&  y-1 != prey  &&  y>0) //checking right
            {
                if(c>0) //if this is not the first choice
                {
                    push(x, y-1, headAlt); //pushed to "alternatives stack"
                    c++;
                }
                else
                {
                    temp1->x = x;
                    temp1->y = y-1;
                    c++;
                }
            }
            if(!maze[x+1][y]  &&  x+1 != prex  &&  x<N) //checking down
            {
                if(c>0)
                {
                    push(x+1, y, headAlt);
                    c++;
                }
                else
                {
                    temp1->x = x+1;
                    temp1->y = y;
                    c++;
                }
            }
            if(!maze[x-1][y]  &&  x-1 != prex  &&x>0) //checking up
            {
                if(c>0)
                {
                    push(x-1, y, headAlt);
                    c++;
                }
                else
                {
                    temp1->x = x-1;
                    temp1->y = y;
                    c++;
                }
            }
            if(c>1) //if more than one path is found
            {
                //pushing (-1,-1) as a "decision token" as instructed in the textbook.
                push(-1, -1, headVst);
                cout<<"(!) at a decision point; choosing one of the possible routes..."<<endl;
            }
            if(c>0) //if at least one path is found
            {
                //storing the current co-ordinates before changing them
                prex = x;
                prey = y;
                //the mouse takes a step ahead
                x = temp1->x;
                y = temp1->y;
                push(temp1->x, temp1->y, headVst); //pushed to "visited stack"
                cout<<temp1->x<<","<<temp1->y<<" "<<endl;
            }
            else //at a dead-end
            {
                cout<<"(!) at a dead-end; backtracking...\n";
                struct node* temp2;
                pop(headVst);
                temp2 = pop(headVst);
                x = temp2->x;
                y = temp2->y;
                while(x>0 && y>0) //running the loop until we find the (-1,-1) "decision token"
                {
                    cout<<temp2->x<<","<<temp2->y<<" "<<"\n";
                    temp2 = pop(headVst);
                    x = temp2->x;
                    y = temp2->y;
                }
                temp2 = pop(headVst); //backtracked to the intersection
                cout<<temp2->x<<","<<temp2->y<<"*\n(!) backtracked to the last decision point; choosing an alternate route...\n";
                prex = temp2->x;
                prey = temp2->y;
                temp2 = pop(headAlt); //popping from the "alternatives stack" for a new route
                x = temp2->x;
                y = temp2->y;
                cout<<temp2->x<<","<<temp2->y<<"\n";
            }
            if(x == EX && y ==EY)
            {
                cout<<"(!) the mouse has exited the maze."<<endl<<endl;
                break;
            }
        }
    }
}

bool isValid(int x, int y) //checks to see if given (x,y) are valid co-ordinates
{
    if(x >= 0 && y >= 0 && x < N  && y < N && !maze[x][y]) return true;
    else return false;
}

int main()
{
    headVst = init(headVst); //stack 1; i.e. the "visited stack"
    headAlt = init(headAlt); //stack 2; i.e. the "alternatives stack"
    int x, y;
    cout<<"insert start-point co-ordinates. (for example: x=1, y=1)\n"<<endl;
    cout<<"x = ";
    cin>>x;
    cout<<"y = ";
    cin>>y;
    cout<<endl<<endl;
    if(isValid(x,y)) runMaze(x,y);
    else cout<<"(!) error: invalid co-ordinates."<<endl;
    return 0;
}
