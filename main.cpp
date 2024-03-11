#include <graphics.h>
#include <conio.h>
#include <queue>
#include <iostream>

#define width 40 //横向40格子
#define height 30 // 纵向30格子
#define size 20 //格子尺寸

using namespace std ;


typedef struct point //BFS坐标
{
    int x , y , step ;
    point ( int _x , int _y , int _step ) : x ( _x ) , y ( _y ) , step ( _step ) {} //初始化
}P;

int Sblocks [ width ] [ height ] = { 0 } ; //蛇区块,赋予的值代表此坐标是蛇的第个？区块 ,用于绘制
int tra     [ width ] [ height ] = { 0 } ; // BFS标记
int Step    [ width ] [ height ] = { 0 } ; //标记步数
int foodX , foodY ; //食物坐标
int headX , headY ; //蛇头坐标
bool shouldExit = false ; //结束游戏

void initGame ( void ) //游戏初始化
{
    initgraph ( width * size , height * size ) ; //初始化窗口
    setbkcolor ( LIGHTGRAY ) ; //颜色
    cleardevice ( ) ;

    setlinecolor ( WHITE ) ; //划线
    int i ;
    for ( i = size ; i < width * size ; i += size )
    {
        line (i ,0 ,i ,width * size ) ;

    }
    for ( i = size ; i < height * size ; i += size )
    {
        line (  0 ,  i ,  width * size , i ) ;
    }

    Sblocks [ width / 2 ] [ height / 2 ] = 1 ; //初始化蛇头位于中心
    for ( i = 1 ; i < 4 ; i ++ ) //蛇初始长度为4
    {
        Sblocks [ width / 2 - i ] [ height / 2 ] = 1 + i ;
    }
    headX = width  /2 ; //初始蛇头坐标
    headY = height /2 ;
    foodX = 25 ; // 第一次生成位置固定
    foodY = 25 ;
}

void show ( void ) // 绘制图像
{
    for ( int i = 0 ; i < width ; i ++ )
    {
        for ( int t = 0 ; t < height ; t ++ )
        {
            if ( Sblocks [ i ] [ t ] != 0 )
            {
                setfillcolor ( LIGHTRED ) ;
            }
            else
            {
                setfillcolor ( LIGHTGRAY ) ;
            }
            fillrectangle ( i * size , t * size , ( i + 1 ) * size , ( t + 1 ) * size ) ;
        }
    }
    setfillcolor ( LIGHTGREEN ) ;
    fillrectangle ( foodX * size , foodY * size , ( foodX + 1 ) * size , ( foodY + 1 ) * size ) ;
    if ( shouldExit )
    {
        exit ( 0 ) ;
    }
}

void movesnake ( int _hx , int _hy ) //移动逻辑
{
    int i, t ;
    int oldtailX , oldtailY ; //移动后旧蛇尾的坐标
    int tailSblocks = 0 ; //蛇尾区块

    for ( i = 0 ; i < width ; i ++ ) //所有蛇区块加1
    {
        for ( t = 0 ; t < height ; t ++ )
        {
            if ( Sblocks [ i ] [ t ] != 0 )
            {
                 Sblocks [ i ] [ t ] ++ ;
            }
        }
    }

    for ( i = 0 ; i < width ; i++ )
    {
        for ( t = 0 ; t < height ; t++ )
        {
            if ( tailSblocks <= Sblocks [ i ] [ t ] ) //蛇尾一定是最大的
            {
                 tailSblocks = Sblocks [ i ] [ t ] ;
                 oldtailX = i ;
                 oldtailY = t ;
            }
        }
    }

    int newheadX = _hx , newheadY = _hy ; //更新蛇头

    if (   newheadX >= width  || newheadX < 0
        || newheadY >= height || newheadY < 0 ) //越界
    {
        shouldExit = 1 ;
        cout << "游戏结束！原因：新蛇头越界" << endl ;
    }

    if ( Sblocks [ newheadX ] [ newheadY ] != 0 ) //自食
    {
        shouldExit = 1 ;
        cout << "游戏结束！原因：自食" << endl ;
    }
    Sblocks [ newheadX ] [ newheadY ] = 1 ; //更新蛇头坐标

    if ( newheadX == foodX && newheadY == foodY ) //若吃到食物则不更改蛇尾坐标，而重新生成食物
    {
        foodX = rand ( ) % ( width  -  2 ) + 1 ;
        foodY = rand ( ) % ( height -  2 ) + 1 ;
    }
    else //反之则不重新生成新的食物，而更新蛇尾
    {
        Sblocks [ oldtailX ] [ oldtailY ] = 0 ;
    }
    show ( ) ;
}

void BFS ( void ) //BFS逻辑
{
    queue < P > q ; //BFS队列
    int headstep = 0 ; //步数
    q.push (P (foodX, foodY, 0 ) ) ; //从食物出发BFS蛇头
    tra [ foodX ] [ foodY ] = 1 ; //起点已访问

    int dx [ 4 ] = {0 ,0 ,-1,1 } ; // 上 下 左 右
    int dy [ 4 ] = {-1,1 ,0 ,0 } ;

    while  ( !q.empty ( ) ) //BFS逻辑
    {
        P tmp = q.front ( ) ;
        q.pop ( ) ;

        for ( int n = 0 ; n < 4 ; n++ )
        {
            int newfoodX = tmp.x + dx [ n ] ;
            int newfoodY = tmp.y + dy [ n ] ;

            if ( newfoodX >= 0 && newfoodX < width
              && newfoodY >= 0 && newfoodY < height
              && tra     [ newfoodX ] [ newfoodY ] == 0
              && Sblocks [ newfoodX ] [ newfoodY ] == 0 ) // BFS路径规则
            {
                q.push (P( newfoodX, newfoodY, tmp.step + 1 ) ) ;
                Step [ newfoodX ] [ newfoodY ] = q.front().step ; //标记步数
                tra  [ newfoodX ] [ newfoodY ] = 1 ;
            }

            if ( newfoodX == headX && newfoodY == headY ) //抵达目标
            {
                headstep = tmp.step ;
                while ( headstep > 0 )
                {
                    for ( int m = 0 ; m < 4 ; m++ )
                    {
                        int newheadX = headX + dx [ m ] ;
                        int newheadY = headY + dy [ m ] ;

                        if ( newheadX >= 0 && newheadX < width
                          && newheadY >= 0 && newheadY < height
                          && Step [ newheadX ] [ newheadY ] == headstep -1 )
                        {
                            Sleep ( 150 ) ; // 每150ms更新一次
                            headX = newheadX , headY = newheadY ; //更新蛇头坐标
                            movesnake ( newheadX , newheadY ) ; //提供蛇头坐标与移动方向
                            headstep -- ;
                        }
                    }
                }
            }
        }
    }
}

void updateWithoutInput ( void ) //与输入无关的更新
{
    if ( shouldExit )
    {
        return ;
    }
    BFS ( ) ;
}

int main ( void )
{
    initGame ( ) ;
    while ( !shouldExit )
    {
        updateWithoutInput ( ) ;
    }
    return 0 ;
}