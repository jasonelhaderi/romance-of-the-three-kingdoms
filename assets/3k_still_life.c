#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define GRID_SIZE 256
#define CELL_SIZE 1 // CELL_SIZE = 256 / GRID_SIZE

// Box-Muller transform
double gaussianNoise(double mu, double sigma)
{
    static double v = 0, fac = 0;
    static int phase = 0;
    double S, Z, U1, U2, u;

    // uses v and fac from previous call if a previous call has happened.
    if(phase) Z = v * fac;

    else {
        do {
            U1 = (double) rand() / RAND_MAX;
            U2 = (double) rand() / RAND_MAX;

            u = 2.0 * U1 - 1.0;
            v = 2.0 * U2 - 1.0;
            S = u * u + v * v;
        } while(S >= 1);

        fac = sqrt(-2.0 * log(S) / S);
        Z = u * fac;
    }

    phase = 1 - phase;

    return Z * sigma + mu;
}

// generate instructions to draw a square
void square(FILE *fp, int xpos, int ypos, int w, int h, int c)
{
    int px, py;

    for(py = 0; py < h; py++) {
        for(px = 0; px < w; px++) {
            fprintf(fp, "%d %d %d pxpt\n",
                    px + xpos, py + ypos, c);
        }
    }
}

// generates instructions to draw the states array for three_kingdoms()
void draw_state_3k(FILE *fp, int states[3][GRID_SIZE][GRID_SIZE])
{
    int px, py, i;
    // set color scheme black for unclaimed, dark gray for states[2] claimed,
    // red, green, blue, and faded versions for states[0] and states[1] claimed
    fprintf(fp, "0 0 0 0 pxclr\n");
    fprintf(fp, "1 30 30 30 pxclr\n");
    fprintf(fp, "2 100 30 30 pxclr\n");
    fprintf(fp, "3 200 30 30 pxclr\n");
    fprintf(fp, "4 30 100 30 pxclr\n");
    fprintf(fp, "5 30 200 30 pxclr\n");
    fprintf(fp, "6 30 30 100 pxclr\n");
    fprintf(fp, "7 30 30 200 pxclr\n");

    // first draw occupied cells from states[2] in dark gray
    for(px = 0; px < GRID_SIZE; px++) {
        for(py = 0; py < GRID_SIZE; py++) {
            if(states[2][px][py] == 'r') {
                square(fp, px * CELL_SIZE,
                      py * CELL_SIZE, CELL_SIZE, CELL_SIZE, 1);
            }
            if(states[2][px][py] == 'g') {
                square(fp, px * CELL_SIZE,
                      py * CELL_SIZE, CELL_SIZE, CELL_SIZE, 1);
            }
            if(states[2][px][py] == 'b') {
                square(fp, px * CELL_SIZE,
                      py * CELL_SIZE, CELL_SIZE, CELL_SIZE, 1);
          }
        }
    }


    // now draw occupied cells from states[1] in faded on first loop,
    // occupied cells from states[0] in color on second loop
    for(i = 0; i < 2; i++) {
        for(px = 0; px < GRID_SIZE; px++) {
            for(py = 0; py < GRID_SIZE; py++) {
                if(states[1 - i][px][py] == 'r') {
                    square(fp, px * CELL_SIZE,
                          py * CELL_SIZE, CELL_SIZE, CELL_SIZE, 2 + i);
                }
                if(states[1 - i][px][py] == 'g') {
                    square(fp, px * CELL_SIZE,
                          py * CELL_SIZE, CELL_SIZE, CELL_SIZE, 4 + i);
                }
                if(states[1 - i][px][py] == 'b') {
                    square(fp, px * CELL_SIZE,
                          py * CELL_SIZE, CELL_SIZE, CELL_SIZE, 6 + i);
                }
            }
        }
    }
}

// makes a new grid state for each cell in three_kingdoms()
void update_cells_3k(int states[][GRID_SIZE][GRID_SIZE])
{
    int px, py;
    int n_r, n_g, n_b;
    int i;
    for(px = 0; px < GRID_SIZE; px++) {
        for(py = 0; py < GRID_SIZE; py++) {
            int neighbors[8] = {states[1][(px - 1) % GRID_SIZE][(py - 1) % GRID_SIZE],
                                states[1][px % GRID_SIZE][(py - 1) % GRID_SIZE],
                                states[1][(px + 1) % GRID_SIZE][(py - 1) % GRID_SIZE],
                                states[1][(px - 1) % GRID_SIZE][(py + 1) % GRID_SIZE],
                                states[1][px % GRID_SIZE][(py + 1) % GRID_SIZE],
                                states[1][(px + 1) % GRID_SIZE][(py + 1) % GRID_SIZE],
                                states[1][(px - 1) % GRID_SIZE][py % GRID_SIZE],
                                states[1][(px + 1) % GRID_SIZE][py % GRID_SIZE]};

            n_r = n_g = n_b = 0;
            // count number of neighboring red, green, and blue cells
            for(i = 0; i < 8; i++){
                if(neighbors[i] == 'r') {
                    n_r += 1;
                } else if(neighbors[i] == 'g') {
                    n_g += 1;
                } else if(neighbors[i] == 'b') {
                    n_b += 1;
                }
            }

            // rules for generating a new state if current cell is alive
            if(states[1][px][py] == 'r') {
                if(n_r < 2) {
                    states[0][px][py] = 0;
                }
                if(n_r >= 2 && n_r < 6) {
                    // if there are more neighboring cells of different types,
                    // the current cell dies
                    if(n_r < n_b + n_g) {
                        states[0][px][py] = 0;
                    } else states[0][px][py] = 'r';
                }
                if(n_r >= 6) {
                    states[0][px][py] = 0;
                }
            }

            if(states[1][px][py] == 'g') {
                if(n_g < 2) {
                    states[0][px][py] = 0;
                }
                if(n_g >= 2 && n_g < 5) {
                    // if there are more neighboring cells of different types,
                    // the current cell dies
                    if(n_g < n_r + n_b) {
                        states[0][px][py] = 0;
                    } else states[0][px][py] = 'g';
                }
                if(n_g >= 5) {
                    states[0][px][py] = 0;
                }
            }

            if(states[1][px][py] == 'b') {
                if(n_b < 2) {
                    states[0][px][py] = 0;
                }
                if(n_b >= 2 && n_b < 7) {
                    // if there are more neighboring cells of different types,
                    // the current cell dies
                    if(n_b < n_r + n_g) {
                        states[0][px][py] = 0;
                    } else states[0][px][py] = 'b';
                }
                if(n_b >= 7) {
                    states[0][px][py] = 0;
                }
            }

            // rules for generating new state if current cell is dead
            if(states[1][px][py] == 0) {
                // if the number of one color outnumbers either color,
                // and there are exactly 3 neighbors of that color,
                // cell turns to that color
                if(n_r > n_g && n_r > n_b) {
                    if(n_r >= 3) {
                        states[0][px][py] = 'r';
                    }
                } else if(n_g > n_b && n_g > n_r) {
                    if(n_g >= 2) {
                      states[0][px][py] = 'g';
                    }
                } else if(n_b > n_r && n_b > n_g) {
                    if(n_b >= 4) {
                      states[0][px][py] = 'b';
                    }
                }
            }
        }
    }
}

