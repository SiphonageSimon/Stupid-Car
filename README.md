# Stupid-Car
诸君，闷声发大财

IO口占用列表：
（**交给吴兵总结了**）

/*液晶屏数据传输*/
PTH4 OLED_SCL 		
PTH3 OLED_SDA 	

/*开关控制模式选择*/
PTE6 置低开始电感归一化

/*电感0-6*/
PTB0 ADC0 ADC_CHANNEL_AD4        
PTB1 ADC1 ADC_CHANNEL_AD5        
PTF4 ADC2 ADC_CHANNEL_AD12    
PTC0 ADC3 ADC_CHANNEL_AD8        
PTF5 ADC4 ADC_CHANNEL_AD13      
PTF6 ADC5 ADC_CHANNEL_AD14      

/*电机*/
PTE3 高电平
PTI3 低电平
PTD1 pwm控制
PTE1 高电平
PTG7 低电平
PTG6 pwm控制

/*舵机*/
PTC5 舵机pwm

/*led控制*/
LED0-LED4
PTC2
PTB4
PTE5
PTI4
PTH6
