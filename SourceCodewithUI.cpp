#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>
#include <functional>
#include <iostream>
#include <stack>
#include <algorithm>
#include <cmath>



std::vector<std::pair<float, float>> points;
std::vector<std::function<void()>> functionsToCall;
std::vector<std::pair<float, float>> eraserCurves;
std::vector<std::pair<float, float>> lineCurves(2);
std::vector<std::pair<float, float>> lineCurves1(2);
std::vector<std::pair<float, float>> paintVector;
std::vector<std::pair<float, float>> smallLoop;
std::vector<std::pair<float, float>> paintedPoints;
std::vector<std::pair<float, float>> rectPoints;
std::vector<std::pair<float, float>> circleVector;

bool ctrlPressed = false;
bool zPressed = false;

//std::pair<float,float> lineCurves[2];
bool isCircling = false;
bool isDrawingRect = false;
bool  isLining = false;
bool isDrawing = false;
bool pencilTool = false;
bool eraserTool = false;
bool isErasing = false;
bool lineTool = false;
bool paintTool = false;
bool rectTool = false;
bool circleTool = false;
int countt = 0;
int lastX = 2000;
int lastY = 2001;
bool floodfillRequested = true;
bool isHoveringA = false;
bool isHoveringB = false;
bool isHoveringC = false;
bool isHoveringD = false;
bool isHoveringE = false;
bool isHoveringF = false;
GLfloat currColorArray[3];
//float fillColor[3];
GLubyte targetColorr[3];
const float PI = 3.14159265358979323846;
float centreofX;
float centreofY;


// Define variables for the box
float boxXA = -0.94f;  // Initial X position
float boxYA = 0.43f;   // Initial Y position
float boxSizeXA;
float boxSizeYA;

// Define variables for the box
float boxXB = -0.88f;  // Initial X position
float boxYB = 0.43f;   // Initial Y position
float boxSizeXB;
float boxSizeYB;

float boxXC = -0.94f;  // Initial X position
float boxYC = 0.32f;
float boxSizeXC;
float boxSizeYC;

float boxXD = -0.88f;  // Initial X position
float boxYD = 0.32f;
float boxSizeXD;
float boxSizeYD;

float boxXE = -0.94f;  // Initial X position
float boxYE = 0.21f;
float boxSizeXE;
float boxSizeYE;

float boxXF = -0.88f;  // Initial X position
float boxYF = 0.21f;
float boxSizeXF;
float boxSizeYF;

bool isPointInsideBoxA(float x, float y) {
    //std::cout<<"any1  ";
    return (x >= (boxXA - boxSizeXA) && x <= (boxXA + boxSizeXA) &&
        y >= (boxYA - boxSizeYA) && y <= (boxYA + boxSizeYA));
}
bool isPointInsideBoxB(float x, float y) {
    //std::cout<<"any2  ";
    return (x >= (boxXB - boxSizeXB) && x <= (boxXB + boxSizeXB) &&
        y >= (boxYB - boxSizeYB) && y <= (boxYB + boxSizeYB));
}
bool isPointInsideBoxC(float x, float y) {
    //std::cout<<"any2  ";
    return (x >= (boxXC - boxSizeXC) && x <= (boxXC + boxSizeXC) &&
        y >= (boxYC - boxSizeYC) && y <= (boxYC + boxSizeYC));
}
bool isPointInsideBoxD(float x, float y) {
    //std::cout<<"any2  ";
    return (x >= (boxXD - boxSizeXD) && x <= (boxXD + boxSizeXD) &&
        y >= (boxYD - boxSizeYD) && y <= (boxYD + boxSizeYD));
}
bool isPointInsideBoxE(float x, float y) {
    //std::cout<<"any2  ";
    return (x >= (boxXE - boxSizeXE) && x <= (boxXE + boxSizeXE) &&
        y >= (boxYE - boxSizeYE) && y <= (boxYE + boxSizeYE));
}
bool isPointInsideBoxF(float x, float y) {
    //std::cout<<"any2  ";
    return (x >= (boxXF - boxSizeXF) && x <= (boxXF + boxSizeXF) &&
        y >= (boxYF - boxSizeYF) && y <= (boxYF + boxSizeYF));
}

void drawCircle(float centerX, float centerY, float radius, int numSegments) {
    glBegin(GL_TRIANGLE_FAN);
    //glColor3f(0.723f, 0.255f, 0.390f); // Set the fill color to white
    glColor3f(0.2f, 0.49f, 0.49f);

    glVertex2f(centerX, centerY); // Center vertex

    for (int i = 0; i <= numSegments; i++) {
        float theta = 2.0f * PI * static_cast<float>(i) / static_cast<float>(numSegments);
        float x = radius * cos(theta);
        float y = radius * sin(theta);
        glVertex2f(centerX + x, centerY + y);
    }

    glEnd();
}

void drawfCircle(float centerX, float centerY, float radius, int numSegments) {
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);

    for (int i = 0; i < numSegments; i++) {
        float theta = 2.0f * PI * static_cast<float>(i) / static_cast<float>(numSegments);
        float x = radius * cos(theta);
        float y = radius * sin(theta);
        glVertex2f(centerX + x, centerY + y);
    }
    glEnd();
}
void drawfrCircle(float centerX, float centerY, float radius, int numSegments) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    int widther = glutGet(GLUT_WINDOW_WIDTH);
    int heighter = glutGet(GLUT_WINDOW_HEIGHT);
    float aspect_ratioer = static_cast<float>(widther) / static_cast<float>(heighter);

    for (int i = 0; i < numSegments; i++) {
        float theta = 2.0f * PI * static_cast<float>(i) / static_cast<float>(numSegments);
        float x = radius * cos(theta);
        float y = radius * sin(theta);

        x = x / aspect_ratioer;
        glVertex2f(centerX + x, centerY + y);
    }
    glEnd();
}



void floodFillStack(int x, int y, GLfloat fillColor[3], GLubyte targetColor[3]) {
    std::stack<std::pair<int, int>> pointsToProcess;
    pointsToProcess.push(std::make_pair(x, y));

    // Create a 2D boolean array to keep track of visited pixels
    bool visited[glutGet(GLUT_WINDOW_WIDTH)][glutGet(GLUT_WINDOW_HEIGHT)];
    std::fill(&visited[0][0], &visited[0][0] + sizeof(visited), false);

    while (!pointsToProcess.empty()) {
        std::pair<int, int> current = pointsToProcess.top();
        pointsToProcess.pop();

        x = current.first;
        y = current.second;

        // Check if the pixel is out of bounds or already visited
        if (x < 0 || x >= glutGet(GLUT_WINDOW_WIDTH) || y < 0 || y >= glutGet(GLUT_WINDOW_HEIGHT) || visited[x][y]) {
            continue;
        }

        visited[x][y] = true; // Mark the pixel as visited

        GLubyte pixelColor[3];
        glReadPixels(x, glutGet(GLUT_WINDOW_HEIGHT) - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixelColor);

        // Compare the pixel color with the target color
        if (pixelColor[0] == targetColor[0] && pixelColor[1] == targetColor[1] && pixelColor[2] == targetColor[2]) {
            float jposX = 2.0f * ((float)x / glutGet(GLUT_WINDOW_WIDTH)) - 1.0f;
            float jposY = 1.0f - 2.0f * ((float)y / glutGet(GLUT_WINDOW_HEIGHT));
            paintedPoints.push_back(std::make_pair(jposX, jposY));

            // Set the pixel color to the fill color
            //glColor3ub(fillColor[0], fillColor[1], fillColor[2]);
            //glPointSize(1.0f);
            //glBegin(GL_POINTS);
            // Draw a single pixel at the current position
            //glVertex2f(jposX,jposY);
            //glEnd();
            glFlush();

            // Push neighboring pixels onto the stack
            pointsToProcess.push(std::make_pair(x + 1, y));
            pointsToProcess.push(std::make_pair(x, y + 1));
            pointsToProcess.push(std::make_pair(x - 1, y));
            pointsToProcess.push(std::make_pair(x, y - 1));
        }
    }
    //functionsToCall.push_back([tempLinePoints]() {
                //linetool(tempLinePoints);
}




