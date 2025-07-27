## 목차

* [개요](#개요)   
* [게임설명](#게임설명)  
* [팀원소개](#팀원소개)   
* [역할분담](#역할분담)   
* [플레이방식](#플레이방식)  
* [프로젝트 후기](#프로젝트-후기)  
* [개선점](#개선점)


---
## 개요 

-   프로젝트 이름: :video_game: **AmericanSniper**
-   프로젝트 지속기간: **2024.01-2024.02**
-   개발 엔진 및 언어: **ue5** / **C++ 95%**
-   멤버: :computer: **Programmer** ( **고범길**, **민기찬** )


---
## 게임설명 
**“적진에 잠입해서, 타겟을 암살하라!”**
-
<a href="https://youtu.be/upSMpmmItRo?feature=shared" target="blank"><img align="center" src="https://raw.githubusercontent.com/rahuldkjain/github-profile-readme-generator/master/src/images/icons/Social/youtube.svg" alt="AmericanSniper" height="30" width="40" /></a> [잠입 암살 요소를 담은 **TPS**(Third Person Shooter) Game  ](https://youtu.be/upSMpmmItRo?feature=shared)
![Group 23](https://github.com/PohangCandy/lfstest/assets/130345776/d9f571fd-56fa-4ab3-8c2a-fcd3a9d137c2)


---
## 플레이방식
적 진에서 정해진 타겟을 암살하는 게임


---
## 팀원소개 
고범길 | 민기찬
----|----|
![image](https://github.com/ImGunHyoeng/Ghospital_merge/assets/102517991/c0c9aa6a-d7ef-420c-a1a1-0481e615af4a)|![화면 캡처 2024-06-10 201748](https://github.com/PohangCandy/Slayer-Unity-/assets/130345776/143a15af-e71a-40de-8554-7971a73a6d2f)|
[@github](https://github.com/sethman7)|[@github](https://github.com/PohangCandy) |


---
## 역할분담 
![header](https://capsule-render.vercel.app/api?type=waving&text=고범길&color=755139FF&fontColor=F2EDD7FF&fontAlign=90&fontAlignY=40&fontSize=50)


##
![header](https://capsule-render.vercel.app/api?type=waving&text=민기찬&color=2BAE66FF&fontColor=FCF6F5FF&fontAlign=90&fontAlignY=40&fontSize=50)

#### 1. Figma, UMG
<a href="https://www.figma.com/design/96yMo6HbDTwvgvhsNtRZyD/Sniper-Elite?node-id=0-1&t=cPLs9rgKckOMBE9u-1" target="blank"><img align="center" src="https://i.namu.wiki/i/tyR9148Wphjb2F4cAstF0NdEfTnxF5gEmmMzzjPmNzF7u7gwmk2D3USUfjJ3JA-nrvkZQAynHevRmGyrm7ciU3rdiV-rxeS2CQk_15tnzhMfVScDbzl4aMQBerHC5vZPXCT_ihMWrHh7QVBbHk3LNQ.svg" alt="Figma" height="27" width="20" /></a> [Figma 프로토타입](https://www.figma.com/design/96yMo6HbDTwvgvhsNtRZyD/Sniper-Elite?node-id=0-1&t=cPLs9rgKckOMBE9u-1)  
![Group 24](https://github.com/PohangCandy/lfstest/assets/130345776/9eb35bae-527f-460e-868b-cf10f25a2fbe)

* **Figma** : 기존 TPS 게임 분석, UI 기획, **컴포넌트 기능**과 **프로토타입 기능** 활용 -> 기획 추가, 수정에 용이  

* **UMG** : **UserWidget 클래스**를 상속받은 WidgetBluePrint로 UI 배치 및 구현, 해당 UI를 UserWidget 생성시 GetWidgetfromName매서드로 찾아옴.  
플레이어의 데이터가 바뀔 때마다 수정되도록 **델리게이트**로 연결  
**시각적 효과**를 더하기위해 파티클과 미디어 플레이어도 적극 사용


#### 2. Player
![Group 25](https://github.com/PohangCandy/lfstest/assets/130345776/4cde8aa5-310e-4636-a4a3-f62347c64624)  

* **HFSM** : 적과의 상호작용, 많은 키 입력으로 상태 변화 -> 플레이어가 가지는 3인칭 시점과 1인칭 시점을 크게 분류하고 공통된 기능들이 **반복**되어서 HFSM으로 리팩토링. 조건문을 줄이기 위해 상태를 객체화시켜서 **StatePattern**으로 리팩토링 중.

각 상태별로 일어나는 변화, 동작을 **델리게이트**를 통해서 이벤트 처리함으로써 각 액터와 컴포넌트 간 개방성을 줄이고 **캡슐화**.
ex) 애니메이션 몽타주가 끝날 때 발생하는 AnimMontageEnded 델리게이트에 SetIdleState() 연결하므로써 Tick함수 사용을 줄이고 상태 전환 실현. 


![Group 26](https://github.com/PohangCandy/lfstest/assets/130345776/e9dd486d-17da-4954-85d9-4b801e0a77ae)  

* **Aiming View**, **UpperBody Rotation**  : 스코프 맺히는 사물의 상이 마우스 스크롤에 의해 확대되고, 축소될수 있도록 **PostProcessVolume**을 활용하여 Aiming View를 구현. 해상도가 떨어진다고 느껴  **Scene Capture Camera **사용 방법을 배워서 이를 구현.  
  저격 시 마우스를 회전하면 캐릭터의 허리 부위가 회전해서 상체만 돌아갈 수 있도록 구현.  

* **Head Shot**: TPS 게임에 흔히 있는 헤드샷을 구현. SkelatalMesh의 **Collision Channel**에 접근해서 트레이스 라인에 맞은 BoneName 정보 얻음. 


#### 2. Item
![Group 27](https://github.com/PohangCandy/lfstest/assets/130345776/f1c16be7-f9ea-4b8f-9f53-197764091649)  

플레이어에게 **Colider**를 두어 충돌한 아이템의 이펙트와 이름을 나타내는 함수를 실행시키도록 구현  
아이템 수집 시 UI에 반영

---
## 프로젝트 후기
#### 고범길

##
#### 민기찬



---
## 개선점
#### 고범길


##
#### 민기찬

