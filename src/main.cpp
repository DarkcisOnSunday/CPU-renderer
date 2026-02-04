#define _USE_MATH_DEFINES

#include "image.h"
#include "window.h"
#include "math/vec3.h"
#include <iostream>
#include <cmath>

uint32_t MakeColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
    return (a << 24) | (r << 16) | (g << 8) | b;
}

uint32_t WHITE = MakeColor(255, 255, 255);
uint32_t BLACK = MakeColor(0, 0, 0);
uint32_t RED = MakeColor(255, 0, 0);
uint32_t GREEN = MakeColor(0, 255, 0);
uint32_t BLUE = MakeColor(0, 0, 255);

Vec3 ConvertScreenCord(Vec3 vec3, Image& image) {
    vec3.x = (vec3.x + 1) / 2 * image.GetWidth();
    vec3.y =  (1 - (vec3.y + 1) / 2) * image.GetHeight();
    return vec3;
}

void DrawRightRect(float x, float y, int width, int height, uint32_t color, Image& image) {
    Vec3 vec3 = ConvertScreenCord(Vec3(x, y, 0), image);
    image.DrawRect((int)(vec3.x - width/2), (int)(vec3.y - height/2), width, height, color);
}

Vec3 Project(Vec3 vec3) {
    return Vec3(vec3.x / vec3.z, vec3.y / vec3.z, 0);
}
void DrawRightLine(Vec3 a, Vec3 b, uint32_t color, Image& image) {
    Vec3 vec3_0 = ConvertScreenCord(Project(a), image);
    Vec3 vec3_1 = ConvertScreenCord(Project(b), image);
    image.DrawLine((int)vec3_0.x, (int)vec3_0.y, (int)vec3_1.x, (int)vec3_1.y, color);
}

void DrawRightPoint(Vec3 vec3, Image& image) {
    int s = 5;

    vec3 = Project(vec3);
    DrawRightRect(vec3.x, vec3.y, s, s, GREEN, image);
}

Vec3 Translate_z(Vec3 vec3, float dz) {
    return Vec3(vec3.x, vec3.y, vec3.z + dz);
}

Vec3 Rotate_xz(Vec3 vec3, float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return Vec3(
        vec3.x*c - vec3.z*s,
        vec3.y,
        vec3.x*s + vec3.z*c
    );
}

/*std::vector<Vec3> vs = {
    Vec3( 0.25,  0.25,  0.25),
    Vec3(-0.25,  0.25,  0.25),
    Vec3(-0.25, -0.25,  0.25),
    Vec3( 0.25, -0.25,  0.25),

    Vec3( 0.25,  0.25, -0.25),
    Vec3(-0.25,  0.25, -0.25),
    Vec3(-0.25, -0.25, -0.25),
    Vec3( 0.25, -0.25, -0.25)
};

std::vector<std::vector<int>> fs = {
    {0, 1, 2, 3},
    {4, 5, 6, 7},
    {0, 4},
    {1, 5},
    {2, 6},
    {3, 7}

};*/

std::vector<Vec3> vs;
std::vector<std::vector<int>> fs;

void AddRing(std::vector<Vec3>& vs, float z, float y_offset, float width, float height, int segments) {
    for (int i = 0; i < segments; i++) {
        // Угол от 0 до 2 PI
        float angle = (float)i / segments * 2.0f * M_PI;
        
        // Формируем овал в плоскости XY, смещенный по Z
        vs.push_back(Vec3(
            cos(angle) * width,           // X (ширина)
            y_offset + sin(angle) * height, // Y (высота)
            z                             // Z (глубина)
        ));
    }
}

// Сшивает ТЕКУЩЕЕ добавленное кольцо с ПРЕДЫДУЩИМ
// ring_index - номер текущего кольца (начиная с 1)
void ConnectRingToPrevious(std::vector<std::vector<int>>& fs, int segments, int ring_index) {
    int start_curr = ring_index * segments;     // Начало текущего кольца
    int start_prev = (ring_index - 1) * segments; // Начало предыдущего
    
    for (int i = 0; i < segments; i++) {
        int next = (i + 1) % segments; // Индекс следующей точки в кольце (замыкает круг)
        
        // Создаем квадрат (два треугольника или quad) между кольцами
        fs.push_back({
            start_prev + i,
            start_prev + next,
            start_curr + next,
            start_curr + i
        });
    }
}

// --- Исправленная Генерация (Метод "Единой Трубы") ---

