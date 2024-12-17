#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> // 소켓 생성 및 서버 설정
#include <pthread.h> // POSIX 쓰레드 라이브러리
#include <time.h> // 시간 라이브러리

#define PORT 8080 //정적으로 포트번호 정의
#define MAX_CLIENTS 10 
#define BUFFER_SIZE 1024
#define VERSION_FILE "버전관리.txt" //수정 내용 로그 파일

typedef struct {
    int socket; // 클라이언트 소켓
    char nickname[50]; // 클라이언트 별명
} Client;

// 클라이언트 처리 함수 선언
void *handle_client(void *arg); // 클라이언트 요청 처리
void send_version_to_client(int client_socket); // "버전관리.txt" 로그 전송
void get_current_time(char *buffer, size_t size); // 현재 시간 반환
void save_version_to_file(const char *nickname, const char *content); // 수정 내용 파일에 저장
void broadcast_update(const char *nickname, const char *time_str, int sender_socket); //업데이트 브로드캐스트

#endif // SERVER_H
