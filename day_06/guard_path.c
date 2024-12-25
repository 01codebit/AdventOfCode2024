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

// -----------------------
// Returns:
// -1: UNKNOWN
//  0: exit
//  1: obstacle
//  2: free
//  3: marker +
//  4: marker -
//  5: marker |
// -----------------------
int look_forward(map m, position p, direction d)
{
    position look_at = go_forward(p, d);
    int result;
    if(look_at.x<0 || look_at.x>=m.cols || look_at.y<0 || look_at.y>=m.rows)
    {
        result = 0;
    }
    else
    {
        char ch_look = m.data[look_at.y][look_at.x];
        result = ch_look==OBSTACLE ? 1 
            : ch_look==FREE ? 2
            : ch_look==MARKER_X ? 3 
            : ch_look==MARKER_H ? 4 
            : ch_look==MARKER_V ? 5 
            : -1;
    }
    return result;
}

int look_right(map m, position p, direction d)
{
    direction right = turn_right(d);
    return look_forward(m, p, right);
}

int count_path_cells(map m)
{
    int result = 0;
    for(int r=0; r<m.rows; r++)
    {
        for(int c=0; c<m.cols; c++)
        {
            char x = m.data[r][c];
            if(x==MARKER_X || x==MARKER_V || x==MARKER_H)
            {
                result++;
            }
        }
    }
    return result;
}

int check_turn(position p, direction d, position t)
{
    int result = 0;
    if(d.dx==0 && d.dy==-1)
    {
        if(p.y == t.y && p.x < t.x) result = 1;
    }
    else if(d.dx==1 && d.dy==0)
    {
        if(p.x == t.x && p.y < t.y) result = 1;
    }
    else if(d.dx==0 && d.dy==1)
    {
        if(p.y == t.y && p.x > t.x) result = 1;
    }
    else if(d.dx==-1 && d.dy==0)
    {
        if(p.x == t.x && p.y > t.y) result = 1;
    }

    return result;
}

int check_for_loop(map m, position gp, direction gd, position* turns, int turns_count, direction* turn_directions, int debug)
{
    int result = 0;

    if(turns_count<=2) return result;

    direction right = turn_right(gd);

    for(int i=turns_count-3; i>=0; i--)
    {
        position current_turn = turns[i];
        direction td = turn_directions[i];
        int align = check_turn(gp, gd, current_turn);
        if(align)
        {
            int look_at_forward = look_forward(m, gp, gd);
            if(look_at_forward>=2 && td.dx==right.dx && td.dy==right.dy)
            {
                position obs = go_forward(gp, gd);
                result++;
                if(debug)
                {
                    printf("[turns_count: %d] align with: i: %d (%d, %d)\n", turns_count, i, current_turn.x, current_turn.y);
                    print_matrix_marker(m.data, m.rows, m.cols, 'O', obs.x, obs.y);
                }
            }
        }
    }
    return result;
}

int check_for_loop_2(map m, position gp, direction gd)
{
    int result = 0;

    int look_at_forward = look_forward(m, gp, gd);
    int look_at_right = look_right(m, gp, gd);

    if(look_at_forward>=2)
    {
        if(look_at_right==3 || (gd.dx==0 && look_at_right==4) || (gd.dy==0 && look_at_right==5))
        {
            result = 1;
        }
    }

    if(result)
    {
        position obs = go_forward(gp, gd);
        if(DEBUG) print_matrix_marker(m.data, m.rows, m.cols, 'O', obs.x, obs.y);
    }
    return result;
}

void mark_current_position(char** data, position gp, direction gd, int* turn_flag, int prev_look_at)
{
    // prev_look_at:
    //  3: marker +
    //  4: marker -
    //  5: marker |

    int marker;

    if(*turn_flag)
    {
        marker = MARKER_X;
        *turn_flag = 0;
    }
    else
    {
        marker = gd.dx==0 ? MARKER_V : MARKER_H;

        if(prev_look_at==3 || prev_look_at==4 && marker==MARKER_V || prev_look_at==5 && marker==MARKER_H) 
        {
            marker = MARKER_X;
        }
    }

    data[gp.y][gp.x] = marker;
}



result_struct evaluate(map m, int debug)
{
    printf("guard starting position: (%d, %d)\n", m.guard_start_x, m.guard_start_y);

    int path_cells = 0;

    position guard_pos;
    guard_pos.x = m.guard_start_x;
    guard_pos.y = m.guard_start_y;

    direction guard_dir;
    guard_dir.dx = 0;
    guard_dir.dy = -1;

    int turns = 0;

    position turns_positions[MAX_TURNS];
    direction turn_directions[MAX_TURNS];
    int loop_options = 0;
    int turn_flag = 0;

    int look_at = look_forward(m, guard_pos, guard_dir);
    int prev_look_at = 0;
    int iterations = 0;
    while(1)
    {
        // if(DEBUG) 
        // printf("[iteration #%d] guard pos (%d, %d) guard dir (%d, %d) look_at: %d, turns: %d, loop_options: %d\n", iterations, guard_pos.x, guard_pos.y, guard_dir.dx, guard_dir.dy, look_at, turns, loop_options);
        if(turns>2)
        {
            loop_options += check_for_loop(m, guard_pos, guard_dir, turns_positions, turns, turn_directions, debug);
            // if(turn_flag==0) loop_options += check_for_loop_2(m, guard_pos, guard_dir);
            // if(look_at>2) // MARKER
            // {
            //     if(look_forward(m, go_forward(guard_pos, guard_dir), guard_dir)>=2)
            //         loop_options += 1;
            // }
        }

        if(look_at==0) // exit
        {
            mark_current_position(m.data, guard_pos, guard_dir, &turn_flag, prev_look_at);

            path_cells++;
            if(DEBUG) print_matrix(m.data, m.rows, m.cols);
            break;
        }
        else if(look_at==1) // obstacle
        {
            if(DEBUG) printf("  guard_pos (%d, %d)\n", guard_pos.x, guard_pos.y);

            turn_directions[turns] = guard_dir;
            guard_dir = turn_right(guard_dir);
            turn_flag = 1;
            if(turns<MAX_TURNS)
            {
                turns_positions[turns] = guard_pos;
                if(DEBUG) printf("  turn_positions[%d] (%d, %d)\n", turns, turns_positions[turns].x, turns_positions[turns].y);
            }
            else {
                for(int i=1; i<MAX_TURNS; i++)
                {
                    turns_positions[i-1] = turns_positions[i];
                }
                turns_positions[MAX_TURNS-1] = guard_pos;
            }
            turns++;
        }
        else if(look_at>=2) // free or marker
        {
            mark_current_position(m.data, guard_pos, guard_dir, &turn_flag, prev_look_at);

            path_cells++;
            guard_pos = go_forward(guard_pos, guard_dir);
            m.data[guard_pos.y][guard_pos.x] = GUARD;

            if(DEBUG) print_matrix(m.data, m.rows, m.cols);
        }
        
        prev_look_at = look_at;
        look_at = look_forward(m, guard_pos, guard_dir);
        iterations++;
    }

    path_cells = count_path_cells(m);

    if(debug) 
    {
        printf("turns: %d\n", turns);
        printf("path cells: %d\n", path_cells);
        print_matrix(m.data, m.rows, m.cols);
        printf("loop options: %d\n", loop_options);
    }

    result_struct res;
    res.path_cells = path_cells;
    res.loop_options = loop_options;

    return res;
}