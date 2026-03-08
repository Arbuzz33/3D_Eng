#include <windows.h>    //стандартный модуль для работы с окнами
#include <GLFW/glfw3.h> //подключаем OpenGL

void getHelp() { //процедура вызывает помощь (здесь всё понятно)
    system("cls"); //очищаем консоль
    std::cout << "Это памятка, что здесь можно делать." << std::endl;
    std::cout << "Несколько замечаний перед этим: " << std::endl;
    std::cout << "1. Не стоит расширять окно, это вам не поможет. " << std::endl;
    std::cout << "2. Если выходит курсор из окна и камера непонятно двигается - это нормально. " << std::endl;
    std::cout << "3. Движение фигур за пространство пола - это нормально. " << std::endl;
    std::cout << "4. Фигура встаёт на половину клетки - это нормально. " << std::endl;
    std::cout << "5. Если строк станет слишком много - это нормально. " << std::endl;
    std::cout << "6. Оси координат: x+  синяя, x-  голубая, y+  оранжевая, y-  красная." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Итак, что можно делать: " << std::endl;
    std::cout << "Помощь - нажать H" << std::endl;
    std::cout << "Ходить - AWSD" << std::endl;
    std::cout << "Летать вверх - пробел, летать вниз - шифт" << std::endl;
    std::cout << "Поворот камеры - мышь" << std::endl;
    std::cout << "Узнать ID клетки - навестись на неё и нажать E" << std::endl;
    std::cout << "Выбрать фигуру - навестись на неё и нажать F" << std::endl;
    std::cout << "Поставить фигуру на определённую клетку - выбрать фигуру, узнать ID клетки, нажать I и вписать ID клетки, ENTER" << std::endl;
    std::cout << "Двинать фигуру - стрелочки(x, y) и R (+z) или T (-z)" << std::endl;
    std::cout << "Создать фигуру - нажать C и вписать её тип (cube-куб, pyramid-пирамида)" << std::endl;
    std::cout << "Удалить фигуру - навестись на неё и нажать G" << std::endl;
}

void drawWorld() { //процедура рисует местность
    glEnableClientState(GL_VERTEX_ARRAY); //разрешаем рисование массива вершин
        glVertexPointer(3, GL_FLOAT, 0, &vert); //закидываем массив вершин в стек (для пола)
        for (int i = -4; i < 5; i++) { //цикл переберает от -4 до 5
            for (int j = -4; j < 5; j++) { //цикл переберает от -4 до 5
                floor_color_val += 0.005f; //прибавка для разности цветов между клетками
                glPushMatrix(); //сохраняем текущую матрицу изображений
                (i + j) % 2 == 0 ? glColor3f(0, 0.5f-floor_color_val, 0) : glColor3f(1- floor_color_val, 1- floor_color_val, 1- floor_color_val); //определяем цвет клетки (белый или зелённый)
                glTranslatef(i*2, j*2, 0); //выставляем позицию клетки
                glDrawArrays(GL_QUADS, 0, 4); //рисуем нашу клетку
                glPopMatrix(); //очищаем текущую матрицу изображений
            }
        }
        floor_color_val = 0.f; //обнуляем значение

        glVertexPointer(3, GL_FLOAT, 0, &vert1); //закидываем массив вершин в стек (для потолка)
        for (int i = -4; i < 5; i++) { //цикл переберает от -4 до 5
            for (int j = -4; j < 5; j++) { //цикл переберает от -4 до 5
                glPushMatrix(); //сохраняем текущую матрицу изображений
                (i + j) % 2 == 0 ? glColor3f(0, 0.5f, 0) : glColor3f(1, 1, 1); //определяем цвет клетки (белый или зелённый)
                glTranslatef(i * 2, j * 2, 0); //выставляем позицию клетки
                glDrawArrays(GL_QUADS, 0, 4); //рисуем нашу клетку
                glPopMatrix(); //очищаем текущую матрицу изображений
            }
        }
        glVertexPointer(3, GL_FLOAT, 0, &line_of_coords_y_p); glColor3f(1, 0.2f, 0); glDrawArrays(GL_LINES, 0, 2); //рисуем ось ординат+
        glVertexPointer(3, GL_FLOAT, 0, &line_of_coords_x_p); glColor3f(0, 0, 1); glDrawArrays(GL_LINES, 0, 2); //рисуем ось абцисс+
        glVertexPointer(3, GL_FLOAT, 0, &line_of_coords_y_m); glColor3f(1, 0, 0); glDrawArrays(GL_LINES, 0, 2); //рисуем ось ординат-
        glVertexPointer(3, GL_FLOAT, 0, &line_of_coords_x_m); glColor3f(0, 0.4f, 1); glDrawArrays(GL_LINES, 0, 2); //рисуем ось абцисс-
    glDisableClientState(GL_VERTEX_ARRAY); //запрещаем рисование массива вершин
}

