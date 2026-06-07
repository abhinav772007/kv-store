#include "server.h"
#include "parser.h"
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>
#include <algorithm> 
#include<thread>

void Server::handleClient(int clientsocket){
    char buffer[1024]={0};
    while(true){
      memset(buffer,0,sizeof(buffer));
      int bytesRead=read(clientsocket,buffer,sizeof(buffer));
      if(bytesRead<=0)break;
      std::string input(buffer);
      input.erase(std::remove(input.begin(),input.end(),'\n'),input.end());
      input.erase(std::remove(input.begin(),input.end(),'\r'),input.end());

      auto tokens=tokenize(input);
      std::string response;
      if(tokens.empty())response="error\n";
      else if(tokens[0]=="SET" && tokens.size()>=3){
        response=kvstore.set(tokens[1],tokens[2])+"\n";
      } 
      else if(tokens[0]=="GET" && tokens.size()>=2){
        response=kvstore.get(tokens[1])+"\n";
      }
      else if(tokens[0]=="DEL" && tokens.size()>=2){
        response=kvstore.del(tokens[1])+"\n";
      }
      else{
        response="INVALID CMD\n";
      }
      send(clientsocket,response.c_str(),response.size(),0);

    }
    close(clientsocket);
}

void Server::start(int port){
    int server_fd=socket(AF_INET,SOCK_STREAM,0);
    int opt=1;
    setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    sockaddr_in address;
    address.sin_family=AF_INET;
    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_port=htons(port);

    bind(server_fd,(struct sockaddr*)&address,sizeof(address));
    listen(server_fd,5);

    std::cout<<"Server listening on port "<<port<<"\n";
int THREAD_COUNT=4;
for(int i=0;i<THREAD_COUNT;i++){
    connections.emplace_back(&Server::workerThread,this);
}
    while(true){
        int addrlen=sizeof(address);
        int clientSocket=accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

        std::cout<<"Client connected\n";
        {
            std::lock_guard<std::mutex> lock(queuemutex);
            tasks.push(clientSocket);
        }
        cv.notify_one();
    }
}

void Server::workerThread(){
    while(true){
        int clientsocket;
        {
std::unique_lock<std::mutex> lock(queuemutex);
while (tasks.empty()) {
    cv.wait(lock);
}
clientsocket=tasks.front();
tasks.pop();
        }
        handleClient(clientsocket);
    }
}