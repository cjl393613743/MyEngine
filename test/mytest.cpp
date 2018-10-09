#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <cstring>
#include <errno.h>

const int MAX_EVENT = 100;
const int BUF_LEN = 1024;
const char * const LOCAL_ADDR = "127.0.0.1";

using namespace std;

int MakeSocketNonBlocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if(-1 == flags)
    {
        cout<<"error: get fd status fail"<<endl;
        return -1;
    }

    flags |= O_NONBLOCK;

    int s = fcntl(fd, F_SETFL, flags);
    if(-1 == s)
    {
        cout<<"error: set fd status fail"<<endl;
        return -1;
    }

    return 0;
}

void InitServer()
{
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == lfd)
    {
        cout<<"error: open listen fd fail"<<endl;
        return;
    }

    int on = 1;
    if(-1 == setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)))
    {
        cout<<"error: set sock opt fail"<<endl;
        return;
    }

    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    inet_aton(LOCAL_ADDR, &(serverAddr.sin_addr));
    serverAddr.sin_port = htons(8000);

    if(-1 == bind(lfd, (const struct sockaddr *)&serverAddr, sizeof(serverAddr)))
    {
        cout<<"error: bind fail"<<endl;
        return;
    }

    if(-1 == MakeSocketNonBlocking(lfd))
    {
        cout<<"error: make socket non blocking fail"<<endl;
        return;
    }

    if(-1 == listen(lfd, 10))
    {
        cout<<"error: start listen fail"<<endl;
        return;
    }

    int epfd = epoll_create1(0);
    if(-1 == epfd)
    {
        cout<<"error: create epoll fail"<<endl;
        return;
    }

    struct epoll_event ev, arrEvent[MAX_EVENT];
    ev.data.fd = lfd;
    ev.events = EPOLLIN | EPOLLET;

    if(-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev))
    {
        cout<<"error: epoll ctl fail"<<endl;
        return;
    }

    cout<<"服务器已启动"<<endl;

    while(1)
    {
        int waitEventCount = epoll_wait(epfd, arrEvent, MAX_EVENT, -1);
        for(int i = 0; i < waitEventCount; ++i)
        {
            unsigned int events = arrEvent[i].events;
            if(events & EPOLLERR || events & EPOLLHUP || (!events & EPOLLIN))
            {
                cout<<"error: epoll has error"<<endl;
                close(arrEvent[i].data.fd);
                continue;
            }else if(lfd == arrEvent[i].data.fd)
            {
                while(1)
                {
                    struct sockaddr inAddr = {0};
                    socklen_t inAddrLen = sizeof(inAddr);
                    int accpfd = accept(lfd, &inAddr, &inAddrLen);
                    if(-1 == accpfd)
                    {
                        if(EAGAIN == accpfd)
                        {
                            cout<<"error: accept socket fail"<<endl;
                        }

                        break;
                    }

                    if(-1 == MakeSocketNonBlocking(accpfd))
                    {
                        cout<<"error: accept socket make socket non block fail"<<endl;
                        return;
                    }

                    ev.data.fd = accpfd;
                    ev.events = EPOLLIN | EPOLLET;
                    if(-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, accpfd, &ev))
                    {
                        cout<<"error: accept epoll ctl fail"<<endl;
                        return;
                    }
                }
                continue;
            }else
            {
                int done = 0;
                while(1)
                {
                    char buf[BUF_LEN] = {0};
                    int resultLen = read(arrEvent[i].data.fd, buf, sizeof(buf) / sizeof(buf[0]));

                    if(-1 == resultLen)
                    {
                        if(EAGAIN != errno)
                        {
                            cout<<"error: read data fail"<<endl;
                            done = 1;
                        }
                        break;
                    }else if(!resultLen)
                    {
                        done = 1;
                        break;
                    }

                    cout<<"服务器收到消息："<<buf<<":"<<resultLen<<endl;
                    write(arrEvent[i].data.fd, buf, resultLen);
                }

                if(done)
                {
                    cout<<"Closed connection"<<endl;
                    close(arrEvent[i].data.fd);
                }
            }
        }
    }

    close(epfd);
}

