#include "context.h"
#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void OnKeyEvent(GLFWwindow* window,int key, int scancode, int action, int mods) {
    SPDLOG_INFO("key: {}, scancode: {}, action: {}, mods: {}{}{}",
        key, scancode,
        action == GLFW_PRESS ? "Pressed" :
        action == GLFW_RELEASE ? "Released" :
        action == GLFW_REPEAT ? "Repeat" : "Unknown",
        mods & GLFW_MOD_CONTROL ? "C" : "-",
        mods & GLFW_MOD_SHIFT ? "S" : "-",
        mods & GLFW_MOD_ALT ? "A" : "-");
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void OnFramebufferSizeChange(GLFWwindow* window, int width, int height) {
    SPDLOG_INFO("framebuffer size changed: ({} x {})", width, height);
    glViewport(0, 0, width, height);
}

int main(int argc,const char**argv)
{
    SPDLOG_INFO("start program");
    SPDLOG_INFO("arg count:{}", argc);
    for(int i=0;i<argc;i++){
        SPDLOG_INFO("argv[{}]:{}",i,argv[i]);
    }
    float big_Radius=0.75f;//////큰 반지름
    float small_Radius=0.5f;//////작은 반지름
    int circleSegmentCount=16;
    float startPoint=0.0f;
    float endPoint=360.0f;
    float RGB_R=1.0f;///////레드
    float RGB_G=1.0f;///////그린
    float RGB_B=1.0f;///////블루
    //////////////////////////////////////////////큰 반지름 작은 반지금 세그먼트 시작각 끝각 R,G,B
    if(argc>=2){/////큰 반지름
        big_Radius=std::stof(argv[1]);
    }
    if (argc >= 3){ /////작은 반지름
        small_Radius = std::stof(argv[2]);
    }
    if(argc>=4){///////세그먼트
        circleSegmentCount=std::stoi(argv[3]);
    }
    if(argc>=5){///////시작각
        startPoint=std::stof(argv[4]);
    }
    if(argc>=6){///////끝각
        endPoint=std::stof(argv[5]);
    }
    if(argc>=7){///////레드
        RGB_R=std::stof(argv[6]);
    }
    if(argc>=8){///////그린
        RGB_G=std::stof(argv[7]);
    }
    if(argc>=9){///////블루
        RGB_B=std::stof(argv[8]);
    }

        // glfw 라이브러리 초기화, 실패하면 에러 출력후 종료
    SPDLOG_INFO("Initialize glfw");
    if (!glfwInit()) {
        const char* description = nullptr;
        glfwGetError(&description);
        SPDLOG_ERROR("failed to initialize glfw: {}", description);
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

     // glfw 윈 도우 생성, 실패하면 에러 출력후 종료
    SPDLOG_INFO("Create glfw window");
    auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
    if (!window) {
        SPDLOG_ERROR("failed to create glfw window");
        glfwTerminate();
        return -1;
    }
    	
    glfwMakeContextCurrent(window);
    // glad를 활용한 OpenGL 함수 로딩
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        SPDLOG_ERROR("failed to initialize glad");
        glfwTerminate();
        return -1;
    }
    auto glVersion = glGetString(GL_VERSION);
    SPDLOG_INFO("OpenGL context version: {}", glVersion);

    auto context = Context::Create();
    if (!context){
        SPDLOG_ERROR("failed to create context");
        glfwTerminate();
        return -1;
    }

///////////////////큰 반지름   작은 반지름  세그먼트  시작각 끝각 R G B 
    context->CreatCircle(big_Radius,small_Radius,circleSegmentCount,startPoint,endPoint,RGB_R,RGB_G,RGB_B);


    // ShaderPtr vertShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);
    // ShaderPtr fragShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);
    // SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
    // SPDLOG_INFO("fragment shader id: {}", fragShader->Get());

    // auto program = Program::Create({fragShader, vertShader});
    // SPDLOG_INFO("program id: {}", program->Get());

    OnFramebufferSizeChange(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChange);
    glfwSetKeyCallback(window, OnKeyEvent);
      // glfw 루프 실행, 윈도우 close 버튼을 누르면 정상 종료
    SPDLOG_INFO("Start main loop");
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        context->Render();
        glfwSwapBuffers(window);
    }
    context.reset();
 

    glfwTerminate();
    return 0;
}