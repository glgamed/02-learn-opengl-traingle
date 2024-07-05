#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

const char* vertexShaderSource =
    "#version 330 core\n"
    "layout(location = 0) in vec2 pos;\n"
    "void main() {\n"
    "  gl_Position = vec4(pos, 0.0f, 1.0f);\n"
    "}\n";

const char* fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 color;\n"
    "void main() {\n"
    "  color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n";

void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

GLuint createShader(const char* shaderSource, GLenum shaderType) {
  GLuint shaderId = glCreateShader(shaderType);
  glShaderSource(shaderId, 1, &shaderSource, NULL);
  glCompileShader(shaderId);
  GLint success;
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[1024];
    glGetShaderInfoLog(shaderId, 1024, NULL, infoLog);
    fprintf(stderr, "%s\n", infoLog);
  }

  return shaderId;
}

GLuint createProgram(GLuint vertexShaderId, GLuint fragmentShaderId) {
  GLuint programId = glCreateProgram();

  glAttachShader(programId, vertexShaderId);
  glAttachShader(programId, fragmentShaderId);
  glLinkProgram(programId);
  GLint success;
  glGetProgramiv(programId, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[1024];
    glGetProgramInfoLog(programId, 1024, NULL, infoLog);
    fprintf(stderr, "%s\n", infoLog);
  }

  glValidateProgram(programId);
  glGetProgramiv(programId, GL_VALIDATE_STATUS, &success);
  if (!success) {
    char infoLog[1024];
    glGetProgramInfoLog(programId, 1024, NULL, infoLog);
    fprintf(stderr, "%s\n", infoLog);
  }

  glDeleteShader(vertexShaderId);
  glDeleteShader(fragmentShaderId);

  return programId;
}

int main(int argc, char** argv) {
  const int width = 800;
  const int height = 600;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(width, height, "triangle", NULL, NULL);
  if (window == NULL) {
    fprintf(stderr, "%s\n", "Failed to create GLFW window");
    return 1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "%s\n", "Failed to initialize GLAD");
    return 1;
  }

  GLuint vertexShaderId = createShader(vertexShaderSource, GL_VERTEX_SHADER);
  GLuint fragmentShaderId =
      createShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
  GLuint programId = createProgram(vertexShaderId, fragmentShaderId);

  float vertices[] = {
      -0.5f, -0.5f,  // left
      0.5f,  -0.5f,  // right
      0.0f,  0.5f    // top
  };

  GLuint vbo, vao;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glViewport(0, 0, width, height);
  glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(programId);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);
    glUseProgram(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
  glDeleteProgram(programId);

  glfwTerminate();

  return 0;
}