void readLoopfunc(int x, int y) {
    countt++;
    std::cout << "entry with " << x << "  " << y << std::endl;

    int iposX = x;
    int iposY = glutGet(GLUT_WINDOW_HEIGHT) - y;

    GLubyte RpixelColor[3];
    if (countt >= 50) {
        //return;
    }
    //if (smallLoop.empty()) {
            //int lastX=2000;
            //int lastY=2001;

    //}



    if (!smallLoop.empty()) {
        std::pair<int, int> lastPair = smallLoop.back();
        lastX = lastPair.first;
        lastY = lastPair.second;
        std::cout << "Last element: (" << lastX << ", " << lastY << ")" << std::endl;
        std::cout << "First element: (" << smallLoop[0].first << ", " << smallLoop[0].second << ")" << std::endl;
        if (smallLoop.size() >= 2) {
            if (smallLoop[0].first == lastX && smallLoop[0].second == lastY) {
                return;
            }

        }


    }

    glReadPixels(iposX + 1, iposY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, RpixelColor);
    if (RpixelColor[0] == 0 && RpixelColor[1] == 0 && RpixelColor[2] == 0) {
        //std::cout<<"mei pehla"<<std::endl;
        //std::cout<<" x is "<<iposX+1<<"   y is "<<y<<std::endl;
        if (lastX != x || lastY != y) {
            smallLoop.push_back(std::make_pair(x, y));
            readLoopfunc(x + 1, y);
            return;

        }


    }

    glReadPixels(iposX + 1, iposY - 1, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, RpixelColor);
    if (RpixelColor[0] == 0 && RpixelColor[1] == 0 && RpixelColor[2] == 0) {
        //std::cout<<"mei dusra"<<std::endl;
        //std::cout<<" x is "<<iposX+1<<"   y is "<<y+1<<std::endl;

        if (lastX != x || lastY != y) {
            smallLoop.push_back(std::make_pair(x, y));
            readLoopfunc(x + 1, y + 1);
            return;
        }
    }

    glReadPixels(iposX, iposY - 1, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, RpixelColor);
    if (RpixelColor[0] == 0 && RpixelColor[1] == 0 && RpixelColor[2] == 0) {
        //std::cout<<"mei tisra"<<std::endl;
        //std::cout<<" x is "<<iposX<<"   y is "<<y+1<<std::endl;
        if (lastX != x || lastY != y) {
            smallLoop.push_back(std::make_pair(x, y));
            readLoopfunc(x, y + 1);
            return;
        }
    }

    glReadPixels(iposX - 1, iposY - 1, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, RpixelColor);
    if (RpixelColor[0] == 0 && RpixelColor[1] == 0 && RpixelColor[2] == 0) {
        //std::cout<<" x is "<<iposX-1<<"   y is "<<y+1<<std::endl;
        if (lastX != x || lastY != y) {
            smallLoop.push_back(std::make_pair(x, y));
            readLoopfunc(x - 1, y + 1);
            return;
        }
    }

    glReadPixels(iposX - 1, iposY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, RpixelColor);
    if (RpixelColor[0] == 0 && RpixelColor[1] == 0 && RpixelColor[2] == 0) {
        //std::cout<<" x is "<<iposX-1<<"   y is "<<y<<std::endl;
        if (lastX != x || lastY != y) {
            smallLoop.push_back(std::make_pair(x, y));
            readLoopfunc(x - 1, y);
            return;
        }
    }

    glReadPixels(iposX - 1, iposY + 1, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, RpixelColor);
    if (RpixelColor[0] == 0 && RpixelColor[1] == 0 && RpixelColor[2] == 0) {
        //std::cout<<" x is "<<iposX-1<<"   y is "<<y-1<<std::endl;
        if (lastX != x || lastY != y) {
            smallLoop.push_back(std::make_pair(x, y));
            readLoopfunc(x - 1, y - 1);
            return;
        }
    }

    glReadPixels(iposX, iposY + 1, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, RpixelColor);
    if (RpixelColor[0] == 0 && RpixelColor[1] == 0 && RpixelColor[2] == 0) {
        //std::cout<<" x is "<<iposX<<"   y is "<<y-1<<std::endl;
        if (lastX != x || lastY != y) {
            smallLoop.push_back(std::make_pair(x, y));
            readLoopfunc(x, y - 1);
            return;
        }
    }

    glReadPixels(iposX + 1, iposY + 1, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, RpixelColor);
    if (RpixelColor[0] == 0 && RpixelColor[1] == 0 && RpixelColor[2] == 0) {
        //std::cout<<" x is "<<iposX+1<<"   y is "<<y-1<<std::endl;
        if (lastX != x || lastY != y) {
            smallLoop.push_back(std::make_pair(x, y));
            readLoopfunc(x + 1, y - 1);
            return;
        }
    }
    std::cout << "last tk pahuch gya";
    return;







}

void InsideShape(int x, int y) {
    std::cout << "euij";
    int iposX = x;
    int iposY = glutGet(GLUT_WINDOW_HEIGHT) - y;
    GLubyte RpixelColor[3];
    //glReadPixels(iposX, iposY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, RpixelColor);

    int leftgoLine = iposX;
    std::cout << "ye dekho   " << leftgoLine;
    for (leftgoLine;true;leftgoLine--) {
        //std::cout<<"chl na reeeeeio";
        glReadPixels(leftgoLine, iposY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, RpixelColor);
        //std::cout << "R: " << (int)RpixelColor[0] << " G: " << (int)RpixelColor[1] << " B: " << (int)RpixelColor[2] << std::endl;
        if (RpixelColor[0] == 0 && RpixelColor[1] == 0 && RpixelColor[2] == 0) {
            //std::cout<<"aagiya  at "<< "R: " << RpixelColor[0] << " G: " << (int)RpixelColor[1] << " B: " << (int)RpixelColor[2] << std::endl;
            std::cout << "points are  " << leftgoLine << "    " << iposY << std::endl;
            //smallLoop.push_back(leftgoLine,y);
            readLoopfunc(leftgoLine, y);

            return;
        }
        //leftgoLine--;

    }




}


void drawCircleRealTime() {
    glColor3f(currColorArray[0], currColorArray[1], currColorArray[2]);
    glBegin(GL_LINE_LOOP);
    for (const auto point : circleVector) {
        glVertex2f(point.first, point.second);
    }



    glEnd();
}

