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
<a href="https://youtu.be/AtsgU5bLuS8?feature=shared" target="blank"><img align="center" src="https://raw.githubusercontent.com/rahuldkjain/github-profile-readme-generator/master/src/images/icons/Social/youtube.svg" alt="AmericanSniper" height="30" width="40" /></a> [잠입 암살 요소를 담은 **TPS**(Third Person Shooter) Game  ](https://youtu.be/AtsgU5bLuS8?feature=shared)
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

* **Figma** : 피그마를 통해 기존의 CCG게임  **UI 분석** 후 프로젝트 UI 기획, **컴포넌트 기능**을 활용해 자주 쓰는 UI의 **재사용성** 높임. **ProtoType**을 수행하며 클래스 다이어그래임으로 생각해내지 못한 기획을 자세하게 수정.

* **UGUI** : 피그마에서 작성한 기획 내용을 바탕으로 구현. 배경, 오브젝트 배치 시 **Layer**를 확실하게 분리해주기 위한 카메라 설정 작업 수행

#### 2. Enemy
![Group 22](https://github.com/PohangCandy/Slayer-Unity-/assets/130345776/a12aaddc-f7e4-422f-90e9-b87b0d22adde)

* **턴 계산** : 플레이어 턴과 Enemy 턴에 따라 각 버프/디버프의 남은 **지속 시간**을 업데이트
**성능적 효율**을 위해 Update문을 쓰지 않고 턴 업데이트, 상태 전환 시 해당 지속 효과 업데이트하도록 리팩토링.

* **행동 패턴** : **FSM**을 사용해서 공격, 방어, 버프, 디버프 4가지 상태 구현. 각 상태가 끝날때마다 머리 위로 **다음 상태 예고**하도록 업데이트.

* **버프/디버프** : **Static 클래스**를 개별적으로 만들어 플레이어가 카드, 포션, 유물 등을 사용해서 Enemy 객체에 각기 다른 동작을 취할 수 있도록 만듬.


---
## 프로젝트 후기
#### 고범길

##
#### 민기찬
갑작스러운 일정이었음에도 끝까지 열심히 참여해준 팀원 분들께 너무 감사하고, 지난번 프로젝트때 같이 작업 했던 분들과 함께해서 빠른 시간안에 좋은 결과물이 나온 것 같아 좋았습니다.

지난번 작품과 달리 **성능 향상**과 **최적화**에 신경을 쓰기위해서, **Update문**을 사용하지 않고 적의 **FSM**을 구현함으로써 현재 행동과 다음 행동 예고에 대한 로직을 세워보았습니다. 특히 적이 시작한 버프의 경우 다시 적의 턴이 되었을 때 1턴의 효과를 지나고, 플레이어가 건 디버프의 경우 다시 플레이어의 턴이 되었을 때 1턴이 지나는 기능을 매프레임 단위로 적의 버프, 디버프를 체크하지 않고 구현하는 작업이 상당히 까다로웠으나 턴제 게임이기에 가능한 턴 종료 버튼을 누렀을 때의 확실한 턴 전환이 있닫는 점을 생각해서 **턴 계산 방법**을 생각해낸 것이 개인적으로 뿌듯했습니다.


---
## 개선점
#### 고범길


##
#### 민기찬
Map을 선택했을때, 경로에 따라 갈 수 있는 다음 구간이 나눠지도록 하는 작업을 **배열**과 **상속**을 통해서 리팩토링 할 예정입니다.  
시간상 아직 구현하지 못한 **상점**과 **유물**을 구현해볼 생각입니다.
