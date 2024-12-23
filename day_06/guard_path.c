#include "guard_path.h"

typedef struct {
    int dx;
    int dy;
} direction;

typedef struct {
    int x;
    int y;
} position;

position go_forward(position start, direction d)
{
    position result;
    result.x = start.x + d.dx;
    result.y = start.y + d.dy;

    return result;
}

direction turn_right(direction d)
{
    direction result;
    result.dx = 0;
    result.dy = 0;

    if(d.dx==0)
    {
        result.dy = 0;
        result.dx = -d.dy;
    }
    else if(d.dy==0)
    {
        result.dx = 0;
        result.dy = d.dx;
    }

    return result; 
}

// 0: exit - 1: obstacle - 2: free
int look_forward(map m, position p, direction d, char marker)
{
    position look_at = go_forward(p, d);
    int result;
    if(look_at.x<0 || look_at.x>=m.rows || look_at.y<0 || look_at.y>=m.cols)
    {
        result = 0;
    }
    else
    {
        char ch_look = m.data[look_at.y][look_at.x];
        result = ch_look==OBSTACLE ? 1 : ch_look==marker ? 3 : 2;
    }
    return result;
}

int count_path_cells(map m, char ch)
{
    int result = 0;
    for(int r=0; r<m.rows; r++)
    {
        for(int c=0; c<m.cols; c++)
        {
            if(m.data[r][c]==ch)
            {
                result++;
            }
        }
    }
    return result;
}

int check_for_loop(map m, position gp, direction gd, position turn1, char marker)
{
    int result = 0;

    if(gp.x == turn1.x || gp.y == turn1.y)
    {
        printf("x: %d %d - y: %d %d\n", gp.x, turn1.x, gp.y, turn1.y);
        int look_at_forward = look_forward(m, gp, gd, marker);
        if(look_at_forward==2 || look_at_forward==3)
        {
            position obs = go_forward(gp, gd);
            printf("obstacle for loop: %d %d\n", obs.x, obs.y);
            result = 1;
            print_matrix_marker(m.data, m.rows, m.cols, 'O', obs.x, obs.y);
        }
    }

    return result;
}


int evaluate(map m)
{
    printf("guard starting position: %d, %d\n", m.guard_start_x, m.guard_start_y);

    int path_cells = 0;

    position guard_pos;
    guard_pos.x = m.guard_start_x;
    guard_pos.y = m.guard_start_y;

    direction guard_dir;
    guard_dir.dx = 0;
    guard_dir.dy = -1;

    char marker = 'X';

    int turns = 0;
    position turns_positions[3];
    int loop_options = 0;

    int look_at = look_forward(m, guard_pos, guard_dir, marker);
    while(1)
    {
        if(turns>2)
        {
            loop_options += check_for_loop(m, guard_pos, guard_dir, turns_positions[0], marker);
        }

        if(look_at==0) // exit
        {
            m.data[guard_pos.y][guard_pos.x] = 'X';
            path_cells++;
            if(DEBUG) print_matrix(m.data, m.rows, m.cols);
            break;
        }
        else if(look_at==1) // obstacle
        {
            guard_dir = turn_right(guard_dir);
            if(turns<2)
            {
                turns_positions[turns] = guard_pos;
                printf("  turn_positions[%d] (%d, %d)\n", turns, guard_pos.x, guard_pos.x);
            }
            else {
                turns_positions[0] = turns_positions[1];
                turns_positions[1] = turns_positions[2];
                turns_positions[2] = guard_pos;

                printf("  turns_positions: ");
                for(int i=0; i<3; i++)
                {
                    printf("[%d](%d, %d) ", i, turns_positions[i].x, turns_positions[i].y);
                }
                printf("\n");
            }
            turns++;
        }
        else if(look_at>=2) // free
        {
            m.data[guard_pos.y][guard_pos.x] = marker;
            path_cells++;
            guard_pos = go_forward(guard_pos, guard_dir);
            m.data[guard_pos.y][guard_pos.x] = GUARD;

            if(DEBUG) print_matrix(m.data, m.rows, m.cols);
        }

        look_at = look_forward(m, guard_pos, guard_dir, marker);
    }

    path_cells = count_path_cells(m, marker);

    printf("loop options: %d\n", loop_options);

    return path_cells;
}