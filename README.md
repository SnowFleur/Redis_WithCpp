# Redis_WithCpp

### 개요 
Hiredis를 C++ 클래스로 Wrapping  하는 프로젝트

### 목적
Redis R&D 및 C++ 개인 서버 엔진 DB 파트에 RDBMS 외에 NOSQL도 지원할 수 있도록 만들기 위해 제작 </br>
클라이언트는 윈도우 환경이며 RedisServer는 공식적으로 Windows를 지원하지 않기 때문에 Linux를 설치해서 진행


### 환경
|App|OS|설명|
|------|---|---|
|RedisServer|Linux ubuntu| wsl2 사용|
|RedisClient|Windows 10|Hiredis 사용|
|RedisStressTool|Windows 10| 제작한 Lib 이용해서 할까말까 고민..|

### 사용 개발 툴
VisualStudio 2022(C++17)

### 목표
1. 많이 사용하는 자료구조 CRUD 제작(String, Set, ZSet, List)
2. RedisServer에서 잘 작동하는지 체크
3. 클래스 세분화 및 확장성 고려해서 작성
4. 더미 클라이언트를 만들어서 테스트(고민중)
5. 개인 서버 엔진에 기능 합치기


### 완료 예상 날짜
2023년 7월~8월 사이


### UML
![UML](./UML.PNG)



### 클래스 
|클래스|설명|
|------|---|
|DBConnector|모든 DB커넥터가 상속받는 베이스 클래스|
|RedisConnector|Redis와 커넥션 및 쿼리를 실행하는 Redis 클래스|
|RedisQuery |  RedisQuery를 담당하는 베이스 클래스|
|RedisResult| Redis Query 실행 결과를 가지고 있는 클래스|
|RedisSet, RedisString, RedisZset| RedisQuery를 상속받으면서 각 각 redis의 자료구조의 CRUD를 지원하는 클래스|



### Reference
- hiredis 사용법(한글)</br>
http://redisgate.kr/redis/clients/hiredis_intro.php    

- redisServer 설치법(영어)</br>
https://redis.io/docs/getting-started/installation/

- C++ Windows에서 Redis 사용하기(한글)</br>
https://jacking75.github.io/cpp_use_simple_win32_hiredis/  </br>
