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
int look_forward(map m, position p, direction d)
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
        result = ch_look==OBSTACLE ? 1 : 2;
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

    int look_at = look_forward(m, guard_pos, guard_dir);
    while(1)
    {
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
        }
        else if(look_at==2) // free
        {
            m.data[guard_pos.y][guard_pos.x] = marker;
            path_cells++;
            guard_pos = go_forward(guard_pos, guard_dir);
            m.data[guard_pos.y][guard_pos.x] = GUARD;

            if(DEBUG) print_matrix(m.data, m.rows, m.cols);
        }
        look_at = look_forward(m, guard_pos, guard_dir);
    }

    path_cells = count_path_cells(m, marker);

    return path_cells;
}