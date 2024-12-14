#include <SPI.h>
#include <stdio.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"


#define PTRED         0xC064
#define PTBLUE        0x255B
#define PTORG         0xF4A0
#define PTGRN         0x04C8

void draw_Cheeks() {
    Paint_DrawCircle(220, 160, 50, WHITE, DOT_PIXEL_7X7, DRAW_FILL_EMPTY);
    Paint_DrawCircle(20, 160, 50, WHITE, DOT_PIXEL_7X7, DRAW_FILL_EMPTY);
}

void draw_RedFace(){
  Paint_Clear(PTRED);

  Paint_DrawCircle(120, 120, 118, WHITE ,DOT_PIXEL_3X3, DRAW_FILL_EMPTY);

  Paint_DrawLine  (69, 68, 81, 92, WHITE ,DOT_PIXEL_7X7,LINE_STYLE_SOLID);
  Paint_DrawCircle(69, 68, 8, WHITE ,DOT_PIXEL_3X3, DRAW_FILL_FULL);
  Paint_DrawCircle(81, 92, 8, WHITE ,DOT_PIXEL_3X3, DRAW_FILL_FULL);

  Paint_DrawLine  (159, 92, 171, 68,WHITE ,DOT_PIXEL_7X7,LINE_STYLE_SOLID);
  Paint_DrawCircle(159, 92, 8, WHITE ,DOT_PIXEL_3X3, DRAW_FILL_FULL);
  Paint_DrawCircle(171, 68, 8, WHITE ,DOT_PIXEL_3X3, DRAW_FILL_FULL);

  Paint_DrawLine (70, 135, 170, 135, WHITE, DOT_PIXEL_7X7, LINE_STYLE_SOLID);
  Paint_DrawLine (70, 175, 170, 175, WHITE, DOT_PIXEL_7X7, LINE_STYLE_SOLID);
  Paint_DrawLine (70, 135, 70, 175, WHITE, DOT_PIXEL_7X7, LINE_STYLE_SOLID);
  Paint_DrawLine (170, 135, 170, 175, WHITE, DOT_PIXEL_7X7, LINE_STYLE_SOLID);

  // Draw cheeks
  draw_Cheeks();

}

void draw_BlueFace()
{
  Paint_Clear(PTBLUE);
  Paint_DrawCircle(120, 120, 118, WHITE ,DOT_PIXEL_3X3, DRAW_FILL_EMPTY);
//L_EYE
  Paint_DrawLine  (70, 80, 83, 80, WHITE ,DOT_PIXEL_7X7,LINE_STYLE_SOLID);
  Paint_DrawCircle(65, 80, 6, WHITE ,DOT_PIXEL_1X1, DRAW_FILL_FULL);
  Paint_DrawCircle(88, 80, 6, WHITE ,DOT_PIXEL_1X1, DRAW_FILL_FULL);
//R_EYE
  Paint_DrawLine  (157, 80, 170, 80,WHITE ,DOT_PIXEL_7X7,LINE_STYLE_SOLID);
  Paint_DrawCircle(152, 80, 6, WHITE ,DOT_PIXEL_1X1, DRAW_FILL_FULL);
  Paint_DrawCircle(175, 80, 6, WHITE ,DOT_PIXEL_1X1, DRAW_FILL_FULL);
//MOUTH
  Paint_DrawLine (70, 160, 170, 160, WHITE, DOT_PIXEL_7X7, LINE_STYLE_SOLID);
  
  // Draw cheeks
  draw_Cheeks();
}

