#include "include/server.h"


Client clients[MAX_CLIENTS]; // 연결된 클라이언트 배열
int client_count = 0; // 현재 연결된 클라이언트 수
pthread_mutex_t client_mutex; // 뮤택스 초기화

int main() {
    int server_socket;
    struct sockaddr_in server_addr;
    int opt = 1;  // SO_REUSEADDR 옵션 활성화

    pthread_mutex_init(&client_mutex, NULL); // 뮤텍스 초기화

    // 소켓 생성(AF_INET-SOCK_STREAM)
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // SO_REUSEADDR 소켓 옵션 설정
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // 서버 주소 설정
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; //모든 IP에서 연결 허용
    server_addr.sin_port = htons(PORT); //정적 포트 번호 사용

    // 소켓 바인딩
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // 클라이언트 연결 대기
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // 클라이언트 연결 수락 루프
    while (1) {
        int client_socket; 
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);

        // 클라이언트 연결 수락
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len)) < 0) {
            perror("Accept failed");
            continue;
        }

        // 새 클라이언트를 처리하기 위한 쓰레드 생성 및 클라이언트 소켓 동적 할당
        int *new_sock = malloc(sizeof(int));
        *new_sock = client_socket;

        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, new_sock); //클라이언트 처리 함수 호출
        pthread_detach(thread); // 쓰레드 분리
    }

    // 서버 종료 시 자원 해제
    close(server_socket);
    pthread_mutex_destroy(&client_mutex);
    return 0;
}

// 클라이언트 요청 처리 함수
void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    free(arg); // 동적 할당 해제
    char buffer[BUFFER_SIZE];
    char nickname[50] = {0};

    // 별명 입력받기
    recv(client_socket, nickname, sizeof(nickname), 0);
    nickname[strcspn(nickname, "\n")] = 0; // 개행 문자 제거
    printf("New client connected: %s\n", nickname);

    // 클라이언트 등록
    pthread_mutex_lock(&client_mutex);
    clients[client_count].socket = client_socket;
    strncpy(clients[client_count].nickname, nickname, sizeof(clients[client_count].nickname));
    client_count++;
    pthread_mutex_unlock(&client_mutex);

    // 클라이언트 요청 처리 루프
    while (1) {
        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            break; // 클라이언트 연결 종료
        }

        buffer[bytes_received] = '\0'; // 문자열 끝 처리
	// 클라이언트 '버전관리.txt' 로그 요청 처리 함수 호출
        if (strncmp(buffer, "REQUEST_LOGS", 12) == 0) {
            send_version_to_client(client_socket);
        } else {
        // '수정 내용' 저장 및 브로드캐스트
            char time_str[100];
            get_current_time(time_str, sizeof(time_str));

            save_version_to_file(nickname, buffer);
            broadcast_update(nickname, time_str, client_socket);
        }
    }

    // 클라이언트 연결 종료
    close(client_socket);
    pthread_mutex_lock(&client_mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i].socket == client_socket) {
            clients[i] = clients[client_count - 1];
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&client_mutex);

    printf("Client disconnected: %s\n", nickname);
    return NULL;
}

// 버전관리.txt 전체 로그 클라이언트로 전송
void send_version_to_client(int client_socket) {
    pthread_mutex_lock(&client_mutex);

    FILE *file = fopen(VERSION_FILE, "r");
    if (file) {
        char line[BUFFER_SIZE];
        while (fgets(line, sizeof(line), file)) {
            send(client_socket, line, strlen(line), 0);
        }
        fclose(file);
    } else {
        const char *error_message = "[ERROR] 로그 파일을 찾을 수 없습니다.\n";
        send(client_socket, error_message, strlen(error_message), 0);
    }

    pthread_mutex_unlock(&client_mutex);
}

// 현재 시간 형식 얻기
void get_current_time(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", t);
}

// JSON 형식으로 수정 내용을 파일에 저장
void save_version_to_file(const char *nickname, const char *content) {
    pthread_mutex_lock(&client_mutex);

    char content_no_newline[BUFFER_SIZE];
    strncpy(content_no_newline, content, sizeof(content_no_newline));
    content_no_newline[strcspn(content_no_newline, "\n")] = 0;  // 개행 문자 제거

    FILE *file = fopen(VERSION_FILE, "a");
    if (file) {
        char time_str[100];
        get_current_time(time_str, sizeof(time_str));

        fprintf(file, "{\n"
                "  \"수정 내용\": \"%s\",\n"
                "  \"수정 시간\": \"%s\",\n"
                "  \"사용자\": \"%s\"\n"
                "}\n", 
                content_no_newline, time_str, nickname);
        fclose(file);
    }

    pthread_mutex_unlock(&client_mutex);
}

// 다른 클라이언트에 브로드캐스트
void broadcast_update(const char *nickname, const char *time_str, int sender_socket) {
    pthread_mutex_lock(&client_mutex);
    char message[BUFFER_SIZE];
    snprintf(message, sizeof(message), "[알림] %s님이 %s에 수정했습니다.\n", nickname, time_str);

    for (int i = 0; i < client_count; i++) {
        if (clients[i].socket != sender_socket) {
            send(clients[i].socket, message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&client_mutex);
}
