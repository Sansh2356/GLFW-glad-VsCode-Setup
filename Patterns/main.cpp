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
int pattern = -1;
int thickness = -1;
bool flag = true;
vector<float> vertices; // Store vertices dynamically
vector<float> startingPoints;
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
// Simple DDA
void ddaLine(float x1, float y1, float x2, float y2, int height, int width)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    float lle = max(abs(dx), abs(dy));
    float xIncrement = dx / lle;
    float yIncrement = dy / lle;

    float x = x1;
    float y = y1;

    cout << "Line length estimate is:=" << lle << endl;
    for (int i = 0; i <= lle; i++)
    {
        Vec2 point = windowToNDC(x, y, height, width);

        vertices.push_back(point.x);
        vertices.push_back(point.y);
        vertices.push_back(0.0f);
        x += xIncrement;
        y += yIncrement;
    }
    for (int x = 0; x < vertices.size(); x++)
    {
        cout << vertices[x] << " ";
    }
    cout << endl;
    return;
}

void dottedLine(float x1, float y1, float x2, float y2, int height, int width)
{
  float dx = x2 - x1;
  float dy = y2 - y1;
  float lle = max(abs(dx), abs(dy));
  float xIncrement = dx / lle;
  float yIncrement = dy / lle;

  float x = x1;
  float y = y1;
  int dot = 0;

  for (int i = 0; i <= lle; i++)
  {
    if (dot % 2 == 0)
    {
      Vec2 points = windowToNDC(x, y, width, height);
      vertices.push_back(points.x);
      vertices.push_back(points.y);
      vertices.push_back(0.0f);
    }
    x += xIncrement;
    y += yIncrement;
    dot++;
  }
  return;
}
string hextodec(int pattern)
{
  string s = "";
  while (pattern >= 1)
  {
    int rem = pattern % 2;
    pattern /= 2;
    s.push_back(to_string(rem)[0]);
  }
  reverse(s.begin(), s.end());
  return s;
}
void drawHexPattern(float x1, float y1, float x2, float y2, int height, int width, int pattern)
{

  float dx = x2 - x1;
  float dy = y2 - y1;
  float lle = max(abs(dx), abs(dy));
  float xIncrement = dx / lle;
  float yIncrement = dy / lle;

  float x = x1;
  float y = y1;

  for (int i = 0; i <= lle; i++)
  {
    Vec2 points = windowToNDC(x, y, width, height);
    vertices.push_back(points.x);
    vertices.push_back(points.y);
    vertices.push_back(0.0f);
    x += xIncrement;
    y += yIncrement;
  }
  vector<vector<float>> v;
  for (int j = 0; j < vertices.size() && (j + 3) < vertices.size(); j += 3)
  {
    float v1 = vertices[j];
    float v2 = vertices[j + 1];
    float v3 = vertices[j + 2];
    v.push_back({v1, v2, v3});
  }
  string dec = hextodec(pattern);
  set<float> RemoveIndices;
  for (int i = 0; i < dec.length(); i++)
  {
    if (dec[i] == '0')
    {
      for (int j = i; j < v.size() && (j + 4 < v.size()); j += 4)
      {
        RemoveIndices.insert(j);
      }
    }
  }
  vector<float> finalPoints;
  for (int i = 0; i < v.size(); i++)
  {
    if (RemoveIndices.find(i) != RemoveIndices.end())
    {
      continue;
    }
    else
    {
      finalPoints.push_back(v[i][0]);
      finalPoints.push_back(v[i][1]);
      finalPoints.push_back(v[i][2]);
    }
  }
  vertices = finalPoints;
  float start_x = vertices[0];
  float start_y = vertices[1];

  float start_z = vertices[2];

  float end_x = vertices[0];

  float end_y = vertices[1];

  float end_z = vertices[2];
  // Making sure to add the end points //
  if (start_x != x1 || start_y != y1)
  {
    vertices.push_back(windowToNDC(x1, y1, width, height).x);
    vertices.push_back(windowToNDC(x1, y1, width, height).y);
    vertices.push_back(0.0f);
  }
  if (end_x != x2 || end_y != y2)
  {
    vertices.push_back(windowToNDC(x2, y2, width, height).x);
    vertices.push_back(windowToNDC(x2, y2, width, height).y);
    vertices.push_back(0.0f);
  }

  for (int i = 0; i < vertices.size(); i++)
  {
    cout << vertices[i] << " ";
  }
  cout << endl;
  return;
}
void Bold(float x1, float y1, float x2, float y2, int height, int width)
{
  ddaLine(x1, y1, x2, y2, height, width);
  for (int x = 1; x <= thickness; x++)
  {
    ddaLine(x1, y1 + float(0.5f * x), x2, y2 + float(0.5 * x), height, width);
    ddaLine(x1, y1 - float(0.5f * x), x2, y2 - float(0.5 * x), height, width);
  }
  return;
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
      startingPoints.push_back(xpos);
      startingPoints.push_back(ypos);
      if (startingPoints.size() == 4)
      {
        dottedLine(startingPoints[0], startingPoints[1], startingPoints[2], startingPoints[3], height, width);
        startingPoints.clear();
      }
    }
    else if (option == 2)
    {
      startingPoints.push_back(xpos);
      startingPoints.push_back(ypos);
      if (startingPoints.size() == 4)
      {
        drawHexPattern(startingPoints[0], startingPoints[1], startingPoints[2], startingPoints[3], height, width, pattern);
        startingPoints.clear();
      }
    }
    else if (option == 3)
    {
      startingPoints.push_back(xpos);
      startingPoints.push_back(ypos);
      if (startingPoints.size() == 4)
      {
        Bold(startingPoints[0], startingPoints[1], startingPoints[2], startingPoints[3], height, width);
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
    cout << "Failed to initialize GLFW" << endl;
    return -1;
  }
  cout << "Enter the integer to select line drawing algorithm" << endl;
  cout << "1)Dotted Lines" << endl;
  cout << "2)Drawing Hexagonal pattern" << endl;
  cout << "3)Bold line" << endl;
  cin >> option;
  if (pattern == -1 && option == 2)
  {
    cout << "Enter a pattern to make" << endl;
    cin >> pattern;
  }
  if (option == 3)
  {
    cout << "Assign value of thickness of line =:" << endl;
    cin >> thickness;
  }

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