void draw_YellowFace(){
  Paint_Clear(PTORG);

  Paint_DrawCircle(120, 120, 118, WHITE, DOT_PIXEL_3X3, DRAW_FILL_EMPTY);

  Paint_DrawLine(69, 92, 81, 68, WHITE, DOT_PIXEL_7X7, LINE_STYLE_SOLID);
  Paint_DrawCircle(69, 92, 8, WHITE, DOT_PIXEL_3X3, DRAW_FILL_FULL);
  Paint_DrawCircle(81, 68, 8, WHITE, DOT_PIXEL_3X3, DRAW_FILL_FULL);

  Paint_DrawLine(159, 68, 171, 92, WHITE, DOT_PIXEL_7X7, LINE_STYLE_SOLID);
  Paint_DrawCircle(159, 68, 8, WHITE, DOT_PIXEL_3X3, DRAW_FILL_FULL);
  Paint_DrawCircle(171, 92, 8, WHITE, DOT_PIXEL_3X3, DRAW_FILL_FULL);

  int centerX = 120;  // 곡선 중심 X 좌표
  int centerY = 165;  // 기존 Y 좌표 155에서 10픽셀 아래로 이동
  int radiusX = 50;   // 곡선의 가로 반경
  int radiusY = 20;   // 곡선의 세로 반경

  for (int angle = 180; angle <= 360; angle++) { // 180°에서 360°까지 점으로 그리기
    float rad = angle * 3.14159 / 180.0;       // 각도를 라디안으로 변환
    int x = centerX + radiusX * cos(rad);     // X 좌표 계산
    int y = centerY + radiusY * sin(rad);     // Y 좌표 계산
    Paint_DrawPoint(x, y, WHITE, DOT_PIXEL_7X7, DOT_STYLE_DFT); // 두꺼운 점으로 그리기
  }

  // Draw cheeks
  draw_Cheeks();

}

void draw_GreenFace(){
  Paint_Clear(PTGRN);

  Paint_DrawCircle(120, 120, 118, WHITE, DOT_PIXEL_3X3, DRAW_FILL_EMPTY);

  Paint_DrawLine(69, 92, 85, 80, WHITE, DOT_PIXEL_7X7, LINE_STYLE_SOLID);
  Paint_DrawLine(155, 80, 171, 92, WHITE, DOT_PIXEL_7X7, LINE_STYLE_SOLID);
  Paint_DrawCircle(69, 92, 8, WHITE, DOT_PIXEL_3X3, DRAW_FILL_FULL);
  Paint_DrawCircle(171, 92, 8, WHITE, DOT_PIXEL_3X3, DRAW_FILL_FULL);
  Paint_DrawCircle(85, 80, 8, WHITE, DOT_PIXEL_3X3, DRAW_FILL_FULL);

  Paint_DrawLine  (69, 68, 85, 80, WHITE ,DOT_PIXEL_7X7,LINE_STYLE_SOLID);  
  Paint_DrawLine  (155, 80, 171, 68,WHITE ,DOT_PIXEL_7X7,LINE_STYLE_SOLID);
  Paint_DrawCircle(69, 68, 8, WHITE ,DOT_PIXEL_3X3, DRAW_FILL_FULL);
  Paint_DrawCircle(171, 68, 8, WHITE ,DOT_PIXEL_3X3, DRAW_FILL_FULL);
  Paint_DrawCircle(155, 80, 8, WHITE ,DOT_PIXEL_3X3, DRAW_FILL_FULL);

  int centerX = 120;  // 곡선 중심 X 좌표
  int centerY = 165;  // 곡선 중심 Y 좌표
  int radiusX = 50;   // 곡선의 가로 반경
  int radiusY = 20;   // 곡선의 세로 반경

  for (int angle = 0; angle <= 180; angle++) { // 0°에서 180°까지 점으로 그리기
    float rad = angle * 3.14159 / 180.0;       // 각도를 라디안으로 변환
    int x = centerX + radiusX * cos(rad);     // X 좌표 계산
    int y = centerY + radiusY * sin(rad);     // Y 좌표 계산
    Paint_DrawPoint(x, y, WHITE, DOT_PIXEL_7X7, DOT_STYLE_DFT);
  }

  // Draw cheeks
  draw_Cheeks();

}

void LCD_DrawFace(int elapsedState){
  switch (elapsedState){
    case 1:
      draw_GreenFace();
      break;
    case 2:
      draw_BlueFace();
      break;
    case 3:
      draw_YellowFace();
      break;
    case 4:
      draw_RedFace();
      break;
  }
}

