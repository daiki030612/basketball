
#include <GL/glut.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h> // 乱数生成のためのヘッダファイル
#define STB_IMAGE_IMPLEMENTATION
#include <string.h>
#include "stb_image.h"

#define PI 3.14159265358979323846

// グローバル変数として宣言
GLuint tailTexture;

// ボールの位置と移動に関する変数
float ballX = 0.0f;
float ballZ = 1.0f;
float ballY;  // 初期位置をアニメーション開始位置に合わせる
float ballSpeed = 0.05f;
bool isMoving0 = false;
bool isMoving1 = false;
bool isMoving2 = false;
bool isMoving3 = false;
bool isMoving4 = false;
bool isMoving5 = false;

// カメラの位置と向きに関する変数
float cameraAngleZ = 0.0f;
float cameraAngleY = 0.0f;
float cameraDistance = 5.0f;


int point=0;
int rCount=0;
int lCount=0;

// 初期位置を設定する関数
void initialize() {
    ballY = -1.5 * ballZ * ballZ;

	tailTexture = loadTexture("basket2.bmp");
}

GLuint loadTexture(const char* filename) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
    if (data == NULL) {
        fprintf(stderr, "Failed to load texture: %s\n", filename);
        exit(1);
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, channels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    return texture;
}


// 球体を描画する関数
void drawSphere(float radius, int slices, int stacks) {
    glPushMatrix();
    glColor3f(1.0f, 0.5f, 0.0f); // オレンジ色

    for (int i = 0; i < slices; i++) {
        float theta1 = (float)i * 2.0f * PI / slices;
        float theta2 = (float)(i + 1) * 2.0f * PI / slices;

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= stacks; j++) {
            float phi = (float)j * PI / stacks;
            float x, y, z;

            x = cos(theta2) * sin(phi);
            y = cos(phi);
            z = sin(theta2) * sin(phi);
            glNormal3f(x, y, z);
            glVertex3f(radius * x, radius * y, radius * z);

            x = cos(theta1) * sin(phi);
            y = cos(phi);
            z = sin(theta1) * sin(phi);
            glNormal3f(x, y, z);
            glVertex3f(radius * x, radius * y, radius * z);
        }
        glEnd();
    }
    glPopMatrix();
}

// 円を描画する関数
void drawCircle(float cx, float cy, float r, int num_segments) {
    glPushMatrix();
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < num_segments; i++) {
        float theta = 2.0f * PI * i / num_segments;
        float x = r * cos(theta);
        float y = r * sin(theta);
        glVertex3f(x + cx, -0.45, y + cy - 0.6);
    }
    glEnd();
    glPopMatrix();
}

// ボールを描画する関数
void drawBall() {
    glTranslatef(ballX, ballY, ballZ); // ボールの位置に移動
    drawSphere(0.05f, 20, 20); // ボールを描画
}

// 地面を描画する関数
void drawGround() {
    // テクスチャを読み込む
    GLuint textureID = loadTexture("basket2.bmp");

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID); // テクスチャをバインド

    glColor3f(225.0f / 255.0f, 188.0f / 255.0f, 126.0f / 255.0f); // 木の色

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.2f, -1.55f, -0.8f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.2f, -1.55f, -0.8f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.2f, -1.55f, 1.4f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.2f, -1.55f, 1.4f);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glColor3f(225.0f / 255.0f, 188.0f / 255.0f, 126.0f / 255.0f); // 木の色

    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(-1.5f, -1.55f, -1.2f);
    glVertex3f(-1.2f, -1.55f, -1.2f);;
    glVertex3f(-1.2f, -1.55f, 1.4f);
    glVertex3f(-1.5f, -1.55f, 1.4f);

    glVertex3f(1.5f, -1.55f, -1.2f);
    glVertex3f(1.2f, -1.55f, -1.2f);;
    glVertex3f(1.2f, -1.55f, 1.4f);
    glVertex3f(1.5f, -1.55f, 1.4f);

    glVertex3f(-1.2f, -1.55f, -1.2f);
    glVertex3f(1.2f, -1.55f, -1.2f);;
    glVertex3f(1.2f, -1.55f, -0.8f);
    glVertex3f(-1.2f, -1.55f, -0.8f);

    glEnd();
    glPopMatrix();

}


