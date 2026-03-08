#include "settings.h"   // Подключаем настройки
#include "functions.h"  // Подключаем функции

int main(void) {  //Точка входа(не принимает аргументов здесь)
    setlocale(LC_ALL, "RU");  //добавляем русскую локализацию
    GLFWwindow* window;  //создаем объект класса GLFWwindow указателем на него 
    if (!glfwInit()) return -1; //проверка на инициализацию OpenGL
    window = glfwCreateWindow(win_width, win_height, "Redactor 3D", NULL, NULL); //создаем окно OpenGL
    if (!window){ glfwTerminate(); return -1; } //проверка на создание окна

    glfwMakeContextCurrent(window);  //делаем окно текущем и активным
    glLoadIdentity(); //загружаем базовые координаты (обязательная вещь)
    glFrustum(-1, 1, -1, 1, 2, 80); //указываем, что наша проекция - перспективная (для 3D)
    glEnable(GL_DEPTH_TEST); //разрешаем буфер глубины
    glfwSetWindowPos(window, 300, 31); //ставим окно в позицию 300 на 0 на экране
    glLineWidth(5.f); //ширина линий теперь - 5 пикселей
    SetCursorPos(650, 400); //ставим курсор мыши примерно в середину окна
    getHelp(); //показываем помощь
    while (!glfwWindowShouldClose(window)){ //пока окно не закрылось
        glClearColor(0, 0, 0, 0); //цвет очистки области рисования
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //чистим область рисования и буфер глубины

        glPushMatrix(); //сохраняем текущую матрицу изображений
            getCursorState(window); //получаем позицию курсора через нашу функцию
            moveCam(); //движение камеры через нашу функцию
            drawWorld(); //Отрисовка местности через нашу функцию
            setFigures(); //Отрисовка фигур через нашу функцию
            if (GetKeyState('E') < 0) getSelectedCell(); //Если нажата клавиша Е, то получаем id клетки через нашу функцию
            if (GetKeyState('F') < 0) getSelectedObj(); //Если нажата клавиша F, то получаем id объекта через нашу функцию
            if (GetKeyState('C') < 0) createFigure(); //Если нажата клавиша C, то создаём объект через нашу функцию
            if (GetKeyState('G') < 0) deleteFigure(); //Если нажата клавиша G, то удаляем объект через нашу функцию
            if (GetKeyState('H') < 0) getHelp(); //Если нажата клавиша H, то получаем помощь через нашу функцию
            moveFigure(); //Двигаем фигуры через нашу фунцию
        glPopMatrix(); //очищаем текущую матрицу изображений

        Sleep(1); //задержка, потому что слишком всё быстро
        glfwSwapBuffers(window); //меняем буферы(задний и передний) местами
        glfwPollEvents(); //принимем события
    }
    glfwTerminate(); //закрываем окно OpenGL
    return 0; //возврат нуля - успешное завершение программы
}