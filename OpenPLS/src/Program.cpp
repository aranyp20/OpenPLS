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

//returns TRUE if init was successful
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
    std::vector<Point*> v;
    std::vector<Point*> points;
    points.push_back(new Point(-0.5f, -0.5f, 0.5f));
    points.push_back(new Point(0.5f, -0.5f, 0.5f));
    points.push_back(new Point(0.5f, -0.5f, -0.5f));
    points.push_back(new Point(-0.5f, -0.5f, -0.5f));
    points.push_back(new Point(-0.5f, 0.5f, 0.5f));
    points.push_back(new Point(0.5f, 0.5f, 0.5f));
    points.push_back(new Point(0.5f, 0.5f, -0.5f));
    points.push_back(new Point(-0.5f, 0.5f, -0.5f));

    points.push_back(new Point(0.0f, 1.0f, 0.5f));
    points.push_back(new Point(0.0f, 1.0f, -0.5f));

    Mesh cube;

   


    for (int i = 0; i<points.size(); i++) {
        cube.AddPoint(points[i], v);
    }

    std::vector<unsigned int> sinds{0, 1, 2, 3};
    cube.AddSide(sinds);
    sinds = { 4,5,6,7 };
    cube.AddSide(sinds);
    //sinds = { 0,1,5,4 };
    //cube.AddSide(sinds);
    sinds = { 1,2,6,5 };
    cube.AddSide(sinds);
    sinds = { 2,6,7,3 };
    cube.AddSide(sinds);
    sinds = { 0,3,7,4 };
    cube.AddSide(sinds);

    sinds = { 8,9,7,4 };
    cube.AddSide(sinds);
    sinds = { 8,9,6,5 };
    cube.AddSide(sinds);


    RenderData rData = MeshRenderer::GiveSides(cube);
    

    //float positions[] = {
    //    //A
    //    -0.5f,-0.5f,0.5f,
    //    0,0,1,
    //    0.5f,-0.5f,0.5f,
    //    0,0,1,
    //    0.5f,0.5f,0.5f,
    //    0,0,1,
    //    -0.5f,0.5f,0.5f,
    //    0,0,1,
    //    //B
    //    0.5f,0.5f,0.5f,
    //    1,0,0,
    //    0.5f,-0.5f,0.5f,
    //    1,0,0,
    //    0.5f,-0.5f,-0.5f,
    //    1,0,0,
    //    0.5f,0.5f,-0.5f,
    //    1,0,0,
    //    //C
    //     -0.5f,-0.5f,-0.5f,
    //    0,0,-1,
    //    0.5f,-0.5f,-0.5f,
    //    0,0,-1,
    //    0.5f,0.5f,-0.5f,
    //    0,0,-1,
    //    -0.5f,0.5f,-0.5f,
    //    0,0,-1,
    //    //D
    //    -0.5f,0.5f,0.5f,
    //    -1,0,0,
    //    -0.5f,-0.5f,0.5f,
    //    -1,0,0,
    //    -0.5f,-0.5f,-0.5f,
    //    -1,0,0,
    //    -0.5f,0.5f,-0.5f,
    //    -1,0,0,
    //    //e
    //    -0.5f,0.5f,0.5f,
    //    0,1,0,
    //    0.5f,0.5f,0.5f,
    //    0,1,0,
    //    0.5f,0.5f,-0.5f,
    //    0,1,0,
    //    -0.5f,0.5f,-0.5f,
    //    0,1,0,
    //    //f
    //    -0.5f,-0.5f,0.5f,
    //    0,-1,0,
    //    0.5f,-0.5f,0.5f,
    //    0,-1,0,
    //    0.5f,-0.5f,-0.5f,
    //    0,-1,0,
    //    -0.5f,-0.5f,-0.5f,
    //    0,-1,0
    //};
 
    //unsigned int indicies[] = {
    //    //A
    //    0,1,2,
    //    0,2,3,
    //    //B
    //    4,5,6,
    //    4,6,7,
    //    //C
    //    8,9,10,
    //    8,10,11,
    //    //D
    //    12,13,14,
    //    12,14,15,
    //    //E
    //    16,17,18,
    //    16,18,19,
    //    //F
    //    20,21,22,
    //    20,22,23
    //};

    //std::vector<float> tempV;
    //for (int i = 0; i < 144; i++) {
    //    tempV.push_back(positions[i]);
    //}
    //std::vector<unsigned int> tempI;
    //for (int i = 0; i < 36; i++) {
    //    tempI.push_back(indicies[i]);
    //}

    VAO va;
    //VBO3f3f vb(tempV);
    va.AddVBO(*(rData.vbo));
    //IBO ib(tempI);


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

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        
        
        

        Mod = Mod * RotationMatrix(0.0002f, vec3(0, 1, 0));
        Modinv = RotationMatrix(-0.0002f, vec3(0, 1, 0)) * Modinv;

        shader.SetUniform("wEye", eye);
        camera->ReplaceEye(eye);
        shader.SetUniform("V", camera->V());
        shader.SetUniform("P", camera->P());
        shader.SetUniform("M", Mod);
        shader.SetUniform("Minv", Modinv);



        renderer.Draw(va, *(rData.ibo), shader);

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