//バックボードの描画
void drawBackboad1() {
    glColor3f(0.0f, 0.0f, 0.0f); // 黒色
    glPushMatrix();
    // 各面を描画
    glBegin(GL_QUADS);
    // 上面
    glVertex3f(0.25f, -0.2f, -0.65f);
    glVertex3f(0.25f, -0.2f, -0.7f);
    glVertex3f(-0.25f, -0.2f, -0.7f);
    glVertex3f(-0.25f, -0.2f, -0.65f);
    // 下面
    glVertex3f(0.25f, -0.55f, -0.65f);
    glVertex3f(0.25f, -0.55f, -0.7f);
    glVertex3f(-0.25f, -0.55f, -0.7f);
    glVertex3f(-0.25f, -0.55f, -0.65f);
    // 前面
    glVertex3f(-0.24f, -0.2f, -0.65f);
    glVertex3f(-0.24f, -0.55f, -0.65f);
    glVertex3f(-0.25f, -0.55f, -0.65f);
    glVertex3f(-0.25f, -0.2f, -0.65f); 
   
    glVertex3f(0.25f, -0.2f, -0.65f);
    glVertex3f(0.25f, -0.55f, -0.65f);
    glVertex3f(0.24f, -0.55f, -0.65f);
    glVertex3f(0.24f, -0.2f, -0.65f);

    glVertex3f(0.24f, -0.54f, -0.65f);
    glVertex3f(0.24f, -0.55f, -0.65f);
    glVertex3f(-0.24f, -0.55f, -0.65f);
    glVertex3f(-0.24f, -0.54f, -0.65f);

    glVertex3f(0.24f, -0.2f, -0.65f);
    glVertex3f(0.24f, -0.19f, -0.65f);
    glVertex3f(-0.24f, -0.19f, -0.65f);
    glVertex3f(-0.24f, -0.2f, -0.65f);


    glVertex3f(-0.09f, -0.3f, -0.65f);
    glVertex3f(-0.09f, -0.45f, -0.65f);
    glVertex3f(-0.1f, -0.45f, -0.65f);
    glVertex3f(-0.1f, -0.3f, -0.65f); 
   
    glVertex3f(0.09f, -0.3f, -0.65f);
    glVertex3f(0.09f, -0.45f, -0.65f);
    glVertex3f(0.1f, -0.45f, -0.65f);
    glVertex3f(0.1f, -0.3f, -0.65f);

    glVertex3f(0.09f, -0.29f, -0.65f);
    glVertex3f(0.09f, -0.3f, -0.65f);
    glVertex3f(-0.09f, -0.3f, -0.65f);
    glVertex3f(-0.09f, -0.29f, -0.65f);

    glVertex3f(0.09f, -0.45f, -0.65f);
    glVertex3f(0.09f, -0.44f, -0.65f);
    glVertex3f(-0.09f, -0.44f, -0.65f);
    glVertex3f(-0.09f, -0.45f, -0.65f);
    // 後面
    glVertex3f(0.25f, -0.2f, -0.7f);
    glVertex3f(0.25f, -0.55f, -0.7f);
    glVertex3f(-0.25f, -0.55f, -0.7f);
    glVertex3f(-0.25f, -0.2f, -0.7f);
    // 左側面
    glVertex3f(-0.25f, -0.2f, -0.65f);
    glVertex3f(-0.25f, -0.55f, -0.65f);
    glVertex3f(-0.25f, -0.55f, -0.7f);
    glVertex3f(-0.25f, -0.2f, -0.7f);
    // 右側面
    glVertex3f(0.25f, -0.2f, -0.65f);
    glVertex3f(0.25f, -0.55f, -0.65f);
    glVertex3f(0.25f, -0.55f, -0.7f);
    glVertex3f(0.25f, -0.2f, -0.7f);

    //柱
    glVertex3f(-0.2f, -0.45f, -0.9f);
    glVertex3f(-0.2f, -1.55f, -1.0f);
    glVertex3f(-0.2f, -1.55f, -1.05f);
    glVertex3f(-0.2f, -0.45f, -0.95f);

    glVertex3f(-0.15f, -0.45f, -0.9f);
    glVertex3f(-0.15f, -1.55f, -1.0f);
    glVertex3f(-0.15f, -1.55f, -1.05f);
    glVertex3f(-0.15f, -0.45f, -0.95f);

    glVertex3f(-0.2f, -0.45f, -0.9f);
    glVertex3f(-0.2f, -1.55f, -1.0f);
    glVertex3f(-0.15f, -1.55f, -1.0f);
    glVertex3f(-0.15f, -0.45f, -0.9f);

    glVertex3f(-0.2f, -0.45f, -0.95f);
    glVertex3f(-0.2f, -1.55f, -1.05f);
    glVertex3f(-0.15f, -1.55f, -1.05f);
    glVertex3f(-0.15f, -0.45f, -0.95f);


    glVertex3f(0.2f, -0.45f, -0.9f);
    glVertex3f(0.2f, -1.55f, -1.0f);
    glVertex3f(0.2f, -1.55f, -1.05f);
    glVertex3f(0.2f, -0.45f, -0.95f);

    glVertex3f(0.15f, -0.45f, -0.9f);
    glVertex3f(0.15f, -1.55f, -1.0f);
    glVertex3f(0.15f, -1.55f, -1.05f);
    glVertex3f(0.15f, -0.45f, -0.95f);

    glVertex3f(0.2f, -0.45f, -0.9f);
    glVertex3f(0.2f, -1.55f, -1.0f);
    glVertex3f(0.15f, -1.55f, -1.0f);
    glVertex3f(0.15f, -0.45f, -0.9f);

    glVertex3f(0.2f, -0.45f, -0.95f);
    glVertex3f(0.2f, -1.55f, -1.05f);
    glVertex3f(0.15f, -1.55f, -1.05f);
    glVertex3f(0.15f, -0.45f, -0.95f);

    glVertex3f(0.2f, -0.45f, -0.9f);
    glVertex3f(0.2f, -0.35f, -0.7f);
    glVertex3f(0.15f, -0.35f, -0.7f);
    glVertex3f(0.15f, -0.45f, -0.9f);

    glVertex3f(0.2f, -0.5f, -0.95f);
    glVertex3f(0.2f, -0.4f, -0.7f);
    glVertex3f(0.15f, -0.4f, -0.7f);
    glVertex3f(0.15f, -0.5f, -0.95f);

    glVertex3f(0.2f, -0.45f, -0.95f);
    glVertex3f(0.2f, -0.35f, -0.7f);
    glVertex3f(0.2f, -0.4f, -0.7f);
    glVertex3f(0.2f, -0.5f, -0.95f);

    glVertex3f(0.15f, -0.45f, -0.95f);
    glVertex3f(0.15f, -0.35f, -0.7f);
    glVertex3f(0.15f, -0.4f, -0.7f);
    glVertex3f(0.15f, -0.5f, -0.95f);

    glVertex3f(-0.2f, -0.45f, -0.9f);
    glVertex3f(-0.2f, -0.35f, -0.7f);
    glVertex3f(-0.15f, -0.35f, -0.7f);
    glVertex3f(-0.15f, -0.45f, -0.9f);

    glVertex3f(-0.2f, -0.5f, -0.95f);
    glVertex3f(-0.2f, -0.4f, -0.7f);
    glVertex3f(-0.15f, -0.4f, -0.7f);
    glVertex3f(-0.15f, -0.5f, -0.95f);

    glVertex3f(-0.2f, -0.45f, -0.95f);
    glVertex3f(-0.2f, -0.35f, -0.7f);
    glVertex3f(-0.2f, -0.4f, -0.7f);
    glVertex3f(-0.2f, -0.5f, -0.95f);

    glVertex3f(-0.15f, -0.45f, -0.95f);
    glVertex3f(-0.15f, -0.35f, -0.7f);
    glVertex3f(-0.15f, -0.4f, -0.7f);
    glVertex3f(-0.15f, -0.5f, -0.95f);


    glEnd();
    glPopMatrix();
}

