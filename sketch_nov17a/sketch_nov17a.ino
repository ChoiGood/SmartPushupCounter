#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

SoftwareSerial mySerial(5,7);

#define BUTTON1 2
#define ECHO 8
#define TRIGGER 9
#define BUTTON2 3
int buzzerPin = 1;

LiquidCrystal lcd(4,6,10,11,12,13);

#include <MsTimer2.h>
int mode = 0;
int day = 1;
int sec = 0;
void time()
{
    sec++;
}

int dataset[2][10]={{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0}};

int count = 0;
float kcal = 0;
int set=1;
int status = 0;
int count_btn1 = 0;
int count_btn2 = 0;
int d1=0;
int d2=0;
int goals=0;
int resttime=0;
int sum = 0;
boolean down = false;
boolean up = false;

// 사공탁
void sendData() 
{
      Serial.println(day);
      Serial.println(sum);
      mySerial.write(day);
      mySerial.write(sum);
      
      day++;
      sum = 0;
      mode = 0;
      
      lcd.clear();
}

// 최지원
void training(int btn1,int btn2) 
{
  int temp;
    //어플로부터 데이터를 받음(목표개수와 쉬는시간)
    if(mySerial.available()){
      String strData = mySerial.readStringUntil('\n');
      goals=atoi(strData.c_str());
      Serial.println(goals);
    }

    if(mySerial.available()){
      String strData = mySerial.readStringUntil('\n');
      resttime=atoi(strData.c_str());
      Serial.println(resttime);
    }

    // 시작 및 종료 버튼 : btn2, 세트 수 증가 버튼 : btn1
    
    if (btn2==0){
      count_btn2++;  // 시작 및 종료 버튼
    }

    if(count_btn2==1) // 시작 버튼 클릭 (btn2 첫번째 누른 상태)
    {
      if(btn1) // btn1 안누른 상태(시작 버튼 누르고 아무것도 안누른 상태입니다) -> 푸쉬업 수행
      {
        Pushup(15,20); // 푸쉬업 수행
        print(set);    // lcd 에 출력
        
        // 소리부저 기능 (목표개수 넘기면 소리출력)
        if(count>goals){ 
          Serial.println("gg"); //Serial 출력이 있어야 소리가 나와서 넣었습니다.
          tone(buzzerPin, 200);
        }
        else
        {
          noTone(buzzerPin);
        }
      }
      else // 세트 수 증가 버튼 클릭 (btn1을 눌렀을 때 입니다)
      {
        count_btn1++;
        if(count_btn1==1) // (btn1을 한번 누른 상태) -> 입력받은 쉬는시간 만큼 쉬어준다
        {
          noTone(buzzerPin);
          //데이터(수행한 세트,팔굽 수) 저장
          dataset[0][set]=set;
          dataset[1][set]=count;

          //lcd에 Rest 화면 출력
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Rest for NextSet");
          lcd.setCursor(0,1);
          lcd.print("Record : ");
          lcd.setCursor(9,1);
          lcd.print(count);
          delay(resttime*1000); //입력받은 시간만큼 쉬어준다
          lcd.clear();
          Serial.println("ffff");
          tone(buzzerPin,200); //쉬는 시간이 지나면 소리 출력됨
          
        }
        else if(count_btn1==2) // (btn1을 두 번째 누른 상태) -> 다음 세트로 넘어감
        {
          // 세트 수 증가, 데이터 초기화(count,count_btn1)
          set++;
          count=0;
          count_btn1=0;
        }
      }
    }
    else if(count_btn2==2) // (btn2를 두 번째 누른 상태) -> 종료(총 팔굽 수를 sum에 더한다)
    { 
      //데이터(수행한 세트,팔굽 수) 저장
      dataset[0][set]=set;
      dataset[1][set]=count;
      temp=set;
      
      Serial.println("set and count");

      //sum에 총 팔굽개수 더하기
      for(int i=0;i<=temp;i++)
      {
        sum+=dataset[1][i];
      }

      //데이터 초기화(mode,count,set,count_btn1,count_btn2)
      mode=0; //mode가 0이 되어 종료 -> loading 화면으로 넘어감
      count=0;
      set=1;
      count_btn1=0;
      count_btn2=0;

      // dataset 초기화 (personal이나 training 을 다시 수행하기위해 필요한 코드)
      for(int i=1;i<=temp;i++)
      {
        dataset[0][i]=0;
        dataset[1][i]=0;
      }

      lcd.clear();
    }
}

