#ifndef __NIXIE_H__
#define __NIXIE_H__

/**
 * @brief 数码管显示函数
 * @param location 数码管位置(1-8)
 * @param number 要显示的数字(0-9)，超过9将显示横杠
 */
void NixieDisplay(unsigned char location, unsigned char number);

#endif
