UNIX-Programming
유닉스 프로그래밍 기말 프로젝트
주제:
프로젝트 협업 과정에서의 프로젝트 버전 관리 문서 작성을 유닉스의 IPC 통신을 이용하여 구현하였다.

클라이언트는 협업 프로젝트 진행 중 수정 내용이 있으면 서버에게 간단한 수정 내용 메시지를 보낼 수 있다.
서버는 특정 클라이언트로부터 받은 수정 내용을 버전 관리 문서에 저장하고 다른 클라이언트에게 새로운 수정 내용이 있음을 알린다.
수정 내용을 사용자의 이름과 수정 내용 받은 시간을 함께 파일로 저장하여 남길 수 있다.
클라이언트는 필요에 따라 전체 수정 내용 로그를 받아 볼 수 있다.
시스템 설계도
시스템 설계도

사용 예 및 매뉴얼
/src 폴더 내에서 make 명령어를 사용하여 컴파일 후 서버 실행:

./server
서버 실행

같은 위치에서 다른 터미널 2개를 더 사용하여 클라이언트를 실행하고 사용자 별명을 입력:

./client
클라이언트 실행

서버는 수정 내용을 입력 받으면 다른 클라이언트에게 브로드캐스트:
브로드캐스트1
브로드캐스트2

REQUEST_LOGS 명령어를 입력하면 전체 로그 확인 가능:

REQUEST_LOGS
전체 로그 확인

버전관리.txt 파일에 클라이언트 요청이 저장됨을 확인:

버전관리.txt
사용된 기술
<stdio.h>

입출력 및 파일 처리를 위한 함수 제공.
주요 함수: printf(), fopen(), fclose(), fprintf(), fgets().
<stdlib.h>

메모리 관리 및 프로그램 종료 관련 함수 제공.
주요 함수: malloc(), free(), exit().
<string.h>

문자열 처리 및 비교.
주요 함수: strlen(), strncpy(), strncmp(), strcspn().
<unistd.h>

파일 디스크립터 및 시스템 호출 제공.
주요 함수: close(), read(), write().
<arpa/inet.h>

네트워크 주소 변환 및 소켓 통신 지원.
주요 함수: inet_pton(), htonl(), htons().
<pthread.h>

멀티스레드 프로그래밍.
주요 함수: pthread_create(), pthread_detach(), pthread_mutex_lock().
<time.h>

시간 처리 및 포맷팅.
주요 함수: time(), localtime(), strftime().
참고 자료
[Socket 프로그래밍] 멀티쓰레드 기반의 서버 구현:
https://andjjip.tistory.com/291
