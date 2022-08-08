

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

    WorldInit();


    return true;
}

void Program::WorldInit()
{
    surface = new Surface();
    surface->meshHandler = new MeshHandler(surface);
    


    InputManager::AddIP(surface->meshHandler);
    InputManager::AddIP(surface);
    
    InputManager::SetCallbacks(window);


    Mesh* cube = new Mesh(Mesh::Shape::CUBE);
    
    Material* material = new Material();
    material->kd = vec3(0.2f, 0.4f, 0.6f);
    material->ks = vec3(4, 4, 4);
    material->ka = vec3(0.4f, 0.4f, 0.4f);
    material->shininess = 10;

    cube->material = material;
   
    surface->meshHandler->AddMesh(cube);


    Light* light = new Light();
    light->wLightPos = vec4(5, 5, 4, 1);
    light->La = vec3(1, 1, 1);
    light->Le = vec3(1, 1, 3);

    surface->light = light;

    surface->viewCamera = new Camera();
}




void Program::Run()
{
   


    Renderer renderer;

   
    

    

    while (!glfwWindowShouldClose(window))
    {
        
        
        surface->Render(renderer);
       
        
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




