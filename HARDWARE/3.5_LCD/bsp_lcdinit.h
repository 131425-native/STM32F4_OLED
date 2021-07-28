#ifndef _BSP_LCDINIT_H_
#define _BSP_LCDINIT_H_

#include "sys.h"
#include "stm32f4xx.h"

/****************************************************************************************************
//=========================================电源接线================================================//
//     LCD模块                STM32单片机
//      VCC          接        DC5V/3.3V      //电源
//      GND          接          GND          //电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为SPI总线
//     LCD模块                STM32单片机    
//    SDI(MOSI)      接          PD5          //液晶屏SPI总线数据写信号
//    SDO(MISO)      接          PD6          //液晶屏SPI总线数据读信号，如果不需要读，可以不接线
//=======================================液晶屏控制线接线==========================================//
//     LCD模块 					      STM32单片机 
//       LED         接          PD7         //液晶屏背光控制信号，如果不需要控制，接5V或3.3V
//       SCK         接          PD8          //液晶屏SPI总线时钟信号
//      DC/RS        接          PD9         //液晶屏数据/命令控制信号
//       RST         接          PD10       //液晶屏复位控制信号
//       CS          接          PD11         //液晶屏片选控制信号
//=========================================触摸屏触接线=========================================//
//如果模块不带触摸功能或者带有触摸功能，但是不需要触摸功能，则不需要进行触摸屏接线
//	   LCD模块                STM32单片机 
//      T_IRQ        接          PE0          //触摸屏触摸中断信号
//      T_DO         接          PB2          //触摸屏SPI总线读信号
//      T_DIN        接          PC11         //触摸屏SPI总线写信号
//      T_CS         接          PB8          //触摸屏片选控制信号
//      T_CLK        接          PE1          //触摸屏SPI总线时钟信号
**************************************************************************************************/		

///////////////////////////////////////////////////////////////////
//-----------------LCD端口定义---------------- 

#define LED      7        //背光控制引脚
#define CS   		 11       //片选引脚
#define RS       9       //寄存器/数据选择引脚  
#define RST      10       //复位引脚

//QDtech全系列模块采用了三极管控制背光亮灭，用户也可以接PWM调节背光亮度
#define	LCD_LED PDout(LED) //LCD背光    		 
#define LCD_CS  PDout(CS)
#define LCD_RS  PDout(RS)
#define LCD_RST PDout(RST)
//如果使用官方库函数定义下列底层，速度将会下降到14帧每秒，建议采用我司推荐方法
//以下IO定义直接操作寄存器，快速IO操作，刷屏速率可以达到28帧每秒！ 

#define	LCD_CS_SET  LCD_CS=1 //GPIO_TYPE->BSRRL=1<<LCD_CS    //片选端口  	PB11
#define	LCD_RS_SET	LCD_RS=1 //GPIO_TYPE->BSRRL=1<<LCD_RS    //数据/命令  PB10	  
#define	LCD_RST_SET	LCD_RST=1 //GPIO_TYPE->BSRRL=1<<LCD_RST    //复位			PB12

 							    
#define	LCD_CS_CLR  LCD_CS=0 //GPIO_TYPE->BSRRH=1<<LCD_CS     //片选端口  	PB11
#define	LCD_RS_CLR	LCD_RS=0 //GPIO_TYPE->BSRRH=1<<LCD_RS     //数据/命令  PB10	 
#define	LCD_RST_CLR	LCD_RST=0 //GPIO_TYPE->BSRRH=1<<LCD_RST    //复位			  PB12

/*--------------------SPI_INIT------------------------------------------*/
#define SCLK        8	//Pd3--->>TFT --SCL/SCK
#define MISO        6	
#define MOSI        5	//Pd5 MOSI--->>TFT --SDA/DIN

#define SPI_MOSI  PDout(MOSI)
#define SPI_SCLK  PDout(SCLK)
#define SPI_MISO  PDin(MISO)

//液晶控制口置1操作语句宏定义

#define	SPI_MOSI_SET  	SPI_MOSI=1 //LCD_CTRL->BSRR=SPI_MOSI    
#define	SPI_SCLK_SET  	SPI_SCLK=1 //LCD_CTRL->BSRR=SPI_SCLK    


//液晶控制口置0操作语句宏定义

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