void circleToolPermanent(std::vector<std::pair<float, float>> tempCirclePoints, const float* temppaintColorarr13) {
    glColor3f(temppaintColorarr13[0], temppaintColorarr13[1], temppaintColorarr13[2]);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    for (const auto point : tempCirclePoints) {
        glVertex2f(point.first, point.second);
    }

    glEnd();

}



void rectangletoolrealtime() {
    glColor3f(currColorArray[0], currColorArray[1], currColorArray[2]);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (const auto point : rectPoints) {
        glVertex2f(point.first, point.second);
    }

    glEnd();

}

void recttoolP(std::vector<std::pair<float, float>> temprectPoints, const float* temppaintColorarr12) {
    glColor3f(temppaintColorarr12[0], temppaintColorarr12[1], temppaintColorarr12[2]);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (const auto point : temprectPoints) {
        glVertex2f(point.first, point.second);
    }

    glEnd();


}

void lineToolrealtime() {
    glColor3f(currColorArray[0], currColorArray[1], currColorArray[2]);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (const auto point : lineCurves) {
        glVertex2f(point.first, point.second);
    }

    glEnd();
}

void linetool(std::vector<std::pair<float, float>> tempLinePoints, const float* temppaintColorarr7) {
    glColor3f(temppaintColorarr7[0], temppaintColorarr7[1], temppaintColorarr7[2]);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (const auto point : tempLinePoints) {
        glVertex2f(point.first, point.second);
    }

    glEnd();
}

void PaintFillBuffer(std::vector<std::pair<float, float>> temppaintedPoints, const float* temppaintColorarr) {
    glColor3f(temppaintColorarr[0], temppaintColorarr[1], temppaintColorarr[2]);
    //glLineWidth(1.0f);
    glPointSize(1.0f);
    glBegin(GL_POINTS);
    for (const auto point : temppaintedPoints) {
        glVertex2f(point.first, point.second);
    }

    glEnd();
}

void drawline(std::vector<std::pair<float, float>> tempPoints, const float* tempColorarr) {
    // Draw lines

    glColor3f(tempColorarr[0], tempColorarr[1], tempColorarr[2]);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (const auto point : tempPoints) {
        glVertex2f(point.first, point.second);
    }
    glEnd();



}
void drawlinerealtime() {
    glColor3f(currColorArray[0], currColorArray[1], currColorArray[2]);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (const auto point : points) {
        glVertex2f(point.first, point.second);
    }
    glEnd();

}

void PaintToolMemory(std::vector<std::pair<float, float>> rempPoints) {
    glColor3f(1.0f, 0.0f, 0.0f); // Red

    // Draw filled shape using GL_TRIANGLE_FAN
    glBegin(GL_TRIANGLE_FAN);
    for (const auto point : rempPoints) {
        glVertex2f(point.first, point.second);
    }

    glEnd();

}

