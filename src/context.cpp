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

void Context::CreatCircle(float radius,int segment){
    std::vector<float> vertices;
    std::vector<uint32_t> indices;

    const float pi=3.141592f;
    vertices.push_back(0.0f);//x
    vertices.push_back(0.0f);//y
    vertices.push_back(0.0f);//z///원 중심좌표

    for(int i=0;i<segment;i++){
        float angle = (360.0f/segment*i)*pi/180.0f;
        float x=cosf(angle)*radius;
        float y=sinf(angle)*radius;
        vertices.push_back(x); 
        vertices.push_back(y); 
        vertices.push_back(0.0f); 
    }

    for(int i=0;i<segment;i++){
        indices.push_back(0);
        indices.push_back(i+1);
        if(i==segment-1)
            indices.push_back(1);
        else
            indices.push_back(i+2);
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
    glUniform4f(loc,1.0f,1.0f,1.0f,1.0f);
}