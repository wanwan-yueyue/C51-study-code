#include <REGX52.H>
#include "Timer0.h"
#include "NonBlockKey.h"
#include "MatrixLED.h"
#include "delay.h"
#include "score.h"
#include "stdlib.h"

//点阵屏参数：8*8
#define GAME_WIDTH		8		//8列
#define GAME_HEIGHT		8		//8行

//方块状态、形状数组
//格式：[形状类型][旋转状态][行][列]
unsigned char code Block[4][4][3][3] = {
		//1.单点方块
		{
			//r0
		 {
			{0,0,0},
			{0,1,0},
			{0,0,0}
		 },
		  //r90
		 {
			{0,0,0},
			{0,1,0},
			{0,0,0}
		 },
			//r180
		 {
			{0,0,0},
			{0,1,0},
			{0,0,0}
		 },
			//r270
		 {
			{0,0,0},
			{0,1,0},
			{0,0,0}
		 },
		},
		
		//2.双点方块
		{
			//r0
		 {
			{0,0,0},
			{1,1,0},
			{0,0,0}
		 },
			//r90
		 {
			{0,1,0},
			{0,1,0},
			{0,0,0}
		 },
			//r180
		 {
			{0,0,0},
			{0,1,1},
			{0,0,0}
		 },
			//r270
		 {
			{0,0,0},
			{0,1,0},
			{0,1,0}
		 },
		},
		
		//3.L形三点方块
		{
			//r0
		 {
			{0,1,0},
			{0,1,1},
			{0,0,0}
		 },
			//r90
		 {
			{0,0,0},
			{0,1,1},
			{0,1,0}
		 },
			//r180
		 {
			{0,0,0},
			{1,1,0},
			{0,1,0}
		 },
			//r270
		 {
			{0,1,0},
			{1,1,0},
			{0,0,0}
		 },
		},
		
		//4.T形四点方块
		{
			//r0
		 {
			{0,1,0},
			{1,1,1},
			{0,0,0}
		 },
			//r90
		 {
			{0,1,0},
			{0,1,1},
			{0,1,0}
		 },
			//r180
		 {
			{0,0,0},
			{1,1,1},
			{0,1,0}
		 },
			//r270
		 {
			{0,1,0},
			{1,1,0},
			{0,1,0}
		 },
		}

};

//游戏状态变量
//1.标志类变量
bit isGameOver = 0;																									//游戏结束标志
bit isPaused = 0;																										//游戏暂停标志
bit blockFallFlag = 0;																							//方块下落标志
bit gameOverAnimState = 0;																					//游戏结束动画状态
//2.参数类变量
unsigned char gameArea[GAME_WIDTH + 1][GAME_HEIGHT + 1] = {0};			//游戏区域参数（0：空，1：方块）
unsigned char curBlockType;																					//当前方块类型（0-3）
unsigned char curRotate;																						//当前方块旋转类型（0-3）
unsigned char curX, curY;																						//当前方块坐标
unsigned char nextBlockType;																				//下一个方块类型
unsigned char newY, newX;																						//更新后的方块坐标
unsigned char key;																									//读取按键键码
unsigned int gameOverAnimCount = 0;																	//游戏结束动画计数器


/**
 * @brief  初始化游戏状态
 * @param  无
 * @retval 无
 */
void GameInit(){
		unsigned char col, row;
    isGameOver = 0;
    isPaused = 0;
    gameOverAnimCount = 0;
    gameOverAnimState = 0;
		Score_Init();				//积分系统初始化
    Timer0Init();				//定时器初始化
    MatrixLED_Init();		//8*8点阵屏初始化
    srand(TH0);					//以定时器初始值作为随机数种子
    
    //清空游戏区域
    for(col = 0; col < GAME_WIDTH; col++){
        for(row = 0; row < GAME_HEIGHT; row++){
            gameArea[col][row] = 0;
        }
    }
    
    //初始化方块生成
    curBlockType = rand() % 4;
    curRotate = rand() % 4;
    curX = 3;			//X初值
    curY = 7;			//Y初值
    nextBlockType = rand() % 4;
}

/**
 * @brief  绘制游戏画面
 * @param  无
 * @retval 无
 */
void DrawGame(){
    unsigned char frame[8] = {0};  //帧缓存，8列，每列1字节（bit7=最上行，bit0=最下行）
    unsigned char col, row, r, c, realCol, realRow;
		
    //1.绘制游戏区域中已固定的方块
    for(col = 0; col < GAME_WIDTH; col++){
        for(row = 0; row < GAME_HEIGHT; row++){
            if(gameArea[col][row] == 1){
                //行号row对应bit位（row=0→bit0，row=7→bit7），置1表示点亮
                frame[col] |= (1 << row);
            }
        }
    }
    
    //2.绘制当前活动方块（3x3矩阵）
    for(r = 0; r < 3; r++){  //遍历方块的3行
        for(c = 0; c < 3; c++){  //遍历方块的3列
            //若方块当前位置有像素（1表示有方块）
            if(Block[curBlockType][curRotate][r][c] == 1){
                //计算实际坐标（以方块中心为基准偏移，3x3矩阵中心在(1,1)）
                realCol = curX + (c - 1);  //列坐标：当前列 + 列偏移
                realRow = curY + (r - 1);  //行坐标：当前行 + 行偏移
                
                //确保坐标在8x8点阵范围内（防止越界）
                if(realCol < GAME_WIDTH && realRow < GAME_HEIGHT){
                    frame[realCol] |= (1 << realRow);  //点亮对应像素
                }
            }
        }
    }
    
    //3.显示最终帧缓存
    MatrixLED_ShowFrame(frame);
}

