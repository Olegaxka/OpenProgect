//устал -_-(
// Я В ТУПИКЕ ТЕКСТУРА НЕ ОТРИСОВУЕТСЯ
//ВЖАНО что б glad был с верху заголовочных файлов OpenGl
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "shader.h"
#include "stb_image.h"

#include <filesystem>

//функция обратного вызова, вызываеться при каждом изменении окна
void framebuffer_size_callback(GLFWwindow* window, int width, int height); 


int main()
{
    //инициализируем GLFW 
    //glfwInit() - инициализация библиотеки для доступа к GLFW
    if (!glfwInit()) //
    {
        std::cout << "Error" << std::endl;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //создание объекта windows, glfwCreateWindow - функция создания окна(контекста) OpenGL 
    GLFWwindow* window = glfwCreateWindow(800, 600, " OpenGL ", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //мы сообщаем что хотим вызывать функцию framebuffer_size_callback при каждом изменении размеров окна
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //инициализация GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    //окно просмотра (видимая область окна)
    glViewport(0, 0, 800, 600);

    // Компилирование нашей шейдерной программы
    Shader ourShader("vertex_shader.glsl", "fragment_shader.glsl");  // путь к файлам шейдеров

                             //Указывание вершин (и буферов) и настройка вершинных атрибутов            
    /*
    //координаты вершин треугольника по xyz
    float vertices1[] = {
    -0.8f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // лево
     0.8f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // право
     0.0f,  0.7f, 0.0f,  1.0f, 0.0f, 0.0f   // верх      
    };
    */

    float vertices1[] = {
        // координаты        // цвета            // текстурные координаты
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // верхняя правая
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // нижняя правая
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // нижняя левая
       -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // верхняя левая 
    };

                            
    unsigned int indices[] = {  // помните, что мы начинаем с 0!
    0, 1, 3, // первый треугольник
    1, 2, 3  // второй треугольник
    };

    //Обект вершинного буфера объект вершинного буфера имеет тип GL_ARRAY_BUFFER
    //создаем VAO и VBO , EBO
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
 
    //С этого момента любые вызовы буфера, с параметром GL_ARRAY_BUFFER, 
    //будут использоваться для конфигурирования текущего связанного буфера которым  является буфер VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    //Функция glBufferData() специально предназначена для копирования пользовательских данных в текущий связанный буфер
    /*Аргумент №1 : Тип буфера, в который мы хотим скопировать данные — объект вершинного буфера,
    привязанный в данный момент к целевому типу GL_ARRAY_BUFFER.
     Аргумент №2 : Определяем размер данных(в байтах), которые мы хотим передать в буфер; достаточно применить оператор sizeof к данным вершины.
     Аргумент №3 : Данные, которые мы хотим отправить.
     Аргумент №4 : Определяем то, как мы хотим, чтобы видеокарта управляла переданными ей данными.Это может иметь 3 формы :
                 GL_STREAM_DRAW — данные указываются только один раз и используются графическим процессором не более нескольких раз.
                 GL_STATIC_DRAW — данные указываются только один раз и используются много раз.
                 GL_DYNAMIC_DRAW — данные часто изменяются и используются много раз.*/
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //glVertexAttribPointer()- мы сообщаем OpenGL, как он должен интерпретировать данные вершин (для каждого атрибута вершины)
    //Координатные атрибуты
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Цветовые атрибуты
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //текстура
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Обратите внимание, что данное действие разрешено, вызов glVertexAttribPointer() зарегистрировал VBO как привязанный вершинный буферный объект
    // для вершинного атрибута, так что после этого мы можем спокойно выполнить отвязку
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Вы можете отменить привязку VAO после этого, чтобы другие вызовы VAO случайно не изменили этот VAO (но подобное довольно редко случается).
    // Модификация других VAO требует вызова glBindVertexArray(), поэтому мы обычно не снимаем привязку VAO (или VBO), когда это не требуется напрямую
    glBindVertexArray(0);

    // Раскомментируйте следующую строку для отрисовки полигонов в режиме каркаса
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  
                            //Создание текстуры
    unsigned int texture;
    glGenTextures(1, &texture);

    glActiveTexture(GL_TEXTURE0); // сначала активируем текстурный юнит, прежде чем связывать текстуру
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // Устанавливаем параметры текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
    // Загрузка изображения, создание текстуры и генерирование мипмап-уровней
    int width, height, nrChannels;
    unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //цикл рендеринга,
    while (!glfwWindowShouldClose(window))
    {
        //Закрытие окна при нажатии кнопки escape
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        //задаем цвет и очищаем буфер цвета
        //функция glClear() использует текущие настройки цвета, установленные при помощи функции glClearColor().
        glClearColor(0.5f, 0.2f, 0.5f, 0.7f);
        glClear(GL_COLOR_BUFFER_BIT);       
        
        ourShader.use();
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3); //треугольник
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //квадрат
        // glBindVertexArray(0); // не нужно каждый раз его отвязывать

        // glfw: обмен содержимым front- и back-буферов. Отслеживание событий ввода/вывода (была ли нажата/отпущена кнопка, перемещен курсор мыши и т.п.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Опционально: освобождаем все ресурсы, как только они выполнили свое предназначение
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    //Очистка выделенных ресурсов 
    glfwTerminate(); 
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}