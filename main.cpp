#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>//for random
#include <iostream>//for debug writing
#include <fstream>//ifstream, ofstream
#include <string>//useful for reading and writing

#define MAX_IMAGESIZE 1280/*想定する縦・横の最大画素数*/
#define MAX_BRIGHTNESS  255/*想定する最大階調値*/
#define GRAYLEVEL 256/*想定する階調数（=最大階調値+1）*/
#define MAX_FILENAME 256/*想定するファイル名の最大長*/
#define MAX_BUFFERSIZE 256/*利用するバッファ最大長*/
#define MAX_NUM_OF_IMAGES 5/*利用する画像の枚数*/
/*大域変数の宣言*/
/*画像データimage[n][x][y]*/
unsigned char image[MAX_NUM_OF_IMAGES][MAX_IMAGESIZE][MAX_IMAGESIZE];
unsigned char image2[MAX_NUM_OF_IMAGES][MAX_IMAGESIZE][147461];

#define COLOR_TYPE_INDEX 0  /**< インデックスカラー方式 */
#define COLOR_TYPE_GRAY  1  /**< グレースケール方式 */
#define COLOR_TYPE_RGB   2  /**< RGB方式 */
#define COLOR_TYPE_RGBA  3  /**< RGBA方式 */

typedef struct image_t {
    /*intで十分だが負の値はとらないので*/
  uint32_t width;       /**< 幅 */
  uint32_t height;      /**< 高さ */
  uint16_t color_type;  /**< 色表現の種別 */
  uint16_t palette_num; /**< カラーパレットの数 */
  //color_t *palette;     /**< カラーパレットへのポインタ */
  //pixcel_t **map;       /**< 画像データ */
} image_t;

typedef struct color_t {/*符号なし8bit整数の定義*/
  uint8_t r; /**< Red */
  uint8_t g; /**< Green */
  uint8_t b; /**< Blue */
  uint8_t a; /**< Alpha */
} color_t;

typedef union pixcel_t {
  color_t c; /**< RGBA */
  uint8_t g; /**< グレースケール */
  uint8_t i; /**< カラーインデックス */
} pixcel_t;


//#define SMPL 44100//1[s]あたり
#define SMPL 48000
//#define SMPL 11025
#define BIT  8

void wav_write(const char *filename, unsigned char *buffer, ::size_t size);

