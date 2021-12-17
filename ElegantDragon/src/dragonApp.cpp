#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "GOs/dragon.h"
#include "DragonDB.h"
#include "projectDefines.h"
#include "graphicUtilities.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <cstdlib>
#include <utility>
#include <vector>
#include <algorithm>

#include <chrono>

static int dragonLoop(GLFWwindow*, DragonDB& DragDB);


int dragonApp(const unsigned int& xRes, const unsigned int& yRes)
{
    std::cout << "DRAGON!!!!" << std::endl;

    //prompt something!!
    float homeScaling = 1;//0.029;
    std::pair<unsigned int, unsigned int> resolution(xRes, yRes); //width, height

    GLFWwindow* window;

    glfwSetErrorCallback(errorCallbackFunc);

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(resolution.first, resolution.second, "Dragon Test Window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << ("[ERROR] GLEW NOT OK") << std::endl;
        return -1;
    }
    GL_SAFE(std::cout << glGetString(GL_VERSION) << std::endl);

    //v-sync
    glfwSwapInterval(1);


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    // 
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    //Create and initialize Dragon context
    DragonDB DrgnDB(0, homeScaling, resolution);

    if (dragonLoop(window, DrgnDB))
    {
        PROJ_WARNING("Main Loop returned non-zero");
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


static int dragonLoop(GLFWwindow* window, DragonDB& DrgnDB)
{
    //ImGUI states & variables
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


    //Object setup
    unsigned len = 4096;

    Drgn testDragon4(DrgnDB, len, DrgnType::SolidMVP);
    DrgnDB.RenderBlocks[1]->insert(&testDragon4);
    Drgn testDragon5(DrgnDB, len, DrgnType::Style0MVP);
    DrgnDB.RenderBlocks[1]->insert(&testDragon5);
    testDragon5.editModelMat()[0][3] = 0;
    testDragon4.editModelMat()[2][3] = -5;


    //Renderer setup
    Renderer* pRend = &DrgnDB.rend;
    pRend->enableBlend();
    pRend->enableDepth();
    pRend->setBackground(0.38, 0.624, 0.569, 1.0);

    //Timer setup
    auto generalTimerInit = std::chrono::steady_clock::now(); //std::chrono::high_resolution_clock::now();
    auto perfClock = std::chrono::high_resolution_clock::now();

    std::chrono::microseconds lastFrameTimes[PERF_COUNT_BUFFER_SIZE]; // = std::chrono::microseconds(0);
    for (unsigned int i = 0; i < PERF_COUNT_BUFFER_SIZE; ++i)
        lastFrameTimes[i] = std::chrono::microseconds(0);

    unsigned long frameNum = 0;
    //testDragon2.setCurLen(3067);
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        pRend->clear();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            static float f = 0.0f;
            static int counter = 0;

            static float rotX = 0;
            static float rotY = 0;
            static float rotZ = 0;

            ImGui::Begin("Controls");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("Camera");               // Display some text (you can use a format strings too)
            //ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            //ImGui::Checkbox("Another Window", &show_another_window);

            //edits x, y, z values for the camera transform (negative of camera location aka. world's offset)
            //applies -30.0f to 30.0f offset to original value
            ImGui::SliderFloat("x", &(DrgnDB.RenderBlocks[1]->editMat(0)[0][3]), -100.0f, 100.0f);
            ImGui::SliderFloat("y", &(DrgnDB.RenderBlocks[1]->editMat(0)[1][3]), -100.0f, 100.0f);
            ImGui::SliderFloat("z", &(DrgnDB.RenderBlocks[1]->editMat(0)[2][3]), -100.0f, 100.0f);

            //edits x, y, z values for the camera transform (negative of camera location aka. world's offset)
            //applies -10.0f to 40.0f offset to original value
            ImGui::SliderFloat("rotX", &rotX, -2.0f * PI, 2.0f * PI);
            ImGui::SliderFloat("rotY", &rotY, -2.0f * PI, 2.0f * PI);
            ImGui::SliderFloat("rotZ", &rotZ, -2.0f * PI, 2.0f * PI);

            DrgnDB.RenderBlocks[1]->setRotCard(1, 1, rotX);
            DrgnDB.RenderBlocks[1]->applyRotCard(1, 2, rotY);
            DrgnDB.RenderBlocks[1]->applyRotCard(1, 0, rotZ);

            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
            pRend->setBackground(clear_color.x, clear_color.y, clear_color.z, clear_color.w);

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);
            ImGui::End();
        }

        {
            static float inputData[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
            static MathMatRMaj<float> InputPoint(1, 4, inputData);
            static MathMatRMaj<float> ZeroMap;


            ImGui::Begin("Debug Info");
            ImGui::Text("World To Screen Map Test:");
            ImGui::Text("World Point");
            ImGui::SliderFloat("x", &(InputPoint[0][0]), -30.0f, 30.0f);
            ImGui::SliderFloat("y", &(InputPoint[1][0]), -30.0f, 30.0f);
            ImGui::SliderFloat("z", &(InputPoint[2][0]), -30.0f, 30.0f);
            ZeroMap = DrgnDB.RenderBlocks[1]->viewRes(0) * InputPoint;
            ImGui::Text("Mapping <%.2f, %.2f, %.2f>WS to <%.2f, %.2f, %.2f>SS", 
                InputPoint[0][0], InputPoint[1][0], InputPoint[2][0],
                ZeroMap[0][0] / ZeroMap[3][0], ZeroMap[1][0] / ZeroMap[3][0], ZeroMap[2][0] / ZeroMap[3][0]);
            ImGui::Text("Homogeneous Component: %.2f", ZeroMap[3][0]);

            ImGui::Text("VP Matrix:\n <%5.2f, %5.2f, %5.2f, %5.2f>,\n <%5.2f, %5.2f, %5.2f, %5.2f>,\n <%5.2f, %5.2f, %5.2f, %5.2f>,\n <%5.2f, %5.2f, %5.2f, %5.2f>",
                DrgnDB.RenderBlocks[1]->viewRes(0, 0, 0), DrgnDB.RenderBlocks[1]->viewRes(0, 0, 1), DrgnDB.RenderBlocks[1]->viewRes(0, 0, 2), DrgnDB.RenderBlocks[1]->viewRes(0, 0, 3),
                DrgnDB.RenderBlocks[1]->viewRes(0, 1, 0), DrgnDB.RenderBlocks[1]->viewRes(0, 1, 1), DrgnDB.RenderBlocks[1]->viewRes(0, 1, 2), DrgnDB.RenderBlocks[1]->viewRes(0, 1, 3),
                DrgnDB.RenderBlocks[1]->viewRes(0, 2, 0), DrgnDB.RenderBlocks[1]->viewRes(0, 2, 1), DrgnDB.RenderBlocks[1]->viewRes(0, 2, 2), DrgnDB.RenderBlocks[1]->viewRes(0, 2, 3),
                DrgnDB.RenderBlocks[1]->viewRes(0, 3, 0), DrgnDB.RenderBlocks[1]->viewRes(0, 3, 1), DrgnDB.RenderBlocks[1]->viewRes(0, 3, 2), DrgnDB.RenderBlocks[1]->viewRes(0, 3, 3));
            
            //ImGui::Text("P Matrix (hardcoded to Component #3):\n <%5.2f, %5.2f, %5.2f, %5.2f>,\n <%5.2f, %5.2f, %5.2f, %5.2f>,\n <%5.2f, %5.2f, %5.2f, %5.2f>,\n <%5.2f, %5.2f, %5.2f, %5.2f>",
            //    DrgnDB.RenderBlocks[1]->viewMat(3, 0, 0), DrgnDB.RenderBlocks[1]->viewMat(3, 0, 1), DrgnDB.RenderBlocks[1]->viewMat(3, 0, 2), DrgnDB.RenderBlocks[1]->viewMat(3, 0, 3),
            //    DrgnDB.RenderBlocks[1]->viewMat(3, 1, 0), DrgnDB.RenderBlocks[1]->viewMat(3, 1, 1), DrgnDB.RenderBlocks[1]->viewMat(3, 1, 2), DrgnDB.RenderBlocks[1]->viewMat(3, 1, 3),
            //    DrgnDB.RenderBlocks[1]->viewMat(3, 2, 0), DrgnDB.RenderBlocks[1]->viewMat(3, 2, 1), DrgnDB.RenderBlocks[1]->viewMat(3, 2, 2), DrgnDB.RenderBlocks[1]->viewMat(3, 2, 3),
            //    DrgnDB.RenderBlocks[1]->viewMat(3, 3, 0), DrgnDB.RenderBlocks[1]->viewMat(3, 3, 1), DrgnDB.RenderBlocks[1]->viewMat(3, 3, 2), DrgnDB.RenderBlocks[1]->viewMat(3, 3, 3));

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        //a temporary terrible implementation
        unsigned int time = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - generalTimerInit)).count();

        unsigned int turnLen = time / 20000 % len;
        
        testDragon4.setCurLen(turnLen);
        testDragon5.setCurLen(turnLen);
        
        //draws everything onto the screen
        DrgnDB.RenderBlocks[1]->drawAll(*pRend);

        //Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        //Store frame time
        lastFrameTimes[frameNum % PERF_COUNT_BUFFER_SIZE] = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - perfClock);
        //Generate performance metrics
        if (frameNum % 120 == 0)
        {
            std::vector<long> sorted;
            long mean = 0;
            unsigned int effectiveBufferSize = (frameNum >= PERF_COUNT_BUFFER_SIZE ? PERF_COUNT_BUFFER_SIZE : frameNum + 1);
            for (unsigned int i = 0; i < effectiveBufferSize; ++i)
            {
                sorted.push_back(lastFrameTimes[i].count());
                mean += lastFrameTimes[i].count();
            }
            std::sort(sorted.begin(), sorted.end());
            mean /= effectiveBufferSize;

            std::cout << "(Microseconds) Average Frame time: " << mean << \
                ", 99% Low: " << sorted[effectiveBufferSize * 99 / 100] << ", 90% Low: " << sorted[effectiveBufferSize * 9 / 10] << 
                ", 10% Low: " << sorted[effectiveBufferSize / 10] << ", 1% Low: " << sorted[effectiveBufferSize / 100] << \
            std::endl;
            /*for (unsigned int i = 0; i < effectiveBufferSize; ++i)
                std::cout << lastFrameTimes[i].count() << " ";
            std::cout << std::endl;*/
        }
        glfwSwapBuffers(window);

        perfClock = std::chrono::high_resolution_clock::now();
        frameNum++;
        /* Poll for and process events */
        glfwPollEvents();
    }
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();


    return 0;
}