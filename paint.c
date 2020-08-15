// Assignment 1 19T3 COMP1511: CS Paint
// paint.c
// This program was written by SONALI SHARMA (z5258346)
// on 20 - 10 - 2019

#include <stdio.h>
#include <math.h>

// The dimensions of the canvas (20 rows x 36 columns).
#define N_ROWS 20
#define N_COLS 36

// Shades (assuming your terminal has a black background).
#define BLACK 0
#define WHITE 4

#define FALSE 0
#define TRUE 1

// Provided helper functions:
// Display the canvas.
void displayCanvas(int canvas[N_ROWS][N_COLS]);

// Clear the canvas by setting every pixel to be white.
void clearCanvas(int canvas[N_ROWS][N_COLS]);

// Calculate the distance between two points
double distance(int row1, int col1, int row2, int col2);

// My function prototypes
void line(int start_row, int start_col, int length, int direction, 
    int canvas[N_ROWS][N_COLS], int shade);

void fill_square(int row, int column, int length, int direction,
    int canvas[N_ROWS][N_COLS], int shade);

int is_valid(int start_row, int start_col, int length, int direction,
    int canvas[N_ROWS][N_COLS]);

int is_valid_square(int start_row, int start_col, int length, int direction,
    int canvas[N_ROWS][N_COLS]);

int check_starting_bounds(int start_row, int start_col, 
    int canvas[N_ROWS][N_COLS]);

int negative_length(int length);

int new_direction(int direction);

int change_shade(int shade, int command);

void copy_paste(int start_row, int start_col, int length, int direction,
    int target_row, int target_col, int canvas[N_ROWS][N_COLS], int shade);
    
void diagonal_line(int start_row, int start_col, int length, int direction,
    int canvas[N_ROWS][N_COLS], int shade);
    
void draw_ellipse(int focus1_row, int focus1_col, int focus2_row,
    int focus2_col, double length, int fill, int canvas[N_ROWS][N_COLS],
    int shade);

int main(void) {
    int canvas[N_ROWS][N_COLS];

    clearCanvas(canvas);

    // initialising variables
    int command[7] = {0};
    int shade = 0;
    int focus1_row = 0;
    int focus1_col = 0;
    int focus2_row = 0;
    int focus2_col = 0;
    double length = 0;
    int fill = 0;
      
    // scan first integer of command
    while (scanf("%d", &command[0]) == 1) {
    
        // draw line
        if (command[0] == 1) {
            scanf("%d %d %d %d", &command[1], &command[2], &command[3],
            &command[4]); 
            line(command[1], command[2], command[3], command[4], canvas, shade);
            
            // fill square    
        } else if (command[0] == 2) {
            scanf("%d %d %d %d", &command[1], &command[2], &command[3],
            &command[4]); 
            fill_square(command[1], command[2], command[3], command[4],
            canvas, shade);
            
            // change shade    
        } else if (command[0] == 3) {
            scanf("%d", &command[1]);
            shade = change_shade(shade, command[1]);
            
            // copy paste
        } else if (command[0] == 4) {
            scanf("%d %d %d %d %d %d", &command[1], &command[2], &command[3], 
            &command[4], &command[5], &command[6]);
            copy_paste(command[1], command[2], command[3], command[4],
            command[5], command[6], canvas, shade);
              
        } else if (command[0] == 0) {
            scanf("%d %d %d %d %lf %d", &focus1_row, &focus1_col, &focus2_row,
            &focus2_col, &length, &fill );
            draw_ellipse(focus1_row, focus1_col, focus2_row,
            focus2_col, length, fill, canvas,
            shade);
        
        }
    
    }
    
    displayCanvas(canvas);

    return 0;
}



// Draw line using command inputs
void line(int start_row, int start_col, int length, int direction,
    int canvas[N_ROWS][N_COLS], int shade) {

    // handle negative lengths
    if (length < 0) {
        length = negative_length(length);
        direction = new_direction(direction);
    }
    
    if (is_valid(start_row, start_col, length, direction, canvas) == FALSE) {
        return;
    }
    
    // line going up 
    if (direction%360 == 0) {
        int i = start_row;
        while (start_row > i - length) {
            canvas[start_row][start_col] = shade;
            start_row--;
        }
        
        // line going right 
    } else if (direction%360 == 90) {
        int i = start_col;
        while (start_col < length + i) {
            canvas[start_row][start_col] = shade;
            start_col++;
        }
    
        // line going down
    } else if (direction%360 == 180) {
        int i = start_row;
        while (start_row < length + i) {
            canvas[start_row][start_col] = shade;
            start_row++;
        }
    
        // line going left
    } else if (direction%360 == 270) {
        int i = start_col;
        while (start_col > i - length) {
            canvas[start_row][start_col] = shade;
            start_col--;
        }
    
        // diagonal lines
    } else {
        diagonal_line(start_row, start_col, length, direction,
    canvas, shade);
    }
    
    
}