void drawWaterDrop(int centerX, int centerY, int triangleHeight, int circleRadius) {
  // 삼각형 윗부분 (채워진 삼각형)
  Paint_DrawLine(centerX, centerY - triangleHeight, centerX - circleRadius, centerY, GRAYBLUE, DOT_PIXEL_2X2, LINE_STYLE_SOLID); // 왼쪽 선
  Paint_DrawLine(centerX, centerY - triangleHeight, centerX + circleRadius, centerY, GRAYBLUE, DOT_PIXEL_2X2, LINE_STYLE_SOLID); // 오른쪽 선
  Paint_DrawLine(centerX - circleRadius, centerY, centerX + circleRadius, centerY, GRAYBLUE, DOT_PIXEL_2X2, LINE_STYLE_SOLID);    // 바닥 선

  // 삼각형 내부 채우기
  for (int y = 0; y < triangleHeight; y++) { // 세모를 채우기 위해 선을 반복 그리기
    int leftX = centerX - (circleRadius * y / triangleHeight);
    int rightX = centerX + (circleRadius * y / triangleHeight);
    Paint_DrawLine(leftX, centerY - triangleHeight + y, rightX, centerY - triangleHeight + y, GRAYBLUE, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
  }

  // 동그라미 (아랫부분)
  Paint_DrawCircle(centerX, centerY, circleRadius, GRAYBLUE, DOT_PIXEL_2X2, DRAW_FILL_FULL);
}

void drawDrankVolume(float drankVolume_ml) {
    Paint_Clear(WHITE); // Clear the screen
    Paint_DrawCircle(120, 120, 117, GRAYBLUE, DOT_PIXEL_3X3, DRAW_FILL_EMPTY); // Draw a border

    int drankVolumeCase = (int)(drankVolume_ml / 500); // Calculate number of drops based on 500 ml intervals

    // Debug print for drank volume case
    Serial.println(drankVolumeCase);

    // Draw water drops based on drankVolumeCase
    switch (drankVolumeCase) {
        case 1:
            drawWaterDrop(48, 110, 40, 18); // First drop
            break;
        case 2:
            drawWaterDrop(48, 110, 40, 18); // First drop
            drawWaterDrop(96, 110, 40, 18); // Second drop
            break;
        case 3:
            drawWaterDrop(48, 110, 40, 18); // First drop
            drawWaterDrop(96, 110, 40, 18); // Second drop
            drawWaterDrop(144, 110, 40, 18); // Third drop
            break;
        case 4:
            drawWaterDrop(48, 110, 40, 18); // First drop
            drawWaterDrop(96, 110, 40, 18); // Second drop
            drawWaterDrop(144, 110, 40, 18); // Third drop
            drawWaterDrop(192, 110, 40, 18); // Fourth drop
            break;
        default:
            if (drankVolumeCase > 4) {
                drawWaterDrop(48, 110, 40, 18); // First drop
                drawWaterDrop(96, 110, 40, 18); // Second drop
                drawWaterDrop(144, 110, 40, 18); // Third drop
                drawWaterDrop(192, 110, 40, 18); // Fourth drop
            }
            break;
    }

    // Format the drank volume as "X.XX L"
    char volumeString[10]; // Buffer to store the formatted string
    dtostrf(drankVolume_ml / 1000.0, 4, 1, volumeString); // Convert float to string
    strcat(volumeString, "L");
    Serial.print("Volume String: ");
    Serial.println(volumeString);

    // Draw the drank volume text below the drops
    Paint_DrawString_EN(70, 159, volumeString, &Font24, WHITE, GRAYBLUE);
    Paint_DrawString_EN(71, 160, volumeString, &Font24, WHITE, GRAYBLUE);
    Paint_DrawString_EN(72, 161, volumeString, &Font24, WHITE, GRAYBLUE); // Display the text
}

