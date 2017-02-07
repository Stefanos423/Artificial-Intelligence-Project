#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <new>
#include <map>

#define vathos 10
#define exchange_collision temp1->row == temp2->parent->row && temp1->col == temp2->parent->col && temp2->row == temp1->parent->row && temp2->col == temp1->parent->col
int gridL, gridC;

int (*dis)(int, int, int, int);

int what=0;
 
void quickSort(int* a, int first, int last, int** b);
int pivot(int* a, int first, int last, int** b);
void swap(int& a, int& b);

void quickSort( int* a, int first, int last, int** b) 
{
    int pivotElement;
    if(first < last)
    {
        pivotElement = pivot(a, first, last, b);
        quickSort(a, first, pivotElement, b);
        quickSort(a, pivotElement+1, last, b);
    }
}
 
int pivot(int* a, int first, int last, int** b) 
{
    int  p = first;
    int pivotElement = a[first];
    for(int i = first+1 ; i < last ; i++)
    {
        if(a[i] <= pivotElement)
        {
            p++;
            swap(a[i], a[p]);
	    swap(b[i][0], b[p][0]);
	    swap(b[i][1], b[p][1]);
        }
    }
    swap(a[p], a[first]);
    swap(b[p][0], b[first][0]);
    swap(b[p][1], b[first][1]);
    return p;
}
 
void swap(int& a, int& b)
{
    int temp = a;
    a = b;
    b = temp;
}

struct node
{
        struct node* parent;
        int row;
        int col;
        int g;
        int count;
        struct node* north;
        struct node* east;
        struct node* south;
        struct node* west;
};

typedef std::multimap<int, node *> map_type;

inline int dis1(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}

