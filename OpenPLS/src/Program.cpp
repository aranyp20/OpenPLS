

#include "Program.h"

unsigned int Program::windowWidth = 1000;
unsigned int Program::windowHeight = 600;

Surface* Program::surface = NULL;

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


    surface = new Surface();
    MeshHandler* mh = new MeshHandler(surface);
    surface->meshHandler = mh;


    InputManager::AddIP(mh);
    InputManager::AddIP(surface);
    InputManager::SetCallbacks(window);


    surface->viewCamera = new Camera();

    return true;
}

void Program::Run()
{
    std::vector<Mesh::Point*> v;
    std::vector<Mesh::Point*> points;
    points.push_back(new Mesh::Point(-0.5f, -0.5f, 0.5f));
    points.push_back(new Mesh::Point(0.5f, -0.5f, 0.5f));
    points.push_back(new Mesh::Point(0.5f, -0.5f, -0.5f));
    points.push_back(new Mesh::Point(-0.5f, -0.5f, -0.5f));
    points.push_back(new Mesh::Point(-0.5f, 0.5f, 0.5f));
    points.push_back(new Mesh::Point(0.5f, 0.5f, 0.5f));
    points.push_back(new Mesh::Point(0.5f, 0.5f, -0.5f));
    points.push_back(new Mesh::Point(-0.5f, 0.5f, -0.5f));

    points.push_back(new Mesh::Point(0.0f, 1.0f, 0.5f));
    points.push_back(new Mesh::Point(0.0f, 1.0f, -0.5f));

    Mesh cube;
    surface->meshHandler->AddMesh(&cube);
   
    cube.AddPoint(points[0], v);
    v = { points[0] };
    cube.AddPoint(points[1], v);
    v.clear();
    v = { points[1] };
    cube.AddPoint(points[2], v);
    v.clear();
    v = { points[2],points[0]};
    cube.AddPoint(points[3], v);
    v.clear();
    v = { points[0] };
    cube.AddPoint(points[4], v);
    v.clear();
    v = { points[1],points[4]};
    cube.AddPoint(points[5], v);
    v.clear();
    v = { points[2],points[5] };
    cube.AddPoint(points[6], v);
    v.clear();
    v = { points[3],points[6],points[4]};
    cube.AddPoint(points[7], v);
    v.clear();
    v = { points[5],points[4] };
    cube.AddPoint(points[8], v);
    v.clear();
    v = { points[6],points[7],points[8]};
    cube.AddPoint(points[9], v);
    v.clear();

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

    cube.PrintVerts();




    RenderData rData = MeshRenderer::GiveSides(cube);
    VBO* vVBO = new VBO3f3f(MeshRenderer::GiveVertices(cube));
    VBO* eVBO = new VBO3f3f(MeshRenderer::GiveEdges(cube));
  
    VBO* tVBO = new VBO3f3f(surface->toloka.GiveData());
    VAO vat;

    vat.AddVBO(*tVBO);


    VAO va;
    //VBO3f3f vb(tempV);
    va.AddVBO(*(rData.vbo));
    //IBO ib(tempI);
    VAO va1;
    va1.AddVBO(*vVBO);

    std::string sh1 = "../shaders/Shader1.shader";
    std::string shg = "../shaders/GouraudShader.shader";

    //ezek kozott van kulonbseg?
    Shader shader1(sh1);
    Shader shader2(sh1);

    VAO va2;
    va2.AddVBO(*eVBO);


    va.Bind();
    Shader shader(shg);
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
    
    shader.SetUniform("material", *material);
    shader.SetUniform("light", *light);
  

    mat4 Mod = mat4();
    mat4 Modinv = mat4();

    Renderer renderer;

   
    

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        
        
        

        //Mod = Mod * RotationMatrix(0.0002f, vec3(0, 1, 0));
        //Modinv = RotationMatrix(-0.0002f, vec3(0, 1, 0)) * Modinv;
        shader.Bind();
        shader.SetUniform("wEye", surface->viewCamera->GetEye());
        shader.SetUniform("V", surface->viewCamera->V());
        shader.SetUniform("P", surface->viewCamera->P());
        shader.SetUniform("M", Mod);
        shader.SetUniform("Minv", Modinv);
        shader1.Bind();
        shader1.SetUniform("V", surface->viewCamera->V());
        shader1.SetUniform("P", surface->viewCamera->P());
        shader1.SetUniform("M", Mod);
        shader2.Bind();
        shader2.SetUniform("V", surface->viewCamera->V());
        shader2.SetUniform("P", surface->viewCamera->P());
        shader2.SetUniform("M", Mod);

       
        
        if (cube.Corrupted()) {
            
            va1.Bind();
            vVBO->RefreshData(MeshRenderer::GiveVertices(cube));
            va2.Bind();
            eVBO->RefreshData(MeshRenderer::GiveEdges(cube));   
            cube.UnCorrupt();
        }

        renderer.Draw(Renderer::TriangleData(va, *(rData.ibo), shader),Renderer::PointData(va1,shader1),Renderer::LineData(va2,shader2));

        glClear(GL_DEPTH_BUFFER_BIT);
        vat.Bind();
    
        
    
        tVBO->RefreshData(surface->toloka.GiveData());
        renderer.DrawL(vat,shader2);
        
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