void EraserToolrealtime() {
    //ERASER

    glPointSize(20.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    std::cout << "-";
    for (const auto era : eraserCurves) {
        glVertex2f(era.first, era.second);

    }
    //glVertex2f(0.0f,0.0f);
    glEnd();



}
void EraserTool(std::vector<std::pair<float, float>> temperasePoints) {
    //ERASER
    glPointSize(20.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    std::cout << "-";
    for (const auto era : temperasePoints) {
        glVertex2f(era.first, era.second);

    }
    //glVertex2f(0.0f,0.0f);
    glEnd();



}

void scanLine(int x, int y, GLubyte fillColor[3], GLubyte targetColor[3]) {
    GLubyte blackColor[] = { 0, 0, 0 };



    //float posX = (2.0f * (float)x / glutGet(GLUT_WINDOW_WIDTH)) - 1.0f;
    //float posY = 1.0f - (2.0f * (float)y / glutGet(GLUT_WINDOW_HEIGHT));

    for (int j = 100;j <= 500;j++) {
        for (int i = 0;i <= 800;i++) {

            int iposX = i;
            int iposY = (glutGet(GLUT_WINDOW_HEIGHT) - j);

            GLubyte pixelColor[3];
            glReadPixels(iposX, iposY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixelColor);
            if (pixelColor[0] == blackColor[0] && pixelColor[1] == blackColor[1] && pixelColor[2] == blackColor[2]) {
                paintVector.push_back(std::make_pair(i, j));
                std::cout << "x :" << i << "y :" << j << std::endl;

            }

        }
    }


}

void keyboard(unsigned char key, int x, int y) {

    if (key == 69 || key == 101) {
        eraserTool = true;
        pencilTool = false;
        lineTool = false;
        paintTool = false;
        rectTool = false;
        //isErasing=false;
        std::cout << "Eraser tool enabled" << std::endl;
    }
    else if (key == 80 || key == 112) {
        eraserTool = false;
        pencilTool = true;
        lineTool = false;
        paintTool = false;
        rectTool = false;
        std::cout << "Pencil tool enabled" << std::endl;
    }
    else if (key == 108 || key == 86) {
        eraserTool = false;
        pencilTool = false;
        lineTool = true;
        paintTool = false;
        rectTool = false;
        std::cout << "Line tool enabled" << std::endl;
    }
    else if (key == 102 || key == 80) {

        GLubyte fillColor[] = { 0, 100, 100 };
        GLubyte targetColor[] = { 255,255,255 };
        //rango=true;



        rectTool = false;
        eraserTool = false;
        pencilTool = false;
        lineTool = false;
        paintTool = true;
        //insideshape


        glutPostRedisplay();
        std::cout << "paint tool enabled" << std::endl;
    }
    else if (key == 'G' || key == 'g') {
        currColorArray[0] = 0.0f;
        currColorArray[1] = 1.0f;
        currColorArray[2] = 0.0f;
    }
    else if (key == 'B' || key == 'b') {
        currColorArray[0] = 0.0f;
        currColorArray[1] = 0.0f;
        currColorArray[2] = 0.0f;
    }
    else if (key == 'R' || key == 'r') {
        currColorArray[0] = 1.0f;
        currColorArray[1] = 0.0f;
        currColorArray[2] = 0.0f;
    }
    else if (key == 'Y' || key == 'y') {
        currColorArray[0] = 1.0f;
        currColorArray[1] = 1.0f;
        currColorArray[2] = 0.0f;
    }
    else if (key == 'Q' || key == 'q') {
        rectTool = true;
        eraserTool = false;
        pencilTool = false;
        lineTool = false;
        paintTool = false;
    }

    else if (key == 'Z' || key == 'z') {
        if (!functionsToCall.empty()) {
            functionsToCall.pop_back();
        }
        glutPostRedisplay();


    }







}



void handleMouseClick(int button, int state, int x, int y) {
    float posX = (2.0f * (float)x / glutGet(GLUT_WINDOW_WIDTH)) - 1.0f;
    float posY = 1.0f - (2.0f * (float)y / glutGet(GLUT_WINDOW_HEIGHT));

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {




            std::cout << "Red: " << currColorArray[0] << std::endl;
            std::cout << "Green: " << currColorArray[1] << std::endl;
            std::cout << "Blue: " << currColorArray[2] << std::endl;


            if ((posX >= 0.64f && posX <= 0.995) && (posY >= 0.0f && posY <= 0.6f)) {
                std::cout << "   posX= " << posX << "  posY=  " << posY;
                std::cout << "  hora  ";
                std::cout << x << " and  " << glutGet(GLUT_WINDOW_HEIGHT) - y;
                glReadPixels(x, glutGet(GLUT_WINDOW_HEIGHT) - y, 1, 1, GL_RGB, GL_FLOAT, currColorArray);
                std::cout << "Red: " << currColorArray[0] << std::endl;
                std::cout << "Green: " << currColorArray[1] << std::endl;
                std::cout << "Blue: " << currColorArray[2] << std::endl;

            }
            if (isPointInsideBoxA(posX, posY)) {
                eraserTool = false;
                pencilTool = true;
                lineTool = false;
                paintTool = false;
                circleTool = false;
                rectTool = false;
                //isSelectedA=true;
                std::cout << "Pencil tool enabled" << std::endl;

            }
            if (isPointInsideBoxB(posX, posY)) {
                eraserTool = true;
                pencilTool = false;
                lineTool = false;
                paintTool = false;
                circleTool = false;
                rectTool = false;

                std::cout << "Eraser tool enabled" << std::endl;

            }
            if (isPointInsideBoxC(posX, posY)) {
                eraserTool = false;
                pencilTool = false;
                lineTool = false;
                paintTool = true;
                circleTool = false;
                rectTool = false;

                std::cout << "paint tool enabled" << std::endl;

            }
            if (isPointInsideBoxD(posX, posY)) {
                eraserTool = false;
                pencilTool = false;
                lineTool = true;
                paintTool = false;
                circleTool = false;
                rectTool = false;

                std::cout << "line tool enabled" << std::endl;

            }
            if (isPointInsideBoxE(posX, posY)) {
                eraserTool = false;
                pencilTool = false;
                lineTool = false;
                paintTool = false;
                circleTool = true;
                rectTool = false;

                std::cout << "paint tool enabled" << std::endl;

            }
            if (isPointInsideBoxF(posX, posY)) {
                eraserTool = false;
                pencilTool = false;
                lineTool = false;
                paintTool = false;
                circleTool = false;
                rectTool = true;

                std::cout << "paint tool enabled" << std::endl;

            }

            if (paintTool) {
                glReadPixels(x, glutGet(GLUT_WINDOW_HEIGHT) - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, targetColorr);

                floodFillStack(x, y, currColorArray, targetColorr);

                std::vector<std::pair<float, float>> temppaintingPoints = paintedPoints; // Capture the current state
                //float tempColorarr[3] = currColorArray;

                float temppaintColorarr[3];  // Uninitialized array


                for (int i = 0; i < 3; ++i) {
                    temppaintColorarr[i] = currColorArray[i];  // Copies whatever values happen to be in sourceArray
                }


                functionsToCall.push_back([temppaintingPoints, temppaintColorarr]() {
                    float colorpaintCopy[3] = { temppaintColorarr[0], temppaintColorarr[1], temppaintColorarr[2] };
                    PaintFillBuffer(temppaintingPoints, colorpaintCopy);
                    });
                paintedPoints.clear();






            }



            if (pencilTool) {
                points.push_back(std::make_pair(posX, posY));
                isDrawing = true;

            }

            if (eraserTool) {
                std::cout << "erasing re";
                eraserCurves.push_back(std::make_pair(posX, posY));
                isErasing = true;

            }
            if (lineTool) {
                std::cout << "line ";
                lineCurves[0] = std::make_pair(posX, posY);
                lineCurves[1] = std::make_pair(posX, posY);
                isLining = true;

            }
            if (paintTool) {
                //innerPolygon.push_back(std::make_pair(posX, posY));
                //InsideShape(x,y);


                std::cout << "runnning yes";
                //std::vector<std::pair<float, float>> rempPoints = points; // Capture the current state
                //functionsToCall.push_back([rempPoints]() {
                //PaintToolMemory(rempPoints);
                //});
                //glutPostRedisplay();


            }
            if (rectTool) {
                rectPoints.push_back(std::make_pair(posX, posY));
                rectPoints.push_back(std::make_pair(posX, posY));
                rectPoints.push_back(std::make_pair(posX, posY));
                rectPoints.push_back(std::make_pair(posX, posY));
                rectPoints.push_back(std::make_pair(posX, posY));
                isDrawingRect = true;
                //isDrawingRect=true;

            }
            if (circleTool) {
                centreofX = posX;
                centreofY = posY;
                isCircling = true;
                //drawCircleRealTime(, float centerY, float radius, int numSegments)
            }


            //functionsToCall.push_back([tempPoints = points]() {
               //drawline(tempPoints);
           //});
        }
        else if (state == GLUT_UP) {
            if (pencilTool) {
                isDrawing = false;

                std::vector<std::pair<float, float>> tempPoints = points; // Capture the current state
                //float tempColorarr[3] = currColorArray;

                float tempColorarr[3];  // Uninitialized array


                for (int i = 0; i < 3; ++i) {
                    tempColorarr[i] = currColorArray[i];  // Copies whatever values happen to be in sourceArray
                }


                functionsToCall.push_back([tempPoints, tempColorarr]() {
                    float colorCopy[3] = { tempColorarr[0], tempColorarr[1], tempColorarr[2] };
                    drawline(tempPoints, colorCopy);
                    });


                points.clear();
            }
            //ERASER TOOL START
            if (eraserTool) {
                std::vector<std::pair<float, float>> temperasePoints = eraserCurves; // Capture the current state
                functionsToCall.push_back([temperasePoints]() {
                    EraserTool(temperasePoints);
                    });
                eraserCurves.clear();
                //isErasing=false;
            }

            if (rectTool) {
                rectPoints[1].first = rectPoints[0].first;
                rectPoints[1].second = posY;
                rectPoints[2].first = posX;
                rectPoints[2].second = posY;
                rectPoints[3].first = posX;
                rectPoints[3].second = rectPoints[0].second;
                rectPoints[4].first = rectPoints[0].first;
                rectPoints[4].second = rectPoints[0].second;
                isDrawingRect = false;

                float tempColorarr12[3];  // Uninitialized array
                std::vector<std::pair<float, float>> temprectanglPoints = rectPoints;


                for (int i = 0; i < 3; ++i) {
                    tempColorarr12[i] = currColorArray[i];  // Copies whatever values happen to be in sourceArray
                }




                //std::vector<std::pair<float, float>> tempLinePoints = lineCurves; // Capture the current state
                functionsToCall.push_back([temprectanglPoints, tempColorarr12]() {
                    float colorCopy12[3] = { tempColorarr12[0], tempColorarr12[1], tempColorarr12[2] };
                    recttoolP(temprectanglPoints, colorCopy12);
                    });
                rectPoints.clear();

                //rectPoints.push_back(std::make_pair(rectPoints[0].first,posY));
                //rectPoints.push_back(std::make_pair(posX,posY));
               // rectPoints.push_back(std::make_pair(posX,rectPoints[0].second));
                //rectPoints.push_back(std::make_pair(rectPoints[0].first,rectPoints[0].second));
                //isDrawingRect=true;

            }

            if (lineTool) {

                std::vector<std::pair<float, float>> tempLinePoints = lineCurves; // Capture the current state
                //float tempColorarr[3] = currColorArray;

                float tempColorarr6[3];  // Uninitialized array


                for (int i = 0; i < 3; ++i) {
                    tempColorarr6[i] = currColorArray[i];  // Copies whatever values happen to be in sourceArray
                }



                isLining = false;
                //std::vector<std::pair<float, float>> tempLinePoints = lineCurves; // Capture the current state
                functionsToCall.push_back([tempLinePoints, tempColorarr6]() {
                    float colorCopy7[3] = { tempColorarr6[0], tempColorarr6[1], tempColorarr6[2] };
                    linetool(tempLinePoints, colorCopy7);
                    });
                lineCurves = lineCurves1;
                //lineCurves.clear();
                    //isErasing=false;
            }
            if (circleTool) {
                isCircling = false;

                std::vector<std::pair<float, float>> tempCirclePoints = circleVector; // Capture the current state
                //float tempColorarr[3] = currColorArray;

                float tempColorarr13[3];  // Uninitialized array


                for (int i = 0; i < 3; ++i) {
                    tempColorarr13[i] = currColorArray[i];  // Copies whatever values happen to be in sourceArray
                }



                isLining = false;
                //std::vector<std::pair<float, float>> tempLinePoints = lineCurves; // Capture the current state
                functionsToCall.push_back([tempCirclePoints, tempColorarr13]() {
                    float colorCopy13[3] = { tempColorarr13[0], tempColorarr13[1], tempColorarr13[2] };
                    circleToolPermanent(tempCirclePoints, colorCopy13);
                    });
                circleVector.clear();



            }


            glutPostRedisplay();



        }
    }
}

void handleMouseMotion(int x, int y) {

    float currX = (2.0f * (float)x / glutGet(GLUT_WINDOW_WIDTH)) - 1.0f;
    float currY = 1.0f - (2.0f * (float)y / glutGet(GLUT_WINDOW_HEIGHT));
    if (pencilTool && isDrawing) {
        points.push_back(std::make_pair(currX, currY));
    }
    if (eraserTool && isErasing) {
        eraserCurves.push_back(std::make_pair(currX, currY));
        //std::cout<<"testingyusd";
    }

    if (lineTool && isLining) {
        lineCurves[1] = std::make_pair(currX, currY);
        //std::cout<<lineCurves[1];
    }
    if (rectTool && isDrawingRect) {
        rectPoints[1].first = rectPoints[0].first;
        rectPoints[1].second = currY;
        rectPoints[2].first = currX;
        rectPoints[2].second = currY;
        rectPoints[3].first = currX;
        rectPoints[3].second = rectPoints[0].second;
        rectPoints[4].first = rectPoints[0].first;
        rectPoints[4].second = rectPoints[0].second;
    }
    if (circleTool && isCircling) {
        circleVector.clear();

        int widthii = glutGet(GLUT_WINDOW_WIDTH);
        int heightii = glutGet(GLUT_WINDOW_HEIGHT);
        float aspect_ratioii = static_cast<float>(widthii) / static_cast<float>(heightii);

        float radii = sqrt((centreofX - currX) * (centreofX - currX) + (centreofY - currY) * (centreofY - currY));
        for (int i = 0; i < 100; i++) {
            float theta = 2.0f * PI * static_cast<float>(i) / static_cast<float>(100);
            float x = radii * cos(theta);
            float y = radii * sin(theta);

            // Normalize the coordinates based on the aspect ratio
            x /= aspect_ratioii;

            circleVector.push_back(std::make_pair(centreofX + x, centreofY + y));

        }

    }
    glutPostRedisplay();

}

void passiveMouseMotion(int x, int y) {
    float posX = (2.0f * (float)x / glutGet(GLUT_WINDOW_WIDTH)) - 1.0f;
    float posY = 1.0f - (2.0f * (float)y / glutGet(GLUT_WINDOW_HEIGHT));
    //std::cout<<"passiveness   ";
    if (isPointInsideBoxA(posX, posY)) {
        isHoveringA = true;
        //std::cout<<"hovering true   ";
        glutPostRedisplay();


    }
    else {
        isHoveringA = false;
        //std::cout<<"hovering false   ";
        glutPostRedisplay();
    }

    if (isPointInsideBoxB(posX, posY)) {
        isHoveringB = true;
        // std::cout<<"hovering true   ";
        glutPostRedisplay();


    }
    else {
        isHoveringB = false;
        //std::cout<<"hovering false   ";
        glutPostRedisplay();
    }
    if (isPointInsideBoxC(posX, posY)) {
        isHoveringC = true;
        std::cout << "hovering true   ";
        glutPostRedisplay();


    }
    else {
        isHoveringC = false;
        //std::cout<<"hovering false   ";
        glutPostRedisplay();
    }
    if (isPointInsideBoxD(posX, posY)) {
        isHoveringD = true;
        std::cout << "hovering true   ";
        glutPostRedisplay();


    }
    else {
        isHoveringD = false;
        //std::cout<<"hovering false   ";
        glutPostRedisplay();
    }
    if (isPointInsideBoxE(posX, posY)) {
        isHoveringE = true;
        std::cout << "hovering true   ";
        glutPostRedisplay();


    }
    else {
        isHoveringE = false;
        //std::cout<<"hovering false   ";
        glutPostRedisplay();
    }
    if (isPointInsideBoxF(posX, posY)) {
        isHoveringF = true;
        std::cout << "hovering true   ";
        glutPostRedisplay();


    }
    else {
        isHoveringF = false;
        //std::cout<<"hovering false   ";
        glutPostRedisplay();
    }




}


void display() {

    glClear(GL_COLOR_BUFFER_BIT);

    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);

    //std::cout<<std::endl<<"display getting called"<<std::endl;

    glColor3f(1.0f, 1.0f, 1.0f); // Set the fill color to white
    glBegin(GL_QUADS);
    glVertex2f(-0.77f, -0.9f); // Bottom-left corner (shifted to the left)
    glVertex2f(-0.77f, 0.9f);  // Top-left corner (shifted to the left)
    glVertex2f(0.63f, 0.9f);   // Top-right corner (shifted to the left)
    glVertex2f(0.63f, -0.9f);  // Bottom-right corner (shifted to the left)
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(4.0f);

    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.77f, -0.9f); // Bottom-left corner (shifted to the left)
    glVertex2f(-0.77f, 0.9f);  // Top-left corner (shifted to the left)
    glVertex2f(0.63f, 0.9f);   // Top-right corner (shifted to the left)
    glVertex2f(0.63f, -0.9f);  // Bottom-right corner (shifted to the left)
    glEnd();

    glLineWidth(1.0f);



    glColor3f(0.0f, 0.0f, 1.0f); // Set the fill color to white



    GLubyte fillColor[] = { 0, 100, 100 };
    //GLubyte blackColor[] = {0, 0, 0};
    GLubyte targetColor[] = { 255,255,255 };





    for (const auto& func : functionsToCall) {
        func();
    }
    drawlinerealtime();
    lineToolrealtime();
    //PaintFillBuffer(paintedPoints);
    rectangletoolrealtime();

    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //glOrtho(-1.0 * aspect_ratio, 1.0 * aspect_ratio, -1.0, 1.0, -1.0, 1.0);
   // glMatrixMode(GL_MODELVIEW);

    drawCircleRealTime();

    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //glMatrixMode(GL_MODELVIEW);

    EraserToolrealtime();

    //PaintToolMemory(points);



    glColor3f(0.2f, 0.49f, 0.49f);

    glBegin(GL_QUADS);
    glVertex2f(-0.97f, 0.5f); // Top-left corner
    glVertex2f(-0.97f, -0.5f); // Bottom-left corner
    glVertex2f(-0.85f, -0.5f); // Bottom-right corner
    glVertex2f(-0.85f, 0.5f); // Top-right corner
    glEnd();



    //glColor3f(0.0f, 0.0f, 0.0f); // Set color to black
    glLineWidth(2.0f);           // Set line width



    // Draw the rectangle
    //float ndcX = 2.0f * (200.0f / glutGet(GLUT_WINDOW_WIDTH)) - 1.0f;
    //float ndcY = 1.0f - 2.0f * (300.0f / glutGet(GLUT_WINDOW_HEIGHT));

    //float size = 600.0f / glutGet(GLUT_WINDOW_WIDTH); // Size in NDC

    //Draw the rectangle
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(4.0f);

    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.97f, 0.5f); // Top-left corner
    glVertex2f(-0.97f, -0.5f); // Bottom-left corner
    glVertex2f(-0.85f, -0.5f); // Bottom-right corner
    glVertex2f(-0.85f, 0.5f); // Top-right corner
    glEnd();
    glLineWidth(1.0f);

    if (isHoveringA || pencilTool) {
        glLineWidth(2.0f);
        glColor3f(0.0f, 0.0f, 0.0f);

    }
    else {
        glColor3f(0.2f, 0.49f, 0.49f);

    }

    // Draw the rectangle
    float ndcX = -0.94f;
    float ndcY = 0.43f;
    int windowWidthl = glutGet(GLUT_WINDOW_WIDTH);
    int windowHeightl = glutGet(GLUT_WINDOW_HEIGHT);

    float sizeX = 33.0f / windowWidthl; // Width in NDC
    float sizeY = 30.0f / windowHeightl; // Height in NDC

    // Draw the square
    glBegin(GL_LINE_LOOP);
    glVertex2f(ndcX - sizeX, ndcY + sizeY); // Top-left corner
    glVertex2f(ndcX - sizeX, ndcY - sizeY); // Bottom-left corner
    glVertex2f(ndcX + sizeX, ndcY - sizeY); // Bottom-right corner
    glVertex2f(ndcX + sizeX, ndcY + sizeY); // Top-right corner
    glEnd();
    glLineWidth(1.0f);


    if (isHoveringB || eraserTool) {
        glColor3f(0.0f, 0.0f, 0.0f);
        glLineWidth(2.0f);

    }
    else {
        glColor3f(0.2f, 0.49f, 0.49f);

    }

    ndcX = -0.88f;
    ndcY = 0.43f;
    windowWidthl = glutGet(GLUT_WINDOW_WIDTH);
    windowHeightl = glutGet(GLUT_WINDOW_HEIGHT);

    sizeX = 33.0f / windowWidthl; // Width in NDC
    sizeY = 30.0f / windowHeightl; // Height in NDC

    // Draw the square
    glBegin(GL_LINE_LOOP);
    glVertex2f(ndcX - sizeX, ndcY + sizeY); // Top-left corner
    glVertex2f(ndcX - sizeX, ndcY - sizeY); // Bottom-left corner
    glVertex2f(ndcX + sizeX, ndcY - sizeY); // Bottom-right corner
    glVertex2f(ndcX + sizeX, ndcY + sizeY); // Top-right corner
    glEnd();
    glLineWidth(1.0f);

    if (isHoveringC || paintTool) {
        glLineWidth(2.0f);
        glColor3f(0.0f, 0.0f, 0.0f);

    }
    else {
        glColor3f(0.2f, 0.49f, 0.49f);

    }



    ndcX = -0.94f;
    ndcY = 0.32f;
    windowWidthl = glutGet(GLUT_WINDOW_WIDTH);
    windowHeightl = glutGet(GLUT_WINDOW_HEIGHT);

    sizeX = 33.0f / windowWidthl; // Width in NDC
    sizeY = 30.0f / windowHeightl; // Height in NDC

    // Draw the square
    glBegin(GL_LINE_LOOP);
    glVertex2f(ndcX - sizeX, ndcY + sizeY); // Top-left corner
    glVertex2f(ndcX - sizeX, ndcY - sizeY); // Bottom-left corner
    glVertex2f(ndcX + sizeX, ndcY - sizeY); // Bottom-right corner
    glVertex2f(ndcX + sizeX, ndcY + sizeY); // Top-right corner
    glEnd();
    glLineWidth(1.0f);

    if (isHoveringD || lineTool) {
        glLineWidth(2.0f);
        glColor3f(0.0f, 0.0f, 0.0f);

    }
    else {
        glColor3f(0.2f, 0.49f, 0.49f);

    }



    ndcX = -0.88f;
    ndcY = 0.32f;

    windowWidthl = glutGet(GLUT_WINDOW_WIDTH);
    windowHeightl = glutGet(GLUT_WINDOW_HEIGHT);

    sizeX = 33.0f / windowWidthl; // Width in NDC
    sizeY = 30.0f / windowHeightl; // Height in NDC

    // Draw the square
    glBegin(GL_LINE_LOOP);
    glVertex2f(ndcX - sizeX, ndcY + sizeY); // Top-left corner
    glVertex2f(ndcX - sizeX, ndcY - sizeY); // Bottom-left corner
    glVertex2f(ndcX + sizeX, ndcY - sizeY); // Bottom-right corner
    glVertex2f(ndcX + sizeX, ndcY + sizeY); // Top-right corner
    glEnd();

    glLineWidth(1.0f);

    if (isHoveringE || circleTool) {
        glLineWidth(2.0f);
        glColor3f(0.0f, 0.0f, 0.0f);

    }
    else {
        glColor3f(0.2f, 0.49f, 0.49f);

    }
    //glLineWidth(1.0f);


    ndcX = -0.94f;
    ndcY = 0.21f;

    windowWidthl = glutGet(GLUT_WINDOW_WIDTH);
    windowHeightl = glutGet(GLUT_WINDOW_HEIGHT);

    sizeX = 33.0f / windowWidthl; // Width in NDC
    sizeY = 30.0f / windowHeightl; // Height in NDC

    // Draw the square
    glBegin(GL_LINE_LOOP);
    glVertex2f(ndcX - sizeX, ndcY + sizeY); // Top-left corner
    glVertex2f(ndcX - sizeX, ndcY - sizeY); // Bottom-left corner
    glVertex2f(ndcX + sizeX, ndcY - sizeY); // Bottom-right corner
    glVertex2f(ndcX + sizeX, ndcY + sizeY); // Top-right corner
    glEnd();

    if (isHoveringF || rectTool) {
        glLineWidth(2.0f);
        glColor3f(0.0f, 0.0f, 0.0f);

    }
    else {
        glColor3f(0.2f, 0.49f, 0.49f);

    }
    //glLineWidth(1.0f);


    ndcX = -0.88f;
    ndcY = 0.21f;

    windowWidthl = glutGet(GLUT_WINDOW_WIDTH);
    windowHeightl = glutGet(GLUT_WINDOW_HEIGHT);

    sizeX = 33.0f / windowWidthl; // Width in NDC
    sizeY = 30.0f / windowHeightl; // Height in NDC


    // Draw the square
    glBegin(GL_LINE_LOOP);
    glVertex2f(ndcX - sizeX, ndcY + sizeY); // Top-left corner
    glVertex2f(ndcX - sizeX, ndcY - sizeY); // Bottom-left corner
    glVertex2f(ndcX + sizeX, ndcY - sizeY); // Bottom-right corner
    glVertex2f(ndcX + sizeX, ndcY + sizeY); // Top-right corner
    glEnd();
    glColor3f(0.2f, 0.49f, 0.49f);
    glLineWidth(1.0f);

    glPushMatrix();

    // Define a scaling factor
    float Jscale = 0.07f;  // Adjust the scale factor as needed

    // Define the translation values
    float JtranslateX = -0.89f;
    float JtranslateY = 0.445f;

    // Apply the translation
    glTranslatef(JtranslateX, JtranslateY, 0.0f);

    // Apply a 45-degree rotation
    glRotatef(30.0f, 0.0f, 0.0f, 1.0f);

    glColor3f(1.0f, 1.0f, 1.0f);  // Set the drawing color to white

    glBegin(GL_LINES);

    // Define the vertices for the lower rectangle ABCD
    float A[2] = { -0.1f * Jscale, -0.6f * Jscale };  // Vertex A
    float B[2] = { 0.1f * Jscale, -0.6f * Jscale };   // Vertex B
    float C[2] = { 0.1f * Jscale, -0.2f * Jscale };   // Vertex C
    float D[2] = { -0.1f * Jscale, -0.2f * Jscale };  // Vertex D

    // Define the vertices for the upper rectangle EFGH
    // Move the lower rectangle up by 1/5th the length of the larger side
    float E[2] = { A[0], (A[1] + 0.2f * (C[1] - B[1])) * Jscale };  // Vertex E
    float F[2] = { B[0], (B[1] + 0.2f * (C[1] - B[1])) * Jscale };  // Vertex F
    float G[2] = { C[0], (C[1] + 0.2f * (C[1] - B[1])) * Jscale };  // Vertex G
    float H[2] = { D[0], (D[1] + 0.2f * (C[1] - B[1])) * Jscale };  // Vertex H

    // Draw the lower rectangle ABCD as line segments
    glVertex2f(A[0], A[1]);  // Vertex A
    glVertex2f(B[0], B[1]);  // Vertex B

    glVertex2f(B[0], B[1]);  // Vertex B
    glVertex2f(C[0], C[1]);  // Vertex C

    glVertex2f(C[0], C[1]);  // Vertex C
    glVertex2f(D[0], D[1]);  // Vertex D

    glVertex2f(D[0], D[1]);  // Vertex D
    glVertex2f(A[0], A[1]);  // Vertex A

    // Draw the upper rectangle EFGH as line segments
    glVertex2f(E[0], E[1]);  // Vertex E
    glVertex2f(F[0], F[1]);  // Vertex F

    glVertex2f(F[0], F[1]);  // Vertex F
    glVertex2f(G[0], G[1]);  // Vertex G

    glVertex2f(G[0], G[1]);  // Vertex G
    glVertex2f(H[0], H[1]);  // Vertex H

    glVertex2f(H[0], H[1]);  // Vertex H
    glVertex2f(E[0], E[1]);  // Vertex E

    // Draw lines connecting the corresponding vertices
    glVertex2f(A[0], A[1]);  // Vertex A
    glVertex2f(E[0], E[1]);  // Vertex E

    glVertex2f(B[0], B[1]);  // Vertex B
    glVertex2f(F[0], F[1]);  // Vertex F

    glVertex2f(C[0], C[1]);  // Vertex C
    glVertex2f(G[0], G[1]);  // Vertex G

    glVertex2f(D[0], D[1]);  // Vertex D
    glVertex2f(H[0], H[1]);  // Vertex H

    glEnd();

    glPopMatrix();










    // Save the current modelview matrix
    glPushMatrix();

    // Define a scaling factor
    float scale = 0.08f;  // Adjust the scale factor as needed

    // Define the translation values
    float translateX = -0.95f;
    float translateY = 0.45f;

    // Apply the translation
    glTranslatef(translateX, translateY, 0.0f);

    // Apply a 45-degree rotation
    glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);  // Set the drawing color to white  // Set the drawing color to white

    // Draw the rectangle with scaled coordinates
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.05f * scale, -0.6f * scale); // Bottom-left
    glVertex2f(-0.05f * scale, 0.0f * scale);  // Top-left
    glVertex2f(0.05f * scale, 0.0f * scale);   // Top-right
    glVertex2f(0.05f * scale, -0.6f * scale);  // Bottom-right
    glEnd();

    // Calculate the coordinates for the inverted equilateral triangle
    float triangleHeight = 0.0866f * scale; // Height of the equilateral triangle
    float triangleBase = 0.1f * scale;     // Base of the equilateral triangle

    // Draw the inverted equilateral triangle with scaled coordinates
    glBegin(GL_LINE_LOOP);
    glVertex2f(-triangleBase / 2.0f, -0.6f * scale);                    // Bottom vertex of triangle
    glVertex2f(triangleBase / 2.0f, -0.6f * scale);                     // Bottom-right vertex of triangle
    glVertex2f(0.0f, -0.6f * scale - triangleHeight); // Top vertex of triangle (inverted)
    glEnd();

    // Restore the previous modelview matrix
    glPopMatrix();

    glPushMatrix();
    float translate4X = -0.93f;
    float translate4Y = 0.295f;

    // Apply the translation
    glTranslatef(translate4X, translate4Y, 0.0f);


    glColor3f(currColorArray[0], currColorArray[1], currColorArray[2]); // Blue color
    float scaleFactor = 0.06f;
    glRotatef(20.0f, 0.0f, 0.0f, 1.0f);

    glBegin(GL_POLYGON);
    glVertex2f(-0.12 * scaleFactor, 0.0 * scaleFactor);  // Bottom left (slightly reduced width)
    glVertex2f(0.12 * scaleFactor, 0.0 * scaleFactor);   // Bottom right (slightly reduced width)
    glVertex2f(0.2 * scaleFactor, 0.8 * scaleFactor);    // Top right (increased height)
    glVertex2f(-0.2 * scaleFactor, 0.8 * scaleFactor);   // Top left (increased height)
    glEnd();

    glPopMatrix();

    glPushMatrix();
    float translate5X = -0.885f;
    float translate5Y = 0.32f;

    // Apply the translation
    glTranslatef(translate5X, translate5Y, 0.0f);


    glColor3f(1.0f, 1.0f, 1.0f); // Blue color
    float scale5Factor = 0.09f;
    //glRotatef(20.0f, 0.0f, 0.0f, 1.0f);

    glBegin(GL_LINES);
    glVertex2f(0.2 * scale5Factor, 0.2 * scale5Factor);  // Bottom left (slightly reduced width)
    glVertex2f(-0.2 * scale5Factor, -0.2 * scale5Factor);   // Bottom right (slightly reduced width)

    glEnd();

    glPopMatrix();






    if (paintTool) {
        //floodFillStack(640,360,fillColor,targetColor);

        //glutPostRedisplay();
        //floodfillRequested=false;


    }




    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0 * aspect_ratio, 1.0 * aspect_ratio, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    //glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    glTranslatef(1.51f, 0.3f, 0.0f);

    glColor3f(0.31f, 0.31f, 0.31f); // Set color to red

    // Draw a circle at the center of the window with a radius of 0.2 units and 100 segments


    drawCircle(0.0f, 0.0f, 0.255f, 100);
    drawCircle(0.0f, 0.0f, 0.28f, 100);
    drawfCircle(0.0f, 0.0f, 0.03f, 100);
    drawfCircle(0.0f, 0.0f, 0.12f, 100);
    drawfCircle(0.0f, 0.0f, 0.18f, 100);
    glPopMatrix();

    glPushMatrix();

    glTranslatef(1.51f, 0.3f, 0.0f);
    glScalef(0.25f, 0.25f, 0.25f);
    //GLubyte colorpickcolor[3]={255,0,0};