//如果仍然觉得速度不够快，可以使用下面的宏定义,提高速度.
//注意要去掉lcd.c中void LCD_WR_DATA(u16 data)函数定义哦
/*
#define LCD_USE8BIT_MODEL 0
#if LCD_USE8BIT_MODEL//使用8位并行数据总线模式
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
	#else//使用16位并行数据总线模式
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
	
//画笔颜色
#define BLACK						 				0										//黑色
#define DIMGRAY					 				27469								//昏黑
#define GRAY						 				33808								//灰色
#define DARK_GRAY				 				44373								//暗灰
#define SILVER				   				50712								//银色
#define LIGHT_GRAY			 				54938								//亮灰色
#define GAINSBORO				 				57083								//庚斯博罗灰
#define WHITE_SMOKE			 				63422								//白烟色
#define WHITE         	 				65535								//白色
#define SNOW				     				65503								//雪色
#define IRON_GRAY				 				25290								//铁灰色
#define SAND_BEIGE			 				58904								//沙棕
#define ROSY_BROWN			 				48241								//玫瑰褐
#define LIGHT_CORAL			 				62480								//亮珊瑚色
#define INDIAN_RED			 				51947								//印度红
#define BROWN				   	 				41285								//褐色
#define FIRE_BRICK			 				45316								//耐火砖红
#define MAROON				   				32768								//栗色
#define DARK_RED				 				34816								//暗红
#define STRONG_RED			 				57344								//鲜红
#define RED				   						63488								//红色
#define PERSIMMON				 				64104								//柿子橙
#define MISTY_ROSE							65340								//雾玫瑰色
#define SALMON				  				64526								//鲑色
#define SCARLET				   				63776								//腥红
#define TOMATO				   				64264								//番茄红
#define DARK_SALMON				    	60591								//暗鲑红
#define CORAL				   					64490								//珊瑚红
#define ORANGE_RED							64032								//橙红
#define LIGHT_SALMON						64783								//亮鲑红
#define VERMILION								64096								//朱红
#define SIENNA				  				41605								//赤黄
#define TROPICAL_ORANGE					64518								//热带橙
#define CAMEL				   					41800								//驼色
#define APRICOT				   				58572								//杏黄
#define COCONUT_BROWN						18656								//椰褐色
#define SEASHELL				  			65469								//海贝色
#define SADDLE_BROWN						35362								//鞍褐色
#define CHOCOLATE				  			54083								//巧克力色
#define BURNT_ORANGE						51872								//燃橙色
#define SUN_ORANGE							64384								//阳橙色
#define PEACH_PUFF							65239								//粉扑桃色
#define SAND_BROWN							62764								//沙褐色
#define BRONZE				   				48006								//古铜色
#define LINEN				   					65436								//亚麻色
#define HONEY_ORANGE						64908								//密橙色
#define PERU				  					52263								//秘鲁色
#define SEPIA				  					29186								//乌贼墨黑
#define OCHER				  					52132								//赤色
#define BISQUE									65336								//陶坯黄
#define TANGERINE				   			62496								//橘色
#define DARK_ORANGE				 			64608								//暗橙
#define ANTIQUE_WHITE						65370								//古董白
#define TAN				   						54705								//日晒色
#define BURLY_WOOD				  		56784								//硬木色
#define BLANCHED_ALMOND					65369								//杏仁白
#define NAVAJO_WHITE						65269								//那瓦霍白
#define MARIGOLD				   			64704								//万寿菊黄
#define PAPAYA_WHIP				  		65402								//蕃木瓜色
#define PALE_OCRE				   			52625								//灰土色
#define KHAKI				   					39747								//卡其色
#define MOCCASIN				   			65334								//鹿皮鞋色
#define OLD_LACE				  			65468								//旧蕾丝色
#define WHEAT				  	 				63222								//小麦色
#define PEACH				   					65334								//桃色
#define ORANGE				 					64800								//橙色
#define FLORAL_WHITE						65502								//花卉白
#define GOLDENROD								56612								//金菊色
#define DARK_GOLDENROD					48161								//暗金菊色
#define COFFEE				  				18880								//咖啡色
#define JASMINE				  				58891								//茉莉黄
#define AMBER				   					64992								//琥铂色
#define CORNSILK								65499								//玉米丝色
#define CHROME_YELLOW						58816								//铬黄
#define GOLDEN				 	 				65184								//金色
#define LEMON_CHIFFON						65497								//柠檬绸色
#define LIGHT_KHAKI							63281								//亮卡其色
#define PALE_GOLDENROD					61269								//灰金菊色
#define DARK_KHAKI							48557								//暗卡其色
#define MIMOSA				  				59078								//含羞草黄
#define CREAM				   					65530								//奶油色
#define IVORY				   					65534								//象牙色
#define BEIGE				   					63419								//米黄色
#define LIGHT_YELLOW						65532								//亮黄
#define LIGHT_GOLDENROD_YELLOW	65498								//亮金菊黄
#define CHAMPAGNE_YELLOW				65523								//香槟黄
#define MUSTARD				  			 	52841								//芥末黄
#define MOON_YELLOW							65513								//月黄
#define OLIVE				  					33792								//橄榄色
#define CANARY_YELLOW				  	65504								//鲜黄色
#define YELLOW				   				65504								//黄色
#define MOSS_GREEN							27556								//苔藓绿
#define LIGHT_LIME							53216								//亮柠檬绿
#define OLIVE_DRAB							27748								//橄榄军服绿
#define YELLOW_GREEN						40550								//黄绿
#define DARK_OLIVE_GREEN				21317								//暗橄榄绿
#define APPLE_GREEN							36640								//苹果绿
#define GREEN_YELLOW						45029								//绿黄
#define GRASS_GREEN							40745								//草绿
#define LAWN_GREEN				 			32736								//草坪绿
#define CHARTREUSE				 			32736								//查特酒绿
#define FOLIAGE_GREEN						30151								//叶绿
#define FRESH_LEAVES						40937								//嫩绿
#define BRIGHT_GREEN						26592								//明绿
#define COBALT_GREEN						26603								//钴绿
#define HONEYDEW				   			63486								//蜜瓜绿
#define DARK_SEA_GREEN					36337								//暗海绿
#define LIGHT_GREEN				  		38770								//亮绿
#define PALE_GREEN				   		40915								//灰绿
#define IVY_GREEN				  			13798								//常春藤绿
#define FOREST_GREEN						9284								//森林绿
#define LIME_GREEN				   		13926								//柠檬绿
#define DARK_GREEN				   		800									//暗绿
#define GREEN				   					1024								//绿色
#define LIME				   					2016								//鲜绿色
#define MALACHITE				   			9733								//孔雀石绿
#define MINT				   					5317								//薄荷绿
#define CELADON_GREEN				   	30513								//青瓷绿
#define EMERALD				   				22095								//碧绿
#define TURQUOSISE_GREEN				20272								//绿松石绿
#define VERIDIAN				   			5030								//铬绿
#define HORIZON_BLUE				   	43001								//苍色
#define SEA_GREEN				  			11338								//海绿
#define MEDIUM_SEA_GREEN				15758								//中海绿
#define MINT_CREAM				   		63487								//薄荷奶油色
#define SPRING_GREEN				   	2032								//春绿
#define PEACOCK_GREEN				   	1291								//孔雀绿
#define MEDIUM_SPRING_GREEN			2003								//中春绿色
#define MEDIUM_AQUAMARINE				26229								//中碧蓝色
#define AQUAMARINE				 			32762								//碧蓝色
#define CYAN_BLUE				 				3569								//青蓝
#define AQUA_BLUE					 			26620								//水蓝
#define TURQUOISE_BLUE				 	14137								//绿松石蓝
#define TURQUOISE				 				14009								//绿松石色
#define LIGHT_SEA_GREEN				 	9621								//亮海绿
#define MEDIUM_TURQUOISE				20121								//中绿松石色
#define LIGHT_CYAN				 			59391								//亮青
#define BABY_BLUE				 				59391								//浅蓝
#define PALE_TURQUOISE				 	44925								//灰绿松石色
#define DARK_SLATE_GRAY				 	10857								//暗岩灰
#define TEAL				 						1040								//绿
#define DARK_CYAN				 				1105								//暗青
#define CYAN				 						2047								//青色
#define AQUA				 						44796								//水色
#define DARK_TURQUOISE				 	1658								//暗绿松石色
#define CADET_BLUE				 			23796								//军服蓝
#define PEACOCK_BLUE				 		1041								//孔雀蓝
#define POWDER_BLUE				 			46876								//婴儿粉蓝
#define STRONG_BLUE				 			782									//浓蓝
#define LIGHT_BLUE				 			44764								//亮蓝
#define PALE_BLUE				 				32217								//灰蓝
#define SAXE_BLUE				 				17622								//萨克斯蓝
#define DEEP_SKY_BLUE						1535								//深天蓝
#define SKY_BLUE				 				34429								//天蓝
#define LIGHT_SKY_BLUE				 	34431								//亮天蓝
#define MARINE_BLUE				 			559									//水手蓝
#define PRUSSIAN_BLUE				 		394									//普鲁士蓝
#define STEEL_BLUE				 			17430								//钢青色
#define ALICE_BLUE				 			63455								//爱丽丝蓝
#define SLATE_GRAY				 			29714								//岩灰
#define LIGHT_SLATE_GRAY				29779								//亮岩灰
#define DODGER_BLUE				 			7327								//道奇蓝
#define MINERAL_BLUE					 	627									//矿懒
#define AZURE				 						1023								//湛蓝
#define WEDGWOOD_BLUE				 		21559								//韦奇伍德瓷蓝
#define LIGHT_STEEL_BLUE				46651								//亮钢蓝
#define COBALT_BLUE				 			565									//钴蓝
#define PALE_DENIM				 			23608								//灰丁宁澜
#define CORNFLOWER_BLUE				 	25789								//矢车菊蓝
#define SALVIA_BLUE				 			19484								//鼠尾草蓝
#define DARK_POWDER_BLUE				403									//暗婴儿粉蓝
#define SAPPHIRE				 				2348								//蓝宝石色
#define CERULEAN_BLUE				 		10903								//蔚蓝
#define ROYAL_BLUE				 			17244								//品蓝
#define DARK_MINERAL_BLUE				8623								//暗矿蓝
#define ULTRAMARINE				 			415									//极浓海蓝
#define LAPIS_LAZULI				 		2463								//天青石蓝
#define GHOST_WHITE				 			65503								//幽灵白
#define LAVENDER				 				59199								//薰衣草紫
#define PERIWINKLE				 			52863								//长春花色
#define MIDNIGHT_BLUE				 		6350								//午夜蓝
#define NAVY_BLUE				 				16									//藏青
#define DARK_BLUE				 				17									//暗蓝
#define MEDIUM_BLUE				 			25									//中蓝
#define BLUE				 						31									//蓝色
#define WISTERIA								23196								//紫藤色
#define DARK_SLATE_BLUE				 	18929								//暗岩蓝
#define SLATE_BLUE				 			27353								//岩蓝
#define MEDIUM_SLATE_BLUE				31581								//中岩蓝
#define MAUVE										25119								//木槿紫
#define LILAC										46303								//紫丁香色
#define MEDIUM_PURPLE						37787								//中紫红
#define AMETHYST								24985								//紫水晶色
#define GRAYISH_PURPLE					33716								//浅灰紫红
#define HELIOTROPE							20503								//草紫
#define MINERAL_VIOLET					48409								//矿紫
#define BLUE_VIOLET							35164								//蓝紫
#define VIOLET									34847								//紫罗兰色
#define INDIGO									18448								//靛色
#define DARK_ORCHID							39321								//暗兰紫
#define DARK_VIOLET							36890								//暗紫
#define PANSY										28692								//三色堇紫
#define MALLOW									55935								//锦葵紫
#define OPERA_MAUVE							58399								//优品紫红
#define MEDIUM_ORCHID						47802								//中兰紫
#define PAIL_LILAC							59004								//淡紫丁香色
#define THISTLE									56827								//浕紫
#define CLEMATIS								52505								//铁线莲紫
#define PLUM										56603								//梅红色
#define LIGHT_VIOLET						60445								//亮紫
#define PURPLE									32784								//紫色
#define DARK_MAGENTA						34833								//暗洋红
#define MAGENTA									63519								//洋红
#define FUCHSIA									61460								//品红
#define ORCHID									56218								//兰紫
#define PEARL_PINK							64924								//浅珍珠红
#define OLD_ROSE								47795								//陈玫红
#define ROSE_PINK								64313								//浅玫瑰红
#define MEDIUM_VIOLET_RED				49328								//中青紫红
#define MAGENTA_ROSE						63604								//洋玫瑰红
#define ROSE										63503								//玫瑰红
#define RUBY										51216								//红宝石色
#define CAMELLIA								57810								//山茶红
#define DEEP_PINK								63666								//深粉红
#define FLAMINGO								58455								//火鹤红
#define CORAL_PINK							64535								//浅珊瑚红
#define HOT_PINK								64342								//暖粉红
#define BURGUNDY								16388								//勃艮第酒红
#define SPINEL_RED							64406								//尖晶石红
#define CARMINE									57355								//胭脂红
#define BABY_PINK								65244								//浅粉红
#define CARDINAL_RED						38918								//枢机红
#define LAVENDER_BLUSH					65438								//薰衣草紫红
#define PALE_VIOLET_RED					56210								//灰紫红
#define CERISE									55692								//樱桃红
#define SALMON_PINK							64531								//浅鲑红
#define CRIMSON									55463								//绯红
#define PINK										65049								//粉红
#define LIGHT_PINK							64952								//亮粉红
#define SHELL_PINK							64919								//壳黄红
#define ALIZARIN_CRIMSON				57638								//茜红


#endif /*_BSP_LCDINIT_H_*/


