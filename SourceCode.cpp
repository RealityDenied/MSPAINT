
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
std::vector<std::pair<float, float>> paintedPoints;
std::vector<std::pair<float, float>> rectPoints;
std::vector<std::pair<float, float>> circleVector;


bool isCircling, isDrawingRect, isLining, isDrawing, pencilTool,
eraserTool, isErasing, lineTool, paintTool, rectTool, circleTool = false;



bool floodfillRequested = true;

GLfloat currColorArray[3];
//float fillColor[3];
GLubyte targetColorr[3];
const float PI = 3.14159265358979323846;
float centreofX;
float centreofY;

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

            glFlush();

            pointsToProcess.push(std::make_pair(x + 1, y));
            pointsToProcess.push(std::make_pair(x, y + 1));
            pointsToProcess.push(std::make_pair(x - 1, y));
            pointsToProcess.push(std::make_pair(x, y - 1));
        }
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

void setTool(bool eraser, bool pencil, bool line, bool paint, bool rect) {
    eraserTool = eraser;
    pencilTool = pencil;
    lineTool = line;
    paintTool = paint;
    rectTool = rect;
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'E':
    case 'e':
        setTool(true, false, false, false, false);
        break;

    case 'P':
    case 'p':
        setTool(false, true, false, false, false);
        break;

    case 'L':
    case 'l':
        setTool(false, false, true, false, false);
        break;

    case 'F':
    case 'f':
        setTool(false, false, false, true, false);
        glutPostRedisplay();
        break;

    case 'G':
    case 'g':
        currColorArray[0] = 0.0f;
        currColorArray[1] = 1.0f;
        currColorArray[2] = 0.0f;
        break;

    case 'B':
    case 'b':
        currColorArray[0] = 0.0f;
        currColorArray[1] = 0.0f;
        currColorArray[2] = 0.0f;
        break;

    case 'R':
    case 'r':
        currColorArray[0] = 1.0f;
        currColorArray[1] = 0.0f;
        currColorArray[2] = 0.0f;
        break;

    case 'Y':
    case 'y':
        currColorArray[0] = 1.0f;
        currColorArray[1] = 1.0f;
        currColorArray[2] = 0.0f;
        break;

    case 'Q':
    case 'q':
        setTool(false, false, false, false, true);
        break;

    case 'Z':
    case 'z':
        if (!functionsToCall.empty()) {
            functionsToCall.pop_back();
        }
        glutPostRedisplay();
        break;
    }
}




void handleMouseClick(int button, int state, int x, int y) {
    float posX = (2.0f * static_cast<float>(x) / glutGet(GLUT_WINDOW_WIDTH)) - 1.0f;
    float posY = 1.0f - (2.0f * static_cast<float>(y) / glutGet(GLUT_WINDOW_HEIGHT));

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {

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

            if (rectTool) {
                rectPoints.push_back(std::make_pair(posX, posY));
                rectPoints.push_back(std::make_pair(posX, posY));
                rectPoints.push_back(std::make_pair(posX, posY));
                rectPoints.push_back(std::make_pair(posX, posY));
                rectPoints.push_back(std::make_pair(posX, posY));
                isDrawingRect = true;
            }

            if (paintTool) {
                glReadPixels(x, glutGet(GLUT_WINDOW_HEIGHT) - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, targetColorr);
                floodFillStack(x, y, currColorArray, targetColorr);
                std::vector<std::pair<float, float>> temppaintingPoints = paintedPoints;
                float temppaintColorarr[3];

                for (int i = 0; i < 3; ++i) {
                    temppaintColorarr[i] = currColorArray[i];
                }

                functionsToCall.push_back([temppaintingPoints, temppaintColorarr]() {
                    float colorpaintCopy[3] = { temppaintColorarr[0], temppaintColorarr[1], temppaintColorarr[2] };
                    PaintFillBuffer(temppaintingPoints, colorpaintCopy);
                    });
                paintedPoints.clear();
            }

            if (circleTool) {
                centreofX = posX;
                centreofY = posY;
                isCircling = true;
            }

        }
        else if (state == GLUT_UP) {
            if (pencilTool) {
                isDrawing = false;
                std::vector<std::pair<float, float>> tempPoints = points;
                float tempColorarr[3];

                for (int i = 0; i < 3; ++i) {
                    tempColorarr[i] = currColorArray[i];
                }

                functionsToCall.push_back([tempPoints, tempColorarr]() {
                    float colorCopy[3] = { tempColorarr[0], tempColorarr[1], tempColorarr[2] };
                    drawline(tempPoints, colorCopy);
                    });
                points.clear();
            }

            if (eraserTool) {
                std::vector<std::pair<float, float>> temperasePoints = eraserCurves;
                functionsToCall.push_back([temperasePoints]() {
                    EraserTool(temperasePoints);
                    });
                eraserCurves.clear();
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

                float tempColorarr12[3];
                std::vector<std::pair<float, float>> temprectanglPoints = rectPoints;

                for (int i = 0; i < 3; ++i) {
                    tempColorarr12[i] = currColorArray[i];
                }

                functionsToCall.push_back([temprectanglPoints, tempColorarr12]() {
                    float colorCopy12[3] = { tempColorarr12[0], tempColorarr12[1], tempColorarr12[2] };
                    recttoolP(temprectanglPoints, colorCopy12);
                    });
                rectPoints.clear();
            }

            if (lineTool) {
                std::vector<std::pair<float, float>> tempLinePoints = lineCurves;
                float tempColorarr6[3];

                for (int i = 0; i < 3; ++i) {
                    tempColorarr6[i] = currColorArray[i];
                }

                isLining = false;

                functionsToCall.push_back([tempLinePoints, tempColorarr6]() {
                    float colorCopy7[3] = { tempColorarr6[0], tempColorarr6[1], tempColorarr6[2] };
                    linetool(tempLinePoints, colorCopy7);
                    });
                lineCurves = lineCurves1;
            }

            if (circleTool) {
                isCircling = false;
                std::vector<std::pair<float, float>> tempCirclePoints = circleVector;
                float tempColorarr13[3];

                for (int i = 0; i < 3; ++i) {
                    tempColorarr13[i] = currColorArray[i];
                }

                isLining = false;

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
    /*
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
    */
    glutPostRedisplay();

}



void display() {

    glClear(GL_COLOR_BUFFER_BIT);

    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);

    glLineWidth(1.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    GLubyte fillColor[] = { 0, 100, 100 };
    GLubyte targetColor[] = { 255,255,255 };
    for (const auto& func : functionsToCall) {
        func();
    }
    drawlinerealtime();
    lineToolrealtime();
    rectangletoolrealtime();
    drawCircleRealTime();
    EraserToolrealtime();
    glColor3f(0.2f, 0.49f, 0.49f);
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

    glutPositionWindow(100, 50);


    glutDisplayFunc(display);

    glutMouseFunc(handleMouseClick);
    glutMotionFunc(handleMouseMotion);
    glutKeyboardFunc(keyboard);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glutMainLoop();

    return 0;
}
