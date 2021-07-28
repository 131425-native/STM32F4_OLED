#ifndef _BSP_LCDINIT_H_
#define _BSP_LCDINIT_H_

#include "sys.h"
#include "stm32f4xx.h"

/****************************************************************************************************
//=========================================��Դ����================================================//
//     LCDģ��                STM32��Ƭ��
//      VCC          ��        DC5V/3.3V      //��Դ
//      GND          ��          GND          //��Դ��
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������ΪSPI����
//     LCDģ��                STM32��Ƭ��    
//    SDI(MOSI)      ��          PD5          //Һ����SPI��������д�ź�
//    SDO(MISO)      ��          PD6          //Һ����SPI�������ݶ��źţ��������Ҫ�������Բ�����
//=======================================Һ���������߽���==========================================//
//     LCDģ�� 					      STM32��Ƭ�� 
//       LED         ��          PD7         //Һ������������źţ��������Ҫ���ƣ���5V��3.3V
//       SCK         ��          PD8          //Һ����SPI����ʱ���ź�
//      DC/RS        ��          PD9         //Һ��������/��������ź�
//       RST         ��          PD10       //Һ������λ�����ź�
//       CS          ��          PD11         //Һ����Ƭѡ�����ź�
//=========================================������������=========================================//
//���ģ�鲻���������ܻ��ߴ��д������ܣ����ǲ���Ҫ�������ܣ�����Ҫ���д���������
//	   LCDģ��                STM32��Ƭ�� 
//      T_IRQ        ��          PE0          //�����������ж��ź�
//      T_DO         ��          PB2          //������SPI���߶��ź�
//      T_DIN        ��          PC11         //������SPI����д�ź�
//      T_CS         ��          PB8          //������Ƭѡ�����ź�
//      T_CLK        ��          PE1          //������SPI����ʱ���ź�
**************************************************************************************************/		

///////////////////////////////////////////////////////////////////
//-----------------LCD�˿ڶ���---------------- 

#define LED      7        //�����������
#define CS   		 11       //Ƭѡ����
#define RS       9       //�Ĵ���/����ѡ������  
#define RST      10       //��λ����

//QDtechȫϵ��ģ������������ܿ��Ʊ��������û�Ҳ���Խ�PWM���ڱ�������
#define	LCD_LED PDout(LED) //LCD����    		 
#define LCD_CS  PDout(CS)
#define LCD_RS  PDout(RS)
#define LCD_RST PDout(RST)
//���ʹ�ùٷ��⺯���������еײ㣬�ٶȽ����½���14֡ÿ�룬���������˾�Ƽ�����
//����IO����ֱ�Ӳ����Ĵ���������IO������ˢ�����ʿ��Դﵽ28֡ÿ�룡 

#define	LCD_CS_SET  LCD_CS=1 //GPIO_TYPE->BSRRL=1<<LCD_CS    //Ƭѡ�˿�  	PB11
#define	LCD_RS_SET	LCD_RS=1 //GPIO_TYPE->BSRRL=1<<LCD_RS    //����/����  PB10	  
#define	LCD_RST_SET	LCD_RST=1 //GPIO_TYPE->BSRRL=1<<LCD_RST    //��λ			PB12

 							    
#define	LCD_CS_CLR  LCD_CS=0 //GPIO_TYPE->BSRRH=1<<LCD_CS     //Ƭѡ�˿�  	PB11
#define	LCD_RS_CLR	LCD_RS=0 //GPIO_TYPE->BSRRH=1<<LCD_RS     //����/����  PB10	 
#define	LCD_RST_CLR	LCD_RST=0 //GPIO_TYPE->BSRRH=1<<LCD_RST    //��λ			  PB12

/*--------------------SPI_INIT------------------------------------------*/
#define SCLK        8	//Pd3--->>TFT --SCL/SCK
#define MISO        6	
#define MOSI        5	//Pd5 MOSI--->>TFT --SDA/DIN