void setFigures() { //процедура рисует фигуры
    glEnableClientState(GL_VERTEX_ARRAY); //разрешаем рисование массива вершин
    if (max_pyramid_count > 0) { //если хоты бы 1 пирамида на поле
        for (int i = 1; i <= max_pyramid_count; i++) { //цикл рисует все существующие на поле пирамиды
            pyr_color_val += 0.01f; //меняем смещение цвета пирамиды
            if (i != deletedP[i]) { //если фигура не удалена
                    glPushMatrix(); //сохраняем текущую матрицу изображений
                    glVertexPointer(3, GL_FLOAT, 0, &pyramid); //закидываем массив вершин в стек (для пирамиды)
                    glColor3f(1 - pyr_color_val, 0, 0); //выбираем цвет (красный)
                    glTranslatef(objectsWithCoordsP[i][0], objectsWithCoordsP[i][1], objectsWithCoordsP[i][2]); //задаем позицию объекта
                    glDrawArrays(GL_TRIANGLE_FAN, 0, 12); //рисуем нашу пирамиду
                    glPopMatrix(); //очищаем текущую матрицу изображений
            }
        }
        pyr_color_val = 0.f; //обнуляем смещение цвета пирамиды
    }
    if (max_cube_count > 0) { //если хоты бы 1 куб на поле
        for (int i = 1; i <= max_cube_count; i++) { //цикл рисует все существующие на поле пирамиды
            cub_color_val += 0.01f; //меняем смещение цвета куба
            if (i != deletedC[i]) { //если фигура не удалена
                    glPushMatrix(); //сохраняем текущую матрицу изображений
                    glVertexPointer(3, GL_FLOAT, 0, &cube); //закидываем массив вершин в стек (для куба)
                    glColor3f(0, 0, 1 - cub_color_val); //выбираем цвет (синий)
                    glTranslatef(objectsWithCoordsC[i][0], objectsWithCoordsC[i][1], objectsWithCoordsC[i][2]); //задаем позицию объекта
                    glDrawArrays(GL_QUADS, 0, 20); //рисуем наш куб
                    glPopMatrix(); //очищаем текущую матрицу изображений
            }
        }
        cub_color_val = 0.f; //обнуляем смещение цвета куба
    }
    glDisableClientState(GL_VERTEX_ARRAY); //запрещаем рисование массива вершин
}

void getCursorState(GLFWwindow* win) { //процедура получает позицию курсора
    POINT curs_pos; //переменная хранит позицию курсора по x и y
    int win_pos[2]; //переменная хранит позицию окна
    GetCursorPos(&curs_pos); //получаем позицию курсора
    glfwGetWindowPos(win, &win_pos[0], &win_pos[1]); //получаем позицию окна OpenGL
    curs_pos_in_win[0] = (curs_pos.x - win_pos[0]) - 350; //рассчитываем позицию курсора в окне по x
    curs_pos_in_win[1] = (curs_pos.y - win_pos[1]) - 700; //рассчитываем позицию курсора в окне по y
}