// Fill square using command inputs 
void fill_square(int row, int column, int length, int direction,
    int canvas[N_ROWS][N_COLS], int shade) {
    // if direction is either of these then command should act same as draw line
    // command 
    if (direction == 0 || direction == 90 || direction == 180
        || direction == 270) {
        line(row, column, length, direction, canvas, shade);
    }
    
    // handle negative lengths
    if (length < 0) {
        length = negative_length(length);
        direction = new_direction(direction);
    }
    
    if (is_valid_square(row, column, length, direction, canvas) == FALSE) {
        return;
    } 
    
    // bottom left to top right
    if (direction%360 == 45) {
        int i = row;
        int j = column;
        while (row > i - length) {
            column = j;
            while (column < length + j) {
                canvas[row][column] = shade;
                column++;
            }
            row--;
        }
    
        // top left to bottom right
    } else if (direction%360 == 135) {
        int i = row;
        int j = column;
        while (row < length + i) {
            column = j;            
            while (column < length + j) {
                canvas[row][column] = shade;
                column++;
            }
            
            row++;  
        }
    
        // top right to bottom left
    } else if (direction%360 == 225) {
        int i = row;
        int j = column;
        while (row < length + i) {
            column = j;
            while (column > j - length) {
                canvas[row][column] = shade;
                column--;
            }
        
            row++;
        }
        
        // bottom right to top left
    } else if (direction%360 == 315) {
        int i = row;
        int j = column;
        while (row > i - length) {
            column = j;
            while (column > j - length) {
                canvas[row][column] = shade;
                column--; 
            }
        
            row--;
        }
    
    
    } else {
        return;
    }  
}
    
// Checks given inputs to determine their validity when
// drawing a line
int is_valid(int start_row, int start_col, int length, int direction, 
    int canvas[N_ROWS][N_COLS]) {
  
    if (check_starting_bounds(start_row, start_col, canvas) == FALSE) {
        return 0;
    }
   
    
    if (direction%360 == 0 && start_row - length  < -1) {
        return 0;
    } else if (direction%360 == 90 && start_col + length > N_COLS ) {
        return 0;
    } else if (direction%360 == 180 && start_row + length > N_ROWS ) { 
        return 0;
    } else if (direction%360 == 270 && start_col - length < -1) {
        return 0;
    } else if ((direction == 45 && start_row - length  < -1 ) ||
        (direction == 45 && start_col + length > N_COLS )) {
        return 0;
    } else if ((direction == 135 && start_col + length > N_COLS) ||
        (direction == 135 && start_row + length > N_ROWS)) {
        return 0;
    } else if ((direction == 225 && start_row + length > N_ROWS) ||
        (direction == 225 && start_col - length < -1 )) {
        return 0;
    } else if ((direction == 315 && start_row - length < -1) ||
        (direction == 315 && start_col - length < -1)) {
        return 0;
    } else {
        return 1;
    }
    
}     

// Checks command inputs for validity when filling square
int is_valid_square(int start_row, int start_col, int length, int direction,
    int canvas[N_ROWS][N_COLS]) { 
    
    if (check_starting_bounds(start_row, start_col, canvas) == FALSE) {
        return 0;
    }
   
    
    if ((direction%360 == 45 && start_row - length  < -1 ) ||
        (direction%360 == 45 && start_col + length > N_COLS )) {
        return 0;
    } else if ((direction%360 == 135 && start_col + length > N_COLS) ||
        (direction%360 == 135 && start_row + length > N_ROWS) ) {
        return 0;
    } else if ((direction%360 == 225 && start_row + length > N_ROWS) ||
        (direction%360 == 225 && start_col - length < -1 )) { 
        return 0;
    } else if ((direction%360 == 315 && start_row - length < -1) ||
        (direction%360 == 315 && start_col - length < -1)) {
        return 0;
    } else {
        return 1;
    }
    
} 

// Checks the starting bounds of the given command
// to determine whether start pixel lies inside canvas
// It returns FALSE if the starting pixel is outside the canvas
int check_starting_bounds(int start_row, int start_col,
    int canvas[N_ROWS][N_COLS]){
    if (start_row < 0 || start_col < 0 ||
        start_row > N_ROWS - 1  || start_col > N_COLS - 1  ) {
        return FALSE;
    } else {
        return TRUE;
    } 

}


    
// Converts the negative length to a positive length
// in the other direction
int negative_length(int length){

    length = length * -1;
    
    return length;
}


// Returns the opposite direction if we are given
// a negative length as input
int new_direction(int direction) {
        
    if (direction == 0 || direction == 180 || direction == 90 
        || direction == 270 || direction == 45 || direction == 225 || 
        direction == 135 || direction == 315 ) {
        return (direction + 180)%360;
            
    } else {
        return 0;
    }
       
}