void drawBackboad2() {
    glColor3f(1.0f, 1.0f, 1.0f); // 白色
        glBegin(GL_QUADS);
   
    // 前面
    glVertex3f(0.25f, -0.2f, -0.65f);
    glVertex3f(0.25f, -0.55f, -0.65f);
    glVertex3f(-0.25f, -0.55f, -0.65f);
    glVertex3f(-0.25f, -0.2f, -0.65f);
    glEnd();
}


// テキストの描画
void renderBitmapString(float x, float y, void *font, const char *string) {
    const char *c;
    glRasterPos3f(-1.5f, -1.4f, -1.0f); // ビットマップ文字の描画位置を設定
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c); // 文字を描画
    }
}

// アニメーションを更新する関数
void update(int value) {
    // ボールが移動中の場合のみ移動する
    if (isMoving0) {
	
        ballZ -= ballSpeed;
        ballY = -1.5 * ballZ * ballZ;
   	if (ballY <= -1.55f) {
            ballZ = 1.0f;
            ballY = -1.5 * ballZ * ballZ;
		if(ballZ>=0.6){
		point = point+3;
		}
            isMoving0 = false; // ボールが止まる        
        }

    }

    if (isMoving1) {

		if(ballY <= -0.3099 && ballZ < 0){

		ballZ -= -0.75 * ballSpeed;	
		ballY = -1.983 * ballZ  -1.55;
    if (ballY <= -1.55f) {
          	  ballZ = 1.0f;
           	 ballY = -1.5 * ballZ * ballZ;
		if(ballZ>=0.6){
		point = point+3;
		}
          	  isMoving1 = false; // ボールが止まる
        	 }
                }else	{
        ballZ -= ballSpeed;
        ballY = -150.0 / 121.0 * (ballZ + 0.1) * (ballZ + 0.1) ;
		}
	
            }

    if (isMoving2) {
		if(ballZ < -0.43 ){
		isMoving2 = false;
		isMoving3 = true;
                }else	{
        ballZ -= ballSpeed;
        ballY = -2.5/0.7*0.7 * (ballZ-0.3) * (ballZ-0.3)+1;
		}
	
            }

if (isMoving3) {
    ballZ += 0.75 * ballSpeed;   
    ballY = -1.79 * ballZ * ballZ;
    if (ballY <= -1.55f) {
        ballZ = 1.0f;
        ballY = -1.5 * ballZ * ballZ;
        isMoving3 = false; // ボールが止まる
    }
}

if (isMoving4) {
    ballX -= 0.025;
    ballY = -1.5/0.49 * (ballX - 0.4f)* (ballX - 0.4f);
    if (ballY <= -1.55f) {
	ballX = 0.0f;
        ballZ = 1.0f;
        ballY = -1.5 * ballZ * ballZ;
        if (ballZ >= 0.6) {
            point = point + 3;
        }
        isMoving4 = false; // ボールが止まる
    }
}

if (isMoving5) {
    ballX += 0.025;
    ballY = -1.5/0.49 * (ballX + 0.4f)* (ballX + 0.4f);
    if (ballY <= -1.55f) {
	ballX = 0.0f;
        ballZ = 1.0f;
        ballY = -1.5 * ballZ * ballZ;
        if (ballZ >= 0.6) {
            point = point + 3;
        }
        isMoving5 = false; // ボールが止まる
    }
}

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // 次の更新をスケジュール
}