void three_kingdoms(FILE *fp, int seed, int n_gens,
                    double rgb_mu[3][2], double rgb_sigma[3][2], double n_start[3])
{
    srand(seed);
    int px, py, i, j, flag;
    int gen;
    int states[3][GRID_SIZE][GRID_SIZE];

    for(px = 0; px < GRID_SIZE; px++) {
        for(py = 0; py < GRID_SIZE; py++) {
            states[0][px][py] = states[1][px][py] = states[2][px][py] = 0;
        }
    }

    for(i = 0; i < 3; i++) {
        for(j = 0; j < n_start[i]; j++) {
            flag = 0;
            while(flag == 0) {
                px = (int) gaussianNoise(rgb_mu[i][0], rgb_sigma[i][0]) % GRID_SIZE;
                py = (int) gaussianNoise(rgb_mu[i][1], rgb_sigma[i][1]) % GRID_SIZE;
                if(states[0][px][py] == 0) {
                    if(i == 0) states[0][px][py] = 'r';
                    if(i == 1) states[0][px][py] = 'g';
                    if(i == 2) states[0][px][py] = 'b';
                    }
                    flag = 1;
            }
        }
    }

    // evolve state n_gens times
    for(gen = 0; gen < n_gens; gen++) {
        // move states[2] to 1 and 1 to 0
        for(px = 0; px < GRID_SIZE; px++) {
            for(py = 0; py < GRID_SIZE; py++) {
                states[2][px][py] = states[1][px][py];
                states[1][px][py] = states[0][px][py];
            }
        }
        update_cells_3k(states);
    }
    draw_state_3k(fp, states);
}

int main(int argc, char *argv[])
{
    FILE *fp;
    long n_gens = strtol(argv[1], NULL, 10);
    double rgb_mu[3][2]; // center of initial gaussian rgb distribution (x, y)
    double rgb_sigma[3][2]; // stdev of initial gaussian rgb distribution (x, y)
    double n_start[3]; // number of red, green, blue initial cells

    // gaussian initial condition parameters
    // mean
    rgb_mu[0][0] = GRID_SIZE / 2.0;          // red_x
    rgb_mu[0][1] = GRID_SIZE /2.0;           // red_y
    rgb_mu[1][0] = GRID_SIZE /2.0;           // green_x
    rgb_mu[1][1] = GRID_SIZE /2.0;           // green_y
    rgb_mu[2][0] = GRID_SIZE / 2.0;          // blue_x
    rgb_mu[2][1] = GRID_SIZE / 2.0;          // blue_y
    // standard dev
    rgb_sigma[0][0] = GRID_SIZE / 10.0;      // red_sigma_x
    rgb_sigma[0][1] = GRID_SIZE / 19.0;      // red_sigma_y
    rgb_sigma[1][0] = GRID_SIZE / 9.0;       // green_sigma_x
    rgb_sigma[1][1] = GRID_SIZE / 11.0;      // green_sigma_y
    rgb_sigma[2][0] = GRID_SIZE / 6.0;       // blue_sigma_x
    rgb_sigma[2][1] = GRID_SIZE / 5.0;       // blue_sigma_y
    // number of live cells at start
    n_start[0] = GRID_SIZE * GRID_SIZE / 10.0; // Fill 10% of the board with red
    n_start[1] = GRID_SIZE * GRID_SIZE / 15.0; // Fill 5% of the board with green
    n_start[2] = GRID_SIZE * GRID_SIZE / 4.0; // Fill 25% of the board with blue

    // opens run.rnt file in write mode
    fp = fopen("run.rnt", "w");

    // comment to start off runt file
    fprintf(fp, "# generated image using 3k_still_life program\n");

    three_kingdoms(fp, 42, n_gens, rgb_mu, rgb_sigma, n_start);

    // generates instructions at end of runt file to save image to ./out.png
    fprintf(fp, "\"out.png\" pxsave\n");

    // closes file
    fclose(fp);

    return 0;
}