int main(){
    char file_name[MAX_FILENAME];/*ファイル名用の文字配列*/
    char copy_file_name[MAX_FILENAME];
    char buffer[MAX_BUFFERSIZE];/*データ読み込み用作業変数*/
    FILE *fp,*fq;/*ファイルポインタ*/
    int max_gray;/*最大階調値*/
    int x,y;/*ループ変数*/
  
    printf("入力ファイル名(*.pgm):");
    scanf("%s",file_name);
    if((fq=fopen(file_name,"rb"))==NULL){
        printf("その名前のファイルは存在しません．\n");
        exit(1);
    }

  /*width[n],height[n]の代入(#から始まるコメントは読み飛ばす)*/
    int width=0;
    int height=0;
    while(width==0||height==0){
      fgets(buffer,MAX_BUFFERSIZE,fq);
      if(buffer[0]!='#'){
        sscanf(buffer,"%d %d",&width,&height);
      }
    }
  
  /*max_grayの代入(#から始まるコメントは読み飛ばす)*/
    max_gray=0;
    while(max_gray==0){
        fgets(buffer,MAX_BUFFERSIZE,fq);
        if(buffer[0]!='#')
        sscanf(buffer,"%d",&max_gray);
    }

  /*画像データを読み込んで画像用配列に代入する*/
  /*データ4行目からファイルをすべてコピー*/
    /*for(y=0;y<height;y++){
        for(x=0;x<width;x++){
            if(y!=1||y!=2||y!=3){
                image2[1][x][y]=(unsigned char)fgetc(fp);
            }
        }
    }*/


    for(int a=0;a<256;a++){/*すべての行を読ませる*/
      for(int b=0;b<320;b++){
        for(int j=0;j<3;j++){
          image[j][b][a]=(unsigned char)fgetc(fq);
        }
      }
    }

    fclose(fq);

    double sig, p = 0;
    ::size_t size = 115 * (SMPL*(BIT/8));    // 24秒間 //BIT/8はByteを表す．sizeは[Byte]
    unsigned char *buf =(unsigned char *) malloc(2*size);//*bufには確保したメモリの先頭アドレスがunsigned char型で格納

    //printf("bufのアドレス：%p",buf);//ここのアドレスは*bufに格納された値の格納先を示す
    
    
    for(int i=0;i<SMPL*0.30;i++){
        p +=1900.0/SMPL;
        p -= floor(p);
        sig=sin(2.0*M_PI*p);
        // 8bit量子化してバッファへ 
        buf[i] = (unsigned char)(127 * sig) + 128;
    }

    for(int i=SMPL*0.30;i<SMPL*0.31;i++){
        p +=1200.0/SMPL;
        p -= floor(p);
        sig=sin(2.0*M_PI*p);
        // 8bit量子化してバッファへ 
        buf[i] = (unsigned char)(127 * sig) + 128;
    }


    for(int i=SMPL*0.31;i<SMPL*0.61;i++){
        p +=1900.0/SMPL;
        p -= floor(p);
        sig=sin(2.0*M_PI*p);
        // 8bit量子化してバッファへ 
        buf[i] = (unsigned char)(127 * sig) + 128;
    }

    for(int i=SMPL*0.61;i<SMPL*0.64;i++){
        p +=1200.0/SMPL;
        p -= floor(p);
        sig=sin(2.0*M_PI*p);
        // 8bit量子化してバッファへ 
        buf[i] = (unsigned char)(127 * sig) + 128;
    }

    for(int i=SMPL*0.64;i<SMPL*0.67;i++){
        p +=1300.0/SMPL;
        p -= floor(p);
        sig=sin(2.0*M_PI*p);
        // 8bit量子化してバッファへ 
        buf[i] = (unsigned char)(127 * sig) + 128;
    }

    for(int i=SMPL*0.67;i<SMPL*0.70;i++){
        p +=1300.0/SMPL;
        p -= floor(p);
        sig=sin(2.0*M_PI*p);
        // 8bit量子化してバッファへ 
        buf[i] = (unsigned char)(127 * sig) + 128;
    }

    for(int i=SMPL*0.70;i<SMPL*0.73;i++){
        p +=1100.0/SMPL;
        p -= floor(p);
        sig=sin(2.0*M_PI*p);
        // 8bit量子化してバッファへ 
        buf[i] = (unsigned char)(127 * sig) + 128;
    }

    for(int i=SMPL*0.73;i<SMPL*0.76;i++){
        p +=1100.0/SMPL;
        p -= floor(p);
        sig=sin(2.0*M_PI*p);
        // 8bit量子化してバッファへ 
        buf[i] = (unsigned char)(127 * sig) + 128;
    }

    for(int i=SMPL*0.76;i<SMPL*0.79;i++){
        p +=1300.0/SMPL;
        p -= floor(p);
        sig=sin(2.0*M_PI*p);
        // 8bit量子化してバッファへ 
        buf[i] = (unsigned char)(127 * sig) + 128;
    }

    for(int i=SMPL*0.79;i<SMPL*0.82;i++){
        p +=1100.0/SMPL;
        p -= floor(p);
        sig=sin(2.0*M_PI*p);
        // 8bit量子化してバッファへ 
        buf[i] = (unsigned char)(127 * sig) + 128;
    }

    for(int i=SMPL*0.82;i<SMPL*0.85;i++){
        p +=1300.0/SMPL;
        p -= floor(p);
        sig=sin(2.0*M_PI*p);
        // 8bit量子化してバッファへ 
        buf[i] = (unsigned char)(127 * sig) + 128;
    }

    for(int i=SMPL*0.85;i<SMPL*0.88;i++){
        p +=1100.0/SMPL;
        p -= floor(p);
        sig=sin(2.0*M_PI*p);
        // 8bit量子化してバッファへ 
        buf[i] = (unsigned char)(127 * sig) + 128;
    }


    for(int i=SMPL*0.88;i<SMPL*0.91;i++){/*stop bit*/
        p +=1200.0/SMPL;
        p -= floor(p);
        sig=sin(2.0*M_PI*p);
        // 8bit量子化してバッファへ 
        buf[i] = (unsigned char)(127 * sig) + 128;
    }

    int i=SMPL*0.91;
    int k;
    int count=0,j;
    double q,r;

    for(y=0;y<256;y++){
      k=i;
          while(i<k+SMPL*0.004862){
            p +=1200.0/SMPL;
            p -= floor(p);
            sig=sin(2.0*M_PI*p);
            // 8bit量子化してバッファへ 
            buf[i] = (unsigned char)(127 * sig) + 128;
            i++;
          }
          count=0;
          while(count<3){
            if(count==0){//1,2,0
              j=1;
            }else if(count==1){
              j=2;
            }else{
              j=0;
            }
            for(int x=0;x<320;x++){
              k=i;
              //if(k>=SMPL*0.440145) break;
              while(i<k+SMPL*0.146432/320.0){
                q=(unsigned char)image[j][x][y];
                //printf("%lf \n",q);
                r=(2300.0-1500.0)*q/255.000000;
                //printf("%lf \n",r);
                p +=(1500.0+r)/SMPL;
                p -= floor(p);
                sig=sin(2.0*M_PI*p);
                //printf("%lf ",sig);
                // 8bit量子化してバッファへ 
                buf[i]=(unsigned char)(127 * sig) + 128;
                //printf("%u ",buf[i]);
                i++;
              }
            }
            k=i;
            while(i<k+SMPL*0.000572){
              //if(count==2) break;
              p +=1500.0/SMPL;
              p -= floor(p);
              sig=sin(2.0*M_PI*p);
              // 8bit量子化してバッファへ 
              buf[i] = (unsigned char)(127 * sig) + 128;
              i++;
            }
            count++;
          }
    }

    
    wav_write("test.wav", buf, size);

    free(buf);

    printf("ok\n");
    return 0;
}