// 描画関数
void render(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // カメラの位置と向きを設定
    gluLookAt(cameraDistance * sin(cameraAngleZ) * cos(cameraAngleY),
              cameraDistance * sin(cameraAngleY),
              cameraDistance * cos(cameraAngleZ) * cos(cameraAngleY),
              0.0f, 0.0f, 0.0f,  // カメラが注視する位置(x, y, z)
              0.0f, 1.0f, 0.0f); // カメラの上方向のベクトル(x, y, z)

    // 地面を描画
    drawGround();

    // バックボードを描画
    drawBackboad1();
    drawBackboad2();

    // 円を描画
    glColor3f(1.0f, 0.0f, 0.0f); // 赤色
    drawCircle(0.0f, 0.05f, 0.08f, 100);

    // テキストを描画
    glColor3f(0.0f, 0.0f, 0.0f);
char pointString[10];
snprintf(pointString, sizeof(pointString),"total " "%d", point); 
    renderBitmapString(0.0f, 0.0f, GLUT_BITMAP_HELVETICA_18, pointString);

    // バスケットボールを描画
    drawBall();
   

    

    glutSwapBuffers();
}

// 0～2の乱数を生成する関数

int generateRandomBinary() {

    return rand() % 3; // 0～2を生成
}

int a;


// キーボード入力を処理する関数
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case ' ': // スペースキーが押された場合
	if (ballZ == 1.0f) {
            a = generateRandomBinary(); // 乱数でボールの速度を0または1に設定
            if(a == 0){
            	isMoving0 = true; // ボールを動かす
	    }else if(a == 1){
            	isMoving1 = true; // ボールを動かす
            }else if(a == 2){
            	isMoving2 = true; // ボールを動かす
            }
	}else if(ballZ == -0.65f || ballX == 1.1f){
		isMoving4 = true; // ボールを動かす
	}else if(ballZ == -0.65f || ballX == -1.1f){
		isMoving5 = true; // ボールを動かす
	}
	break;

        case 'r': // 右矢印キーが押された場合
 rCount++; // rキーの押下回数を増やす
            if (rCount % 2 != 0) { // 奇数回の場合
                ballZ = -0.55f; // 球体の初期位置を設定
                ballY = -1.5f;
                ballX = 1.1f;
            } else { // 偶数回の場合
                ballZ = 1.0f; // 球体の初期位置を設定
                ballY = -1.5f;
                ballX = 0.0f;
            }
            break;       