// 최지원
void personal(int btn1,int btn2) 
{
  int temp;
  // 어플로부터 데이터를 받음 (목표개수,거리 데이터(d2,d1))
  if(mySerial.available()){
      String strData = mySerial.readStringUntil('\n');
      goals=atoi(strData.c_str());
      Serial.println(goals);
    }
    if(mySerial.available()){
      String strData = mySerial.readStringUntil('\n');
      d2=atoi(strData.c_str());
      Serial.println(d2);
    }

    if(mySerial.available()){
      String strData = mySerial.readStringUntil('\n');
      d1=atoi(strData.c_str());
      Serial.println(d1);
    }

    // 시작 및 종료 버튼 : btn2, 세트 수 증가 버튼 : btn1
    
    if (btn2==0){
      count_btn2++;
    }

    if (count_btn2==1) // 시작 버튼 클릭 (btn2 첫번째 누른 상태)
    {
      if (btn1) // btn1 안누른 상태(시작 버튼 누르고 아무것도 안누른 상태입니다) -> 푸쉬업 수행
      {
        Pushup(d1,d2); // 푸쉬업 수행
        print(set);    // lcd에 세트, 팔굽 수 출력
        
        // 소리부저 기능 (목표개수 넘기면 소리출력)
        if(count>goals){ 
          Serial.println("gg");
          tone(buzzerPin, 200);
        }
        else
        {
          noTone(buzzerPin);
        }
      }
      else // 세트 수 증가 버튼 클릭 (btn1을 누른상태)
      {
        //데이터(수행한 세트,팔굽 수) 저장
        dataset[0][set]=set;
        dataset[1][set]=count;
        //세트 수 증가, 데이터 초기화(count)
        count=0;
        set++;
      }
    }
    else if(count_btn2==2) // (btn2를 두 번째 누른 상태) -> 종료(총 팔굽 수를 sum에 더한다)
    {
      //데이터(수행한 세트,팔굽 수) 저장
      dataset[0][set]=set;
      dataset[1][set]=count;
      temp = set;

      //sum 에 수행한 팔굽 개수 더하기
      for (int i=0;i<=temp;i++)
      {
          sum += dataset[1][i];
      }

      // dataset 초기화 (personal이나 training 을 다시 수행하기위해 필요한 코드)
      for(int i=1;i<=temp;i++)
      {
        dataset[0][i]=0;
        dataset[1][i]=0;
      }
      //데이터 초기화(mode,count,set,count_btn2)
      mode = 0; //mode가 0이 되어 종료 -> loading 화면으로 넘어감
      count=0;
      count_btn2 = 0;
      set=1;
      lcd.clear();
    }
}

