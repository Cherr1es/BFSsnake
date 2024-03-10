#include <graphics.h>
#include <conio.h>
#include <queue>

#define width 40 //横向40格子
#define height 30 // 纵向30格子
#define size 20 //格子尺寸

using namespace std ;


typedef struct point //BFS步数
{
    int x , y , step ;
    point ( int _x , int _y , int _step ) : x ( _x ) , y ( _y ) , step ( _step ) {} //初始化
}P;

typedef struct Coordinates
{
    int fx , fy , hx , hy ; //记录坐标
}coor;

int Sblocks [ width ] [ height ] = { 0 } ; //蛇区块,赋予的值代表此坐标是蛇的第个？区块
char moveDirection ; //移动方向
int foodX , foodY ; //食物坐标
bool shouldExit = false ; //结束游戏
int tra [ width ] [ height ] = { 0 } ; // BFS标记

void initGame ( void ) //游戏初始化
{
    initgraph ( width * size , height * size ) ; //初始化窗口
    setbkcolor ( LIGHTGRAY ) ; //颜色
    cleardevice ( ) ;

    setlinecolor ( WHITE ) ; //划线
    int i ;
    for ( i = size ; i < width * size ; i += size )
    {
        line( i , 0 , i , width * size ) ;

    }
    for ( i = size ; i < height * size ; i += size )
    {
        line ( 0 , i , width * size , i ) ;
    }

    Sblocks [width / 2 ] [height / 2 ] = 1 ; //初始化蛇头位于中心
    for ( i = 1 ; i < 4 ; i ++ ) //蛇初始长度为4
    {
        Sblocks[width / 2 - i][height / 2] = 1 + i;
    }

    foodX = 25 ; // 第一次生成位置固定
    foodY = 25 ;
}

void show ( void ) // 绘制图像
{
    for ( int i = 0 ; i < width ; i ++ )
    {
        for ( int t = 0 ; t < height ; t ++ )
        {
            if (Sblocks [ i ] [ t ] != 0 )
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

void movesnake (  ) //移动逻辑
{
    int i, t ;
    int oldheadX , oldheadY , oldtailX , oldtailY ; //移动后旧蛇头和蛇尾的坐标
    int tailSblocks = 0 ; //蛇尾区块

    //移动逻辑
    for ( i = 0 ; i < width ; i++ )
    {
        for ( t = 0 ; t < height ; t++ )
        {
            if ( tailSblocks < Sblocks [ i ] [ t ] )
            {
                tailSblocks = Sblocks [ i ] [ t ] ; //赋值
                oldtailX = i ;
                oldtailY = t ;
            }
            if ( Sblocks [ i ] [ t ]  == 2 ) //旧的蛇头区块的值一定是2
            {
                oldheadX = i ;
                oldheadY = t ;
            }
        }
    }
    int newheadX = oldheadX , newheadY = oldheadY ;

    switch ( moveDirection ) //移动逻辑
    {
        case 'w' :
            newheadY -= 1 ;
            break ;
        case 's' :
            newheadY += 1 ;
            break ;
        case 'a' :
            newheadX -= 1 ;
            break ;
        case 'd' :
            newheadX += 1 ;
            break ;
    }

    if ( newheadX == oldtailX && newheadY == oldtailY ) //新的蛇头位置不与当前蛇尾位置重叠
    {
        shouldExit = 1 ;
    }

    if (   newheadX >= width  || newheadX < 0
        || newheadY >= height || newheadY < 0
        || Sblocks [ newheadX ] [ newheadY ] != 0 ) //越界与自食直接判断失败
    {
        shouldExit = 1 ;
    }

    Sblocks [ newheadX ] [ newheadY ] = 1 ; //更新蛇头坐标
    if ( newheadX == foodX && newheadY == foodY ) //若吃到食物则不更改蛇尾坐标，而重新生成食物
    {
        foodX = rand ( ) % ( width - 2 ) + 1 ;
        foodY = rand ( ) % ( height - 2 ) + 1 ;
    }
    else //反之则不重新生成新的食物，而更新蛇尾
    {
        Sblocks [ oldtailX ] [ oldtailY ] = 0 ;
    }
}

void BFS (   )  //自动寻路核心
{
    queue <coor> q ;
    int dx [ 4 ] = { 0 , 0 , -1 , 1 } ; // 上 下 左 右
    int dy [ 4 ] = { -1 , 1 , 0 , 0 } ;

    int ox ;//食物初始坐标
    int oy ;
    int ohX = width/2 ; //蛇头初始坐标
    int ohY = height/2 ;
    int headstep = -1 ; //步数

    q.push ( P ( ox , oy , 0 ) ) ;
    tra [ ox ] [ oy ] = 1 ; //起点已访问

    while ( !q.empty ( ) )
    {
        P tmp = q.front ( ) ;
        q.pop ( ) ;

        for ( int i = 0 ; i < 4 ; i++ )
        {
            int nfX = tmp.x + dx [ i ] ;
            int nfY = tmp.y + dy [ i ] ;

            if ( nfX >= 0 && nfX < width
              && nfY >= 0 && nfY < height
              && tra [ nfX ] [ nfY ] == 0 )
            {
                q.push ( P ( nfX , nfY , tmp.step + 1 ) ) ;
                tra [ nfX ] [ nfY ] = 1 ;
            }

            if ( nfX == ohX && nfY == ohY )
            {
                headstep = tmp.step + 1 ;
            }
        }
    }
    while ( headstep != -1 )
    {
        for ( int t = 0 ; t < 4 ; t ++ )
        {
            int nhX = ohX + dx [ t ] ;
            int nhY = ohY + dy [ t ] ;

            if ( nhX >= 0 && nhX < width
              && nhY >= 0 && nhY < height
              && q.back ( ).step == headstep )
            {

                headstep -- ;
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
    Sleep ( 50 ) ; // 每50ms检测一次游戏状态
    BFS ( ) ;
}

int main ( void )
{
    initGame ( ) ;
    show ( ) ;
    movesnake() ;
    while ( !shouldExit )
    {
        show ( ) ;
        updateWithoutInput ( ) ;
    }

    return 0 ;
}