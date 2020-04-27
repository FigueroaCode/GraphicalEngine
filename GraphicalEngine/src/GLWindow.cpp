#include "GLWindow.h"
#include "Renderer.h"

#include "test_interface/TestInterface.h"
#include "test_interface/TestClearColor.h"
#include "test_interface/TestTexture2D.h"
#include "test_interface/TestRotation.h"
#include "test_interface/TestGraphing.h"
#include "test_interface/TestTextureGraph.h"
#include "test_interface/TestAxisGraph.h"
#include "test_interface/Test3DGraph.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

GLWindow::GLWindow(float width, float height)
	:width(width), height(height) {

    if (!init()) {
        printf("Failed to init Window\n");
        return;
    }

    // Set how to handle blending situation for textures
    GLCheckError(glEnable(GL_BLEND));
    GLCheckError(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    Renderer renderer;

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    //io.Fonts->AddFontDefault();

    // Create Tests
    test::TestInterface* curTest = new test::Test3DGraph();
    test::TestMenu* testMenu = new test::TestMenu(curTest);
    //curTest = testMenu;

    testMenu->registerTest<test::TestClearColor>("Clear Color");
    testMenu->registerTest<test::TestTexture2D>("Texture 2D");
    testMenu->registerTest<test::TestRotation>("Rotations");
    testMenu->registerTest<test::TestGraphing>("Graphing");
    testMenu->registerTest<test::TestTextureGraph>("Texture Graphing");
    testMenu->registerTest<test::TestAxisGraph>("Axis Graph");
    testMenu->registerTest<test::Test3DGraph>("3D Graph");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        /* Render here */
        GLCheckError(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        renderer.clear();
        
        if (curTest) {
            curTest->onUpdate(io.DeltaTime);
            curTest->onRender();
            ImGui::Begin(curTest->getTestName().c_str());
            if (curTest != testMenu && ImGui::Button("<-")) {
                delete curTest;
                curTest = testMenu;
            }
            curTest->onImGuiRender();
            ImGui::End();
        }

        // ImGui Rendering
        ImGui::Render();
        //int display_w, display_h;
        //glfwGetFramebufferSize(window, &display_w, &display_h);
        //glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Cleanup
    if (curTest != testMenu)
        delete testMenu;
    delete curTest;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}

bool GLWindow::init() {
    /* Initialize the library */
    if (!glfwInit())
        return false;

    /* Setup context profile */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Graphical Engine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return false;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Synchronize frame rate with v sync
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        printf("Failed to init glew\n");
        return false;
    }

    return true;
}

GLWindow::~GLWindow() {
    glfwDestroyWindow(window);
}

void notes() {
    ///* Setup Vertices */
    //float positions[] = {
    //    //( windowX, windowY, textureX, textureY)
    //        384.0f, 0.0f, 0.25f, 0.0f, // 0
    //        640.0f, 0.0f, 0.5f, 0.0f,  // 1
    //        640.0f, 256.0f, 0.5f, 0.5f,  // 2
    //        384.0, 256.0f, 0.25f, 0.5f,  // 3
    //        256.0, 256.0f, 0.0f, 0.5f,   // 4
    //        768.0, 256.0f,  1.0f, 0.5f,  // 5
    //        512.0f, 512.0f, 0.5f, 1.0f    // 6
    //};

    //// Needed to remove duplicate vertices
    //// Clock wise indices
    //unsigned int indices[] = {
    //    0, 3, 1,
    //    3, 2, 1,
    //    4, 6, 5,
    //};
    //// Set how to handle blending situation for textures
    //GLCheckError(glEnable(GL_BLEND));
    //GLCheckError(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    //VertexArray va;
    //VertexBuffer vb(positions, 7 * 4 * sizeof(float));
    //VertexBufferLayout layout;
    //layout.push<float>(2);
    //layout.push<float>(2);
    //va.addBuffer(vb, layout);

    //IndexBuffer ib(indices, 12);

    //// Set coordinate space to window size in pixels
    //glm::mat4 projMatrix = glm::ortho(0.0f, width, 0.0f, height, -1.0f, 1.0f);
    //// Move camera view to the right 100px to make object look like it moved left
    //glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    //// Move object to the right
    //glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    //glm::mat4 mvp = projMatrix * viewMatrix * modelMatrix;
    ///* Setup shaders */
    //Shader shader("resources/shaders/basic.shader");

    //std::string png = "resources/textures/transparentTex.png";
    //std::string jpg = "resources/textures/greyCopperRustyTex.jpg";
    //Texture texture(png);
    //texture.Bind();
    //// Default slot in Bind() is 0
    //shader.bind();
    //shader.setUniform1i("u_Texture", 0);
    //shader.setUnformMat4f("u_ModelViewProjection", mvp);

    //Renderer renderer;

    //// Setup ImGui context
    //IMGUI_CHECKVERSION();
    //ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO(); (void)io;
    ////io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    //    // Setup Platform/Renderer bindings
    //ImGui_ImplGlfw_InitForOpenGL(window, true);
    //const char* glsl_version = "#version 130";
    //ImGui_ImplOpenGL3_Init(glsl_version);

    //// Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    ////ImGui::StyleColorsClassic();
    ////io.Fonts->AddFontDefault();

    //bool show_demo_window = true;
    //bool show_another_window = false;
    //ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //glm::vec3 translation = glm::vec3(0, 0, 0);

    ///* Loop until the user closes the window */
    //while (!glfwWindowShouldClose(window))
    //{
    //    // Start the ImGui frame
    //    ImGui_ImplOpenGL3_NewFrame();
    //    ImGui_ImplGlfw_NewFrame();
    //    ImGui::NewFrame();

    //    /* Render here */
    //    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    //    renderer.clear();

    //    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), translation);
    //    glm::mat4 mvp = projMatrix * viewMatrix * modelMatrix;

    //    shader.bind();
    //    shader.setUnformMat4f("u_ModelViewProjection", mvp);

    //    renderer.draw(va, ib, shader);
    //    // Create ImGui window
    //    {
    //        static float f = 0.0f;
    //        static int counter = 0;

    //        ImGui::Begin("Debug");                                  // Create a window

    //        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

    //        ImGui::SliderFloat3("translation", &translation.x, -1.0f * width, width); // Edit 1 float using a slider from 0.0f to 1.0f

    //        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

    //        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
    //            counter++;
    //        ImGui::SameLine();
    //        ImGui::Text("counter = %d", counter);

    //        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    //        ImGui::End();
    //    }

    //    // ImGui Rendering
    //    ImGui::Render();
    //    int display_w, display_h;
    //    glfwGetFramebufferSize(window, &display_w, &display_h);
    //    glViewport(0, 0, display_w, display_h);
    //    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    //    /* Swap front and back buffers */
    //    glfwSwapBuffers(window);

    //    /* Poll for and process events */
    //    glfwPollEvents();
    //}

    //// Cleanup
    //ImGui_ImplOpenGL3_Shutdown();
    //ImGui_ImplGlfw_Shutdown();
    //ImGui::DestroyContext();

    //glfwTerminate();
}