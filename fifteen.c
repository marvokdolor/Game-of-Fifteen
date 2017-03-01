/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    int tile_number = (d*d) - 1;
    for (int row = 0; row < d; row++)
    {
        for (int col = 0; col < d; col++)
        {
            board[row][col] = tile_number;
                
            /**
             * If the number of tiles is odd, i.e. d is even,
             * Swap tiles 1 and 2.
             */
                 
            if (d % 2 == 0)
            {
                if (tile_number == 1)
                {
                    board[row][col] = 2;
                }
                
                if (tile_number == 2)
                {
                    board[row][col] = 1;
                }
            }
            
            tile_number--;
            
        }
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for (int row = 0; row < d; row++)
    {
        for (int col = 0; col < d; col++)
        {
            if (board[row][col] == 0)
            {
                printf("[  ] ");
            }
            
            else
            {
            printf("[%2d] ", board[row][col]);
            }
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // Find the position of tile on the board.
    if (tile == 0)
    {
        return false;
    }
    
    for (int row = 0; row < d; row++)
    {
        for (int col = 0; col < d; col++)
        {
            if (board[row][col] == tile)
            {
                // If the tile touches the location of the blank tile return true
                if (row < (d-1) && board[row+1][col] == 0) // blank below tile
                {    
                    /** Swap blank and tile i.e. move tile.
                     * There should be a way to make this a function so
                     * I don't repeat it 3 more times below. I'll get there.
                     */
                    int temp = tile;
                    board[row][col] = board[row+1][col];
                    board[row+1][col] = temp;
                    return true;
                }
                if (row > 0 && board[row-1][col] == 0) // blank above tile
                {    
                    int temp = tile;
                    board[row][col] = board[row-1][col];
                    board[row-1][col] = temp;
                    return true;
                }
                if (col < (d-1) && board[row][col+1] == 0) // blank right of tile
                {    
                    int temp = tile;
                    board[row][col] = board[row][col+1];
                    board[row][col+1] = temp;
                    return true;
                }
                if (col > 0 && board[row][col-1] == 0) // blank left of tile
                {    
                    int temp = tile;
                    board[row][col] = board[row][col-1];
                    board[row][col-1] = temp;
                    return true;
                }
            }
        }
    }
    return false;
}


/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // Initialize a tracking variable, num.
    int num = 1;
    
    for (int row = 0; row < d; row++)
    {
        for (int col = 0; col < d; col++)
        {
            if (board[row][col] == num)
            {
                num++;
                // Check that last tile is 0
                if (num == d*d && board[d-1][d-1] == 0)
                {
                    return true;
                }
            }
        }
    }
    return false;
}
