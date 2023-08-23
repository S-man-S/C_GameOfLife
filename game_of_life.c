#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define HEIGHT 25
#define WIDTH 80
#define ALIVE '*'
#define DEAD '.'

int input_field(char **matrix);
void print_field(char **matrix);
int Game(char **matrix, char **prev_matrix);
char check(char **matrix, int ii, int jj);
int check_stable(char **next_matrix, char **matrix);
int check_period(char **next_matrix, char **prev_matrix);
int check_alive(char **next_matrix);

int main(int argc, char *argv[]) {
    float speed;
    printf(
        "Привет пользователь!\nДля начала введи скорость (число от 1 до 100, где "
        "1 - медленно, а 100 - быстро) с которой будет изменяться картинка: ");
    scanf("%f", &speed);
    if (speed <= 100 && speed >= 1) {
        if (argc == 1 || freopen(argv[1], "r+", stdin) != NULL) {
            char **matrix = malloc(HEIGHT * sizeof(char *)), **prev_matrix = malloc(HEIGHT * sizeof(char *));

            for (int i = 0; i < HEIGHT; i++) {
                matrix[i] = malloc(WIDTH * sizeof(char));
                prev_matrix[i] = malloc(WIDTH * sizeof(char));
                for (int j = 0; j < WIDTH; j++) prev_matrix[i][j] = DEAD;
            }

            if (input_field(matrix) == 1) {
                system("clear");
                print_field(matrix);
                while (Game(matrix, prev_matrix) == 1) {
                    system("clear");
                    print_field(matrix);
                    usleep(1000000 / speed);
                }
                printf("The end!");
            } else
                printf("n/a");

            for (int i = 0; i < HEIGHT; i++) {
                free(matrix[i]);
                free(prev_matrix[i]);
            }

            free(matrix);
            free(prev_matrix);
        } else
            printf("Не удалось открыть файл :(");
    } else
        printf("Так мы не договаривались!!!\nn/a");
    return 0;
}

int check_alive(char **next_matrix) {
    int flag = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (next_matrix[i][j] == ALIVE) {
                flag = 1;
                break;
            }
        }
    }
    return flag;
}

int check_period(char **next_matrix, char **prev_matrix) {
    int flag = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (next_matrix[i][j] != prev_matrix[i][j]) {
                flag = 1;
                break;
            }
        }
    }
    return flag;
}

int check_stable(char **next_matrix, char **matrix) {
    int flag = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (next_matrix[i][j] != matrix[i][j]) {
                flag = 1;
                break;
            }
        }
    }
    return flag;
}

char check_neighbours(char **matrix, int ii, int jj) {
    int count = 0;
    char ret = matrix[ii][jj];
    for (int i = ii - 1; i <= ii + 1; i++) {
        for (int j = jj - 1; j <= jj + 1; j++) {
            if ((i != ii || j != jj) && matrix[(i + HEIGHT) % HEIGHT][(j + WIDTH) % WIDTH] == ALIVE) {
                count++;
            }
        }
    }
    if (matrix[ii][jj] == '.') {
        if (count == 3) ret = ALIVE;
    } else {
        if (count < 2 || count > 3) ret = DEAD;
    }
    return ret;
}

int Game(char **matrix, char **prev_matrix) {
    char **next_matrix = malloc(HEIGHT * sizeof(char *));
    for (int i = 0; i < HEIGHT; i++) next_matrix[i] = malloc(WIDTH * sizeof(char));

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            next_matrix[i][j] = check_neighbours(matrix, i, j);
        }
    }
    int flag =
        check_alive(next_matrix) * check_period(next_matrix, prev_matrix) * check_stable(next_matrix, matrix);
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            prev_matrix[i][j] = matrix[i][j];
            matrix[i][j] = next_matrix[i][j];
        }
    }

    for (int i = 0; i < HEIGHT; i++) free(next_matrix[i]);
    free(next_matrix);
    return flag;
}

int input_field(char **matrix) {
    int flag = 1, i = 0, j = 0;
    char cur, x;

    while (i < HEIGHT) {
        while (j < WIDTH) {
            x = scanf("%c", &cur);
            if (x == 0) {
                flag = 0;
                break;
            } else {
                if (cur == ALIVE || cur == DEAD) {
                    matrix[i][j] = cur;
                    j++;
                } else if (cur != ' ' && cur != '\n' && cur != '\t' && cur != '\v') {
                    flag = 0;
                    break;
                }
            }
        }
        if (flag == 0) break;
        j = 0;
        i++;
    }
    return flag;
}

void print_field(char **matrix) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH - 1; j++) {
            printf("%c", matrix[i][j]);
        }
        if (i < HEIGHT) printf("\n");
    }
}
