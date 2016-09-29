/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "adc.h"
#include "i2c.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
//#include "ssd1306.h"
//#include "fonts.h"
#include "fsm.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
extern	SSD1306_t SSD1306;
volatile	uint32_t	key_scan,key_scan_1,key_counter;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void HAL_SYSTICK_Callback(void)
{
	key_counter++;
	key_scan_1 = (GPIOA->IDR & 0xF8);
	key_scan = (key_scan_1 >> 3);
	key_scan ^= 0x1F;
}
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */


//��� ����������
int elem_prog=1; // ����� �������� ��� ���������������� (����� 8 ��������� �� 1 �� 8)
int kol_elem_prog=0; // ���������� ����������� �������� ��� ���������������
int oborot = 0; // ����������, ������� ������� ��������� ��������
int next_oborot=1;//���������� ������� ���������� ����� ���������� �������
int a_i=0; //- ���������� ������� ��� �������� ����������� �������� �������
int mega = 0;// ����������, ������� ������� ���������� ����������(���� �������� - ��� ��������� ���������), ������� �������� ������, ��� 0 ���������� ������� ����
int prev_side = 0; // ����������, ������� ��������� ������ ����� ������� �������� ������� ���, ����� ��� ����, ����� �������� ������ ��� ������������ ������ ������ ������� � ����� �������
uint16_t prog_oborot = 123;//���������� ��� ���������������� ���������� �������� � ���������
uint16_t cv_1[20]={1,2,3,4,5,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//������ ��������� ������� �����
uint16_t cv_2[20]={2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//������ ��������� ������� �����
uint16_t cv_3[20]={3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//������ ��������� �������� �����
uint16_t cv_4[20]={4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//������ ��������� ���������� �����
uint16_t pered_val[20]={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//������ ��������� ��������� ������
uint16_t zad_val[20]={6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//������ ��������� ������� ������
uint16_t stop_mass[20]={7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//������ ���������



//��� ������� ��� ���������� ����������� (���������/���������� �����)
#define CV1_DEF_ON HAL_GPIO_WritePin(GPIOB, CV1_SOLENOID_Pin, GPIO_PIN_SET); // !!!����� �������� ���������� ���� � �����!!!
#define CV1_DEF_OFF HAL_GPIO_WritePin(GPIOB, CV1_SOLENOID_Pin, GPIO_PIN_RESET); // !!!����� �������� ���������� ���� � �����!!!

#define CV2_DEF_ON HAL_GPIO_WritePin(GPIOB, CV2_SOLENOID_Pin, GPIO_PIN_SET); // !!!����� �������� ���������� ���� � �����!!!
#define CV2_DEF_OFF HAL_GPIO_WritePin(GPIOB, CV2_SOLENOID_Pin, GPIO_PIN_RESET); // !!!����� �������� ���������� ���� � �����!!!

#define CV3_DEF_ON HAL_GPIO_WritePin(GPIOB, CV3_SOLENOID_Pin, GPIO_PIN_SET); // !!!����� �������� ���������� ���� � �����!!!
#define CV3_DEF_OFF HAL_GPIO_WritePin(GPIOB, CV3_SOLENOID_Pin, GPIO_PIN_RESET); // !!!����� �������� ���������� ���� � �����!!!

#define CV4_DEF_ON HAL_GPIO_WritePin(GPIOB, CV4_SOLENOID_Pin, GPIO_PIN_SET); // !!!����� �������� ���������� ���� � �����!!!
#define CV4_DEF_OFF HAL_GPIO_WritePin(GPIOB, CV4_SOLENOID_Pin, GPIO_PIN_RESET); // !!!����� �������� ���������� ���� � �����!!!

#define PERED_VAL_DEF_ON HAL_GPIO_WritePin(GPIOA, VAL_PERED_SOLENOID_Pin, GPIO_PIN_SET); // !!!����� �������� ���������� ���� � �����!!!
#define PERED_VAL_DEF_OFF HAL_GPIO_WritePin(GPIOA, VAL_PERED_SOLENOID_Pin, GPIO_PIN_RESET); // !!!����� �������� ���������� ���� � �����!!!

#define ZAD_VAL_DEF_ON HAL_GPIO_WritePin(GPIOA, VAL_ZAD_SOLENOID_Pin, GPIO_PIN_SET); // !!!����� �������� ���������� ���� � �����!!!
#define ZAD_VAL_DEF_OFF HAL_GPIO_WritePin(GPIOA, VAL_ZAD_SOLENOID_Pin, GPIO_PIN_RESET); // !!!����� �������� ���������� ���� � �����!!!


//��� ������� ���

void stop_fun(void){//������� ���������
	HAL_GPIO_WritePin(GPIOA,STOP_RELE_Pin, GPIO_PIN_SET);// ������� ��������� �� ��������� ���� "� ������" ����� ����
	HAL_Delay(200); //0,2 ������� ��� ������������ �����, ���� ����, �� ��������
	HAL_GPIO_WritePin(GPIOA,STOP_RELE_Pin, GPIO_PIN_RESET);//��������� ����
	//����� �������� ��� ������� �� �����
}

void left_side(void){//������� ������������ ������� ����� �������
	// � ����� ������� ������ ���������� � ����������� ��������� ���������� ����������
	if (prev_side<2){//���� ����� ���� �� ����������� ������ ������� (2) ��� ��� ������ �����(0), � ���������� ������� ���� ����� (1)
		return;		//�� ������� � ������ �� ������
	}
	prev_side=1;//��������, ��� ���������� ������� ���� �����
	if (oborot<prog_oborot){//���� ���������� �������� ������ ��� ������������������� ������ ���������
		next_oborot= oborot+1; //�� ��������� ������ ����� ������� ������+1
	}
	else {// ���� ������� ������ ����� ���������� ������������������� ��������
		next_oborot= 1; //�� ��������� ������ ����� 1
	}
	//***************** ������� ��������� ��� ��������� ���������
	CV1_DEF_OFF;// ������ ���������� ������� ��������
	CV2_DEF_OFF;// ������ ���������� ������� ��������
	CV3_DEF_OFF;// ������ ���������� �������� ��������
	CV4_DEF_OFF;// ������ ���������� ���������� ��������

	//������ ��������� ���� ��������� ������ (next_oborot) �������� ������������ ������, �� �������� �������� ���������������� �����
	a_i=0;//���������� ������� ��������� �������
	for (a_i=0; a_i<20; a_i++){
		if(cv_1[a_i]==next_oborot){//���� �������� �������� �������� ������� ����� ���������� �������
			CV1_DEF_ON;	//�� �������� ������ ����
		}
		if(cv_2[a_i]==next_oborot){//���� �������� �������� �������� ������� ����� ���������� �������
			CV2_DEF_ON;	//�� �������� ������ ����
		}
		if(cv_3[a_i]==next_oborot){//���� �������� �������� �������� ������� ����� ���������� �������
			CV3_DEF_ON;	//�� �������� ������ ����
		}
		if(cv_4[a_i]==next_oborot){//���� �������� �������� �������� ������� ����� ���������� �������
			CV4_DEF_ON;	//�� �������� ��������� ����
		}
	}
	a_i=0;//���������� ������� ��������� �������
}
void right_side(void){//������� ������������ ������� ������ �������
	// � ������ ������� ������ ���������� � ����������� ��������� ���������� ��������, ����, ��������� ����� � ���������
		if (prev_side==2){//���� ����� ���� ����������� ������ �������
			return;		//�� ������� � ������ �� ������
		}
		if (oborot==prog_oborot && next_oborot==1){//���� ������� ������, ������� ���� ��� ����� oborot, ����� ���������� �������� � ���������, � ��������� ������ ��� ����� 1
			mega=mega-1; // �� ��������� �������� ���������� ���������� �� 1
		}
		if (mega==0){ //���� ����� ����� ��� �� ��������� ���� ��� ����� 0 - �� �������� ������� ���� � ������� �� �������
		stop_fun();
		return;//������� � ������ �� ������ ������
		}
		prev_side=2;//��������, ��� ���������� ������� ���� �����, ������ ����� ���� ��� ��������� ����
		oborot=next_oborot; // ������������� ������� ������, ���� ��� �����, �� next_oborot ����� 1
		//***************** ������� ��������� ��� ��������� ���������� ��������
		PERED_VAL_DEF_OFF;// ������ ���������� ��������� ��������� ������
		ZAD_VAL_DEF_OFF;// ������ ���������� ��������� ������� ������
		//������ ��������� ���� ������� ������ (oborot) �������� ������, �� �������� �������� ���������������� ������
		//� �����, ���� ������� ������ �������� ����, �� �������� ������� ����
		a_i=0;//���������� ������� ��������� �������
		for (a_i=0; a_i<20; a_i++){
			if(pered_val[a_i]==oborot){//���� �������� �������� �������� ������� ����� �������� �������
				PERED_VAL_DEF_ON;	//�� �������� �������� �����
			}
			if(zad_val[a_i]==oborot){//���� �������� �������� �������� ������� ����� �������� �������
				ZAD_VAL_DEF_ON;	//�� �������� ������ �����
			}
			if(stop_mass[a_i]==oborot){//���� �������� �������� �������� ������� ����� �������� �������
				stop_fun();	//�� �������� ������� ����
			}
		}
		a_i=0;//���������� ������� ��������� �������
}

void sbros(void){//������� ������� ������ ����� � ��������� ����������������
	elem_prog=1; // ����� �������� ��� ���������������� (����� 8 ��������� �� 1 �� 8)
	kol_elem_prog=0; // ���������� ����������� �������� ��� ���������������
	oborot = 0; // ����������, ������� ������� ��������� ��������
	mega = 0;// ����������, ������� ������� ���������� ����������(���� �������� - ��� ��������� ���������), ������� �������� ������, ��� 0 ���������� ������� ����
	prev_side = 0; // ����������, ������� ��������� ������ ����� ������� �������� ������� ���, ����� ��� ����, ����� �������� ������ ��� ������������ ������ ������ ������� � ����� �������
	prog_oborot = 0;//���������� - ������������������� ���������� ��������
	next_oborot = 1;//���������� - ��������� ������
	int sbros_i=0;
	for (sbros_i=0; sbros_i<20; sbros_i++){
		   cv_1[sbros_i]=0;
		   cv_2[sbros_i]=0;
		   cv_3[sbros_i]=0;
		   cv_4[sbros_i]=0;
		   pered_val[sbros_i]=0;
		   zad_val[sbros_i]=0;
		   stop_mass[sbros_i]=0;
	}
}


void ok_prog_fun(void){//������� ������� ������ ��  � ������ ����������������
	if (elem_prog==1){//1 - ��� ����� ���������� ������ � ���������
		if (kol_elem_prog>0){// ������ ���� ���������� �������� � ����� ������ 0
			prog_oborot=kol_elem_prog;//������������� ����� ���������� �������� � ���������
			kol_elem_prog=0;//�������� ������� ���������� ���������
		}
	}
	else if(elem_prog==2){//2- ��� ��������1
		if (kol_elem_prog>0){
			a_i=0; //- ���������� ������� ��� �������� ����������� �������� �������
			while (cv_1[a_i]>0){//��� ������ ������� ������� ����� 0 - ������������� ���� � �� ���� ������� ������� ����������
				a_i++;
			}
			cv_1[a_i]=kol_elem_prog;
			kol_elem_prog=0;//�������� ������� ���������� ���������
			a_i=0;// �������� ������� �������� ����������� �������� �������
		}
	}
	else if(elem_prog==3){//3- ��� ��������2
		if (kol_elem_prog>0){
			a_i=0; //- ���������� ������� ��� �������� ����������� �������� �������
			while (cv_2[a_i]>0){//��� ������ ������� ������� ����� 0 - ������������� ���� � �� ���� ������� ������� ����������
				a_i++;
			}
			cv_2[a_i]=kol_elem_prog;
			kol_elem_prog=0;//�������� ������� ���������� ���������
			a_i=0;// �������� ������� �������� ����������� �������� �������
		}
	}
	else if(elem_prog==4){//4- ��� ��������3
		if (kol_elem_prog>0){
			a_i=0; //- ���������� ������� ��� �������� ����������� �������� �������
			while (cv_3[a_i]>0){//��� ������ ������� ������� ����� 0 - ������������� ���� � �� ���� ������� ������� ����������
				a_i++;
			}
			cv_3[a_i]=kol_elem_prog;
			kol_elem_prog=0;//�������� ������� ���������� ���������
			a_i=0;// �������� ������� �������� ����������� �������� �������
		}
	}
	else if(elem_prog==5){//5- ��� ��������4
		if (kol_elem_prog>0){
			a_i=0; //- ���������� ������� ��� �������� ����������� �������� �������
			while (cv_4[a_i]>0){//��� ������ ������� ������� ����� 0 - ������������� ���� � �� ���� ������� ������� ����������
				a_i++;
			}
			cv_4[a_i]=kol_elem_prog;
			kol_elem_prog=0;//�������� ������� ���������� ���������
			a_i=0;// �������� ������� �������� ����������� �������� �������
		}
	}
	else if(elem_prog==6){//6- ��� ��������� ������
		if (kol_elem_prog>0){
			a_i=0; //- ���������� ������� ��� �������� ����������� �������� �������
			while (pered_val[a_i]>0){//��� ������ ������� ������� ����� 0 - ������������� ���� � �� ���� ������� ������� ����������
				a_i++;
			}
			pered_val[a_i]=kol_elem_prog;
			kol_elem_prog=0;//�������� ������� ���������� ���������
			a_i=0;// �������� ������� �������� ����������� �������� �������
		}
	}
	else if(elem_prog==7){//7- ��� ������� ������
		if (kol_elem_prog>0){
			a_i=0; //- ���������� ������� ��� �������� ����������� �������� �������
			while (zad_val[a_i]>0){//��� ������ ������� ������� ����� 0 - ������������� ���� � �� ���� ������� ������� ����������
				a_i++;
			}
			zad_val[a_i]=kol_elem_prog;
			kol_elem_prog=0;//�������� ������� ���������� ���������
			a_i=0;// �������� ������� �������� ����������� �������� �������
		}
	}
	else if(elem_prog==8){//8- ��� ���������
		if (kol_elem_prog>0){
			a_i=0; //- ���������� ������� ��� �������� ����������� �������� �������
			while (stop_mass[a_i]>0){//��� ������ ������� ������� ����� 0 - ������������� ���� � �� ���� ������� ������� ����������
				a_i++;
			}
			stop_mass[a_i]=kol_elem_prog;
			kol_elem_prog=0;//�������� ������� ���������� ���������
			a_i=0;// �������� ������� �������� ����������� �������� �������
		}
	}

}


/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_I2C1_Init();

  /* USER CODE BEGIN 2 */

	
  ssd1306_Init();
  HAL_Delay(100);
  ssd1306_Fill(White);
  ssd1306_UpdateScreen();

  HAL_Delay(100);
  ssd1306_SetCursor(1,3);
  ssd1306_WriteString("012345678901234567",Font_7x10,Black, Inversion);
  ssd1306_UpdateScreen();	
  HAL_Delay(100);	
  ssd1306_SetCursor(1,13);
  ssd1306_WriteString("012345678901234567",Font_7x10,Black, NoInversion);
  ssd1306_UpdateScreen();	

  ssd1306_UpdateScreen();
	
	HAL_Delay(100);

  ssd1306_Fill(White);
  ssd1306_UpdateScreen();
	

  ssd1306_SetCursor(1,20);
  ssd1306_WriteString("   SETUP",Font_11x18,Black, NoInversion);
  ssd1306_UpdateScreen();	
	HAL_Delay(100);
	FSM_Init ();
  ssd1306_UpdateScreen();	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
		switch(key_scan)
		{
			case 1:
				FSM_press_down();
			break;
			
			case 2:
				FSM_press_up();
			break;
			
			case 4:
				FSM_press_rigth();
			break;
			
			case 8:
				FSM_press_left();
			break;
			
			case 0x10:
				FSM_press_ok();
			break;			
			
			default:
				break;				
		}		
		HAL_Delay(200);
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
/*		
if (HAL_GPIO_ReadPin(GPIOB, KNOP_DOWN_VYBOR_Pin))
	{		
		
	}
 
		
	  	  if (HAL_GPIO_ReadPin(GPIOB, PEREKLUCHATEL_PROG_RABOTA_Pin)){// ���� ����� ������������� � ��������� ���������������� - �� ����������� ��� ��� ������ ���� ��� ���������������� + ������ ������ ����
	  		  ///��� ��� ���������������� ���
	  		  if(HAL_GPIO_ReadPin(GPIOB, KNOP_SBROS_Pin)){// ������� �� ������ �����
	  			sbros();//������ ������� ������� �� ������ �����
	  					//��� ������ ����� ���������� ��� �������
	  		  }
	  		  //����� ��� �������� ���������� ����������:
	  		  // 1-� ������� - ����� �������� ��� ��������� � ������ (�� 1 �� 8)
	  		  // ���� ������� ������ 1, �� ������������� ��� ����� 1
	  		  // ���� ������� ������ 8, �� ������������� ��� ����� 8
	  		  // ��� ������ �������� �������� ���������� ���������� kol_elem_prog
	  		  // 2-� ������� - ��������� ����������� ����������
	  		  // ���� ������� ������ 0, �� ������������� ��� ����� 0
	  		  // ���� prog_oborot ������ 0 � ������� ������ prog_oborot, �� ������������� ���������� ������ prog_oborot

	  		  //����� �� ����� 8 �����������, ��� ������� ������ ������� - ��������������
	  		  // �������� ����� �������� kol_elem_prog
	  		  // �� ��������  ���������� ":" � ����� ��������
	  		  // ��� ��� ������� - ����� �������� ����� ������� (��������� ���� �������� � ����� ����������)
	  	  }

	  	  if (!HAL_GPIO_ReadPin(GPIOB, PEREKLUCHATEL_PROG_RABOTA_Pin)){//���� ����� ������������� � ��������� �������� ��������� - �� ����������� ��� ��� ������ ���� ��� �������� ���������
	  		  ///��� ��� �������� ��������� ���
	  		  if(HAL_GPIO_ReadPin(GPIOB, DATCHIK_LEFT_Pin)){// ������������ ������� ����� �������
	  			left_side();//������ ������� ����� �������
	  		  }
	  		  if(HAL_GPIO_ReadPin(GPIOB, DATCHIK_RIGHT_Pin)){// ������������ ������� ����� �������
	  			right_side();//������ ������� ������ �������
	  		  }

	  		  //����� ��� �������� ���������� ����������:
	  		  // 1-� ������� - ��������� ���������� ����������
	  		  // ���������� ������ ���� prog_oborot>0
	  		  // ���� ������ 0 - �� ������������� 0

	  		  // 2-� ������� - ��������� next_oborot
	  		  // ���������� ������ ���� prog_oborot>0
	  		  // ���� ������� = 0, �� ������������� �������� prog_oborot
	  		  // ���� �������� �������� ������ prog_oborot, �� ������������� 1

	  		  //����� �� �����:
	  		   	  //-���������� ����������
	  		   	  //-������� ����
	  		   	  //-������ � ���������
	  		   	  //-����������� �������� �������
	  		   	  //-������������ ������� ����
	  		  //
	  	  }
*/
  }
	
	
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