#include <GL/glu.h> // Include the necessary OpenGL/GLU header


    GLfloat colorss[30][3] = {
        {0.0f, 0.0f, 0.0f},
        {0.498f, 0.498f, 0.498f},
        {1.0f, 1.0f, 1.0f},

        {0.388f, 0.102f, 0.102f},
        {1.0f, 0.0f, 0.0f},
        {0.968f, 0.388f, 0.388f},

        {0.588f, 0.012f, 0.263f},
        {0.961f, 0.192f, 0.525f},
        {0.961f, 0.549f, 0.725f},

        {0.290f, 0.208f, 0.137f},
        {0.988f, 0.467f, 0.012f},
        {0.918f, 0.639f, 0.396f},

        {0.4f, 0.345f, 0.063f},
        {1.0f, 1.0f, 0.0f},
        {0.98f, 0.98f, 0.518f},

        {0.082f, 0.322f, 0.082f},
        {0.38f, 0.569f, 0.38f},
        {0.0f, 1.0f, 0.0f},

        {0.047f, 0.047f, 0.678f},
        {0.169f, 0.0f, 1.0f},
        {0.525f, 0.455f, 0.89f},





        {0.376f, 0.075f, 0.529f},
        {0.659f, 0.0f, 0.988f},
        {0.827f, 0.478f, 1.0f},



        {0.439f, 0.329f, 0.8f},
        {0.631f, 0.545f, 0.91f},
        {1.118f, 0.686f, 0.871f},

        {0.0f, 0.816f, 1.0f},
        {0.0f, 1.169f, 1.0f},
        {0.0f, 1.0f, 1.0f},

    };




    for (int i = 0;i < 10;i++) {
        //colorpickcolor[0]-=20;
        glPushMatrix();

        glRotatef(static_cast<float>(i) * 36.0f, 0.0f, 0.0f, 1.0f);

        //glScalef(1.0f, ASPECT_RATIO/5.0F, 1.0f);
        glTranslatef(0.0f, 0.1f, 0.0f);
        int j = i * 3;

        //float red = i / 10.0f;  // Varies from 0.0 to 1.0


        glBegin(GL_POLYGON);
        glColor3f(colorss[j][0], colorss[j][1], colorss[j][2]);

        glVertex2f(0.5f / 7, 1.0f / 7);
        glVertex2f(1.0f / 7, 2.3f / 7);
        glVertex2f(-1.0f / 7, 2.3f / 7);
        glVertex2f(-0.5f / 7, 1.0f / 7);

        glEnd();
        //glColor3f(red, 0.0f, 0.31f);

        //glColor3f(0.0f, 0.0f, 1.0f);
        glBegin(GL_POLYGON);

        glColor3f(colorss[j + 1][0], colorss[j + 1][1], colorss[j + 1][2]);

        //glColor3f(1.0f, 0.0f, 0.31f);
        glVertex2f(1.0f / 7, 2.3f / 7);  // Top-left corner
        glVertex2f(1.0f / 7, 4.3f / 7);
        glVertex2f(-1.0f / 7, 4.3f / 7);
        glVertex2f(-1.0f / 7, 2.3f / 7);  // Top-left corner
        // Top-right corner

        glEnd();

        //glColor3f(1.0f, 1.0f, 0.31f);
        glBegin(GL_POLYGON);

        glColor3f(colorss[j + 2][0], colorss[j + 2][1], colorss[j + 2][2]);



        glVertex2f(1.0f / 7, 4.3f / 7);   // Top-right corner
        glVertex2f(0.0f / 7, 6.6f / 7);
        glVertex2f(-1.0f / 7, 4.3f / 7);
        // Bottom-left corner

        glEnd();
        glFlush();
        glPopMatrix();

    }

    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.51f, 0.3f, 0.0f);

    glColor3f(0.91f, 0.31f, 0.31f); // Set color to red

    // Draw a circle at the center of the window with a radius of 0.2 units and 100 segments
    //drawCircle(0.0f, 0.0f, 0.255f, 100);
    glPopMatrix();







    // Revert to normal mode
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    drawfrCircle(-0.94f, 0.21f, 0.025f, 100);

    ndcX = -0.88f;
    ndcY = 0.21f;



    sizeX = 18.0f / windowWidthl; // Width in NDC
    sizeY = 15.0f / windowHeightl; // Height in NDC

    glColor3f(1.0f, 1.0f, 1.0f);
    // Draw the square
    glBegin(GL_LINE_LOOP);
    glVertex2f(ndcX - sizeX, ndcY + sizeY); // Top-left corner
    glVertex2f(ndcX - sizeX, ndcY - sizeY); // Bottom-left corner
    glVertex2f(ndcX + sizeX, ndcY - sizeY); // Bottom-right corner
    glVertex2f(ndcX + sizeX, ndcY + sizeY); // Top-right corner
    glEnd();










    glutSwapBuffers();
}




