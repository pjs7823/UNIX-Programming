# UNIX-Programming
유닉스 프로그래밍 기말 프로젝트

주제 : 프로젝트 협업 과정에서의 프로젝트 버전관리 문서 작성을 유닉스의 IPC 통신을 이용하여 구현하였다. 클라이언트는 협업 프로젝트 진행중에 수정 내용이 있으면 서버에게 간단한 수정 내용 메시지를 보낼 수 있다. 서버는 특정 클라이언트로부터 받은 수정 내용을 버전 관리 문서에 저장하고 다른 클라이언트에게 새로운 수정 내용이 있음을 알린다. 수정 내용을 사용자의 이름과 수정 내용 받은 시간을 함께 파일로 저장하여 남길 수 있고 클라이언트는 필요에 따라 전체 수정 내용 로그를 받아 볼 수 있다.

시스템 설계도:
![image](https://github.com/user-attachments/assets/644c3cd3-1af7-4bdc-83ae-185fd4a97263)




사용 예 캡쳐 및 사용 메뉴얼
![image](https://github.com/user-attachments/assets/53fa9685-8b59-4e4d-9d61-1e752bd815f9)
/src 폴더 내에서 make를 사용하여 컴파일 후 ./server로 서버 실행

![image](https://github.com/user-attachments/assets/bde843fc-3ef5-49e3-a565-c913099ea665)
같은 위치에서 다른 터미널 2개를 더 사용하여 ./client로 클라이언트 실행 후 사용자 별명 입력

![image](https://github.com/user-attachments/assets/9cb7d7de-6f67-4c16-a839-1ddca078018a)
서버는 수정 내용을 입력 받으면 다른 클라이언트에게 수정 내용이 있음을 브로드캐스트

![image](https://github.com/user-attachments/assets/09c6bf9f-8541-4341-a422-952ee13cd807)
![image](https://github.com/user-attachments/assets/a39ef47b-9f48-4426-9d68-843f08ccc687)
‘REQUEST_LOGS’ 명령어를 입력하면 전체 로그 확인 가능

 

![image](https://github.com/user-attachments/assets/a8b707e4-cf4b-4a7a-837c-ce3725d0cce4)
‘버전관리.txt’ 파일에 클라이언트 요청이 저장되고 있음을 확인

사용된 기술

1.	<stdio.h>
 o	입출력 및 파일 처리를 위한 함수 제공.
 o	주요 함수: printf(), fopen(), fclose(), fprintf(), fgets().
2.	<stdlib.h>
 o	메모리 관리 및 프로그램 종료 관련 함수 제공.
 o	주요 함수: malloc(), free(), exit().
3.	<string.h>
 o	문자열 처리 및 비교.
 o	주요 함수: strlen(), strncpy(), strncmp(), strcspn().
4.	<unistd.h>
 o	파일 디스크립터 및 시스템 호출 제공.
 o	주요 함수: close(), read(), write().
5.	<arpa/inet.h>
 o	네트워크 주소 변환 및 소켓 통신 지원.
 o	주요 함수: inet_pton(), htonl(), htons().
6.	<pthread.h>
 o	멀티스레드 프로그래밍.
 o	주요 함수: pthread_create(), pthread_detach(), pthread_mutex_lock().
7.	<time.h>
 o	시간 처리 및 포맷팅.
 o	주요 함수: time(), localtime(), strftime().


-참고 자료
https://andjjip.tistory.com/291
[Socket 프로그래밍] 18. 멀티쓰레드 기반의 서버구현 - 1