/**
 * @brief  检测方块移动到目标位置是否碰撞
 * @param  x 目标X坐标
 * @param  y 目标Y坐标
 * @retval 1-碰撞，0-无碰撞
 */
bit CheckCollision(unsigned char x, unsigned char y){
    unsigned char r, c;
    for(r = 0; r < 3; r++){  //遍历方块3x3矩阵
        for(c = 0; c < 3; c++){
            if(Block[curBlockType][curRotate][r][c] == 1){  //方块当前位置有像素
                unsigned char realCol = x + (c - 1);  //计算实际列坐标
                unsigned char realRow = y + (r - 1);  //计算实际行坐标
                
                //检测是否超出左右边界（列0-7）
                if(realCol >= GAME_WIDTH || realCol < 0){
                    return 1;  //超出边界，碰撞
                }
                
                //检测是否超出下边界（行0-7），允许超出上边界
                if(realRow > GAME_HEIGHT){
                    return 1;  //超出下边界，碰撞
                }
                
                //只有当行坐标在有效范围内时，才检测与已固定方块的碰撞
                if(realRow >= 0 && gameArea[realCol][realRow] == 1){
                    return 1;  //重叠，碰撞
                }
            }
        }
    }
    return 0;  //无碰撞
}

/**
 * @brief  将当前活动方块固定到游戏区域
 * @param  无
 * @retval 无
 */
void FixBlock(){
    unsigned char r, c;
    for(r = 0; r < 3; r++){
        for(c = 0; c < 3; c++){
            if(Block[curBlockType][curRotate][r][c] == 1){
                unsigned char realCol = curX + (c - 1);
                unsigned char realRow = curY + (r - 1);
                //在有效区域内固定方块
                if(realCol < GAME_WIDTH && realRow < GAME_HEIGHT){
                    gameArea[realCol][realRow] = 1;  //标记为固定方块
                }
            }
        }
    }
}

/**
 * @brief  检查并消除已满的行
 * @param  无
 * @retval 消除的行数
 */
unsigned char ClearFullLines(){
    unsigned char row, col, r;
    unsigned char linesCleared = 0, isFull = 0;
    
    //从底部向上检查每一行
    for(row = 0; row < GAME_HEIGHT; row++){
        isFull = 1;
       
        //检查当前行是否填满
        for(col = 0; col < GAME_WIDTH; col++){
            if(gameArea[col][row] == 0){
                isFull = 0;
                break;
            }
        }
        
        //如果行已满，消除该行并将上方所有行下移
        if(isFull){
            linesCleared++;
            
            //将当前行上方所有行下移一行
            for(r = row; r < GAME_HEIGHT - 1; r++){
                for(col = 0; col < GAME_WIDTH; col++){
                    gameArea[col][r] = gameArea[col][r + 1];
                }
            }
            
            //顶部行清空
            for(col = 0; col < GAME_WIDTH; col++){
                gameArea[col][GAME_HEIGHT - 1] = 0;
            }
            
            //重新检查当前行（因为上方行下移后可能又填满了）
            row--;
        }
    }
		
		if(linesCleared > 0){
        Score_Add(linesCleared);  //消除行后增加积分
    }
    
    return linesCleared;
}

/**
 * @brief  生成新的活动方块
 * @param  无
 * @retval 无
 */
void GenerateNewBlock(){
    //从下一个方块类型更新当前方块
    curBlockType = nextBlockType;
    curRotate = rand() % 4;  //随机旋转状态
    curX = 3;  //初始X位置
    curY = 7;  //初始Y位置（顶部区域）
    nextBlockType = rand() % 4;  //预生成下一个方块
    
    //若新方块初始位置就碰撞，游戏结束
    if(CheckCollision(curX, curY)){
        isGameOver = 1;
    }
}

/**
 * @brief  处理方块下落逻辑
 * @param  无
 * @retval 无
 */
void BlockFall(){
    if(isGameOver || isPaused){  //游戏结束或暂停时不处理
        return;
    }
    
    //计算下落后的Y坐标
    newY = curY - 1;
    
    if(!CheckCollision(curX, newY)){  //无碰撞，允许下落
        curY = newY;
    }else{  //有碰撞，固定当前方块并生成新方块
        FixBlock();
        ClearFullLines();  //检查并消除已满的行
        GenerateNewBlock();
    }
}
/**
 * @brief  方块左移
 * @param  无
 * @retval 无
 */