int main(int argc, char** argv) {
    glutInit(&argc, argv);
    currColorArray[0] = 0.0f;
    currColorArray[1] = 0.0f;
    currColorArray[2] = 0.0f;





    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("PAINT APP by Vikash");
    boxSizeXA = 33.0f / glutGet(GLUT_WINDOW_WIDTH);
    boxSizeYA = 30.0f / glutGet(GLUT_WINDOW_HEIGHT);
    boxSizeXB = 33.0f / glutGet(GLUT_WINDOW_WIDTH);
    boxSizeYB = 30.0f / glutGet(GLUT_WINDOW_HEIGHT);
    boxSizeXC = 33.0f / glutGet(GLUT_WINDOW_WIDTH);
    boxSizeYC = 30.0f / glutGet(GLUT_WINDOW_HEIGHT);
    boxSizeXD = 33.0f / glutGet(GLUT_WINDOW_WIDTH);
    boxSizeYD = 30.0f / glutGet(GLUT_WINDOW_HEIGHT);
    boxSizeXE = 33.0f / glutGet(GLUT_WINDOW_WIDTH);
    boxSizeYE = 30.0f / glutGet(GLUT_WINDOW_HEIGHT);
    boxSizeXF = 33.0f / glutGet(GLUT_WINDOW_WIDTH);
    boxSizeYF = 30.0f / glutGet(GLUT_WINDOW_HEIGHT);
    glutPositionWindow(100, 50);
    //glutSetCursor(GLUT_CURSOR_DESTROY);

    glutDisplayFunc(display);

    glutMouseFunc(handleMouseClick);
    glutMotionFunc(handleMouseMotion);
    glutPassiveMotionFunc(passiveMouseMotion);
    glutKeyboardFunc(keyboard);

    glClearColor(0.30f, 0.30f, 0.30f, 1.0f);

    glutMainLoop();

    return 0;
}