/*メモリ確保関数*/
/*image_t *allocate_image(uint32_t width, uint32_t height, uint8_t type) {
  uint32_t i;
  image_t *img;
  if ((img = calloc(1, sizeof(image_t))) == NULL) {
    return NULL;
  }
  img->width = width;
  img->height = height;
  img->color_type = type;
  if (type == COLOR_TYPE_INDEX) {
    if ((img->palette = calloc(256, sizeof(color_t))) == NULL) {
      goto error;
    }
  } else {
    img->palette = NULL;
  }
  img->palette_num = 0;
  if ((img->map = calloc(height, sizeof(pixcel_t*))) == NULL) {
    goto error;
  }
  for (i = 0; i < height; i++) {
    if ((img->map[i] = calloc(width, sizeof(pixcel_t))) == NULL) {
      goto error;
    }
  }
  return img;
  error:
  free_image(img);
  return NULL;
}*/


/*メモリ解放関数*/
/*void free_image(image_t *img) {
  uint32_t i;
  if (img == NULL) {
    return;
  }
  if (img->palette != NULL) {
    free(img->palette);
  }
  for (i = 0; i < img->height; i++) {
    free(img->map[i]);
  }
  free(img->map);
  free(img);
}*/

/*クローン関数*/
/*image_t *clone_image(image_t *img) {
  uint32_t i;
  image_t *new_img = allocate_image(img->width, img->height, img->color_type);
  if (new_img == NULL) {
    return NULL;
  }
  new_img->palette_num = img->palette_num;
  if (img->color_type == COLOR_TYPE_INDEX) {
    memcpy(new_img->palette, img->palette, sizeof(color_t) * img->palette_num);
  }
  for (i = 0; i < img->height; i++) {
    memcpy(new_img->map[i], img->map[i], sizeof(pixcel_t) * img->width);
  }
  return new_img;
}*/

void wav_write(const char *filename, unsigned char *buffer, ::size_t size)
{
    size_t filesize;
    unsigned char head[44];

    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) return;

    // RIFFヘッダ (8バイト) 
    memcpy(head, "RIFF", 4);
    filesize = sizeof(head) + size;
    head[4] = (filesize - 8) >> 0  & 0xff;
    head[5] = (filesize - 8) >> 8  & 0xff;
    head[6] = (filesize - 8) >> 16 & 0xff;
    head[7] = (filesize - 8) >> 24 & 0xff;

    // WAVEヘッダ (4バイト) 
    memcpy(head + 8, "WAVE", 4);

    // fmtチャンク (24バイト) 
    memcpy(head + 12, "fmt ", 4);
    head[16] = 16;
    head[17] = 0;
    head[18] = 0;
    head[19] = 0;
    head[20] = 1;
    head[21] = 0;
    head[22] = 1;
    head[23] = 0;
    head[24] = SMPL >> 0  & 0xff;//下位8ビットを取り出している
    head[25] = SMPL >> 8  & 0xff;
    head[26] = SMPL >> 16 & 0xff;
    head[27] = SMPL >> 24 & 0xff;
    head[28] = (SMPL * (BIT / 8)) >> 0  & 0xff;
    head[29] = (SMPL * (BIT / 8)) >> 8  & 0xff;
    head[30] = (SMPL * (BIT / 8)) >> 16 & 0xff;
    head[31] = (SMPL * (BIT / 8)) >> 24 & 0xff;
    head[32] = (BIT / 8) >> 0 & 0xff;
    head[33] = (BIT / 8) >> 8 & 0xff;
    head[34] = BIT >> 0 & 0xff;
    head[35] = BIT >> 8 & 0xff;

    // dataチャンク (8 + size バイト) 
    memcpy(head + 36, "data", 4);
    head[40] = size >> 0  & 0xff;
    head[41] = size >> 8  & 0xff;
    head[42] = size >> 16 & 0xff;
    head[43] = size >> 24 & 0xff;

    // 書き出し 
    fwrite(head, sizeof(head), 1, fp);
    fwrite(buffer, size, 1, fp);
    fclose(fp);
}