void moveCam() { //процедура двигает камеру
    zAlpha = curs_pos_in_win[0] * -0.52f; //расситываем угол z
    xAlpha = curs_pos_in_win[1] * -0.26f; //расситываем угол x

    float player_angle = -zAlpha / 180 * M_PI; //рассчитываем угол игрока по z
    float player_speed = 0.f; //создаём переменную для скорости игрока
    if (GetKeyState('W') < 0) player_speed = 0.2f;  //Если нажали W, двигаем игрока вперёд
    if (GetKeyState('S') < 0) player_speed = -0.2f; //Если нажали S, двигаем игрока назад
    if (GetKeyState('A') < 0) { player_speed = 0.2f; player_angle -= M_PI * 0.5f; } //Если нажали A, двигаем игрока влево
    if (GetKeyState('D') < 0) { player_speed = 0.2f; player_angle += M_PI * 0.5f; } //Если нажали D, двигаем игрока вправо
    if (GetKeyState(VK_SPACE) < 0) player_z += 0.2f; //Если нажали SPACE, двигаем игрока
    if (GetKeyState(VK_SHIFT) < 0) player_z -= 0.2f; //Если нажали SHIFT, двигаем игрока
    if (player_speed != 0) { //если скорость не ноль
        player_x += sin(player_angle) * player_speed; //меняем позицию игрока по x
        player_y += cos(player_angle) * player_speed; //меняем позицию игрока по y
    }
    glRotatef(-xAlpha, 1, 0, 0); //поворачиваем камеру на угол x
    glRotatef(-zAlpha, 0, 0, 1); //поворачиваем камеру на угол z
    glTranslatef(-player_x, -player_y, -player_z); //двигаем игрока по x, y и z
}

void getSelectedCell() {  //процедура формирует id выбранной клетки
    GLfloat clr[3]; //переменная хранит цвет по RGB
    glReadPixels(curs_pos_in_win[0]+350, curs_pos_in_win[1] * -1, 1, 1, GL_RGB, GL_FLOAT, &clr); //чтение цвета пикселя под курсором
    float raw_id = round((floor(clr[1] * 10000) / 10000) * 1000) / 1000; //формируем сырой id на основе цвета
    raw_id >= 0.6f ? SELECT_ID = int(round((1 - raw_id) * 100)) * 2 : SELECT_ID = (int(round((0.5f - (floor(raw_id * 100) / 100)) * 100)) * 2) - 1; //выдаем id каждой клетке, в соответствии с сырым id
    system("cls"); //очищаем консоль
    SELECT_ID > 81 ? std::cout << "Это не клетка!" << std::endl : std::cout << "ID клетки: " << SELECT_ID << std::endl; //Индикация
}

void getSelectedObj() {  //процедура формирует id выбранного объекта
    GLfloat clr[3]; //переменная хранит цвет по RGB
    glReadPixels(curs_pos_in_win[0] + 350, curs_pos_in_win[1] * -1, 1, 1, GL_RGB, GL_FLOAT, &clr); //чтение цвета пикселя под курсором
    if (clr[0] > 0) { //если красный цвет > 0
        float raw_id = round(clr[0] * 100) / 100; //формируем сырой id на основе цвета
        selected_p_id = int(round((1 - raw_id) * 100)); //получаем id выбранной пирамиды, в соответствии с сырым id
        selected_c_id = 0; //обнуляем id куба
        selType = true; //выбранный тип фигуры - пирамида
        system("cls"); //очищаем консоль
        selected_p_id < 11 ? std::cout << "Выбранная фигура: " << "Пирамида" << " " << selected_p_id << std::endl : std::cout << "Это не фигура!" << std::endl; //Индикация
    }
    if (clr[2] > 0) { //если синий цвет > 0
        float raw_id = round(clr[2] * 100) / 100; //формируем сырой id на основе цвета
        selected_c_id = int(round((1 - raw_id) * 100)); //получаем id выбранного куба, в соответствии с сырым id
        selected_p_id = 0; //обнуляем id пирамиды
        selType = false; //выбранный тип фигуры - куб
        system("cls"); //очищаем консоль
        selected_c_id < 11 ? std::cout << "Выбранная фигура: " << "Куб" << " " << selected_c_id << std::endl : std::cout << "Это не фигура!" << std::endl;  //Индикация
    }
}

