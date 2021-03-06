#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
//#include <time.h>
#include <string.h>
#include "tetris.h"

#include <display.h>
#include <printf.h>
#include <keyboard.h>
#include <uart.h>

#define HARD_RAND

int main()
{
    int time = 0; //タイマをリセット
    int i, x, y;

    total_point = 0;
    for(x = 0; x < 21; x++) {
      for(y = 0; y < 12; y++) {
	background[x][y] = 0;
	view_data[x][y] = 0;
      }
    }
    
    uart_init();
    display_clear(DISPLAY_COLOR_RED);
    
    init();

    //ゲームオーバーになるまで無限ループ
    while (1) {
        block_operate();
	for (i = 0; i < 1000000; i++) {
	  asm volatile("nop");
	}

	//ブロックを１マス下げる
	if (!block_drop()) {
	  //ゲームオーバーの場合
	  break;
	}
    }

    view_gameover();
    return 0;
}

//ゲーム起動直後の初期設定。
//画面と壁のデータを初期化
void init()
{
    int row, col;
    
    //壁を初期化
    for(row = 0; row < NUM_ROWS ; row++) {
        background[row][0] = 2;            //左壁
        background[row][NUM_COLS + 1] = 2; //右壁
    }

    //最下段は壁
    for(col = 0; col < NUM_COLS + 2; col++) {
        background[NUM_ROWS][col] = 2; //壁
    }

    copy_whole(background, view_data);
    block_new(); //最初のブロックを生成
    
    view_render();
}

//新しいブロックを生成して次のブロックに発生させる
//@return 0:ゲームオーバー, 1:ゲーム続く
int block_new()
{
    int r, c; //forループ制御用の変数
    int cell;
    
    //まずブロックの座標を初期位置にリセット
    myblock.pos.c = 4;
    myblock.pos.r = 0;

    //ブロックの種類。ランダムに決まる。
    int new_block_type = rand_block_type();
    //ブロックパターンの中からblock_typeに応じた種類のブロックを読み込む
    copy_block(block_patterns[new_block_type], myblock.pattern);

    //壁＋ブロックをフィールドへ
    //TODO:ここの処理がわかりにくい。直感的でない。
    for(r = 0; r<4; r++) {
        for(c = 0 + myblock.pos.c ; c< 4 +  myblock.pos.c ; c++) {
            cell = background[r][c] + myblock.pattern[r][c - myblock.pos.c];
            //登場した新ブロックが既に固定ブロックに重なっていればゲームオーバー
            if (cell == 2) {
                return 0;
            }
            view_data[r][c] = cell; // 0 or 1
        }
    }

    return 1;
}

//ブロックパターンをコピーする
void copy_block(int src[4][4], int dst[4][4])
{
    int i,j;
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            dst[i][j] = src[i][j];
        }
    }
}

#ifdef HARD_RAND
#define rand() (hard_rand())

uint32_t hard_rand(void)
{
  static uint32_t y = 2463534242;
  asm volatile("rand %0, %1" : "=r"(y) : "r"(y));
  return y;
}
#else
#define rand() (xorshift())

uint32_t xorshift(void)
{
  static uint32_t x = 123456789;
  static uint32_t y = 362436069;
  static uint32_t z = 521288629;
  static uint32_t w = 88675123;
  uint32_t t;

  t = x ^ (x << 11);
  x = y; y = z; z = w;
  return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
}
#endif

//乱数を発生させ、その乱数を7で割った余り（０〜６まで）でブロックの種類を決定
int rand_block_type()
{
  //srand((unsigned)time(NULL));
    return rand() % 7;
}

//キー入力を受けてブロックを操作する(移動/回転)
void block_operate()
{
  char key = keyboard_get();// = getch(); //キーから一文字入力
  printf("key: %d\n", key);

    //キーに応じて各方向へブロックを移動したり、回転させたりする
    switch(key) {

        case 0x2d: // move right
            if(!is_attached(+1, 0)) {
                block_move(+1, 0);
            }
            break;

        case 0x1d: // move left
            if(!is_attached(-1, 0)) {
                block_move(-1, 0);
            }
            break;
        case 0x24: // move down
            if(!is_attached(0, +1)) {
                block_move(0, +1);
            }
            break;
        case 0x25: // rotate
            block_rotate();
	    break;
    case 0xf0:
      block_block_operate();
      break;
    default:
      break;

    }
}

//重なり検査
//落下中のブロックが壁や固定済みブロックに接触していないか判別
//ブロックが向かう位置に、固定ブロックもしくは壁があるかどうかを検査
int is_attached(int dx, int dy)
{
    int r, c;

    for(r = 0; r<4; r++) {
        for(c = 0; c<4; c++) {
            if(myblock.pattern[r][c] &&
                background[myblock.pos.r + dy + r][myblock.pos.c + dx + c]) {
                    return 1;
            }
        }
    }
    return 0;
}

