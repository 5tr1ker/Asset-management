# 📎 Asset-management

> 자산관리 프로그램 프로토타입

![image](https://user-images.githubusercontent.com/49367338/223434307-e614d5aa-1619-4334-83fa-9f8ab2910a3d.png)

<h3>◼ 주요 기능</h3>

> 프로젝트 주요 기능은 다음과 같습니다.

- 컴퓨터 OS 정보
- 소프트웨어 정보 리스트
- 서버에 소프트웨어 및 컴퓨터 OS 정보 전송

# Project Structure

> C++ Winform 을 활용한 프로젝트 입니다.
- WinForm
- curl
- json

<h2>1 . 하드웨어 / 소프트웨어 정보 전송 </h2>

> 해당 시스템의 UUID를 작성 후 전송을 하면 서버에 전송합니다.

![image](https://user-images.githubusercontent.com/49367338/223435867-8208ffe7-ad0f-49dd-a351-a4df9904dd23.png)

- curl 라이브러리를 활용하여 PHP 서버에게 Json 형식의 데이터를 전송합니다.

<h2>2 . 시스템 정보 </h2>

> PHP로 요청을 받을 경우 DB 에 저장합니다.

![image](https://user-images.githubusercontent.com/49367338/223436355-92b119c4-dfe9-4e97-87ab-b611e1d7cb9f.png)

- 레파지토리에 있는 PHP는 DB서버에 저장하는 역할을 하며 화면 인터페이스는 이 프로젝트와 무관합니다. 