// 윤준호
void TimeAttack(){  MsTimer2::start(); //이걸하면 타이머가 작동된다.
  int time;
  ///////////<<앱에서 제한 시간을 받아옴>>///////////////////
  if(mySerial.available()){ //앱에서 제한시간을 받아오는 부분.
      String strData = mySerial.readStringUntil('\n');
      time=atoi(strData.c_str());
      Serial.println(time);
    }
  int btn1, btn2;
  ///////////<<시작하기 전에 1초 대기하고 시작한다>>/////////
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print(" time attack");
  lcd.setCursor(0,1);
  lcd.print("   Ready...");
  delay(1000);
  lcd.clear();
  /////////////////////////////////////////////////////////

  sec=0;//mstimer에 의해서 sec가 0에서 부터 증가한다.
  while (sec < time){// 제한시간동안 팔굽혀 펴기를 한다.
  delay(100);// 이거 없으면 푸쉬업 여러개씩 세지는 듯

  ////////////////////<팔굽혀 펴기를 감지하고, 횟수와 남은 시간을 출력>////////////////////////
                 /////////[팔굽혀 펴기를 감지]///////////
  float distance = Distance(); 
  if (0 < distance && distance < 15)
  {
    down = true;
    up = false;
  }
  else if (20 < distance && distance < 50)
  {
    up = true;
  } 
  if (up == true && down == true)
  {
    count++;
    down = false;
    up = false;
    lcd.clear();
  }
                ////////////////////////////////////////////

                /////////[남은 시간과 횟수를 출력]///////////
  lcd.setCursor(1, 0);
  lcd.print("TIME : ");
  lcd.setCursor(8, 0);
  lcd.print(sec);
  lcd.setCursor(10, 0);
  lcd.print("/");
  lcd.setCursor(11, 0);
  lcd.print(time);

  lcd.setCursor(0,1);
  lcd.print("count : "); 
  lcd.setCursor(9,1);
  lcd.print(count);
                 ///////////////////////////////////////////

  //////////////////////<<도중에 버튼 2를 누르면 임의 종료>>/////////////////////////////  
  btn2=digitalRead(BUTTON2);//버튼 2번을 누르면 중간에 멈추고 종료됨.(버튼 눌림을 읽어들이는 부분)  
                      
  if (btn2==0){//중간에 멈추는 부분(중간에 멈추면 나중에 불러오기로 가져올 총 개수에 포함이 안된다.)
    lcd.clear();

    lcd.setCursor(1,0);
    lcd.print("exit...");
    mode=0;//mode 0은 메인 화면이다.
    count=0;
    delay(1500);
    lcd.clear();
    sec=0;                     MsTimer2::stop(); //이걸하면 타이머가 멈춘다
    return;
  }
  }
  /////////////////////////////////////////////////////////////////////////////////////

  /////////////////////<<제한시간이 끝난 후에 일어나는 일들>>///////////////////////////
  
  sum += count; //중간에 안멈추고 끝까지 하면 sum에 넣음(데이터 전송을 위해서)-나중에 불러오기로 가져올 개수에 포함이 되게 됨.                                 
 
  while(1){//while (True)   
    ///[종료 후에 나오는 화면이 있다(if문을 이용해서 1초에 하나씩 두 화면을 번갈아 보여줌)]///
    if(0==sec%2 ){
      Serial.println(sec%5);
      lcd.setCursor(1,0);
      lcd.print("TIME OVER       ");
    
      lcd.setCursor(0,1);
      lcd.print("count :  ");
      
      lcd.setCursor(9,1);
      lcd.print(count);
      lcd.setCursor(11,1);
      lcd.print("     ");
    }

    if (1==sec%2){                      
    lcd.setCursor(1,0);
    lcd.print("Save Record     ");
      
    lcd.setCursor(0,1);
    lcd.print("Push btn2       ");      
  }

  //[시간 초과 후에 버튼 2를 누르면 종료한다.]//
  btn2=digitalRead(BUTTON2);//다 끝나고 2번 버튼을 누르면 종료한다.
  if (btn2==0){
    
      count=0;kcal=0;
      //[종료 누르고 나면 나오는 화면]//
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("exit");
      
      for (int i=0; i<5 ;i++){
      lcd.setCursor(7+i,0);
      lcd.print(".");
      delay(500);
       }
      lcd.clear();
    
      sec=0;                       MsTimer2::stop(); //이걸하면 타이머가 작동을 멈춘다
      break;
  }  
  return;
}

// 최지원
void print(int n) //lcd에 지금까지 수행한 세트와 팔굽수 출력
{
  int m;
  if (n==1) //1 세트 일때
  {
  lcd.setCursor(0, 0);
  lcd.print(" Set :");
  lcd.print(set);

  lcd.setCursor(0, 1);
  lcd.print("Count:");
  lcd.setCursor(6,1);
  lcd.print("  ");
  
  lcd.setCursor(6,1);
  lcd.print(count);

  }
  else //세트 수 2이상
  {
    lcd.setCursor(0, 0);
    lcd.print(" Set :");
    lcd.print(set);

    lcd.setCursor(0, 1);
    lcd.print("Count:");
    
    //이전까지의 세트 수와 팔굽 수 출력 (dataset이용)
    for (int i=1;i<set;i++)
    {
      m=6+2*(i-1);
      lcd.setCursor(m,0);
      lcd.print(dataset[0][i]);
      lcd.setCursor(m,1);
      lcd.print(dataset[1][i]);
    }
    //현재 세트와 팔굽수 출력
    m=6+2*(n-1);
    lcd.setCursor(m,0);
    lcd.print(set);

    lcd.setCursor(m,1);
    lcd.print("  ");
    
    lcd.setCursor(m,1);
    lcd.print(count);
  }
}

// 사공탁
float Distance() 
{
  float duration, distance;
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);

  duration = pulseIn(ECHO,HIGH);
  distance = duration * 34000 / 1000000 / 2;
  
  return distance;
}

// 사공탁
void Pushup(int d1,int d2) 
{
  float distance = Distance();

  if (0 < distance && distance < d1)
  {
    down = true; 
    up = false;
  }
  else if (d2 < distance && distance < 50)
    up = true;
  
  if (up == true && down == true)
  {
    count++;
    kcal = kcal + 1.225;
    down = false;
    up = false;
  }
}

void setup()
{
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2,INPUT_PULLUP);
  lcd.begin(16, 2);
  MsTimer2::set(1000,time); // 1s timer
}

// 모두
void loop()
{
  int btn1 = digitalRead(BUTTON1);
  int btn2 = digitalRead(BUTTON2);
  

  if (mySerial.available()){
      mode = mySerial.read() - '0';
      Serial.println(mode); 
    }
  if (mode == 0)
  {
    lcd.setCursor(1, 0);
    lcd.print("loading...");
    delay(1500);
    lcd.clear();
  }
  else if (mode==1)
  {
      personal(btn1,btn2);
  } //personal mode
  else if(mode==2) //time attack mode
  {
    TimeAttack();
  }
  else if(mode==3)
  {
    training(btn1,btn2);
  }  //training mode
  else if (mode == 4)
  {
    sendData();
  }
  delay(150);
}