// Changes the shade if it is a valid shade
int change_shade(int shade, int command) {
    if (shade + command >= 0 && shade + command <=4) {
        return shade + command;
    } else {
        return shade;
    }
}

// Copies a square to a new location as indicated by 
// target row and column
// If it is 0, 90, 180 or 270 degrees, it copies a line
void copy_paste(int start_row, int start_col, int length, int direction,
    int target_row, int target_col, int canvas[N_ROWS][N_COLS], int shade) {
    
    if (direction == 0 || direction == 90 ||
        direction == 180 || direction == 270) {
        line(start_row, start_col, length, direction, canvas, shade);
        line(target_row, target_col, length, direction, canvas, shade);
    } else {
        fill_square(start_row, start_col, length, direction, canvas, shade);
        fill_square(target_row, target_col, length, direction, canvas, shade);
    }
    
    
}

// Draws a diagonal line if the direction input 
// is 45, 135, 225, or 315 degrees and returns if it is none of these 
// directions (invalid degrees)
void diagonal_line(int start_row, int start_col, int length, int direction,
    int canvas[N_ROWS][N_COLS], int shade) {

    if (direction == 45) {
        int i = start_row;
        int j = start_col;
        int b_counter = 0;
        
        while (start_row > i - length) {
            start_col = j;
            int a_counter = 0;
            while (start_col < length + j) {
                if (a_counter == b_counter) {
                    canvas[start_row][start_col] = shade;    
                }
                a_counter++;
                start_col++;
            }
            start_row--;
            b_counter++;
        } 
  
    } else if (direction == 135) {
        int i = start_row;
        int j = start_col;
        int b_counter = 0;
        while (start_row < length + i) {
            start_col = j;
            int a_counter = 0;            
            while (start_col < length + j) {
                if (a_counter == b_counter) {
                    canvas[start_row][start_col] = shade;
                }
                a_counter++;
                start_col++;
            }
            
            start_row++;
            b_counter++;  
        }    
    
    } else if (direction == 225) {
        int i = start_row;
        int j = start_col;
        int b_counter = 0;
        while (start_row < length + i) {
            start_col = j;
            int a_counter = 0;
            while (start_col > j - length) {
                if (a_counter == b_counter) {
                    canvas[start_row][start_col] = shade;
                }
                a_counter++;
                start_col--; 
            }
            start_row++;
            b_counter++;
        }  

    } else if (direction == 315) {
        int i = start_row;
        int j = start_col;
        
        int b_counter = 0;
        while (start_row  > i - length  ) {
            start_col = j;
            int a_counter = 0;
            while (start_col  > j - length ) {
                if (a_counter == b_counter) {
                    canvas[start_row][start_col] = shade;
                }
                a_counter++;
                start_col--; 
            }
            start_row--;
            b_counter++;
        }

    } else {
        return;
    }
}

// Draws ellipse by looping through canvas and seeing if
// given pixel is inside the ellipse

void draw_ellipse(int focus1_row, int focus1_col, int focus2_row,
    int focus2_col, double length, int fill, int canvas[N_ROWS][N_COLS],
    int shade) {
    
    int row = 0;
    int col = 0;
    
    if (fill != 0) {
        //loop through canvas
        row = 0;
        while (row < N_ROWS) {
            col = 0;
            while (col < N_COLS) {
                if ((distance(focus1_row, focus1_col, row, col) + 
                 distance(focus2_row, focus2_col, row, col)) <= 2 * length) {
                    canvas[row][col] = shade;
                }
                col++;
            }
            row++;
        }
    
    } else {
        return;
    }  
}

// Calculate the distance between two points (row1, col1) and (row2, col2).
double distance(int row1, int col1, int row2, int col2) {
    int row_dist = row2 - row1;
    int col_dist = col2 - col1;
    return sqrt((row_dist * row_dist) + (col_dist * col_dist));
}

// Displays the canvas, by printing the integer value stored in
// each element of the 2-dimensional canvas array.
void displayCanvas(int canvas[N_ROWS][N_COLS]) {
    int row = 0;
    while (row < N_ROWS) {
        int col = 0;
        while (col < N_COLS) {
            printf("%d ", canvas[row][col]);
            col++;
        }
        row++;
        printf("\n");
    }
}


// Sets the entire canvas to be blank, by setting each element in the
// 2-dimensional canvas array to be WHITE (which is #defined at the top
// of the file).
void clearCanvas(int canvas[N_ROWS][N_COLS]) {
    int row = 0;
    while (row < N_ROWS) {
        int col = 0;
        while (col < N_COLS) {
            canvas[row][col] = WHITE;
            col++;
        }
        row++;
    }
}
