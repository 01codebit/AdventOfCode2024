#include "guard_path.h"

typedef struct {
    int dx;
    int dy;
} direction;

typedef struct {
    int x;
    int y;
} position;

typedef struct {
    position pos;
    direction dir;
} path_cell;

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

int check_for_loop_3(map m, position gp, direction gd, int debug)
{
    int result = 0;

    //  2: free
    //  3: marker +
    //  4: marker -
    //  5: marker |
    int look_at_forward_1 = look_forward(m, gp, gd);
    position next = go_forward(gp, gd);
    int look_at_forward_2 = look_forward(m, next, gd);

    if(look_at_forward_2>1)
    {
        if((look_at_forward_1==4 && gd.dx==0) || (look_at_forward_1==5 && gd.dy==0))
        {
            result = 1;
        }
    }

    if(result && debug)
    {
        position obs = go_forward(next, gd);
        print_matrix_marker(m.data, m.rows, m.cols, 'O', obs.x, obs.y);
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

int ortho_dir(direction d1, direction d2)
{
    return ((d1.dx * d2.dx) + (d1.dy * d2.dy)) == 0;
}

int find_loops(path_cell* path, int path_cells_count)
{
    int result = 0;

    for(int i=0; i<path_cells_count; i++)
    {
        path_cell current_cell = path[i];
        for(int j=0; j<i; j++)
        {
            path_cell cell = path[j];

            if(current_cell.pos.x == cell.pos.x && current_cell.pos.y == cell.pos.y)
            {
                if(ortho_dir(current_cell.dir, cell.dir))
                {
                    printf("loop: pos(%d, %d), dir(%d, %d)\n", cell.pos.x, cell.pos.y, cell.dir.dx, cell.dir.dy);
                    result++;
                }

                // TODO controllare che la cella alla nostra destra sia stata già esplorata

                if(current_cell.dir.dx == cell.dir.dx && current_cell.dir.dy == cell.dir.dy)
                {
                    printf("DEJA VU: pos(%d, %d), dir(%d, %d)\n", cell.pos.x, cell.pos.y, cell.dir.dx, cell.dir.dy);
                }
            }
        }
    }

    return result;
}


result_struct evaluate_with_loops(map m, int debug)
{
    printf("guard starting position: (%d, %d)\n", m.guard_start_x, m.guard_start_y);

    int path_cells_count = 0;

    position guard_pos;
    guard_pos.x = m.guard_start_x;
    guard_pos.y = m.guard_start_y;

    direction guard_dir;
    guard_dir.dx = 0;
    guard_dir.dy = -1;

    int turns = 0;

    path_cell path[MAX_PATH];    

    position turns_positions[MAX_TURNS];
    direction turn_directions[MAX_TURNS];
    int loop_options = 0;
    int turn_flag = 0;

    int look_at = look_forward(m, guard_pos, guard_dir);
    int prev_look_at = 0;
    int iterations = 0;
    int exit = 1;
    while(exit)
    {
        // if(DEBUG) 
        // printf("[iteration #%d] guard pos (%d, %d) guard dir (%d, %d) look_at: %d, turns: %d, loop_options: %d\n", iterations, guard_pos.x, guard_pos.y, guard_dir.dx, guard_dir.dy, look_at, turns, loop_options);
        if(turns>2)
        {
            loop_options += check_for_loop(m, guard_pos, guard_dir, turns_positions, turns, turn_directions, debug);
            // loop_options += check_for_loop_3(m, guard_pos, guard_dir, debug);
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

            path_cells_count++;
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

            path_cell new_path_cell;
            new_path_cell.pos = guard_pos;
            new_path_cell.dir = guard_dir;
            path[path_cells_count] = new_path_cell;
            path_cells_count++;

            guard_pos = go_forward(guard_pos, guard_dir);
            m.data[guard_pos.y][guard_pos.x] = GUARD;

            if(DEBUG) print_matrix(m.data, m.rows, m.cols);
        }
        
        for(int k=0; k<path_cells_count; k++)
        {
            path_cell cell = path[k];
            if(guard_pos.x == cell.pos.x && guard_pos.y == cell.pos.y && guard_dir.dx == cell.dir.dx && guard_dir.dy == cell.dir.dy)
            {
                printf("DEJA VU: pos(%d, %d), dir(%d, %d)\n", cell.pos.x, cell.pos.y, cell.dir.dx, cell.dir.dy);
                exit = 0;
                break;
            }
        }

        prev_look_at = look_at;
        look_at = look_forward(m, guard_pos, guard_dir);
        iterations++;
    }

    path_cells_count = count_path_cells(m);

    // m.data[6][3] = OBSTACLE;
    if(debug) 
    {
        printf("turns: %d\n", turns);
        printf("path cells: %d\n", path_cells_count);
        print_matrix(m.data, m.rows, m.cols);
        printf("loop options: %d\n", loop_options);
    }

    printf("\npath [%d]: ", path_cells_count);
    for(int i=0; i<path_cells_count; i++)
    {
        printf("[pos(%d, %d), dir(%d, %d)] ", path[i].pos.x, path[i].pos.y, path[i].dir.dx, path[i].dir.dy);
    }
    printf("\n");


    int loops = find_loops(path, path_cells_count);
    printf("loops: %d\n", loops);

    result_struct res;
    res.path_cells = path_cells_count;
    res.loop_options = loop_options;

    return res;
}


int evaluate(map m, int max_iterations, int debug)
{
    int path_cells_count = 0;

    position guard_pos;
    guard_pos.x = m.guard_start_x;
    guard_pos.y = m.guard_start_y;

    direction guard_dir;
    guard_dir.dx = 0;
    guard_dir.dy = -1;

    int turns = 0;

    path_cell path[MAX_PATH];    

    position turns_positions[MAX_TURNS];
    direction turn_directions[MAX_TURNS];
    int turn_flag = 0;

    int look_at = look_forward(m, guard_pos, guard_dir);
    int prev_look_at = 0;
    int iterations = 0;

    while(1)
    {
        // if(DEBUG) 
        // printf("[iteration #%d] guard pos (%d, %d) guard dir (%d, %d) look_at: %d, turns: %d, loop_options: %d\n", iterations, guard_pos.x, guard_pos.y, guard_dir.dx, guard_dir.dy, look_at, turns, loop_options);

        if(look_at==0) // exit
        {
            mark_current_position(m.data, guard_pos, guard_dir, &turn_flag, prev_look_at);

            path_cells_count++;
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

            path_cell new_path_cell;
            new_path_cell.pos = guard_pos;
            new_path_cell.dir = guard_dir;
            path[path_cells_count] = new_path_cell;
            path_cells_count++;

            guard_pos = go_forward(guard_pos, guard_dir);
            m.data[guard_pos.y][guard_pos.x] = GUARD;

            if(DEBUG) print_matrix(m.data, m.rows, m.cols);
        }
        
        for(int k=0; k<path_cells_count; k++)
        {
            path_cell cell = path[k];
            if(guard_pos.x == cell.pos.x && guard_pos.y == cell.pos.y && guard_dir.dx == cell.dir.dx && guard_dir.dy == cell.dir.dy)
            {
                //printf("LOOP ALERT: pos(%d, %d), dir(%d, %d)\n", cell.pos.x, cell.pos.y, cell.dir.dx, cell.dir.dy);
                return -1;
            }
        }

        prev_look_at = look_at;
        look_at = look_forward(m, guard_pos, guard_dir);
        iterations++;

        if(iterations>max_iterations) break;
    }

    path_cells_count = count_path_cells(m);

    // m.data[6][3] = OBSTACLE;
    if(debug) 
    {
        printf("turns: %d\n", turns);
        printf("path cells: %d\n", path_cells_count);
        print_matrix(m.data, m.rows, m.cols);

        printf("\npath [%d]: ", path_cells_count);
        for(int i=0; i<path_cells_count; i++)
        {
            printf("[pos(%d, %d), dir(%d, %d)] ", path[i].pos.x, path[i].pos.y, path[i].dir.dx, path[i].dir.dy);
        }
        printf("\n");
    }

    return path_cells_count;
}


void reset_map(map m)
{
    for(int y=0; y<m.rows; y++)
    {
        for(int x=0; x<m.cols; x++)
        {
            if(x==m.guard_start_x && y==m.guard_start_y)
            {
                m.data[y][x] = GUARD;
            }
            else
            {
                char ch = m.data[y][x];
                if(ch!=OBSTACLE)
                {
                    m.data[y][x] = FREE;
                }
            }
        }
    }
}


int brute_force_loops_search(map m, int max_iterations, int debug)
{
    printf("\nstarting brute force loops search...\n");
    int result = 0;

    for(int y=0; y<m.rows; y++)
    {
        for(int x=0; x<m.cols; x++)
        {
            char current_cell = m.data[y][x];
            if(m.data[y][x]==FREE)
            {
                m.data[y][x] = OBSTACLE;

                if(debug) printf("evaluate with obstacle in (%d, %d): ", x, y);
                int go = evaluate(m, max_iterations, 0);
                if(go>0)
                {
                    if(debug) printf("%d\n", go);
                }
                else 
                {
                    result++;
                    if(debug) printf("LOOP #%d\n", result);
                }

                m.data[y][x] = FREE;
            }

            reset_map(m);
        }
    }

    return result;
}