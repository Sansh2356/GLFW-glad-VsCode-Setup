#include <iostream>
#include <vector>
#include<cmath>
#include<math.h>
#include "filename.h"
#include "glad.h"
#include <GLFW/glfw3.h>
using namespace std;

int option = -1;
vector<float> vertices; // Store vertices dynamically
vector<float>startingPoints;
// Simple DDA
void ddaLine(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float lle = max(abs(dx), abs(dy));
    float xIncrement = dx / lle;
    float yIncrement = dy / lle;

    float x = x1;
    float y = y1;

    for (int i = 0; i <= lle; i++) {
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);
        x += xIncrement;
        y += yIncrement;
    }
    vertices.push_back(x2);
    vertices.push_back(y2);
    vertices.push_back(0.0f);
    return;
}

// Symmetric DDA Line Drawing Algorithm
void symmetricDDA(float x1, float y1, float x2, float y2) {
    vector<float> points;
    float dx = x2 - x1;
    float dy = y2 - y1;

    int maxi = max(abs(dx), abs(dy));
    int n = 0;

    while (maxi >= pow(2, n)) {
        n = n + 1;
    }

    float eps = 1 / pow(2, n);
    int steps = pow(2, n);
    float x_inc = eps * dx;
    float y_inc = eps * dy;

    for (int i = 0; i <= steps; i++) {
        vertices.push_back(x1);
        vertices.push_back(y1);
        vertices.push_back(0.0f);
        x1 = x1 + x_inc;
        y1 = y1 + y_inc;
    }
    vertices.push_back(x2);
    vertices.push_back(y2);
    vertices.push_back(0.0f);
    return;
}

// Bresenham Line Drawing Algorithm
void  bresenham(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;

    if (x1 > x2) {
        swap(x1, x2);
        swap(y1, y2);
        dx = x2 - x1;
        dy = y2 - y1;
    }

    float x = x1;
    float y = y1;
    float slope = abs(dy) / abs(dx);

    if (slope > 1) {
        swap(dx, dy);
        swap(x1, y1);
        swap(x2, y2);
        dx = x2 - x1;
        dy = y2 - y1;
    }

    float p = 2 * dy - dx;

    while (x <= x2) {
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);
        x += 1;

        if (p >= 0) {
            y += 1;
            p += 2 * dy - 2 * dx;
        }
        else {
            p += 2 * dy;
        }
    }
    vertices.push_back(x2);
    vertices.push_back(y2);
    vertices.push_back(0.0f);
    return;
}

// Midpoint Line Drawing Algorithm
void midpoint(float x1, float y1, float x2, float y2) {

    float dx = x2 - x1;
    float dy = y2 - y1;

    if (x1 > x2) {
        swap(x1, x2);
        swap(y1, y2);
        dx = x2 - x1;
        dy = y2 - y1;
    }

    float x = x1;
    float y = y1;
    float slope = abs(dy) / abs(dx);

    if (slope < 1) {
        float p = 2 * dy - dx;
        while (x <= x2) {
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(0.0f);
            x += 1;

            if (p >= 0) {
                y += 1;
                p += 2 * dy - 2 * dx;
            }
            else {
                p += 2 * dy;
            }
        }
    }
    else {
        float p = 2 * dx - dy;
        while (y <= y2) {
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(0.0f);
            y += 1;

            if (p >= 0) {
                x += 1;
                p += 2 * dx - 2 * dy;
            }
            else {
                p += 2 * dx;
            }
        }
    }
    vertices.push_back(x2);
    vertices.push_back(y2);
    vertices.push_back(0.0f);
    return;
}
void dottedLine(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float lle = max(abs(dx), abs(dy));
    float xIncrement = dx / lle;
    float yIncrement = dy / lle;

    float x = x1;
    float y = y1;
    int dot = 0;

    for (int i = 0; i <= lle; i++) {
        if (dot % 2 == 0) {
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(0.0f);
        }
        x += xIncrement;
        y += yIncrement;
        dot++;
    }
    vertices.push_back(x2);
    vertices.push_back(y2);
    vertices.push_back(0.0f);
    return;
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Get the window size
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        // Convert window coordinates to OpenGL coordinates
        float xOpenGL = (float)(xpos / width) * 2.0f - 1.0f;
        float yOpenGL = 1.0f - (float)(ypos / height) * 2.0f;
        if (option == -1) {
            return;
        }
        else if (option == 1) {
            cout << xOpenGL << " " << yOpenGL << endl;
            cout << vertices.size() << endl;
            cout << startingPoints.size() << endl;
            startingPoints.push_back(xOpenGL);
            startingPoints.push_back(yOpenGL);

            if (startingPoints.size() == 4) {
                ddaLine(startingPoints[0], startingPoints[1], startingPoints[2], startingPoints[3]);
                startingPoints.clear();
            }
        }
        else if (option == 2) {
            startingPoints.push_back(xOpenGL);
            startingPoints.push_back(yOpenGL);
            if (startingPoints.size() == 4) {
                symmetricDDA(startingPoints[0], startingPoints[1], startingPoints[2], startingPoints[3]);
                startingPoints.clear();
            }
        }
        else if (option == 3) {
            startingPoints.push_back(xOpenGL);
            startingPoints.push_back(yOpenGL);
            if (startingPoints.size() == 4) {
                bresenham(startingPoints[0], startingPoints[1], startingPoints[2], startingPoints[3]);
                startingPoints.clear();
            }
        }
        else if (option == 4) {
            startingPoints.push_back(xOpenGL);
            startingPoints.push_back(yOpenGL);
            if (startingPoints.size() == 4) {
                midpoint(startingPoints[0], startingPoints[1], startingPoints[2], startingPoints[3]);
                startingPoints.clear();
            }
        }
        else if (option == 5) { 
            startingPoints.push_back(xOpenGL);
            startingPoints.push_back(yOpenGL);
            if (startingPoints.size() == 4) {
                dottedLine(startingPoints[0], startingPoints[1], startingPoints[2], startingPoints[3]);
                startingPoints.clear();
            }
        }

    }
}

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
       cout << "Failed to initialize GLFW" <<endl;
        return -1;
    }
    cout << "Enter the integer to select line drawing algorithm" << endl;
    cout << "1)DDA" << endl;
    cout << "2)Symmetric DDA" << endl;
    cout << "3)Bresenhem" << endl;
    cout << "4)Midpoint" << endl;
    cin >> option;
    // Create a GLFWwindow object
    GLFWwindow* window = glfwCreateWindow(800, 800, "Dynamic Vertex Generation", NULL, NULL);
    if (window == NULL)
    {
       cout << "Failed to create GLFW window" <<endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load GLAD
    if (!gladLoadGL())
    {
       cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // Set the mouse button callback
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // Create and bind VAO and VBO
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Bind the VBO and update it with the current vertices
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

        // Define the layout of the vertex data
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Draw the vertices
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, vertices.size() / 3);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}