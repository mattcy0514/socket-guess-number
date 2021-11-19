#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <math.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080

using namespace std;

void swap(int* a, int* b);
int get_rand_num(int size);

int main() {

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[100];

    srand(time(NULL));
    int digits = 4;
    int rand_num = get_rand_num(digits);
    int game_over = 0;
    // Open Socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        cout << "socket failed" << endl;
        exit(EXIT_FAILURE);
    }
    // Setsockopt
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        cout << "setsockopt" << endl;
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    // Bind Socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        cout << "bind failed" << endl;
        exit(EXIT_FAILURE);
    }
    // Listen for client connecting
    if (listen(server_fd, 3) < 0) {
        cout << "listen" << endl;
        exit(EXIT_FAILURE);
    }
    // Accept for client connecting
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        cout << "Error Connection" << endl;
    }
    cout << "Client Connecting" << endl;
    cout << "Game Start" << endl << "Number is " << rand_num << endl;

    while (true) {
        char state[20];
        int guess_num[10] = {0};
        int num_from_server = rand_num;
        int a = 0, b = 0;
        valread = read(new_socket, buffer, 1024);

        int num_from_client = atoi(buffer);
        cout << "Client guesses " << num_from_client << endl;

        while (num_from_client != 0) {
            int client_remainder = num_from_client % 10;
            int server_remainder = num_from_server % 10;
            if (client_remainder == server_remainder) {
                a++;
            }
            guess_num[client_remainder]++;
            guess_num[server_remainder]++;
            num_from_client = num_from_client / 10;
            num_from_server = num_from_server / 10;
        }

        for (int i = 0; i < sizeof(guess_num) / sizeof(guess_num[0]); i++) {
            if (guess_num[i] == 2) {
                b++;
            }
        }

        sprintf(state, "%dA%dB", a, b - a);
        send(new_socket, state, strlen(state), 0);

        if (strcmp(state, "4A0B") == 0) {
            game_over = 1;
        }
        
        if (game_over) {
            cout << "Game over" << endl;
            close(new_socket);
            break;
        }
    }
}

int get_rand_num(int digits) {
    int seed_arr[10];
    int rand_num = 0;
    int first_loop = 1;
    for (int i = 0; i < sizeof(seed_arr) / sizeof(seed_arr[0]); i++) {
        seed_arr[i] = i;
    }
    for (int i = 0; i < digits; i++) {
        int random = rand() % (10 - i - first_loop) + first_loop;
        rand_num += seed_arr[random] * pow(10, 3 - i);
        swap(seed_arr[random], seed_arr[9 - i]);
        first_loop = 0;
    }
    return rand_num;
}

void swap(int* a, int* b) {
    int temp;
    temp = *a;
    *a = *b;
    *b = *a;
}