#define SPI_MOSI  PDout(MOSI)
#define SPI_SCLK  PDout(SCLK)
#define SPI_MISO  PDin(MISO)

//Һ�����ƿ���1�������궨��

#define	SPI_MOSI_SET  	SPI_MOSI=1 //LCD_CTRL->BSRR=SPI_MOSI    
#define	SPI_SCLK_SET  	SPI_SCLK=1 //LCD_CTRL->BSRR=SPI_SCLK    


//Һ�����ƿ���0�������궨��

#define	SPI_MOSI_CLR  	SPI_MOSI=0 //LCD_CTRL->BRR=SPI_MOSI    
#define	SPI_SCLK_CLR  	SPI_SCLK=0 //LCD_CTRL->BRR=SPI_SCLK


void LCD_GPIOInit(void);
void  SPIv_WriteData(u8 Data);
void LCD_WR_REG(u8 data);
void LCD_WR_DATA(u8 data);
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue);
void LCD_WriteRAM_Prepare(void);
void Lcd_WriteData_16Bit(u16 Data);
void LCD_RESET(void);
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void LCD_direction(u8 direction);
void LCD_Init(void);

//�����Ȼ�����ٶȲ����죬����ʹ������ĺ궨��,����ٶ�.
//ע��Ҫȥ��lcd.c��void LCD_WR_DATA(u16 data)��������Ŷ
/*
#define LCD_USE8BIT_MODEL 0
#if LCD_USE8BIT_MODEL//ʹ��8λ������������ģʽ
	#define LCD_WR_DATA(data){\
	LCD_RS_SET;\
	LCD_CS_CLR;\
	DATAOUT(data);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	DATAOUT(data<<8);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	LCD_CS_SET;\
	}
	#else//ʹ��16λ������������ģʽ
	#define LCD_WR_DATA(data){\
	LCD_RS_SET;\
	LCD_CS_CLR;\
	DATAOUT(data);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	LCD_CS_SET;\
	} 	
#endif
*/
	
