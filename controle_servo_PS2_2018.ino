#include <PS2X_lib.h>      // Bibioteca para receber controles do Joystick PS2
#include <VarSpeedServo.h> // Biblioteca para mover o servo motor com controle de velocidade subistitui o "#include <Servo.h>" Fonte: https://github.com/filipeflop/VarSpeedServo & mais atualizado em: https://github.com/netlabtoolkit/VarSpeedServo
PS2X ps2x;                 // cria a classe para operar o controle segundo a Biblioteca
VarSpeedServo Garra;       // cria a classe para operar o servo da Garra
VarSpeedServo Altura;      // cria a classe para operar o servo da Altura
VarSpeedServo Base;        // cria a classe para operar o servo da Base
VarSpeedServo Braco;       // cria a classe para operar o servo do Braço

static int vgarra=60,vbraco=60,vbase=60,valtura=60; // Cria as variáveis estáticas para receber a posição do analógico do controle
static int pgarra=50,paltura=1,pbase=82,pbraco=95; // Cria as variáveis estáticas para informar a posição para os Servos 

//////////////////////////////////////////////////////////////////////////////////////
// OBS: Variáveis estáticas não são reiniciadas no final do loop                    //
//      O normal é sempre que o loop reinicia as variáveis são reiniciadas          //
//  Sem isso o braço não ficaria parado em um mesmo lugar,Dependendo diretamente    //
//  do valor lido no momento pelo direcional analógico                              //
//////////////////////////////////////////////////////////////////////////////////////

static bool tipo=0;
int tmp = 50;
int vel=20;
int error = 0;
int cont =0;
byte type = 0;
byte vibrate = 0;

////////////////////////////////////////////////
////////////configurações//////////////////////
//////////////////////////////////////////////
void setup() {
  
  Braco.attach(9); 	//Inicializa o Servo Braco no pino 9 do arduino
  Garra.attach(6); 	//Inicializa o Servo Garra no pino 6 do arduino
  Altura.attach(3);	//Inicializa o Servo Altura no pino 3 do arduino
  Base.attach(5);	//Inicializa o Servo Base no pino 5 do arduino

  Garra.slowmove(pgarra,vel);
  Altura.slowmove(paltura,vel);
  Base.slowmove(pbase,vel);
  Braco.slowmove(pbraco,vel);
  
  error = ps2x.config_gamepad(13, 11, 10, 12, false, false); //setup pins and settings: GamePad(clock,command, attention, data, Pressures?,Rumble?) check for error
  type = ps2x.readType();
  Serial.begin(57600);
}
////////////////////////////////////////////////
/////////FIM das configurações//////////////////
////////////////////////////////////////////////





//////////////////////////////////////////////
/////////////////PRINCIPAL////////////////////
//////////////////////////////////////////////
void loop() {

  if (error == 1) //skip loop if no controller found
  {
    Serial.println("No Controller Found,check wiring");
    return;
  }
  ps2x.read_gamepad(); //read controller
/////
if(ps2x.ButtonReleased(PSB_BLUE))  
  {
    Serial.println("BOTÃO X AZUL PRESSIONADO");
    Garra.slowmove(cont,45);
    delay(tmp);
  }
//////
  if(ps2x.ButtonReleased(PSB_GREEN))  
  {
    Serial.println("BOTÃO /\ VERDE PRESSIONADO");
    Garra.slowmove(cont,70);
    delay(tmp);
  }
//////  
  if(ps2x.ButtonPressed(PSB_RED))
  {
    Serial.println("BOTÃO O VERMELHO PRESSIONADO"); 
    sequenciaB();
  }
////
  if(ps2x.ButtonReleased(PSB_PINK))  
  {
    Serial.println("BOTÃO [] SAOPAULINO PRESSIONADO");
    sequenciaA();
  }

 if(ps2x.Button(PSB_START))
 {         
    Serial.println("Start PRESSIONADO");
    vgarra=67;vbraco=60;vbase=90;valtura=90;
    pgarra=70;paltura=20;pbase=80;pbraco=90;

	Garra.slowmove(pgarra,vel);
	Altura.slowmove(paltura,vel);
	Base.slowmove(pbase,vel);
	Braco.slowmove(pbraco,vel);
 } 
    if(ps2x.Button(PSB_SELECT))
    {
		Serial.println("Select PRESSIONADO");
		Serial.println(tipo);
		tipo++;
		printposicao();
    }      

    if(ps2x.Button(PSB_PAD_UP)) 
	{
      Serial.print("Up held this hard: ");
	  pbraco=pbraco+1;
    }
	if(ps2x.Button(PSB_PAD_DOWN))
	{
      Serial.print("DOWN held this hard: ");
	  pbraco=pbraco-1;
    } 
    if(ps2x.Button(PSB_PAD_RIGHT))
	{
      Serial.print("Right held this hard: ");
	  pbase=pbase-1;
    }
    if(ps2x.Button(PSB_PAD_LEFT))
	{
      Serial.print("LEFT held this hard: ");
	  pbase=pbase+1;
    }
    if(ps2x.Button(PSB_L3))
    {  
		Serial.println("L3 pressed");
		Oia();
	}
	if(ps2x.Button(PSB_R3))
    {  	
		Serial.println("R3 pressed");
		Pinca();
    }
	if(ps2x.Button(PSB_L2))
    {
		Serial.println("L2 pressed");
		Nega();
	}
    if(ps2x.Button(PSB_R2))
    {
      Serial.println("R2 pressed");
	  paltura=paltura-1;
    }
    if(ps2x.Button(PSB_L1))
    {
		Serial.println("L1 pressed");
		Confirma();
	}
	if(ps2x.Button(PSB_R1))
    {
      Serial.println("R1 pressed"); 
	  paltura=paltura+1;
    }
//// 
vgarra=map(ps2x.Analog(PSS_RX), 0, 255, 0, 255);
valtura=map(ps2x.Analog(PSS_RY), 0, 255, 0, 255);
vbase=map(ps2x.Analog(PSS_LX), 0, 255, 0, 255);
vbraco=map(ps2x.Analog(PSS_LY), 0, 255, 0, 255);
/////

if (vgarra>190)
  pgarra=pgarra+1;
if (vgarra<170)
  pgarra=pgarra-1;
if (valtura>190)
  paltura=paltura+1;
if (valtura<170)
  paltura=paltura-1;
if (vbase>190)
  pbase=pbase-1;
if (vbase<170)
  pbase=pbase+1;
if (vbraco>190)
  pbraco=pbraco+1;
if (vbraco<170)
  pbraco=pbraco-1;  

	delay(tmp);

//clausula de segurança para evitar ficar travando//
//valor positivo
if (pgarra>89){
   pgarra=89;
   delay(tmp);}
if (paltura>89){
   paltura=89;
   delay(tmp);}
if (pbase>189){
   pgarra=189;
   delay(tmp);}
if (pbraco>120){
   pbraco=120;
   delay(tmp);} 
///valor negativo
if (pgarra<45){
   pgarra=45;
   delay(tmp);}
if (paltura<1){
   paltura=1;
   delay(tmp);}
if (pbase<1){
   pgarra=1;
   delay(tmp);}
if (pbraco<1){
   pbraco=1;
   delay(tmp);} 

/////fim da clausula

     

 Garra.slowmove(pgarra,20);
 Altura.slowmove(paltura,20);
 Base.slowmove(pbase,20);
 Braco.slowmove(pbraco,20);

}
////////////////////////////////////////////////////
///////////FUNCOES//////////////////////////////////
////////////////////////////////////////////////////



