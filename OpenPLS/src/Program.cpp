

#include "Program.h"

unsigned int Program::windowWidth = 1200;
unsigned int Program::windowHeight = 800;

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
    surface->meshHandler = new MeshHandler(surface);
    


    InputManager::AddIP(surface->meshHandler);
    InputManager::AddIP(surface);
    
    InputManager::SetCallbacks(window);


    surface->viewCamera = new Camera();

    return true;
}

void Program::Run()
{
   
    Mesh cube(Mesh::Shape::CUBE);
    
   

    surface->meshHandler->AddMesh(&cube);



    VBO* tVBO = new VBO3f3f(surface->toloka->GiveData());
    VAO vat;
    vat.AddVBO(*tVBO);



    std::string sh1 = "../shaders/Shader1.shader";
    std::string shg = "../shaders/GouraudShader.shader";

    //ezek kozott van kulonbseg?
    Shader shader1(sh1);
    Shader shader2(sh1);

   
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

       
        cube.Render(renderer,shader1,shader1,shader);

        glClear(GL_DEPTH_BUFFER_BIT);
        vat.Bind();

        tVBO->RefreshData(surface->toloka->GiveData());
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




