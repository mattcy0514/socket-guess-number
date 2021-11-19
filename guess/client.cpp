#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <math.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 8080

using namespace std;

int main(int argc, char const *argv[])
{
	// Open Socket
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		cout << "socket created failed" << endl;
		return -1;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	// Convert IPv4 and IPv6 addresses
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		cout << "Invalid address/ Address not supported" << endl;
		return -1;
	}
	// Connect to socket
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		cout << "Connection Failed" << endl;
		return -1;
	}

	cout << "Connecting" << endl;
	
    while (true) {
		char state[20] = {0};
		int number;
		int number_correctness = 0;
		int bit_correctness = 0;
		while (number_correctness != 4 || bit_correctness != 4) {
			cout << "Choose a nonrepeated 4 bit number ";
        	cin >> number;

			int check_num = number;
			int guess_num[10] = {0};

			number_correctness = 0;
			bit_correctness = 0;

			while (check_num != 0) {
				guess_num[check_num % 10]++;
				check_num = check_num / 10;
				bit_correctness++;
			}
			for (int i = 0; i < sizeof(guess_num) / sizeof(guess_num[0]); i++) {
				if (guess_num[i] == 1) {
					number_correctness++;
				}
			}
			if (bit_correctness != 4) {
				cout << "Please enter an 4 bit nonrepeated number" << endl;
			}
			if (number_correctness < 4 && bit_correctness == 4) {
				cout << "Please enter an nonrepeated 4 bit number" << endl;
			}
		}
        
        cout << "You guess " << number << " ";

		// Convert number to char*
        string number_str = to_string(number);
        const char* sent_number = number_str.c_str();
        send(sock, sent_number, sizeof(sent_number), 0);
        valread = read(sock, state, sizeof(state));
        cout << state << endl << endl;

        if (strcmp(state, "4A0B") == 0) {
			cout << "Game Over" << endl;
			close(sock);
            break;
        }
    }
}
