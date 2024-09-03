#include "../glad.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <cmath>
#include <string>
#include <set>
#include <iostream>
using namespace std;
int option = -1;
vector<float>vertices;
struct Vec2
{
  float x, y;
};
// Converting the raw window coordinates to opengl coordinates
Vec2 windowToNDC(int windowX, int windowY, int windowWidth, int windowHeight)
{
  // Convert from window coordinates (pixels) to normalized device coordinates (NDC)
  Vec2 ndc;
  ndc.x = (2.0f * windowX) / windowWidth - 1.0f;
  ndc.y = 1.0f - (2.0f * windowY) / windowHeight; // Note the reversal of Y here
  return ndc;
}
// Function to plot a point (replace this with actual plotting in your system)
void plotPoint(int x, int y, int height, int width)
{
  Vec2 points = windowToNDC(x, y, height, width);
  vertices.push_back(points.x);
  vertices.push_back(points.y);
  vertices.push_back(0.0f);
}

// Midpoint circle drawing algorithm
void drawCircle(int centerX, int centerY, int radius, int height, int width)
{
  int x = 0;
  int y = radius;
  int d = 1 - radius; // Initial decision parameter

  // Plot the initial points
  plotPoint(centerX + x, centerY + y, height, width);
  plotPoint(centerX - x, centerY + y, height, width);
  plotPoint(centerX + x, centerY - y, height, width);
  plotPoint(centerX - x, centerY - y, height, width);
  plotPoint(centerX + y, centerY + x, height, width);
  plotPoint(centerX - y, centerY + x, height, width);
  plotPoint(centerX + y, centerY - x, height, width);
  plotPoint(centerX - y, centerY - x, height, width);

  while (x < y)
  {
    x++;

    // Update decision parameter
    if (d < 0)
    {
      d += 2 * x + 1;
    }
    else
    {
      y--;
      d += 2 * (x - y) + 1;
    }

    // Plot the points in all octants
    plotPoint(centerX + x, centerY + y, height, width);
    plotPoint(centerX - x, centerY + y, height, width);
    plotPoint(centerX + x, centerY - y, height, width);
    plotPoint(centerX - x, centerY - y, height, width);
    plotPoint(centerX + y, centerY + x, height, width);
    plotPoint(centerX - y, centerY + x, height, width);
    plotPoint(centerX + y, centerY - x, height, width);
    plotPoint(centerX - y, centerY - x, height, width);
  }
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
  {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // Get the window size
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    if (option == -1)
    {
      return;
    }
    else if (option == 1)
    {
      int x_center, y_center, radius;
      x_center = xpos;
      y_center = ypos;
      cout << "Enter the radius you want to draw" << endl;
      cin >> radius;
      drawCircle(x_center, y_center, radius, height, width);
    }
    else if (option == 2)
    {
    }
  }
}

int main()
{
  // Initialize GLFW
  if (!glfwInit())
  {
    cout << "Failed to initialize GLFW" << endl;
    return -1;
  }
  cout << "Enter the integer to select line drawing algorithm" << endl;
  cout << "1)Circle Drawing" << endl;
  cout << "2)Elliptical Drawing" << endl;

  cin >> option;
  // Create a GLFWwindow object
  GLFWwindow *window = glfwCreateWindow(800, 800, "Dynamic Vertex Generation", NULL, NULL);
  if (window == NULL)
  {
    cout << "Failed to create GLFW window" << endl;
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
    // if (flag == false)
    // {
    //     break;
    // }
    // Bind the VBO and update it with the current vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    // Define the layout of the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Draw the vertices
    glBindVertexArray(VAO);
    cout << vertices.size() << endl;
    glDrawArrays(GL_POINTS, 0, vertices.size() / 3);

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