void GenerateDuck(std::vector<Vec3>& vs, std::vector<std::vector<int>>& fs) {
    vs.clear(); 
    fs.clear();
    
    int segs = 14;    // Количество сегментов в круге
    int ring_id = 0;  // Счетчик колец

    AddRing(vs, -0.45f, 0.10f, 0.01f, 0.01f, segs); 
    
    // 1. Основание хвоста
    ring_id++;
    AddRing(vs, -0.35f, 0.10f, 0.12f, 0.08f, segs);
    ConnectRingToPrevious(fs, segs, ring_id);

    // 2. Попа
    ring_id++;
    AddRing(vs, -0.20f, 0.05f, 0.22f, 0.18f, segs);
    ConnectRingToPrevious(fs, segs, ring_id);

    // 3. Живот (Центр)
    ring_id++;
    AddRing(vs,  0.00f, 0.00f, 0.26f, 0.22f, segs);
    ConnectRingToPrevious(fs, segs, ring_id);

    // 4. Грудь
    ring_id++;
    AddRing(vs,  0.20f, 0.10f, 0.22f, 0.20f, segs);
    ConnectRingToPrevious(fs, segs, ring_id);

    // 5. Основание шеи (сужаем и поднимаем)
    ring_id++;
    AddRing(vs,  0.28f, 0.28f, 0.14f, 0.14f, segs);
    ConnectRingToPrevious(fs, segs, ring_id);

    // 6. Середина головы
    ring_id++;
    AddRing(vs,  0.32f, 0.42f, 0.16f, 0.16f, segs);
    ConnectRingToPrevious(fs, segs, ring_id);

    // 7. Лицо (передняя часть головы)
    ring_id++;
    AddRing(vs,  0.35f, 0.48f, 0.12f, 0.12f, segs);
    ConnectRingToPrevious(fs, segs, ring_id);

    // === КЛЮВ (Пришиваем к последнему кольцу) ===
    // Последнее кольцо (№7) - это лицо.
    // Находим "переднюю" точку этого кольца (угол 0, т.е. cos=1, это справа, 
    // но в нашей системе координат Z вперед).
    // Построим клюв вручную, прицепившись к верху и низу лица.
    
    int face_start = ring_id * segs;
    int beak_tip_idx = vs.size();
    
    // Добавляем кончик клюва далеко впереди
    vs.push_back(Vec3(0.0f, 0.46f, 0.55f)); 

    // Индексы на лице, к которым крепим клюв.
    // Так как мы использовали sin/cos, верхняя точка кольца это index ~ segs/4
    int top_idx = face_start + (segs / 4);
    int bot_idx = face_start + (segs * 3 / 4);
    int left_idx = face_start + (segs / 2);
    int right_idx = face_start;

    // Треугольники клюва
    fs.push_back({top_idx, right_idx, beak_tip_idx}); // Верх-право
    fs.push_back({top_idx, left_idx, beak_tip_idx});  // Верх-лево
    fs.push_back({bot_idx, right_idx, beak_tip_idx}); // Низ-право
    fs.push_back({bot_idx, left_idx, beak_tip_idx});  // Низ-лево

    // === ЛАПЫ (Простые пирамидки снизу) ===
    float leg_x = 0.15f;
    
    for(int sign = -1; sign <= 1; sign += 2) {
        if(sign == 0) continue;
        
        int root = vs.size();
        float x = leg_x * sign;
        
        // 3 точки лапы
        vs.push_back(Vec3(x, -0.10f, 0.0f));        // Колено (в теле)
        vs.push_back(Vec3(x, -0.35f, -0.05f));      // Пятка
        vs.push_back(Vec3(x, -0.35f, 0.15f));       // Носок (пальцы)
        
        // Создаем треугольник (плоская лапа, чтобы не перегружать)
        fs.push_back({root, root+1, root+2});
        
        // Если хочешь объемную лапу, раскомментируй это:
        /*
        vs.push_back(Vec3(x + 0.05f*sign, -0.35f, 0.15f)); // Еще палец
        fs.push_back({root, root+2, root+3});
        fs.push_back({root+1, root+2, root+3});
        */
    }
}

bool duckLoaded = false;

void UpdateScene(Image& image, double time) {
    if (!duckLoaded) {
        GenerateDuck(vs, fs);
        duckLoaded = true;
    }

    float dz = 1;
    float angle = M_PI * time;

    image.Clear(BLACK);

    for (std::vector<int> f : fs) {
        for (int i = 0; i < (int)f.size(); i++) {
            Vec3 a = vs[f[i]];
            Vec3 b = vs[f[(i+1)%(int)f.size()]];

            a = Translate_z(Rotate_xz(a, angle), dz);
            b = Translate_z(Rotate_xz(b, angle), dz);

            DrawRightLine(a, b, GREEN, image);
        }
    }
}

int main() {
    //std::cout << "Starting SoftRender..." << std::endl;
    
    Image image(800, 800);
    
    if (!CreateAppWindow("SoftRender Demo", 800, 800)) {
        //std::cerr << "Failed to create window!" << std::endl;
        return 1;
    }
    
    void* windowHandle = GetWindowHandle();
    image.SetWindowHandle(windowHandle);
    
    auto startTime = std::chrono::steady_clock::now();
    bool running = true;
    
    //std::cout << "Running... Close window or press ESC to exit" << std::endl;
    
    while (running) {
        if (!ProcessAppWindowEvents()) {
            //std::cout << "Window close event received" << std::endl;
            running = false;
            break;
        }
        
        #ifdef _WIN32
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            //std::cout << "ESC pressed, exiting..." << std::endl;
            running = false;
            break;
        }
        #endif
        
        auto currentTime = std::chrono::steady_clock::now();
        double elapsedTime = std::chrono::duration<double>(currentTime - startTime).count();
        
        UpdateScene(image, elapsedTime);
        
        if (!image.DrawToWindow(windowHandle)) {
            //std::cerr << "Failed to draw to window!" << std::endl;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    DestroyAppWindow();
    //std::cout << "Shutting down..." << std::endl;
    
    return 0;
}