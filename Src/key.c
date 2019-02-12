#include "key.h"

/**
  * @brief Key processing function
	* @param mode: 0,Continuous press is not supported 1,Support continuous press
  * @retval Return Key value 0,No key presses n,presses n key
	* @use Note that this function has a response priority, KEY0 > KEY1 > KEY2 > WK_UP
  */

uint8_t KEY_Scan(uint8_t mode)
{
    static uint8_t key_up=1;                        //key release sign
    if(mode==1)key_up=1;                            //Support continuous press
    if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
    {
        HAL_Delay(10);
        key_up=0;
        if(KEY0==0)       return KEY0_PRES;
        else if(KEY1==0)  return KEY1_PRES;
        else if(WK_UP==1) return WKUP_PRES;          
    }else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1;
    return 0;                                       //No key press
}



