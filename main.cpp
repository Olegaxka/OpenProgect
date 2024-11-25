//устал -_-(

//ВЖАНО что б glad был с верху заголовочных файлов OpenGl
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

//функция обратного вызова, вызываеться при каждом изменении окна
void framebuffer_size_callback(GLFWwindow* window, int width, int height); 

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

const char* fragmentShaderSource_yellow = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\0";


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

                              // Компилирование шейдерной программы
    
    //Создание объекта шейдера
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    /*glShaderSource():
      Аргумент №1 : Объект шейдера для компиляции.
      Аргумент №2 : Количество строк, которые мы передаем в качестве исходного кода(в нашем случае — это одна строка).
      Аргумент №3 : Фактический исходный код вершинного шейдера.
      Аргумент №4 : Этот аргумент мы оставляем равным NULL.*/
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //Проверка на ошибки
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    
    //Сначала мы определяем целое число для обозначения успешного исхода компиляции и контейнер для хранения сообщений об ошибках(если таковые имеются).
    // Затем, с помощью функции glGetShaderiv(), мы проверяем, была ли компиляция успешной.
    // Если компиляция не удалась, то мы должны извлечь сообщение об ошибке, используя функцию glGetShaderInfoLog(), и вывести его на экран :
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //компиляции фрагментного шейдера
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Проверка на наличие ошибок компилирования фрагментного шейдера
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader_yellow;
    fragmentShader_yellow = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader_yellow, 1, &fragmentShaderSource_yellow, NULL);
    glCompileShader(fragmentShader_yellow);

    // Проверка на наличие ошибок компилирования фрагментного шейдера
    glGetShaderiv(fragmentShader_yellow, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader_yellow, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //Функция glCreateProgram() создает программу и возвращает идентификатор ссылки на вновь созданный объект шейдерной программы
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    //прикрепляем шейдеры к шейдерской программе и связываем их с помощью  glLinkProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);  
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    //glUseProgram(shaderProgram);

    unsigned int shaderProgram_yellow;
    shaderProgram_yellow = glCreateProgram();

    glAttachShader(shaderProgram_yellow, vertexShader);
    glAttachShader(shaderProgram_yellow, fragmentShader_yellow);
    glLinkProgram(shaderProgram_yellow);

    glGetProgramiv(shaderProgram_yellow, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram_yellow, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    //удаление шейдерских объектов после связывания 
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShader_yellow);

                             //Указывание вершин (и буферов) и настройка вершинных атрибутов            

    //координаты вершин треугольника по xyz
    float vertices1[] = {
     0.1f,  0.5f, 0.0f,  // верхняя правая 0
     0.1f, -0.5f, 0.0f,  // нижняя правая  1
     0.5f,  0.0f, 0.0f,  // правая         2 
    };

    //координаты вершин треугольника по xyz
    float vertices2[] = { 
    -0.1f, -0.5f, 0.0f,  // нижняя левая   3
    -0.1f,  0.5f, 0.0f,  // верхняя левая  4
    -0.5f,  0.0f, 0.0f   //левая           5 
    };

    //unsigned int indices[] = {  // помните, что мы начинаем с 0!
    //0, 1, 2, // первый треугольник
    //3, 4, 5  // второй треугольник
    //};

    //Обект вершинного буфера объект вершинного буфера имеет тип GL_ARRAY_BUFFER
    //создаем VAO и VBO , EBO
    unsigned int VBO, VBO1, VAO, VAO1, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //glVertexAttribPointer()- мы сообщаем OpenGL, как он должен интерпретировать данные вершин (для каждого атрибута вершины)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Обратите внимание, что данное действие разрешено, вызов glVertexAttribPointer() зарегистрировал VBO как привязанный вершинный буферный объект
    // для вершинного атрибута, так что после этого мы можем спокойно выполнить отвязку
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Вы можете отменить привязку VAO после этого, чтобы другие вызовы VAO случайно не изменили этот VAO (но подобное довольно редко случается).
    // Модификация других VAO требует вызова glBindVertexArray(), поэтому мы обычно не снимаем привязку VAO (или VBO), когда это не требуется напрямую
    glBindVertexArray(0);

    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // Раскомментируйте следующую строку для отрисовки полигонов в режиме каркаса
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  
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

        // Рисуем наш первый треугольник
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // поскольку у нас есть только один VAO, то нет необходимости связывать его каждый раз (но мы сделаем это, чтобы всё было немного организованнее)
        glDrawArrays(GL_TRIANGLES, 0, 3); //треугольник
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //квадрат
        // glBindVertexArray(0); // не нужно каждый раз его отвязывать

        glUseProgram(shaderProgram_yellow);
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: обмен содержимым front- и back-буферов. Отслеживание событий ввода/вывода (была ли нажата/отпущена кнопка, перемещен курсор мыши и т.п.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Опционально: освобождаем все ресурсы, как только они выполнили свое предназначение
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    //Очистка выделенных ресурсов 
    glfwTerminate(); 
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}