inline int dis2(int x1, int y1, int x2, int y2)
{
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

void insertion(map_type &open, char* openclosed, node* nextnode, int** cost)
{
    if (openclosed[gridC * nextnode->row + nextnode->col] & 2)
         return;
    openclosed[gridC * nextnode->row + nextnode->col] |= 2;
    open.insert(std::pair<int, node *>(nextnode->g + cost[nextnode->row][nextnode->col], nextnode));
}

node* astar(node root, int gridl, int gridc, int** cost, int gx, int gy)
{
        int counter; //keeps score of the current step
        node* working;
        node* new_root = new node;
        new_root->parent = root.parent;
        new_root->row = root.row;
        new_root->col = root.col;
        new_root->g = root.g;
        new_root->count = root.count;
        new_root->north = root.north;
        new_root->east = root.east;
        new_root->south = root.south;
        new_root->west = root.west;
        char *openclosed = new char[gridL * gridC];
        map_type open;
        
        for (int i = 0; i < gridL * gridC; i++)
            openclosed[i] = 0;

        //initializations
        
        counter = 0;
        
        openclosed[gridC * root.row + root.col] |= 2;
        open.insert(std::pair<int, node *>(root.g + cost[root.row][root.col], new_root));

        //beginning of the main proccess
        while (open.begin() != open.end())
        {
                working = open.begin()->second; //bring the working node forth
		printf("Considering new position (%d, %d)\n", working->col, working->row);
                
                open.erase(open.begin()); //point to the next node we are going to work with

                //with the next lines of code, we put the working node in the closed nodes list
                
                openclosed[gridC * working->row + working->col] |= 1;
                
                
                //we will now check whether the currently working node is the destination node
                if (working->row == gx && working->col == gy)
                {
		    printf("Arrived at intermediate destination.\n");
                    delete[] openclosed;
                    return working;
                }

                //with the next lines, we create to 4 children for the working node
                node* temp2 = new(node);
                counter += 1;

                if (working->row == 0)
                {
			printf("Movement out of borders. Deleting Potential move.\n");
                        working->north = NULL;
			delete temp2;
                }
                else if (cost[working->row - 1][working->col] == 100000)
                {
			printf("Detected obstacle. Deleting Potential move.\n");
                        working->north = NULL;
			delete temp2;
                }
                else
                {

                        temp2->parent = working;
                        temp2->row = working->row - 1;
                        temp2->col = working->col;
                        temp2->g = working->g + 1;
                        temp2->count = counter;
                        temp2->north = NULL;
                        temp2->east = NULL;
                        temp2->south = NULL;
                        temp2->west = NULL;
                        working->north = temp2;
                }
                temp2 = new(node);
                if (working->col == gridc)
                {
			printf("Movement out of borders. Deleting Potential move.\n");
                        working->east = NULL;
			delete temp2;
                }
                else if(cost[working->row][working->col + 1] == 100000)
                {
			printf("Detected obstacle. Deleting Potential move.\n");
                        working->east = NULL;
			delete temp2;
                }
                else
                {
                        temp2->parent = working;
                        temp2->row = working->row;
                        temp2->col = working->col + 1;
                        temp2->g = working->g+1;
                        temp2->count = counter;
                        temp2->north = NULL;
                        temp2->east = NULL;
                        temp2->south = NULL;
                        temp2->west = NULL;
                        working->east = temp2;
                }
                temp2 = new(node);
                if (working->row == gridl)
                {
			printf("Movement out of borders. Deleting Potential move.\n");
                        working->south = NULL;
			delete temp2;
                }
                else if (cost[working->row + 1][working->col] == 100000)
                {
			printf("Detected obstacle. Deleting Potential move.\n");
                        working->south = NULL;
			delete temp2;
                }
                else
                {
                        temp2->parent = working;
                        temp2->row = working->row + 1;
                        temp2->col = working->col;
                        temp2->g = working->g+1;
                        temp2->count = counter;
                        temp2->north = NULL;
                        temp2->east = NULL;
                        temp2->south = NULL;
                        temp2->west = NULL;
                        working->south = temp2;
                }
                temp2 = new(node);
                if (working->col == 0)
                {
			printf("Movement out of borders. Deleting Potential move.\n");
                        working->west = NULL;
			delete temp2;
                }
                else if (cost[working->row][working->col - 1] == 100000)
                {
			printf("Detected obstacle. Deleting Potential move.\n");
                        working->west = NULL;
			delete temp2;
                }
                else
                {
                        temp2->parent = working;
                        temp2->row = working->row;
                        temp2->col = working->col - 1;
                        temp2->g = working->g + 1;
                        temp2->count = counter;
                        temp2->north = NULL;
                        temp2->east = NULL;
                        temp2->south = NULL;
                        temp2->west = NULL;
                        working->west = temp2;
                }
                
                //having created the children we are going to check if any of the children has already been traversed
                //this procedure will take place based on the nodes currently disposed in the closed nodes list
                if (working->north != NULL)
                {
                    if (openclosed[gridC * working->north->row + working->north->col] & 1)
                        working->north = NULL;
                }
                
                if (working->east != NULL)
                {
                    if (openclosed[gridC * working->east->row + working->east->col] & 1)
                        working->east = NULL;
                }

                if (working->south != NULL)
                {
                    if (openclosed[gridC * working->south->row + working->south->col] & 1)
                         working->south = NULL; 
                }
                
                if (working->west != NULL)
                {
                    if (openclosed[gridC * working->west->row + working->west->col] & 1)
                        working->west = NULL;
                }
               
                if (working->north != NULL)
                    insertion(open, openclosed, working->north, cost);
                if (working->east != NULL)
                    insertion(open, openclosed, working->east, cost);
                if (working->south != NULL)
                    insertion(open, openclosed, working->south, cost);
                if (working->west != NULL)
                    insertion(open, openclosed, working->west, cost);    
        }
        delete[] openclosed;
        return NULL;
}


int shortest(int r1x, int r1y, int gridl, int gridc, int interg, int line, int** med, char** grid)
{
        if (interg == 0) return -1;
        if (line >= interg) return -2;

        int* path = new int[interg];
        for(int i = line; i < interg; i++) {path[i] = 0;}
        int i, j, k, gx, gy, w;

	if(interg <= vathos)
	{
		w = interg;
	}
	else
	{
		int* mandis = new int[interg];
		for(j = line; j < interg; j++)
		{
                    mandis[j] = dis(r1x, r1y, med[j][0], med[j][1]);
		}

		quickSort(mandis, line, interg-1, med);
		delete[] mandis;

		if(interg - vathos < line) w = interg;
		else w = line + vathos;
	}	

        for(i = line; i < w; i++)
        {            
                gx = med[i][0];
                gy = med[i][1];
                
                //creates cost  
                int** cost = new int*[gridl+1];
                for(j = 0; j < gridl+1; j++)
                {
                        cost[j] = new int[gridc+1];
                }


                for(j = 0; j < gridl+1; j++)
                {       for(k = 0; k < gridc+1; k++)
                                if (grid[j][k] == 'X') cost[j][k] = 100000;
                                else cost[j][k] = dis(j, k, gx, gy);
                                
                }


                node root;
                root.row = r1x;
                root.col = r1y;
                root.g = 0;
                root.count = 0;
                root.parent = NULL;

                node* toshow = astar(root, gridl, gridc, cost, gx, gy);
                
                if (toshow == NULL) return -3;
                else
                {
                        while(toshow->parent != NULL)
                        {
                                path[i]++;
                                toshow = toshow->parent;
                        }
                }
        }
	short int minline = line;
        int minpath = path[line];
        for(i = line; i < w; i++)
        {
                if(path[i] < minpath)
                {
                        minpath = path[i];
                        minline = i;
                }
        }
	printf("Νext intermediate destination is (%d, %d).\n", med[minline][0], med[minline][1]);
        return minline;
}

node* createplan(node* plan1, int r1x, int r1y, int gridl, int gridc, int interg, int** med, char** grid, int gx, int gy)
{
        int currentstartx = r1x, currentstarty = r1y;
        int** cost;
        int i, j, k, c;
        node root;
        node *toshow;
        
        for (i = 0; i < interg-1; i++)
        {
                c = shortest(currentstartx, currentstarty, gridl-1, gridc-1, interg, i, med, grid);
                if(c==-3) return NULL;

                cost = new int*[gridl];
                for(j = 0; j < gridl; j++)
                {
                        cost[j] = new int[gridc];
                }

                for(j = 0; j < gridl; j++)
                        for(k = 0; k < gridc; k++)
                                if (grid[j][k] == 'X') cost[j][k] = 100000;
                                else cost[j][k] = dis(j, k, med[c][0], med[c][1]);


                root.row = currentstartx;
                root.col = currentstarty;
                root.g = 0;
                root.count = 0;
                root.parent = NULL;

                node* toshow = astar(root, gridl-1, gridc-1, cost, med[c][0], med[c][1]);               

                if (plan1 == NULL) plan1 = toshow;
                else
                {
                        node* temp = toshow;
                        while(temp->parent->parent != NULL) temp = temp->parent;
                        temp->parent = plan1;
                        plan1 = toshow;
                }
                
                int tempx = med[i][0];
                int tempy = med[i][1];
                med[i][0] = med[c][0];
                med[i][1] = med[c][1];
                med[c][0] = tempx;
                med[c][1] = tempy;
                
                currentstartx = plan1->row;
                currentstarty = plan1->col;
        }

        //at this point we are going to calculate the prelast path towards our goal for the robot
        if(interg >= 1){        //very bad implementation, needs to be looked at
        cost = new int*[gridl];
        for(i = 0; i < gridl; i++)
        {
                cost[i] = new int[gridc];
        }

        for(i = 0; i < gridl; i++)
                for(j = 0; j < gridc; j++)
                      if (grid[i][j] == 'X') cost[i][j] = 100000;
                      else cost[i][j] = dis(i, j, med[interg-1][0], med[interg-1][1]);


        root.row = currentstartx;
        root.col = currentstarty;
        root.g = 0;
        root.count = 0;
        root.parent = NULL;

        toshow = astar(root, gridl-1, gridc-1, cost, med[interg-1][0], med[interg-1][1]);
        if(toshow==NULL) return NULL;
                
        if (plan1 == NULL) plan1 = toshow;
        else
        {       
                node* temp = toshow;
                while(temp->parent->parent != NULL) temp = temp->parent;
                temp->parent = plan1;
                plan1 = toshow;
        }
        
        currentstartx = plan1->row;
        currentstarty = plan1->col;
        }       //end of very bad implementation, needs to be looked at

        //at this point we are going to calculate the last path towards our goal for the robot
        cost = new int*[gridl];
        for(i = 0; i < gridl; i++)
        {
                cost[i] = new int[gridc];
        }

        for(i = 0; i < gridl; i++)
                  for(j = 0; j < gridc; j++)
                        if (grid[i][j] == 'X') cost[i][j] = 100000;
                        else cost[i][j] = dis(i, j, gx, gy);


        root.row = currentstartx;
        root.col = currentstarty;
        root.g = 0;
        root.count = 0;
        root.parent = NULL;
        
        toshow = astar(root, gridl-1, gridc-1, cost, gx, gy);
        if(toshow==NULL) return NULL;
                
        if (plan1 == NULL) plan1 = toshow;
        else
        {
                node* temp = toshow;
                while(temp->parent->parent != NULL) temp = temp->parent;
                temp->parent = plan1;
                plan1 = toshow;
        }
        return plan1;
}

node* reverse(node* plan1)
{
        //create head for reversed list
        node* head = new(node);
        head->parent = NULL;
        //head is a duplicate of plan1
        head->row = plan1->row;
        head->col = plan1->col;
        head->g = plan1->g;
        head->count = plan1->count;
        head->north =plan1->north; 
        head->east = plan1->east;
        head->south = plan1->south;
        head->west = plan1->west;

        node* temp5 = plan1;
        while (temp5 != NULL)
        {
                node* temp6 = new(node);
                temp6->parent = NULL;
                
                //temp6 is a duplicate of temp5
                temp6->row = temp5->row;
                temp6->col = temp5->col;
                temp6->g = temp5->g;
                temp6->count = temp5->count;
                temp6->north =temp5->north; 
                temp6->east = temp5->east;
                temp6->south = temp5->south;
                temp6->west = temp5->west;
                
                temp6->parent = head;
                head = temp6;
                temp5 = temp5->parent;                           
        }

        plan1 = head->parent;
        return plan1;
}

node* coldet (node* plan1, node* plan2, int r2x, int r2y, int r1x, int r1y, char** grid)
{
        node* temp1 = plan1;
        node* temp2 = plan2;

        if(plan1->row == plan2->row && plan1->col == plan2->col)
        {
	printf("Detected claiming colission and resolving.\n");
        node* temp = new(node);
        temp->row = r2x;
        temp->col = r2y;
        temp->parent = plan2;
        plan2 = temp;
        temp2 = plan2;
        }

        if(r1x == temp2->row && r1y == temp2->col && r2x == temp1->row && r2y == temp1->col)
        {
		printf("Detected exchange colission and resolving.\n");
                if(temp1->row == r1x+1 || temp1->row == r1x-1)
                {
                        if(r2y + 1 < gridC && grid[r2x][r2y+1] != 'X')
                        {
                                node* temp3 = new(node);
                                temp3->row = r2x;
                                temp3->col = r2y+1;
                                node* temp4 = new(node);
                                temp4->row = r2x;
                                temp4->col = r2y;
                                temp3->parent = temp4;
                                temp4->parent = plan2;
                                plan2 = temp3;
                        }
                        else if(r2y > 0 && grid[r2x][r2y-1] != 'X')
                        {
                                node* temp3 = new(node);
                                temp3->row = r2x;
                                temp3->col = r2y-1;
                                node* temp4 = new(node);
                                temp4->row = r2x;
                                temp4->col = r2y;
                                temp3->parent = temp4;
                                temp4->parent = plan2;
                                plan2 = temp3;
                        }
                        else
                        {
                                if(r1x == r2x-1)
                                {
                                        node* temp3 = new(node);
                                        temp3->row = r2x+1;
                                        temp3->col = r2y;
                                        node* temp4 = new(node);
                                        temp4->row = r2x;
                                        temp4->col = r2y;
                                        temp3->parent = temp4;
                                        temp4->parent = plan2;
                                        plan2 = temp3;
                                        temp2 = plan2;
                                }
                                else
                                {
                                        node* temp3 = new(node);
                                        temp3->row = r2x-1;
                                        temp3->col = r2y;
                                        node* temp4 = new(node);
                                        temp4->row = r2x;
                                        temp4->col = r2y;
                                        temp3->parent = temp4;
                                        temp4->parent = plan2;
                                        plan2 = temp3;
                                        temp2 = plan2;
                                }
                        }
                }
                else
                {
                        if(r2x > 0 && grid[r2x-1][r2y] != 'X')
                        {
                                node* temp3 = new(node);
                                temp3->row = r2x-1;
                                temp3->col = r2y;
                                node* temp4 = new(node);
                                temp4->row = r2x;
                                temp4->col = r2y;
                                temp3->parent = temp4;
                                temp4->parent = plan2;
                                plan2 = temp3;
                                temp2 = temp3;
                        }
                        else if(r2x + 1 < gridL && grid[r2x+1][r2y] != 'X')
                        {
                                node* temp3 = new(node);
                                temp3->row = r2x+1;
                                temp3->col = r2y;
                                node* temp4 = new(node);
                                temp4->row = r2x;
                                temp4->col = r2y;
                                temp3->parent = temp4;
                                temp4->parent = plan2;
                                plan2 = temp3;
                                temp2 = plan2;
                        }
                        else
                        {
                                if(r1y == r2y-1)
                                {
                                        node* temp3 = new(node);
                                        temp3->row = r2x;
                                        temp3->col = r2y+1;
                                        node* temp4 = new(node);
                                        temp4->row = r2x;
                                        temp4->col = r2y;
                                        temp3->parent = temp4;
                                        temp4->parent = plan2;
                                        plan2 = temp3;
                                        temp2 = plan2;
                                }
                                else
                                {
                                        node* temp3 = new(node);
                                        temp3->row = r2x;
                                        temp3->col = r2y-1;
                                        node* temp4 = new(node);
                                        temp4->row = r2x;
                                        temp4->col = r2y;
                                        temp3->parent = temp4;
                                        temp4->parent = plan2;
                                        plan2 = temp3;
                                        temp2 = plan2;
                                }
                        }
                }
        }               

        while(temp1->parent != NULL && temp2->parent != NULL)
        {
                if(temp1->parent->row == temp2->parent->row && temp1->parent->col == temp2->parent->col)
                {
			printf("Detected claiming colission and resolving.\n");
                        node* temp = new(node);
                        temp->row = temp2->row;
                        temp->col = temp2->col;
                        temp->parent = temp2->parent;
                        temp2->parent = temp;
                }
                else if(exchange_collision)
                {
                        printf("Detected exchange collision and resolving.\n");
                        //printf("%d %d\n", temp1->parent->row, temp1->parent->col);
                        if(temp1->parent->row == temp1->row+1 || temp1->parent->row == temp1->row-1)
                        {
                                if(temp2->col + 1 < gridC && grid[temp2->row][temp2->col+1] != 'X')
                                {
                                        node* temp3 = new(node);
                                        temp3->row = temp2->row;
                                        temp3->col = temp2->col+1;
                                        node* temp4 = new(node);
                                        temp4->row = temp2->row;
                                        temp4->col = temp2->col;
                                        temp3->parent = temp4;
                                        temp4->parent = temp2->parent;
                                        temp2->parent = temp3;
                                }
                                else if(temp2->col > 0 && grid[temp2->row][temp2->col-1] != 'X')
                                {
                                        node* temp3 = new(node);
                                        temp3->row = temp2->row;
                                        temp3->col = temp2->col-1;
                                        node* temp4 = new(node);
                                        temp4->row = temp2->row;
                                        temp4->col = temp2->col;
                                        temp3->parent = temp4;
                                        temp4->parent = temp2->parent;
                                        temp2->parent = temp3;
                                }
                                else
                                {
                                        if(temp1->row == temp2->row-1)
                                        {
                                                node* temp3 = new(node);
                                                temp3->row = temp2->row+1;
                                                temp3->col = temp2->col;
                                                node* temp4 = new(node);
                                                temp4->row = temp2->row;
                                                temp4->col = temp2->col;
                                                temp3->parent = temp4;
                                                temp4->parent = temp2->parent;
                                                temp2->parent = temp3;
                                         }
                                         else
                                         {
                                                node* temp3 = new(node);
                                                temp3->row = temp2->row-1;
                                                temp3->col = temp2->col;
                                                node* temp4 = new(node);
                                                temp4->row = temp2->row;
                                                temp4->col = temp2->col;
                                                temp3->parent = temp4;
                                                temp4->parent = temp2->parent;
                                                temp2->parent = temp3;
                                        }
                                }
                        }
                        else
                        {
                                //printf("spam\n");
                                if(temp2->row > 0 && grid[temp2->row-1][temp2->col] != 'X' && temp1->row != temp2->row-1)
                                {
                                        node* temp3 = new(node);
                                        temp3->row = temp2->row-1;
                                        temp3->col = temp2->col;
                                        node* temp4 = new(node);
                                        temp4->row = temp2->row;
                                        temp4->col = temp2->col;
                                        temp3->parent = temp4;
                                        temp4->parent = temp2->parent;
                                        temp2->parent = temp3;
                                }
                                else if(temp2->row + 1 < gridL && grid[temp2->row+1][temp2->col] != 'X' && temp1->row != temp2->row+1)
                                {
                                        node* temp3 = new(node);
                                        temp3->row = temp2->row+1;
                                        temp3->col = temp2->col;
                                        node* temp4 = new(node);
                                        temp4->row = temp2->row;
                                        temp4->col = temp2->col;
                                        temp3->parent = temp4;
                                        temp4->parent = temp2->parent;
                                        temp2->parent = temp3;
                                }
                                else
                                {
                                        if(temp1->col == temp2->col-1)
                                        {
                                                node* temp3 = new(node);
                                                temp3->row = temp2->row;
                                                temp3->col = temp2->col+1;
                                                node* temp4 = new(node);
                                                temp4->row = temp2->row;
                                                temp4->col = temp2->col;
                                                temp3->parent = temp4;
                                                temp4->parent = temp2->parent;
                                                temp2->parent = temp3;
                                        }
                                        else
                                        {
                                                node* temp3 = new(node);
                                                temp3->row = temp2->row;
                                                temp3->col = temp2->col-1;
                                                node* temp4 = new(node);
                                                temp4->row = temp2->row;
                                                temp4->col = temp2->col;
                                                temp3->parent = temp4;
                                                temp4->parent = temp2->parent;
                                                temp2->parent = temp3;  
                                        }
                                }
                        }
                }
                temp1 = temp1->parent;
                temp2 = temp2->parent;
        }

        return plan2;
}

int main(int argc, char **argv)
{
        FILE *fp;
        char c;
        if (argc < 3 || atoi(argv[2]) != 2)
            dis = &dis1;
        else
            dis = &dis2;
        
        if (argc < 2)
            fp = fopen("input.txt", "r");
        else
            fp = fopen(argv[1], "r");
        
        if (fp==NULL) return 2;
        int gridl, gridc, i, j, r1x, r1y, r2x, r2y, gx, gy, interg;
        node *plan1, *plan2;
        fscanf(fp, "%d", &gridc); //scan grid size
        fscanf(fp, "%d", &gridl);
        gridC = gridc;
        gridL = gridl;
        fscanf(fp, "%d", &r1y);
        r1y--;
        fscanf(fp, "%d", &r1x); //scan initial coordinates
        r1x--;
        fscanf(fp, "%d", &r2y);
        r2y--;
        fscanf(fp, "%d", &r2x);
        r2x--;
        fscanf(fp, "%d", &gy);
        gy--;
        fscanf(fp, "%d", &gx);
        gx--;
        fscanf(fp, "%d", &interg); //scan number of intermediate spots
        int** med = new int*[interg];
        for(i = 0; i < interg; i++)
        {
                med[i] = new int[2]; //
        }
        for(i = 0; i < interg; i++) //scan coord. of inter spots.
        {
                fscanf(fp, "%d", &med[i][1]);
                med[i][1]--;
                fscanf(fp, "%d", &med[i][0]);
                med[i][0]--;
        }
        char** grid = new char*[gridl];
        for(i = 0; i < gridl; i++)
        {
            grid[i] = new char[gridc];
        }
        fscanf(fp, "%c", &c);
        for(i = 0; i < gridl; i++)
        {
           for(j = 0; j < gridc; j++)
                fscanf(fp, "%c", &grid[i][j]);
            fscanf(fp, "%c", &c); //gets rid of newline
        }
        fclose(fp);
        
        //at this point we are going to calculate one of the shortest interim paths for robot1
        plan1 = NULL;
        plan1 = createplan(plan1, r1x, r1y, gridl, gridc, interg, med, grid, gx, gy);
        if(plan1==NULL) {printf("Failure to calculate. Please rework the given map layout and try again.\n"); return 1;}

        //having calculated a satisfying path for robot 1, we are going to do the same for robot 2
        plan2 = NULL;
        plan2 = createplan(plan2, r2x, r2y, gridl, gridc, interg, med, grid, gx, gy);
        if(plan2==NULL) {printf("Failure to calculate. Please rework the given map layout and try again.\n"); return 1;}

        int count1 = 0;
        node* temp1 = plan1;
        while(temp1 != NULL){count1++; temp1=temp1->parent;}
        int count2 = 0;
        node* temp2 = plan2;
        while(temp2 != NULL){count2++; temp2=temp2->parent;}
        if(count1 > count2){plan1 = plan1->parent;}
        else{plan2 = plan2->parent;}
        
        int g1x = plan1->row;
        int g1y = plan1->col;
        int g2x = plan2->row;
        int g2y = plan2->col;

        //reversing the lists
        plan1 = reverse(plan1);
        plan2 = reverse(plan2);

        //checking for collisions
        plan2 = coldet(plan1, plan2, r2x, r2y, r1x, r2x, grid);

        //add stalls
        temp1 = plan1;
        temp2 = plan2;
        while(temp1->parent != NULL || temp2->parent != NULL)
        {
                if(temp1->parent == NULL && temp2->parent != NULL)
                {
                        node* temp = new(node);
                        temp->row = temp1->row;
                        temp->col = temp1->col;
                        temp->parent = NULL;
                        temp1->parent = temp;
                }
                if(temp2->parent == NULL && temp1->parent != NULL)
                {
                        node* temp = new(node);
                        temp->row = temp2->row;
                        temp->col = temp2->col;
                        temp->parent = NULL;
                        temp2->parent = temp;
                }
                temp1 = temp1->parent;
                temp2 = temp2->parent;
        }

        //calculate the return1
        temp1 = NULL;
        temp1 = createplan(temp1, g1x, g1y, gridl, gridc, 0, med, grid, r1x, r1y);
        grid[temp1->row][temp1->col] = 'X';
	temp1 = reverse(temp1);

        temp2 = plan1;
        while(temp2->parent != NULL)
        {
                temp2 = temp2->parent;
        }
        temp2->parent = temp1;

        //calculate the return2
        node* temp3 = NULL;
        temp3 = createplan(temp3, g2x, g2y, gridl, gridc, 0, med, grid, r2x, r2y);
	temp3 = reverse(temp3);

	temp3 = coldet(temp1, temp3, g2x, g2y, g1x, g1y, grid);

        node* temp4 = plan2;
        while(temp4->parent != NULL)
        {
                temp4 = temp4->parent;
        }
        temp4->parent = temp3;

        //printing the results
        fp = fopen("robot1.txt", "w");  
        if (plan1 == NULL)
        {
            printf("Failure to calculate the first robot's path. Please rework the given map layout before trying again.\n");
            return 1;
        }
        else
        {
                printf("Robot 1 steps: \n");
                while(plan1->parent != NULL)
                {
			printf("(%d, %d)\n", plan1->col, plan1->row);
                        fprintf(fp, "%d %d\n", plan1->row, plan1->col);
                        plan1 = plan1->parent;
                }
        }
        fclose(fp);

        fp = fopen("robot2.txt", "w");  
        if (plan2 == NULL)
        {
            printf("Failure to calculate the second robot's path. Please rework the given map layout before trying again.\n");
            return 1;
        }
        else
        {
                printf("Robot 2 steps: \n");
                while(plan2->parent != NULL)
                {
			printf("(%d, %d)\n", plan2->col, plan2->row);
                        fprintf(fp, "%d %d\n", plan2->row, plan2->col);
                        plan2 = plan2->parent;
                }
        }       
        fclose(fp);

        return 0;
}