///////////////////////////////////////

void  retorno() // Retorno para o ponto de repouso partindo do ponto A
{
  Base.slowmove(90,vel);
  Altura.slowmove(90,vel);
  Braco.slowmove(90,vel);
}

/////////////////////////////////


//////////////////////////////////

void Oia()
{
      Braco.slowmove(89,200);
      delay(200);
      Braco.slowmove(110,200);
      delay(200);
}


///////////////////////////////////////

void Nega()
{
      Base.slowmove(70,200);
      delay(200);
      Garra.slowmove(110,200);
      delay(200);
}


/////////////////////////////////

void Confirma()
{
      Altura.slowmove(20,200);
      delay(200);
      Altura.slowmove(40,200);
      delay(200);
}

//////////////////////////////////

void Pinca()
{
      Garra.slowmove(50,200);
      delay(200);
      Garra.slowmove(70,200);
      delay(200);
}

void sequenciaB()
{
  tmp=800;
  //pegaB(); 
  Garra.slowmove(70,vel);
  delay(tmp);
  Braco.slowmove(90,vel);
  delay(tmp);
  Base.slowmove(39,vel);
  delay(tmp);
  Altura.slowmove(90,vel);
  delay(tmp);
  Braco.slowmove(55,vel);
  delay(tmp);
  Garra.slowmove(45,vel);
  delay(2000);
  
  retorno();
  delay(2000);
  
  //soltaA();
  Base.slowmove(108,vel);
  delay(tmp);
  Altura.slowmove(50,vel);
  delay(tmp);
  Braco.slowmove(35,vel);
  delay(tmp);
  Garra.slowmove(90,vel);
  delay(2000);

  retorno();
  delay(2000);
}

void sequenciaA()
{
 tmp=800;
  //pegaA();
  Garra.slowmove(70,vel);  
  delay(tmp);
  Base.slowmove(108,vel);
  delay(tmp);
  Altura.slowmove(52,vel);
  delay(tmp);
  Braco.slowmove(38,vel);
  delay(tmp);
  Garra.slowmove(45,vel);  
  delay(2000);
 
  retorno();
  delay(2000);
  
  //soltaB();
  Base.slowmove(39,vel);
  delay(tmp);
  Altura.slowmove(90,vel);
  delay(tmp);
  Braco.slowmove(55,vel);
  delay(tmp);
  Garra.slowmove(90,vel);
  delay(2000);

  retorno();
   
  delay(2000);  
  }


void Manual()
{
  
Serial.println("Movimentação manual em andamento");
}

void printposicao()
{
	Serial.println("pgarra");
	Serial.println(pgarra);
	Serial.println("paltura");
	Serial.println(paltura);
	Serial.println("pbase");
	Serial.println(pbase);
	Serial.println("pbraco");
	Serial.println(pbraco);
}
//////////////////////////////////////////
//////////FIM DAS FUNCOES ///////////////
/////////////////////////////////////////

  
  

