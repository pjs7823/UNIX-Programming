#ifndef CLIENT_H
#define CLIENT_H

//필요한 헤더 파일 선언
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> // 소켓 생성 및 서버 연결
#include <pthread.h> // POSIX 쓰레드 라이브러리

// 정적으로 포트 번호 정의
#define PORT 8080
#define BUFFER_SIZE 1024

// 함수 선언
void *receive_messages(void *socket); // 서버로부터 메시지 수신 함수
int connect_to_server(); // 서버 연결 함수

#endif 
