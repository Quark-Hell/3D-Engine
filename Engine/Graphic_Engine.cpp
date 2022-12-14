#include "Graphic_Engine.h"


inline void Screen::CreateScreen(unsigned int Wight, unsigned int Height, unsigned int BitsPerPixel,std::string Name, sf::ContextSettings Screen_Settings) {
    _wight = Wight;
    _height = Height;
    _bitsPerPixel = BitsPerPixel;
    _name = Name;
    _screen_Settings = Screen_Settings;

    _screen = new sf::RenderWindow(sf::VideoMode(Wight, Height, BitsPerPixel), Name, sf::Style::Resize | sf::Style::Close, Screen_Settings);
}
inline Screen* Render::GetScreenClass() {
    return &(Render::_screenClass);
}

inline void Render::PrepareToRender() {
    glClearColor(0.3f, 0.3f, 0.3f, 0.f);

    //Light
    GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

    GLfloat LightAmbient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);

    GLfloat LightDiffuse[] = { 0.75f, 0.75f, 0.75f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);

    GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

    GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_CULL_FACE); // enable culling face
    glCullFace(GL_BACK); // cull faces from back
    glFrontFace(GL_CCW); // vertex order (counter clock wise)

    glEnable(GL_COLOR_MATERIAL);

    // Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glClearDepth(1.f);
    glDepthFunc(GL_LEQUAL);
    
    float ratio = 4 / 3;
    glFrustum(-ratio, ratio, -1.f, 1.f, 1.0f, 500.f);
}

inline void Render::ApplyCameraTransform(Camera* camera) {
    Vector3 Position = camera->GetParentObject()->GetPosition();
    Vector3 Rotation = camera->GetParentObject()->GetRotation();

    float Fov;
    float Aspect;
    float ZNear;
    float ZFar;

    camera->GetCameraInfo(&Fov, &Aspect, &ZNear, &ZFar);

    //// Setup a perspective projection & Camera position
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(Fov, Aspect, ZNear, ZFar);

    glRotatef(Rotation.X, 1.f, 0.f, 0.f);
    glRotatef(Rotation.Y, 0.f, 1.f, 0.f);
    glRotatef(Rotation.Z, 0.f, 0.f, 1.f);

    glTranslatef(Position.X, Position.Y, Position.Z);
}

inline void Render::ClearFrameBuffer() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

inline void Render::ApplyTransformation(Vector3 Position, Vector3 Rotation, Vector3 Scale) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(Position.X, Position.Y, Position.Z);

    glRotatef(Rotation.X, 1.f, 0.f, 0.f);
    glRotatef(Rotation.Y, 0.f, 1.f, 0.f);
    glRotatef(Rotation.Z, 0.f, 0.f, 1.f);

    glScalef(Scale.X, Scale.Y, Scale.Z);
}

inline void Render::RenderMesh(Mesh* mesh) {
    glBegin(GL_TRIANGLES);
    //TODO:
    glColor3f(1, 0, 0);

    for (size_t i = 0; i < mesh->Points.size(); i++)
    {
        glNormal3f(mesh->Normals[i].X, mesh->Normals[i].Y, mesh->Normals[i].Z);
        glVertex3f(mesh->Points[i].X, mesh->Points[i].Y, mesh->Points[i].Z);
    }

    glEnd();
}

inline void Render::SceneAssembler() {
    for (size_t i = 0; i < World::ObjectsOnScene.size(); i++)
    {
        for (size_t j = 0; j < World::ObjectsOnScene[i]->GetCountOfModules(); j++)
        {
            Mesh* mesh = dynamic_cast<Mesh*>(World::ObjectsOnScene[i]->GetModuleByIndex(j));

            if (mesh != nullptr) {
                Render::ApplyTransformation(World::ObjectsOnScene[i]->GetPosition(), World::ObjectsOnScene[i]->GetRotation(), World::ObjectsOnScene[i]->GetScale());
                RenderMesh(mesh);
            }
        }
    }   
}       

inline void Render::StartRender(Camera* camera) {
    sf::ContextSettings window_settings;
    window_settings.depthBits = 24;
    window_settings.stencilBits = 8;
    window_settings.antialiasingLevel = 2;

    _screenClass.CreateScreen(800, 600, 32, "SFML OpenGL", window_settings);

    Render::PrepareToRender();
    Render::ApplyCameraTransform(camera);

    camera->SetCameraInfo(60, 4.0 / 3.0, 1, 300);

    _screenClass._screen->setVerticalSyncEnabled(true);
}


inline void Render::RenderLoop(Camera* camera) {
    if (_screenClass._screen->isOpen())
    {
        ClearFrameBuffer();

        //PrepareToRender();
        ApplyCameraTransform(camera);

        Render::SceneAssembler();

        _screenClass._screen->display();
    }
}