void InitClient(char *msg)
{
    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == cfd)
    {
        cout<<"error: create socket fail"<<endl;
        return;
    }

    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8000);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int addrLen = sizeof(serverAddr);
    int flag = connect(cfd, (struct sockaddr *)&serverAddr, addrLen);
    if(-1 == flag)
    {
        cout<<"error: connect fail"<<endl;
        return;
    }

    cout<<"成功连接到一个服务器"<<endl;

    char buf[BUF_LEN] = {0};
    for(int i = 0; i < 5; ++i)
    {
        cout<<"第"<<i<<"次发送消息："<<msg<<endl;
        write(cfd, msg, strlen(msg));

        int ret = read(cfd, buf, sizeof(buf) / sizeof(buf[0]));
        cout<<"第"<<i<<"次收到服务器消息："<<buf<<endl;

        if(-1 == ret)
        {
            cout<<"error: read data fail"<<endl;
            break;
        }

        sleep(2);
    }

    close(cfd);
    return;
}

void InitEpollClient(char *msg)
{
    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == cfd)
    {
        cout<<"error: create socket fail"<<cfd<<endl;
        return;
    }

    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8000);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int epfd = epoll_create1(0);
    if(-1 == epfd)
    {
        cout<<"error: create epoll fail"<<endl;
        return;
    }

    struct epoll_event ev, arrEvent[MAX_EVENT];

    int addrLen = sizeof(serverAddr);
    int flag = connect(cfd, (struct sockaddr *)&serverAddr, addrLen);
    if(-1 == flag)
    {
        cout<<"error: connect fail"<<endl;
        return;
    }

    if(-1 == MakeSocketNonBlocking(flag))
    {
        cout<<"error: make socket non blocking fail"<<endl;
        return;
    }

    ev.data.fd = cfd;
    ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
    if(-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev))
    {
        cout<<"error: connect epoll ctl fail"<<endl;
        return;
    }

    cout<<"已连接服务器"<<endl;

    int sendNum = 0;
    int recvNum = 0;
    while(1)
    {
        int waitEventCount = epoll_wait(epfd, arrEvent, MAX_EVENT, -1);
        for(int i = 0; i < waitEventCount; ++i)
        {
            unsigned int events = arrEvent[i].events;
            if(events & EPOLLERR || events & EPOLLHUP)
            {
                cout<<"error: epoll has error"<<endl;
                close(arrEvent[i].data.fd);
                continue;
            }else if(events & EPOLLIN)
            {
                int done = 0;
                while(1)
                {
                    char buf[BUF_LEN] = {0};
                    int resultLen = read(arrEvent[i].data.fd, buf, sizeof(buf) / sizeof(buf[0]));

                    if(-1 == resultLen)
                    {
                        if(EAGAIN != errno)
                        {
                            cout<<"error: read data fail"<<endl;
                            done = 1;
                        }
                        break;
                    }else if(!resultLen)
                    {
                        done = 1;
                        break;
                    }

                    cout<<"第"<<recvNum<<"次收到服务器消息："<<buf<<endl;
                }

                if(done)
                {
                    cout<<"closed connection"<<endl;
                    close(arrEvent[i].data.fd);
                }
            }else if(events & EPOLLOUT)
            {
                while(1)
                {
                    cout<<"第"<<sendNum<<"次发送消息："<<msg<<endl;
                    write(cfd, msg, strlen(msg));

                    if(++sendNum > 5) break;
                }
            }else
            {
                cout<<"还有其他事件？事件："<<events<<endl;
                continue;
            }
        }
    }

    close(epfd);
}

int main(int argc, char *argv[])
{
    char opt = -1;
    while((opt = getopt(argc, argv, "C:c:s")) != -1)
    {
        switch(opt)
        {
            case 's':
                InitServer();
                break;
            case 'c':
                InitClient(optarg);
                break;
            case 'C':
                InitEpollClient(optarg);
                break;
            default:
                cout<<"错误的参数"<<endl;
                break;
        }
    }

    return 0;
}