void moveFigure() {  //процедура двигает фигуру
    if (selType) { //если выбранный тип фигуры - пирамида
        selected_c_id = 0; //обнуляем id куба
        if (GetKeyState(VK_UP) < 0) { Sleep(20); objectsWithCoordsP[selected_p_id][1] += 1; } //двигаем её на 1 по y
        if (GetKeyState(VK_DOWN) < 0) { Sleep(20); objectsWithCoordsP[selected_p_id][1] -= 1; } //двигаем её на -1 по y
        if (GetKeyState(VK_LEFT) < 0) { Sleep(20); objectsWithCoordsP[selected_p_id][0] -= 1; } //двигаем её на -1 по x
        if (GetKeyState(VK_RIGHT) < 0) { Sleep(20); objectsWithCoordsP[selected_p_id][0] += 1; } //двигаем её на 1 по x
        if (GetKeyState('R') < 0) { Sleep(20); objectsWithCoordsP[selected_p_id][2] += 1; } //двигаем её на 1 по z
        if (GetKeyState('T') < 0) { Sleep(20); objectsWithCoordsP[selected_p_id][2] -= 1; } //двигаем её на -1 по z
        if (GetKeyState('I') < 0) { //если нажата кнопка I
            std::cout << "Введите ID клетки: "; //Индикация
            std::cin >> queryFigId; //вводим id клетки с консли в переменную queryFigId
            objectsWithCoordsP[selected_p_id][0] = coordsById[queryFigId][0]; //даем позицию объекта по x, в соответствии с id клетки
            objectsWithCoordsP[selected_p_id][1] = coordsById[queryFigId][1]; //даем позицию объекта по y, в соответствии с id клетки
            std::cout << "ГОТОВО!" << std::endl; //сообщение об успешности процедуры
        }
    }
    else { //иначе: выбранный тип фигуры - куб
        selected_p_id = 0; //обнуляем id пирамиды
        if (GetKeyState(VK_UP) < 0) { Sleep(20); objectsWithCoordsC[selected_c_id][1] += 1; } //двигаем её на 1 по y
        if (GetKeyState(VK_DOWN) < 0) { Sleep(20); objectsWithCoordsC[selected_c_id][1] -= 1; } //двигаем её на -1 по y
        if (GetKeyState(VK_LEFT) < 0) { Sleep(20); objectsWithCoordsC[selected_c_id][0] -= 1; } //двигаем её на -1 по x
        if (GetKeyState(VK_RIGHT) < 0) { Sleep(20); objectsWithCoordsC[selected_c_id][0] += 1; } //двигаем её на 1 по x
        if (GetKeyState('R') < 0) { Sleep(20); objectsWithCoordsC[selected_c_id][2] += 1; } //двигаем её на 1 по z
        if (GetKeyState('T') < 0) { Sleep(20); objectsWithCoordsC[selected_c_id][2] -= 1; } //двигаем её на -1 по z
        if (GetKeyState('I') < 0) { //если нажата кнопка I
            std::cout << "Введите ID клетки: "; //индикация
            std::cin >> queryFigId; //вводим id клетки с консли в переменную queryFigId
            objectsWithCoordsC[selected_c_id][0] = coordsById[queryFigId][0]; //даем позицию объекта по x, в соответствии с id клетки
            objectsWithCoordsC[selected_c_id][1] = coordsById[queryFigId][1]; //даем позицию объекта по y, в соответствии с id клетки
            system("cls"); //очищаем консоль
            std::cout << "ГОТОВО!" << std::endl; //сообщение об успешности процедуры
        }
    }
}

