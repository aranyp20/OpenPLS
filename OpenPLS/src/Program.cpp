

#include "Program.h"

unsigned int Program::windowWidthR = 1500;
unsigned int Program::windowHeightR = 900;

unsigned int Program::surfaceWidth = 1200;
unsigned int Program::surfaceHeight = 750;
unsigned int Program::surfaceStartingX= 30;
unsigned int Program::surfaceStartingY = 50;


unsigned int Program::SurfaceWidth()
{
    return surfaceWidth;
}
unsigned int Program::SurfaceHeight()
{
    return surfaceHeight;
}
unsigned int Program::SurfaceStartingX()
{
    return surfaceStartingX;
}
unsigned int Program::SurfaceStartingY()
{
    return surfaceStartingY;
}


Surface* Program::surface = NULL;
GUI::Hud* Program::hud = NULL;

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


    window = glfwCreateWindow(WindowWidthR(), WindowHeightR(), "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "Error" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    glViewport(0,0,WindowWidthR(),WindowHeightR());
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(SurfaceStartingX(), WindowHeightR()-SurfaceStartingY() - SurfaceHeight(), SurfaceWidth(), SurfaceHeight());
    //glViewport(0,0,60,60);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    WorldInit();


    return true;
}

void Program::WorldInit()
{

    hud = new GUI::Hud(new Shader2D());


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
        
        
        glClearColor(0.28f,0.28f,0.28f,1);
        glClear(GL_COLOR_BUFFER_BIT );
        
        hud->TestHappened();

        glViewport(SurfaceStartingX(), WindowHeightR()-SurfaceStartingY() - SurfaceHeight(), SurfaceWidth(), SurfaceHeight());
       
        surface->Render(renderer);
       
        
        glfwSwapBuffers(window);

        glfwPollEvents();
    }


    glfwTerminate();
}

unsigned int Program::WindowWidthR()
{
    return windowWidthR;
}

unsigned int Program::WindowHeightR()
{
    return windowHeightR;
}




