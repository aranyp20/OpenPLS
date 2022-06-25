#include "Program.h"

unsigned int Program::windowWidth = 640;
unsigned int Program::windowHeight = 480;

Program::Program()
{
    
}

Program& Program::GetInstance()
{
    static Program instance;
    return instance;
	
}

bool Program::Init()
{


    if (!glfwInit())
        return false;


    window = glfwCreateWindow(windowWidth, windowHeight, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "Error" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    glViewport(0, 0, windowWidth, windowHeight);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    return true;
}

void Program::Run()
{
 
    float positions[] = {
        //A
        -0.5f,-0.5f,0.5f,
        0,0,1,
        0.5f,-0.5f,0.5f,
        0,0,1,
        0.5f,0.5f,0.5f,
        0,0,1,
        -0.5f,0.5f,0.5f,
        0,0,1,
        //B
        0.5f,0.5f,0.5f,
        1,0,0,
        0.5f,-0.5f,0.5f,
        1,0,0,
        0.5f,-0.5f,-0.5f,
        1,0,0,
        0.5f,0.5f,-0.5f,
        1,0,0,
        //C
         -0.5f,-0.5f,-0.5f,
        0,0,-1,
        0.5f,-0.5f,-0.5f,
        0,0,-1,
        0.5f,0.5f,-0.5f,
        0,0,-1,
        -0.5f,0.5f,-0.5f,
        0,0,-1,
        //D
        -0.5f,0.5f,0.5f,
        -1,0,0,
        -0.5f,-0.5f,0.5f,
        -1,0,0,
        -0.5f,-0.5f,-0.5f,
        -1,0,0,
        -0.5f,0.5f,-0.5f,
        -1,0,0,
        //e
        -0.5f,0.5f,0.5f,
        0,1,0,
        0.5f,0.5f,0.5f,
        0,1,0,
        0.5f,0.5f,-0.5f,
        0,1,0,
        -0.5f,0.5f,-0.5f,
        0,1,0,
        //f
        -0.5f,-0.5f,0.5f,
        0,-1,0,
        0.5f,-0.5f,0.5f,
        0,-1,0,
        0.5f,-0.5f,-0.5f,
        0,-1,0,
        -0.5f,-0.5f,-0.5f,
        0,-1,0
    };
 
    unsigned int indicies[] = {
        //A
        0,1,2,
        0,2,3,
        //B
        4,5,6,
        4,6,7,
        //C
        8,9,10,
        8,10,11,
        //D
        12,13,14,
        12,14,15,
        //E
        16,17,18,
        16,18,19,
        //F
        20,21,22,
        20,22,23
    };



    VAO va;
    VBO3f3f vb(positions, 216 * sizeof(float));
    va.AddVBO(vb);
    IBO ib(indicies, 36);


    Shader shader("shaders/GouraudShader.shader");
    shader.Bind();

    Material* material = new Material();
    material->kd = vec3(0.2f, 0.4f, 0.6f);
    material->ks = vec3(4, 4, 4);
    material->ka = vec3(0.4f, 0.4f, 0.4f);
    material->shininess = 10;
    Light* light = new Light();
    light->wLightPos = vec4(5, 5, 4, 1);
    light->La = vec3(1, 1, 1);
    light->Le = vec3(1, 1, 3);
    Camera* camera = new Camera();
    shader.SetUniform("material", *material);
    shader.SetUniform("light", *light);

    mat4 Mod = mat4();
    mat4 Modinv = mat4();

    Renderer renderer;

    vec3 eye = vec3(2, 1, 0);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

        Mod = Mod * RotationMatrix(0.0002f, vec3(0, 1, 0));
        Modinv = RotationMatrix(-0.0002f, vec3(0, 1, 0)) * Modinv;

        shader.SetUniform("wEye", eye);
        camera->ReplaceEye(eye);
        shader.SetUniform("V", camera->V());
        shader.SetUniform("P", camera->P());
        shader.SetUniform("M", Mod);
        shader.SetUniform("Minv", Modinv);



        renderer.Draw(va, ib, shader);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }


    glfwTerminate();
}

unsigned int Program::WindowWidth()
{
    return windowWidth;
}

unsigned int Program::WindowHeight()
{
    return windowHeight;
}




