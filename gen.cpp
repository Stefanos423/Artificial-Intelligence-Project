#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <memory>
#include <cstring>

void generate_map(char const *filename, int map_width, int map_height, int checkpoint_count, double wall_ratio)
{
    char *map = new char[map_height * map_width];
    FILE *f = fopen(filename, "w");
    
    for (int i = 0; i < map_height * map_width; i++)
        map[i] = ((double)rand() / RAND_MAX <= wall_ratio ? 'X' : 'O');
    
    fprintf(f, "%d %d\n", map_width, map_height);
    
    for (int i = 0; i < 3 + checkpoint_count; i++)
    {
        int x, y;
        if (i == 3)
            fprintf(f, "%d\n", checkpoint_count);
        do
        {
            x = rand() % map_width + 1;
            y = rand() % map_height + 1;
        } while (map[(x - 1) + (y - 1) * map_width] != 'O');
        map[(x - 1) + (y - 1) * map_width] = 'I';
        fprintf(f, "%d %d\n", x, y);
    }
    
    for (int i = 0; i < map_height; i++)
    {
        for (int j = 0; j < map_width; j++)
        {
            if (map[j + i * map_width] == 'X')
                fprintf(f, "%c", 'X');
            else
                fprintf(f, "%c", 'O');
        }
        fprintf(f,"\n");
    }
    fclose(f);
}

int exec(const char* cmd) {
    FILE *in;
    char buff[512];

    if(!(in = popen(cmd, "r"))){
            return 1;
    }

    while(fgets(buff, sizeof(buff), in)!=NULL){
            std::cout << buff;
    }
    pclose(in);

    return 0;
}

int main(int argc, char **argv)
{
    char c, full_command[40];
    char const *filename;
    int map_width, map_height, checkpoint_count, ret, silent;
    double wall_ratio;
    
    if (argc == 2 && strcmp(argv[1], "-h") == 0)
    {
        printf("Usage: %s [filename] [map_width] [map_height] [checkpoint_count] [wall_ratio] [silent]\n", argv[0]);
        printf("\tfilename [\"map.txt\"]: location of output file\n");
        printf("\tmap_width [20]: width of generated map\n");
        printf("\tmap_height [20]: height of generated map\n");
        printf("\tcheckpoint_count [3]: number of checkpoints to generate\n");
        printf("\twall_ratio [0.2]: percentage of walls in total map spaces\n");
        printf("\tsilent [0]: if 1 then it wont ask any questions\n");
        return 1;
    }
    
    if (argc < 2)
        filename = "map.txt";
    else
        filename = argv[1];
    
    if (argc < 3)
        map_width = 20;
    else
        map_width = atoi(argv[2]);
    
    if (argc < 4)
        map_height = 20;
    else
        map_height = atoi(argv[3]);
    
    if (argc < 5)
        checkpoint_count = 3;
    else
        checkpoint_count = atoi(argv[4]);
    
    if (argc < 6)
        wall_ratio = 0.2;
    else
        wall_ratio = atof(argv[5]);
    
    if (argc < 7)
        silent = 0;
    else
        silent = atoi(argv[6]);

    srand(time(NULL));
    
    generate_map(filename, map_width, map_height, checkpoint_count, wall_ratio);
    
    if (silent)
        return 0;
    
    printf("Map Ready. Preview Now? (Y / n): ");
    
    ret = scanf("%c", &c);
    printf("\n");
    if (c == 'n')
        return 0;
    
    sprintf(full_command, "./ui %s _ _", filename);
    exec(full_command);
    return 0;
}