case 'l': // 右矢印キーが押された場合
 lCount++; // rキーの押下回数を増やす
            if (lCount % 2 != 0||ballX == -1.1f) { // 奇数回の場合
                ballZ = -0.55f; // 球体の初期位置を設定
                ballY = -1.5f;
                ballX = -1.1f;
            } else { // 偶数回の場合
                ballZ = 1.0f; // 球体の初期位置を設定
                ballY = -1.5f;
                ballX = 0.0f;
            }
            break;       
        default:
            break;
	}
    }

// マウスドラッグを処理する関数
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // マウスの左ボタンが押されたとき
        // ドラッグ開始位置を記録
        // 今回は使用しない
    }
}

// マウスドラッグ時の移動を処理する関数
void motion(int x, int y) {
    // マウスのドラッグ中の位置に応じてカメラの角度を更新
    cameraAngleZ = 2.0f * PI * (float)x / glutGet(GLUT_WINDOW_WIDTH);
    cameraAngleY = PI * (float)y / glutGet(GLUT_WINDOW_HEIGHT);
}

// ウィンドウサイズが変更されたときに呼ばれる関数
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    // 初期位置を設定
    initialize();

    // OpenGLの初期化
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Basketball Shoot Animation");

    // 背景色を白に設定
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // レンダリング関数とアニメーション更新関数を設定
    glutDisplayFunc(render);
    glutReshapeFunc(reshape);
    glutTimerFunc(25, update, 0);

    // キーボード入力を処理する関数を設定
    glutKeyboardFunc(keyboard);

    // マウスドラッグを処理する関数を設定
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    // OpenGLの設定
    glEnable(GL_DEPTH_TEST);

    // メインループ
    glutMainLoop();

    return 0;
}