void createFigure() { //процедура для создания новой фигуры
        system("cls"); //очищаем консоль
        std::cout << "Создание новой фигуры" << std::endl; //индикация
        std::cin >> fig_type; //вписываем тип фигуры
        if (fig_type == "cube") { //если тип фигуры - куб
            if (max_cube_count < 10) { //если всего кубов меньше 10
                selected_p_id = 0; //обнулем id выбранной пирамиды
                selected_c_id = 0; //обнулем id выбранного куба
                for (int i = 1; i <= sizeof(deletedC); i++) { //проверяем id удалённых фигур
                    if (deletedC[i] >= 1) { //если фигура с самым маленьким id удаленна
                        deletedC[i] = 0; //то теперь она не удаленна
                        break; //заканчиваем, так как нам нужна только одна фигура
                    }
                    else if (i == 10 && max_cube_count < 10) { //иначе если все имеющиеся фигуры на поле
                        max_cube_count += 1; //то всего кубов стало на 1 больше
                    }
                }
                objectsWithCoordsC[max_cube_count][0] = 0; //новый куб ставится на x = 0
                objectsWithCoordsC[max_cube_count][1] = 0; //y = 0
                objectsWithCoordsC[max_cube_count][2] = 0; //и z = 0
                std::cout << "ГОТОВО!" << std::endl; //сообщение об успешности процедуры
                std::cout << "Созать ещё фигуру - C, Выход в меню - H" << std::endl; //индикация
                Sleep(1); //задержка
            }
            else std::cout << "Слишком много кубов" << std::endl; //иначе: слишком много фигур
        }
        else if (fig_type == "pyramid") { //иначе если тип фигуры - пирамида
            if (max_pyramid_count < 10) { //если всего пирамид меньше 10
                selected_p_id = 0; //обнулем id выбранной пирамиды
                selected_c_id = 0; //обнулем id выбранного куба
                for (int i = 1; i <= sizeof(deletedP); i++) { //проверяем id удалённых фигур
                    if (deletedP[i] >= 1) { //если фигура с самым маленьким id удаленна
                        deletedP[i] = 0; //то теперь она не удаленна
                        break; //заканчиваем, так как нам нужна только одна фигура
                    }
                    else if (i == 10 && max_pyramid_count < 10) { //иначе если все имеющиеся фигуры на поле
                        max_pyramid_count += 1; //всего пирамид стало на 1 больше
                    }
                }
                objectsWithCoordsP[max_pyramid_count][0] = 0; //новая пирамида ставится на x = 0
                objectsWithCoordsP[max_pyramid_count][1] = 0; //y = 0
                objectsWithCoordsP[max_pyramid_count][2] = 0; //и z = 0
                std::cout << "ГОТОВО!" << std::endl; //сообщение об успешности процедуры
                std::cout << "Созать ещё фигуру - C, Выход в меню - H" << std::endl; //индикация
                Sleep(1);
            }
            else std::cout << "Слишком много пирамид" << std::endl; //иначе: слишком много фигур
        }
        else if (fig_type == "stop") { //иначе если ввели stop
            system("cls"); //очищаем консоль
            std::cout << "Операция завершена!" << std::endl; //индикация
            Sleep(3); //задержка
            getHelp(); //вызываем меню
        }
        else std::cout << "Некорректное значение" << std::endl; //иначе: ввод был некорректен
        Sleep(2); //задержка
    fig_type = ""; //обнуляем введённое значение
}

void deleteFigure() { //процедура удаляет фигуру
    getSelectedObj(); //узнаем id удаляемой фигуры через нашу процедуру
    if (selType) { //если это - пирамида
        objectsWithCoordsP[selected_p_id][0] = NULL; //обнуляем её координаты по x
        objectsWithCoordsP[selected_p_id][1] = NULL; //y
        objectsWithCoordsP[selected_p_id][2] = NULL; //и z
        deletedP[selected_p_id] = selected_p_id; //заносим её в удалённые
    }
    else { //иначе - это куб
        objectsWithCoordsC[selected_c_id][0] = NULL; //обнуляем его координаты по x
        objectsWithCoordsC[selected_c_id][1] = NULL; //y
        objectsWithCoordsC[selected_c_id][2] = NULL; //и z
        deletedC[selected_c_id] = selected_c_id; //заносим его в удалённые
    }
    Sleep(2); //ждём, потому что это происходит слишком быстро
    system("cls"); //очищаем консоль
    std::cout << "ГОТОВО!" << std::endl; //сообщение об успешности процедуры
}