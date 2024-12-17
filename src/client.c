#include "include/client.h"

// 서버에 연결하는 함수
int connect_to_server() {
    int client_socket;
    struct sockaddr_in server_addr;

    // 소켓 생성
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // 서버 주소 설정
    server_addr.sin_family = AF_INET;  
    server_addr.sin_port = htons(PORT); // 포트번호 설정
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) { // 서버 IP 설정
        perror("Invalid address");
        exit(1);
    }

    // 서버 연결 시도
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    return client_socket; //연결된 소켓 반환
}

int main() {
    int client_socket;
    char buffer[BUFFER_SIZE];
    pthread_t recv_thread;

    // 서버에 연결
    client_socket = connect_to_server();

    // 사용자 별명 입력 및 서버에 전송
    printf("Enter your nickname: ");
    fgets(buffer, sizeof(buffer), stdin);
    send(client_socket, buffer, strlen(buffer), 0);

    // 명령어 안내
    printf("\n[INFO] 전체 로그 보기 'REQUEST_LOGS'.\n");
    printf("수정 내용: ");
    fflush(stdout);

    // 수신 스레드 시작
    pthread_create(&recv_thread, NULL, receive_messages, (void *)&client_socket);

    // 사용자의 '수정 내용' 입력 처리
    while (1) {
        fgets(buffer, sizeof(buffer), stdin);
        send(client_socket, buffer, strlen(buffer), 0);
        printf("수정 내용: ");
        fflush(stdout);
    }

    close(client_socket);
    return 0;
}

// 서버로부터 수신한 메시지 처리
void *receive_messages(void *socket) {
    int client_socket = *(int *)socket;
    char buffer[BUFFER_SIZE];
    while (recv(client_socket, buffer, BUFFER_SIZE, 0) > 0) {
        // 다른 유저가 입력한 수정 내용 메시지
        printf("\n%s", buffer);
        fflush(stdout);
        printf("수정 내용: ");
        fflush(stdout);
    }
    // 연결 종료 메시지
    printf("Disconnected from server.\n");
    exit(0);
}
