# Simulation-of-FTP-Protocol
Server and Client programs in C to simulate the FTP protocol.
<br></br>

## Commands implimented
- [X] USER
- [X] PASS
- [X] MKD
- [X] CWD
- [X] RMD
- [X] PWD
- [ ] RETR
- [ ] STOR
- [X] LIST
- [X] ABOR
- [X] REIN
- [X] DELE
- [X] CDUP
- [X] QUIT
- [X] PORT
- [X] RNFR
- [X] RNTO
<br></br>

## Working
1. Open two different terminals.
1. Start the server with the help of server side commands in the first terminal
2. Start the client with the help of client side commands in the second terminal
3. Enter the IP address of the server at the client [Localhost: 127.0.0.1 is the IP address used for this project purpose]
4. Enter Username and password 
5. Menu will be displayed and the client is ready to communicate with server

### Server side
cd Sever\
gcc servermain.c -o server\
./server

### Client side
cd Client\
gcc clientmain.c -o client\
./client 127.0.0.1

### Authentication
Username: user\
Password: pass
