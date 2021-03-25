#include "context.h"

ContextUPtr Context::Create(){
    auto context = ContextUPtr(new Context());
    if (!context->Init())
        return nullptr;
    return std::move(context);
}

bool Context::Init(){

    float vertices[] = {
         0.5f,  0.5f, 0.0f, 0.0f, 0.2f, 0.8f,   // top right, blue
         0.5f, -0.5f, 0.0f, 0.8f, 0.4f, 0.7f,  // bottom right, green
        -0.5f, -0.5f, 0.0f, 0.0f, 0.8f, 0.2f, // bottom left, blue
        -0.5f,  0.5f, 0.0f, 0.8f, 0.7f, 0.4f,  // top left, yellow
    };
    uint32_t indices[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };
    m_vertexLayout = VertexLayout::Create();
    m_vertexBuffer=Buffer::CreateWithData(GL_ARRAY_BUFFER,GL_STATIC_DRAW,vertices,sizeof(float)*24);

    //m_vertexLayout->SetAttrib(0,3,GL_FLOAT,GL_FALSE,sizeof(float)*3,0);
    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE,  sizeof(float) * 6, 0);
                            
    m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE,sizeof(float) * 6, sizeof(float) * 3);
                              

    m_indexBuffer=Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER,GL_STATIC_DRAW,indices,sizeof(float)*6);

    ShaderPtr vertShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);
    if (!vertShader || !fragShader)
    return false;
    SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragShader->Get());

    m_program = Program::Create({fragShader, vertShader});
     if (!m_program)
        return false;
    SPDLOG_INFO("program id: {}", m_program->Get());

    glClearColor(0.5f,1.0f,0.8f,0.5f);

    
    return true;    
}

void Context::Render(){
    glClear(GL_COLOR_BUFFER_BIT);

    m_program->Use();
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);   
}

 

void Context::CreatCircle(float big_radius,float small_radius,int segment,
                                            float startPoint,float endPoint,float RGB_R,float RGB_G,float RGB_B ){
                                                ///////////////////////////////////////원 만들기  
    std::vector<float> vertices;
    std::vector<uint32_t> indices;

    const float pi=3.141592f;
    vertices.push_back(0.0f);//x
    vertices.push_back(0.0f);//y
    vertices.push_back(0.0f);//z 사용 x///원 중심좌표 

    for(int i=0;i<segment;i++){///////바깥 점   1~segment
        float angle;
        //angle = (360.0f/segment*i)*pi/180.0f;
        angle = (startPoint+(endPoint-startPoint)/segment*i)*pi/180.0f;
        float x=cosf(angle)*big_radius;
        float y=sinf(angle)*big_radius;
        vertices.push_back(x); 
        vertices.push_back(y); 
        vertices.push_back(0.0f);
        if (((startPoint != 0.0) || (endPoint != 360.0)) && i == segment - 1){
            angle = (startPoint+(endPoint-startPoint)/segment*(i+1))*pi/180.0f;
            x = cosf(angle) * big_radius;
            y = sinf(angle) * big_radius;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(0.0f);
        }
    }

    for (int i = 0; i < segment; i++){////안 점   segment+1~segment*2
        float angle;
        //angle = (360.0f / segment * i) * pi / 180.0f;
        angle = (startPoint+(endPoint-startPoint)/segment*i)*pi/180.0f;
        float x = cosf(angle) * small_radius;
        float y = sinf(angle) * small_radius;
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);
        if (((startPoint != 0.0) || (endPoint != 360.0)) && i == segment - 1){
            angle = (startPoint+(endPoint-startPoint)/segment*(i+1))*pi/180.0f;
            x = cosf(angle) * small_radius;
            y = sinf(angle) * small_radius;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(0.0f);
        }
    }
    if ((startPoint != 0.0) || (endPoint != 360.0)){ ////////시작 각 끝 각이 주어졌을때
        for (int i = 1; i <= (segment+1); i++){
            indices.push_back(i);
            if (i == (segment+1))
                continue;
            else
                indices.push_back(i + 1);
            indices.push_back(i+segment+1);
            //////1번 퍼즐
            indices.push_back(i+segment+1);
            if ((i+segment) == ((segment * 2)+1)) {
                continue;
            }
            else{
                indices.push_back(i+segment + 2);
                indices.push_back(i + 1);
            }
        }
    }
    else{ ////////////////////시작각 끝각이 기본값일때
        for (int i = 1; i <= segment; i++){
            indices.push_back(i);
            if (i == segment)
                indices.push_back(1);
            else
                indices.push_back(i + 1);
            indices.push_back(i+segment);
            //////1번 퍼즐
            indices.push_back(i+segment);
            if ((i+segment) == (segment * 2)){
                indices.push_back(1 + segment);
                indices.push_back(1);
            }
            else {
                indices.push_back(i+segment + 1);
                indices.push_back(i + 1);
            }
        }
        ///////2번 퍼즐
    }
    m_vertexLayout = VertexLayout::Create();
    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices.data(), 
    sizeof(float)*vertices.size());

    m_vertexLayout->SetAttrib(0,3,GL_FLOAT,GL_FALSE,sizeof(float)*3,0);

    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, 
    indices.data(), sizeof(uint32_t) * indices.size());
    m_indexCount=(int)indices.size();
    auto loc = glGetUniformLocation(m_program->Get(),"color");
    m_program->Use();
    glUniform4f(loc,RGB_R,RGB_G,RGB_B,0.0f);
}