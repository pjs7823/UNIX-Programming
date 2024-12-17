# 컴파일러 설정
CC = gcc

# 컴파일러 플래그
CFLAGS = -Wall -Wextra -I./include

# 링크 플래그 (예: pthread)
LDFLAGS = -pthread

# 출력 바이너리
SERVER = server
CLIENT = client

# 소스 파일
SERVER_SRCS = server.c
CLIENT_SRCS = client.c

# 오브젝트 파일
SERVER_OBJS = $(SERVER_SRCS:.c=.o)
CLIENT_OBJS = $(CLIENT_SRCS:.c=.o)

# 기본 타겟
all: $(SERVER) $(CLIENT)

# 서버 바이너리 생성
$(SERVER): $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# 클라이언트 바이너리 생성
$(CLIENT): $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# .o 파일 생성 규칙
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# 클린업
clean:
	rm -f $(SERVER) $(CLIENT) *.o

# 디버깅용
debug:
	$(CC) $(CFLAGS) -g -o $(SERVER) $(SERVER_SRCS) $(LDFLAGS)
	$(CC) $(CFLAGS) -g -o $(CLIENT) $(CLIENT_SRCS) $(LDFLAGS)