void BlockMoveLeft(){
    if(isGameOver || isPaused) return;  //游戏结束或暂停时不响应
    newX = curX - 1;      //计算左移后的X坐标
    if(!CheckCollision(newX, curY)){    //检测无碰撞则移动
        curX = newX;
    }
}

/**
 * @brief  方块右移
 * @param  无
 * @retval 无
 */
void BlockMoveRight(){
    if(isGameOver || isPaused) return;
    newX = curX + 1;      //计算右移后的X坐标
    if(!CheckCollision(newX, curY)){    //检测无碰撞则移动
        curX = newX;
    }
}

/**
 * @brief  方块下移
 * @param  无
 * @retval 无
 */
void BlockMoveDown(){
    if(isGameOver || isPaused) return;
    newY = curY - 1;      //计算下移后的X坐标
    if(!CheckCollision(newX, curY - 1)){    //检测无碰撞则移动
        curY = newY;
    }
}

/**
 * @brief  方块旋转（顺时针）
 * @param  无
 * @retval 无
 */
void BlockRotate(){
		unsigned char oldRotate;
    if(isGameOver || isPaused) return;
    oldRotate = curRotate;          //保存当前旋转状态
    curRotate = (curRotate + 1) % 4;              //计算新旋转状态（0-3循环）
    if(CheckCollision(curX, curY)){               //若旋转后碰撞
        curRotate = oldRotate;                    //恢复原旋转状态
    }
}

/**
 * @brief  处理键盘输入
 * @param  key 按键值（1-16，来自MatrixKey()）
 * @retval 无
 */
void HandleKeyInput(unsigned char key){
    switch(key){
        case 5:  //按键5：左移
            BlockMoveLeft();
            break;
        case 7:  //按键7：右移
            BlockMoveRight();
            break;
				case 6:	 //按键6：下移
						BlockMoveDown();
						break;
        case 2:  //按键2：旋转
            BlockRotate();
            break;
        case 14: //按键4：暂停/继续（可选）
            isPaused = ~isPaused;
            break;
    }
}

/**
 * @brief  游戏结束动画
 * @param  无
 * @retval 无
 */
void GameOverAnimation(){
    unsigned char frame[8] = {0};
    unsigned char col;
    
    //根据动画状态设置帧
    for(col = 0; col < 8; col++){
        //交替显示列
        if((col % 2) == gameOverAnimState){
            frame[col] = 0xFF;  //点亮整列
        }else{
            frame[col] = 0x00;  //熄灭整列
        }
    }
    
    MatrixLED_ShowFrame(frame);
}

/**
 * @brief  重置游戏（开始新一局）
 * @param  无
 * @retval 无
 */
void ResetGame(){
    unsigned char col, row;
    
    //清空游戏区域
    for(col = 0; col < GAME_WIDTH; col++){
        for(row = 0; row < GAME_HEIGHT; row++){
            gameArea[col][row] = 0;
        }
    }
    
    //重新初始化游戏状态
    isGameOver = 0;
    isPaused = 0;
		Score_Init();
    
    //生成新方块
    curBlockType = rand() % 4;
    curRotate = rand() % 4;
    curX = 3;
    curY = 7;
    nextBlockType = rand() % 4;
}

void main(){
    //游戏初始化
    GameInit();
    
    //游戏主循环
    while(1){
        //处理定时下落
        if(blockFallFlag == 1){
            blockFallFlag = 0;
            BlockFall();
        }
        
        //处理键盘输入
        key = NonBlockKey_GetKey();  //使用非阻塞键盘函数
        if(key != 0){
            if(isGameOver){
                //游戏结束时，按16号键重新开始
                if(key == 16){
                    ResetGame();
                }
            }else{
                HandleKeyInput(key);
            }
        }
        
        //绘制画面
        if(isGameOver){
            GameOverAnimation();
        }else{
            DrawGame();
        }
    }
}

//定时器中断函数
void Timer0_Routine() interrupt 1{
    static unsigned int T0Count;
    static unsigned int animCount;
    
    //重新赋初值保证计时为1ms
    TL0 = 0x66;				//设置定时初始值
    TH0 = 0xFC;				//设置定时初始值
    
    //键盘扫描（每1ms扫描一次）
    NonBlockKey_Scan();
    
    if(isGameOver){
        //游戏结束时更新动画计数器
        animCount++;
        if(animCount >= 200){  //约200ms切换一次状态
            animCount = 0;
            gameOverAnimState = ~gameOverAnimState;
        }
    }else{
        //正常游戏时处理方块下落
        T0Count++;
        if (T0Count >= Score_GetFallInterval()){
            T0Count = 0; //初始化计数
            blockFallFlag = 1; //置位下落标志
        }
    }
}