//������ɫ
#define BLACK						 				0										//��ɫ
#define DIMGRAY					 				27469								//���
#define GRAY						 				33808								//��ɫ
#define DARK_GRAY				 				44373								//����
#define SILVER				   				50712								//��ɫ
#define LIGHT_GRAY			 				54938								//����ɫ
#define GAINSBORO				 				57083								//��˹���޻�
#define WHITE_SMOKE			 				63422								//����ɫ
#define WHITE         	 				65535								//��ɫ
#define SNOW				     				65503								//ѩɫ
#define IRON_GRAY				 				25290								//����ɫ
#define SAND_BEIGE			 				58904								//ɳ��
#define ROSY_BROWN			 				48241								//õ���
#define LIGHT_CORAL			 				62480								//��ɺ��ɫ
#define INDIAN_RED			 				51947								//ӡ�Ⱥ�
#define BROWN				   	 				41285								//��ɫ
#define FIRE_BRICK			 				45316								//�ͻ�ש��
#define MAROON				   				32768								//��ɫ
#define DARK_RED				 				34816								//����
#define STRONG_RED			 				57344								//�ʺ�
#define RED				   						63488								//��ɫ
#define PERSIMMON				 				64104								//���ӳ�
#define MISTY_ROSE							65340								//��õ��ɫ
#define SALMON				  				64526								//��ɫ
#define SCARLET				   				63776								//�Ⱥ�
#define TOMATO				   				64264								//���Ѻ�
#define DARK_SALMON				    	60591								//���ٺ�
#define CORAL				   					64490								//ɺ����
#define ORANGE_RED							64032								//�Ⱥ�
#define LIGHT_SALMON						64783								//���ٺ�
#define VERMILION								64096								//���
#define SIENNA				  				41605								//���
#define TROPICAL_ORANGE					64518								//�ȴ���
#define CAMEL				   					41800								//��ɫ
#define APRICOT				   				58572								//�ӻ�
#define COCONUT_BROWN						18656								//Ҭ��ɫ
#define SEASHELL				  			65469								//����ɫ
#define SADDLE_BROWN						35362								//����ɫ
#define CHOCOLATE				  			54083								//�ɿ���ɫ
#define BURNT_ORANGE						51872								//ȼ��ɫ
#define SUN_ORANGE							64384								//����ɫ
#define PEACH_PUFF							65239								//������ɫ
#define SAND_BROWN							62764								//ɳ��ɫ
#define BRONZE				   				48006								//��ͭɫ
#define LINEN				   					65436								//����ɫ
#define HONEY_ORANGE						64908								//�ܳ�ɫ
#define PERU				  					52263								//��³ɫ
#define SEPIA				  					29186								//����ī��
#define OCHER				  					52132								//��ɫ
#define BISQUE									65336								//������
#define TANGERINE				   			62496								//��ɫ
#define DARK_ORANGE				 			64608								//����
#define ANTIQUE_WHITE						65370								//�Ŷ���
#define TAN				   						54705								//��ɹɫ
#define BURLY_WOOD				  		56784								//Ӳľɫ
#define BLANCHED_ALMOND					65369								//���ʰ�
#define NAVAJO_WHITE						65269								//���߻���
#define MARIGOLD				   			64704								//���پջ�
#define PAPAYA_WHIP				  		65402								//ެľ��ɫ
#define PALE_OCRE				   			52625								//����ɫ
#define KHAKI				   					39747								//����ɫ
#define MOCCASIN				   			65334								//¹ƤЬɫ
#define OLD_LACE				  			65468								//����˿ɫ
#define WHEAT				  	 				63222								//С��ɫ
#define PEACH				   					65334								//��ɫ
#define ORANGE				 					64800								//��ɫ
#define FLORAL_WHITE						65502								//���ܰ�
#define GOLDENROD								56612								//���ɫ
#define DARK_GOLDENROD					48161								//�����ɫ
#define COFFEE				  				18880								//����ɫ
#define JASMINE				  				58891								//�����
#define AMBER				   					64992								//����ɫ
#define CORNSILK								65499								//����˿ɫ
#define CHROME_YELLOW						58816								//����
#define GOLDEN				 	 				65184								//��ɫ
#define LEMON_CHIFFON						65497								//���ʳ�ɫ
#define LIGHT_KHAKI							63281								//������ɫ
#define PALE_GOLDENROD					61269								//�ҽ��ɫ
#define DARK_KHAKI							48557								//������ɫ
#define MIMOSA				  				59078								//���߲ݻ�
#define CREAM				   					65530								//����ɫ
#define IVORY				   					65534								//����ɫ
#define BEIGE				   					63419								//�׻�ɫ
#define LIGHT_YELLOW						65532								//����
#define LIGHT_GOLDENROD_YELLOW	65498								//����ջ�
#define CHAMPAGNE_YELLOW				65523								//���Ļ�
#define MUSTARD				  			 	52841								//��ĩ��
#define MOON_YELLOW							65513								//�»�
#define OLIVE				  					33792								//���ɫ
#define CANARY_YELLOW				  	65504								//�ʻ�ɫ
#define YELLOW				   				65504								//��ɫ
#define MOSS_GREEN							27556								//̦޺��
#define LIGHT_LIME							53216								//��������
#define OLIVE_DRAB							27748								//��魾�����
#define YELLOW_GREEN						40550								//����
#define DARK_OLIVE_GREEN				21317								//�������
#define APPLE_GREEN							36640								//ƻ����
#define GREEN_YELLOW						45029								//�̻�
#define GRASS_GREEN							40745								//����
#define LAWN_GREEN				 			32736								//��ƺ��
#define CHARTREUSE				 			32736								//���ؾ���
#define FOLIAGE_GREEN						30151								//Ҷ��
#define FRESH_LEAVES						40937								//����
#define BRIGHT_GREEN						26592								//����
#define COBALT_GREEN						26603								//����
#define HONEYDEW				   			63486								//�۹���
#define DARK_SEA_GREEN					36337								//������
#define LIGHT_GREEN				  		38770								//����
#define PALE_GREEN				   		40915								//����
#define IVY_GREEN				  			13798								//��������
#define FOREST_GREEN						9284								//ɭ����
#define LIME_GREEN				   		13926								//������
#define DARK_GREEN				   		800									//����
#define GREEN				   					1024								//��ɫ
#define LIME				   					2016								//����ɫ
#define MALACHITE				   			9733								//��ȸʯ��
#define MINT				   					5317								//������
#define CELADON_GREEN				   	30513								//�����
#define EMERALD				   				22095								//����
#define TURQUOSISE_GREEN				20272								//����ʯ��
#define VERIDIAN				   			5030								//����
#define HORIZON_BLUE				   	43001								//��ɫ
#define SEA_GREEN				  			11338								//����
#define MEDIUM_SEA_GREEN				15758								//�к���
#define MINT_CREAM				   		63487								//��������ɫ
#define SPRING_GREEN				   	2032								//����
#define PEACOCK_GREEN				   	1291								//��ȸ��
#define MEDIUM_SPRING_GREEN			2003								//�д���ɫ
#define MEDIUM_AQUAMARINE				26229								//�б���ɫ
#define AQUAMARINE				 			32762								//����ɫ
#define CYAN_BLUE				 				3569								//����
#define AQUA_BLUE					 			26620								//ˮ��
#define TURQUOISE_BLUE				 	14137								//����ʯ��
#define TURQUOISE				 				14009								//����ʯɫ
#define LIGHT_SEA_GREEN				 	9621								//������
#define MEDIUM_TURQUOISE				20121								//������ʯɫ
#define LIGHT_CYAN				 			59391								//����
#define BABY_BLUE				 				59391								//ǳ��
#define PALE_TURQUOISE				 	44925								//������ʯɫ
#define DARK_SLATE_GRAY				 	10857								//���һ�
#define TEAL				 						1040								//��
#define DARK_CYAN				 				1105								//����
#define CYAN				 						2047								//��ɫ
#define AQUA				 						44796								//ˮɫ
#define DARK_TURQUOISE				 	1658								//������ʯɫ
#define CADET_BLUE				 			23796								//������
#define PEACOCK_BLUE				 		1041								//��ȸ��
#define POWDER_BLUE				 			46876								//Ӥ������
#define STRONG_BLUE				 			782									//Ũ��
#define LIGHT_BLUE				 			44764								//����
#define PALE_BLUE				 				32217								//����
#define SAXE_BLUE				 				17622								//����˹��
#define DEEP_SKY_BLUE						1535								//������
#define SKY_BLUE				 				34429								//����
#define LIGHT_SKY_BLUE				 	34431								//������
#define MARINE_BLUE				 			559									//ˮ����
#define PRUSSIAN_BLUE				 		394									//��³ʿ��
#define STEEL_BLUE				 			17430								//����ɫ
#define ALICE_BLUE				 			63455								//����˿��
#define SLATE_GRAY				 			29714								//�һ�
#define LIGHT_SLATE_GRAY				29779								//���һ�
#define DODGER_BLUE				 			7327								//������
#define MINERAL_BLUE					 	627									//����
#define AZURE				 						1023								//տ��
#define WEDGWOOD_BLUE				 		21559								//Τ����´���
#define LIGHT_STEEL_BLUE				46651								//������
#define COBALT_BLUE				 			565									//����
#define PALE_DENIM				 			23608								//�Ҷ�����
#define CORNFLOWER_BLUE				 	25789								//ʸ������
#define SALVIA_BLUE				 			19484								//��β����
#define DARK_POWDER_BLUE				403									//��Ӥ������
#define SAPPHIRE				 				2348								//����ʯɫ
#define CERULEAN_BLUE				 		10903								//ε��
#define ROYAL_BLUE				 			17244								//Ʒ��
#define DARK_MINERAL_BLUE				8623								//������
#define ULTRAMARINE				 			415									//��Ũ����
#define LAPIS_LAZULI				 		2463								//����ʯ��
#define GHOST_WHITE				 			65503								//�����
#define LAVENDER				 				59199								//޹�²���
#define PERIWINKLE				 			52863								//������ɫ
#define MIDNIGHT_BLUE				 		6350								//��ҹ��
#define NAVY_BLUE				 				16									//����
#define DARK_BLUE				 				17									//����
#define MEDIUM_BLUE				 			25									//����
#define BLUE				 						31									//��ɫ
#define WISTERIA								23196								//����ɫ
#define DARK_SLATE_BLUE				 	18929								//������
#define SLATE_BLUE				 			27353								//����
#define MEDIUM_SLATE_BLUE				31581								//������
#define MAUVE										25119								//ľ����
#define LILAC										46303								//�϶���ɫ
#define MEDIUM_PURPLE						37787								//���Ϻ�
#define AMETHYST								24985								//��ˮ��ɫ
#define GRAYISH_PURPLE					33716								//ǳ���Ϻ�
#define HELIOTROPE							20503								//����
#define MINERAL_VIOLET					48409								//����
#define BLUE_VIOLET							35164								//����
#define VIOLET									34847								//������ɫ
#define INDIGO									18448								//��ɫ
#define DARK_ORCHID							39321								//������
#define DARK_VIOLET							36890								//����
#define PANSY										28692								//��ɫ����
#define MALLOW									55935								//������
#define OPERA_MAUVE							58399								//��Ʒ�Ϻ�
#define MEDIUM_ORCHID						47802								//������
#define PAIL_LILAC							59004								//���϶���ɫ
#define THISTLE									56827								//����
#define CLEMATIS								52505								//��������
#define PLUM										56603								//÷��ɫ
#define LIGHT_VIOLET						60445								//����
#define PURPLE									32784								//��ɫ
#define DARK_MAGENTA						34833								//�����
#define MAGENTA									63519								//���
#define FUCHSIA									61460								//Ʒ��
#define ORCHID									56218								//����
#define PEARL_PINK							64924								//ǳ�����
#define OLD_ROSE								47795								//��õ��
#define ROSE_PINK								64313								//ǳõ���
#define MEDIUM_VIOLET_RED				49328								//�����Ϻ�
#define MAGENTA_ROSE						63604								//��õ���
#define ROSE										63503								//õ���
#define RUBY										51216								//�챦ʯɫ
#define CAMELLIA								57810								//ɽ���
#define DEEP_PINK								63666								//��ۺ�
#define FLAMINGO								58455								//��׺�
#define CORAL_PINK							64535								//ǳɺ����
#define HOT_PINK								64342								//ů�ۺ�
#define BURGUNDY								16388								//���޵ھƺ�
#define SPINEL_RED							64406								//�⾧ʯ��
#define CARMINE									57355								//��֬��
#define BABY_PINK								65244								//ǳ�ۺ�
#define CARDINAL_RED						38918								//�����
#define LAVENDER_BLUSH					65438								//޹�²��Ϻ�
#define PALE_VIOLET_RED					56210								//���Ϻ�
#define CERISE									55692								//ӣ�Һ�
#define SALMON_PINK							64531								//ǳ�ٺ�
#define CRIMSON									55463								//糺�
#define PINK										65049								//�ۺ�
#define LIGHT_PINK							64952								//���ۺ�
#define SHELL_PINK							64919								//�ǻƺ�
#define ALIZARIN_CRIMSON				57638								//���


#endif /*_BSP_LCDINIT_H_*/


