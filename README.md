# Socket Programming - Guess Number (1A2B)
四資管三 B10809024 江忠晏

## Input
1000 <= N <= 9999$, and every bits are not mutually repeated.

## Output
xAyB, x in [0, 4] , y in [0, 4], x+y=4

## Example
**Random number = 2974**

### Input
- 123
- 45678
- 1111
- 1234
- 5678
- 2974

### Expected Output
- 4 bit nonrepeated number
- 4 bit nonrepeated number
- nonrepeated 4 bit number
- 1A1B
- 1A0B
- 4A0B

## g++ version
g++ (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0

## Makefile
```
run:
	g++ -o server server.cpp
	g++ -o client client.cpp

server-side:
	./server

client-side:
	./client

clean:
	rm ./server
	rm ./client
```

## Build
```
make
```
## Run Server
```
make server-side
```
![](https://i.imgur.com/3aOh3WR.png)

## Run Client
```
make client-side
```
![](https://i.imgur.com/UvJgwxe.png)