//落下中ブロックを移動
void block_move(int dx, int dy)
{
    int r, c;

    //一度ブロックを消して
    for(r = 0; r<4; r++) {
        for(c = 0; c<4; c++) {
            view_data[myblock.pos.r+r][myblock.pos.c+c] -= myblock.pattern[r][c];
        }
    }
    //ブロックの座標を更新
    myblock.pos.c += dx;
    myblock.pos.r += dy;

    //新しい座標にブロックを入れなおし
    for(r = 0; r<4; r++) {
        for(c = 0; c<4; c++) {
            view_data[myblock.pos.r+r][myblock.pos.c+c] += myblock.pattern[r][c];
        }
    }

    view_render();
}

//ブロックを回転する
int block_rotate()
{
    int i, j; //forループ制御用
    int temp[4][4]; //ブロックを一時保存するための配列

    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
	  temp[i][j] = 0;
	}
    }

    //ブロックを回転する前にtempにセーブ
    copy_block(myblock.pattern, temp);

    //ブロックを回転
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            myblock.pattern[i][j] = temp[3-j][i];
        }
    }
    //重なってるブロックが出てしまったらブロックを回転前に戻して中止
    if(is_attached(0, 0)) {
        copy_block(temp, myblock.pattern);
        return 1;
    }

    //一旦フィールドからブロック消して回転後のブロックを再表示
    for(i = 0; i<4; i++) {
        for(j = 0; j<4; j++) {
            view_data[myblock.pos.r+i][myblock.pos.c+j] -= temp[i][j];
            view_data[myblock.pos.r+i][myblock.pos.c+j] += myblock.pattern[i][j];
        }
    }

    view_render();

    return 0;
}

//ブロックを落下させる。
//下に接地した場合はその位置に固定
int block_drop()
{
    //重なりあるか判定
    if(!is_attached(0, +1)) {
        // なければ下に移動
        block_move(0, +1);
    } else {
        // あれば壁にする
        int num_lines = block_lock();
        total_point += add_points[num_lines];
        
        //新ブロックを生成
        if (block_new()) {
            view_render();
        } else {
            //生成時点で接地したらゲームオーバー
            return 0;
        }
    }
    
    return 1;
}

void copy_whole(int src[21][12], int dst[21][12])
{
    int i,j;

    //ブロックを壁に加える
    for(i = 0; i<NUM_ROWS + 1; i++) {
        for(j = 0; j<NUM_COLS + 2; j++) {
            dst[i][j] = src[i][j];
        }
    }
}

//着地したブロックを固定し、横一列がそろってるかの判定を呼び出す
int block_lock()
{
    int i, j, num_lines;

    //ブロックを背景に合体させる
    copy_whole(view_data, background);
    
    //横一列がそろってるか判定して処理する関数を呼ぶ
    num_lines = check_and_delete();

    //列完成判定後の背景を画面データへ反映
    copy_whole(background, view_data);

    //消したラインの数
    return num_lines;
}

//横一段が完成しているか検査。
//そろっていればその段を消して上のブロック群を下ろす
int check_and_delete()
{
    int row, col, k; //forループ制御用
    int is_complete; //横一列がそろっていれば１、一つでも隙間があると０になる
    int num_lines = 0; //同時に消したラインの数

    while(1) {
        for(row = 0; row<NUM_ROWS; row++) {
            is_complete = 1;

            for(col = 1; col<NUM_COLS + 1; col++) {
                if(background[row][col] == 0) {
                    is_complete = 0;
                }
            }
            if( is_complete) break;
        }
        if (! is_complete) break;

        num_lines++;


        // 列を消去
        for(col = 1; col< NUM_COLS + 1; col++) {
            background[row][col] = 0;
        }


        //消えた列より上にあった固定ブロックを列の消えたところへ下ろす
        for(k = row; k>0; k--) {
            for(col = 1; col< NUM_COLS + 1; col++) {
                background[k][col] = background[k-1][col];
            }
        }
    }

    return num_lines;
}

//画面表示
//view_data[][]の内容を画面に出力する
void view_render()
{
  int r, c, i, j;
    char *cell;
    
    char output[25*30] ="";
    char offset [] = "        ";

    //view_dataを画面表示
    for(r = 0; r<NUM_ROWS + 1; r++) {
      printf(offset);

        for(c = 0; c < NUM_COLS + 2; c++) {
            //1セル追加
            cell = (char *)tiles[view_data[r][c]];
            printf(cell);

	    for(i = 0; i < 20; i++) {
	      for(j = 0; j < 20; j++) {
		if(*cell == '#')
		  display_set_pixel(c * 20 + i, r * 20 + j, DISPLAY_COLOR_WHITE);
		else if(*cell == '_')
		  display_set_pixel(c * 20 + i, r * 20 + j, DISPLAY_COLOR_BLUE);
		else
		  display_set_pixel(c * 20 + i, r * 20 + j, DISPLAY_COLOR_BLACK);
	      }
	    }
        }
        //1段終わり
        printf("\n");
    }

    view_clear();
    //printf("%s", output);

    printf("              Total Point : %d\n", total_point);
}


//ゲームオーバー画面を表示する
void view_gameover()
{
    printf("\n              GAME OVER  \n");
}

//画面を全消去
void view_clear()
{
  //display_clear(DISPLAY_COLOR_BLACK);
  //system("cls");
}

 
