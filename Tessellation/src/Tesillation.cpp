// Tessilation final project
// Name: Wesley Strong
// NetID: wfs51

// Using precompiled headers to speed up compilation times.
// To disable this simply copy the include statments from "stdafx.h"
// You will also need to do this for "shader.cpp" and "uniforms.cpp"
// Also using static linking so be sure to add GLEW_STATIC to the preproccessor
#include "stdafx.h"

// Had some very strange issues with compiling shaders.
// Simply deleting a random line or adding comments would case the shader to compile 
// or result in error: c0000. No idea what is causing this. Sometimes it works sometimes it doesnt....
// However, when compiled using debug mode resulted in almost never having this error....
#include "shader/shader.h"
#include "uniforms/uniforms.h"

using namespace std;

// Global variables
// IDeally, we'd wrap a class or something around these. Go C-like for now
GLuint bufferID,	   
   	   arrayID,
	   indexBufferID,
	   texUnit;

GLboolean isDefaultColor;
GLboolean isWireframe = GL_FALSE;
MiniGeometry mini;         // The mini geometry data
GLFWwindow* window = NULL; // The app window


GLfloat tesslevelInner,
tesslevelOuter;

// Map of part names to colors
struct ColorEntry
{
    string part;
    float color[3];
};


ColorEntry colormap[] = {
    {string("Body"), {0.05f, 0.05f, 0.54f}},
    {string("Body Chrome"), {0.95f, 0.96f, 0.93f}},
    {string("Roof"), {0.85f, 0.85f, 0.85f}},
    {string("Headlights"), {0.95f, 0.96f, 0.93f}},
    {string("Mirrors"), {0.85f, 0.85f, 0.85f}},
    {string("Brakelights"), {0.54f, 0.22f, 0.22f}},
    {string("Undercarriage"), {0.2f, 0.2f, 0.2f}},
    {string("Antenna"), {0.2f, 0.2f, 0.2f}},
    {string("Driver Blinker"), {0.9f, 0.5f, 0.1f}},
    {string("Passenger Blinker"), {0.9f, 0.5f, 0.1f}},
    {string("Exhaust"), {0.95f, 0.96f, 0.93f}},
    {string("Upper Driver Wiper"), {0.2f, 0.2f, 0.2f}},
    {string("Upper Passenger Wiper"), {0.2f, 0.2f, 0.2f}},
    {string("Lower Driver Wiper"), {0.2f, 0.2f, 0.2f}},
    {string("Lower Passenger Wiper"), {0.2f, 0.2f, 0.2f}},
    {string("Rear Wiper"), {0.2f, 0.2f, 0.2f}},
    {string("Vents"), {0.1f, 0.1f, 0.1f}},
    {string("License"), {0.94f, 0.64f, 0.19f}},
    {string("Front Driver Rim"), {0.75f, 0.75f, 0.75f}},
    {string("Front Passenger Rim"), {0.75f, 0.75f, 0.75f}},
    {string("Rear Driver Rim"), {0.75f, 0.75f, 0.75f}},
    {string("Rear Passenger Rim"), {0.75f, 0.75f, 0.75f}},
    {string("Front Driver Tire"), {0.1f, 0.1f, 0.1f}},
    {string("Front Passenger Tire"), {0.1f, 0.1f, 0.1f}},
    {string("Rear Driver Tire"), {0.1f, 0.1f, 0.1f}},
    {string("Rear Passenger Tire"), {0.1f, 0.1f, 0.1f}},
    {string("Brakes"), {0.75f, 0.75f, 0.75f}},
    {string("Rear View Mirror"), {0.8f, 0.8f, 0.8f}},
    {string("Interior"), {0.4f, 0.4f, 0.0f}},
    {string("Driver"), {0.9f, 0.9f, 0.9f}},
    {string("Chair"), {0.55f, 0.27f, 0.075f}},
    {string("Windows"), { 0.5f, 0.5f, 0.5f }},
};

// TODO: Add any global variables you need
glm::mat4 ModelTransform;
glm::mat4 ProjectionTransform;
glm::mat4 RotateModelTransform;
glm::mat4 MVP;
glm::mat3 currentColor;
glm::mat4 ModelMatrix;
glm::mat4 View;
glm::mat4 Global;
glm::mat4 Ident = { {1.0f, 0.0f, 0.0f, 0.0f},
					{0.0f, 1.0f, 0.0f, 0.0f},
					{0.0f, 0.0f, 1.0f, 0.0f},
					{0.0f, 0.0f, 0.0f, 1.0f} };

// User defined classes
Shader shaders;
Uniforms uniforms;

////////////////////////////////////////////////////
// Prototypes

// Prototypes: Rendering system management
void init();
void cleanup();

// Prototypes: Vertex buffer management
void createBuffers();
void destroyBuffers();
void loadTexture(const char* filename, int texUnit);

// Create and bind attributes and uniforms to shaders
void bindAttributesUniforms();

// Prototypes: Shader management
void createShaders();


// Prototypes: Rendering and window callbacks
void render();
void onResize(GLFWwindow*, int, int);
void onKey(GLFWwindow*, int, int, int, int);

////////////////////////////////////////////////////
// Functions

// Create context, window, and OGL buffers
void init()
{
    // Initialize GLFW
    if(!glfwInit()) 
    {
        cerr << "glfw failed to initialize" << endl;
        exit(EXIT_FAILURE);
    }
    
    // Open a OpenGL window with at least an OpenGL 4.1 Core Profile context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    window = glfwCreateWindow(1024,600, "Tessellation Demo", NULL, NULL);
    if(!window) 
    {
        cerr << "glfw failed to create window" << endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Make the window's context current 
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    // glew still uses a non-supported way of getting extensions in 
    // Core Profiles. Have to force it to load everything using 
    glewExperimental = GL_TRUE;
    GLenum glewStatus = glewInit();
    if(glewStatus != GLEW_OK)
    {
        cerr << "glew failed to initialize." << endl;
        cerr << glewGetErrorString(glewStatus) << endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Since glew uses an invalid approach, we have to clear the OpenGL error
    // stack. Otherwise, we'll detect an error even if there is not one.
    // Flush errors by calling glError (only GL_INVALID_ENUM is ignored)
    GLenum error = glGetError();
    if(error != GL_INVALID_ENUM)
    {
        // Something actually happned badly
        while(error != GL_NO_ERROR)
        {
            cerr << gluErrorString(error);
            error = glGetError();
        }
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    if(!GLEW_VERSION_4_1)
    {
        cerr << "glew failed to initialize OpenGL 4.1." << endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Initialize vertex buffers, the vertex array, and the shaders
	loadTexture("mini-diffuse.png", 0);
    createBuffers();
    createShaders();
	bindAttributesUniforms();

    // Setup callbacks
    glfwSetWindowSizeCallback(window, onResize);   
    glfwSetKeyCallback(window, onKey); 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	glDepthFunc(GL_LEQUAL);
}

// Clean up: Shut down window system
void cleanup()
{
    // Clean up OpenGL (vertex buffers, shaders, etc.)
	shaders.DeleteShaders();
    destroyBuffers();
    
    // Close window and terminate GLFW
    glfwTerminate();    
}

// TODO: Create, load, and bind your vertex & index buffers. Also
// setup any vertex attributes. Note that the geometry stores 8
// values per vertex (position x,y,z; normal x,y,z; texcoord u,v)
// as floats in that order. You only need vertices for this assignment.
// Your positions must be stored in vertex attribute 0.
void createBuffers()
{
	int part_pos = 0;
	mini = MiniGeometry();

	if (mini.numVertices() == 0)
	{
		cerr << "No vertices's, something went wrong loading mini!" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glGenVertexArrays(1, &arrayID);
	glBindVertexArray(arrayID);

	const GLfloat *data = mini.vertexdata();

	// Create vertex buffer to store vertices's on GPU
	glGenBuffers(1, &bufferID);
	// Inits the buffer as a vertex buffer (as opposed to a different type)
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	// Load data into buffer. Tell OpenGL we won't change it
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mini.numVertices() * 8, data, GL_STATIC_DRAW);

	// Vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, 0);
	glEnableVertexAttribArray(0);

	// Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);

	// Texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid*)(sizeof(GLfloat) * 6));
	glEnableVertexAttribArray(2);

	// Indices data pointer
	const GLushort *indices = mini.indices();
	// Send indeices to GPU and activate it
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * mini.numFaces() * sizeof(GLushort), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);

	delete[] indices;
	delete[] data;
}

// Load a texture
// Textures are loaded into different texture units, starting at unit 0
void loadTexture(const char* filename, int texUnit)
{
	// Activate the texture unit (starting at 0); need 1 per texture
	glActiveTexture(GL_TEXTURE0 + texUnit);
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Texture does not wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// How is the image sampled?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Changed to a different library because had some very strange issues with CImg
	FIBITMAP* bitmap = FreeImage_Load(
        FreeImage_GetFileType(filename, 0),
		filename);

	FIBITMAP *pImage = FreeImage_ConvertTo24Bits(bitmap);
	int nWidth = FreeImage_GetWidth(pImage);
	int nHeight = FreeImage_GetHeight(pImage);



	// Put the image into OpenGL
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// FreeImage reads in GL_BGR instead of GL_RGB. Also did not change the formate at which it is read in the shaders
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, nWidth, nHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));
	FreeImage_Unload(pImage);

	//delete[] data;
}

// TODO: After you are done w/ your buffers, you have to deallocate them
void destroyBuffers()
{
	// Make the vertex array inactive
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	// Unbind the buffer memory and delete it
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &indexBufferID);
	glDeleteBuffers(1, &bufferID);

	// Unbind the vertex array and delete it
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &arrayID);

	glDeleteBuffers(1, &indexBufferID);
}

// Load, compile, link and enable the shaders
void createShaders()
{
	shaders = Shader("project.vert", "project.frag", "project.tessctrl", "project.tesseval", "project.geom");
	cout << shaders;
}

void bindAttributesUniforms()
{
	// Set program ID in the unifroms class
	uniforms = Uniforms(shaders.programID);

	// Create projection matrix
	const float aspect = 1024.0f / 600.0f;
	const float near1 = 100.0f, far1 = 1500.0f;
	ProjectionTransform = { { 1 / (aspect * glm::tan(glm::radians(45.0f) / 2)), 0.0f, 0.0f, 0.0f },
	{ 0.0f, 1 / glm::tan(glm::radians(45.0f) / 2), 0.0f, 0.0f },
	{ 0.0f, 0.0f, -((far1 + near1) / (far1 - near1)), -1.0f },
	{ 0.0f, 0.0f, -((2 * far1 * near1) / (far1 - near1)), 1.0f } };

	// Set all transforms to the identity matrix because we ain't moving anything yet...
	ModelTransform = Ident;
	RotateModelTransform = Ident;
	Global = Ident;

	// SOME MATH! This is pointless but I'm just putting it here for clarification 
	ModelMatrix = ModelTransform * RotateModelTransform;

	// Calculate look at matrix
	glm::vec3 zaxis = glm::normalize(glm::vec3(0, -500, 500));
	glm::vec3 xaxis = glm::normalize(glm::cross(glm::vec3(0, 1, 0), zaxis));
	glm::vec3 yaxis = glm::cross(zaxis, xaxis);
	View = { { xaxis.x, yaxis.x, zaxis.x, 0 },
	{ xaxis.y, yaxis.y, zaxis.y, 0 },
	{ xaxis.z, yaxis.z, zaxis.z, 0 },
	{ -glm::dot(xaxis, glm::vec3(0, -500, 500)),
		-glm::dot(yaxis,glm::vec3(0, -500, 500)),
		-glm::dot(zaxis,glm::vec3(0, -500, 500)), 1 } };

	// Model to view point to projection matrix
	// Wait, the world revolves around me? Must be these lemons....
	MVP = ProjectionTransform * View * ModelMatrix;

	// Associate the vertex shader with the vertex attributes:
	// Attribute array 0 stores the vertices's
	uniforms.AddAttribute("position", 0);
	uniforms.AddAttribute("normals", 1);
	uniforms.AddAttribute("texcoord", 2);

	// Find M, V, P uniforms for lighting
	uniforms.AddUniform("M", GL_FLOAT_MAT4, &ModelMatrix[0][0]);
	uniforms.AddUniform("V", GL_FLOAT_MAT4, &View[0][0]);
	uniforms.AddUniform("P", GL_FLOAT_MAT4, &ProjectionTransform[0][0]);

	// Find the MVP uniform and set it 
	uniforms.AddUniform("MVP", GL_FLOAT_MAT4, &MVP[0][0]);

	// Find the color uniform and set it
	uniforms.AddUniform("color", GL_FLOAT_MAT3, &currentColor[0][0]);
	uniforms.AddUniform("miniTexture", 0);
	uniforms.AddUniform("drawDefault", GL_FALSE);

	// Light uniforms
	glm::vec3 lightPosition(0.0f, -500.0f, 200.0f);
	uniforms.AddUniform("lightPos", GL_FLOAT_VEC3, &lightPosition[0]);
	uniforms.AddUniform("lightColor", GL_FLOAT_VEC3, &glm::vec3(1.0f, 1.0f, 1.0f)[0]);
	uniforms.AddUniform("ambietStrength", 0.1f);
	uniforms.AddUniform("specularStrength", 2.0f);

	// Set default tessilation level and bind it
	tesslevelInner = 1.0f,	tesslevelOuter = 1.0f;
	uniforms.AddUniform("TessLevelInner", 1.0f);
	uniforms.AddUniform("TessLevelOuter", 1.0f);
}


// Render OpenGL
// We just clear the screen and draw the line
void render()
{
	// Set background color to white
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Iterate through each part and send the corresponding color to the GPU
	// to then render that corresponding part.
	list<PartEntry> groups = mini.groups();
	list<PartEntry>::const_iterator it;
	int i = 0;
	for (it = groups.begin(); it != groups.end(); ++it)
	{
		int offset = sizeof(GLushort) * mini.indicesPerFace() * it->start;
		int count = mini.indicesPerFace() * (it->end - it->start);

		// Had a strange issues where my GPU would not work with array uniforms so i just build a 4x4 and set
		// them correctly in the vertex shader.
		currentColor[0][0] = colormap[i].color[0];
		currentColor[1][1] = colormap[i].color[1];
		currentColor[2][2] = colormap[i].color[2];
		if ((it->name == "Front Passenger Rim") || (it->name == "Front Driver Rim") || (it->name == "Rear Driver Rim") || (it->name == "Rear Passenger Rim") ||
			(it->name == "Front Driver Tire") || (it->name == "Front Passenger Tire") || (it->name == "Rear Driver Tire") || (it->name == "Rear Passenger Tire") ||
			(it->name == "Brakes") || (it->name == "Windows"))
		{
			// For drawing the parts that don't have textures
			uniforms.UpdateUniform("drawDefault", GL_TRUE);
			// Update uniform
			uniforms.UpdateUniform("color", &currentColor[0][0]);
			// Draw current part
		}
		else
			uniforms.UpdateUniform("drawDefault", GL_FALSE);

		glDrawRangeElements(GL_PATCHES, it->start, it->end, count, GL_UNSIGNED_SHORT, (void*)offset);
		i++;
	}

	if (isWireframe == GL_TRUE) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		for (it = groups.begin(); it != groups.end(); ++it)
		{
			int offset = sizeof(GLushort) * mini.indicesPerFace() * it->start;
			int count = mini.indicesPerFace() * (it->end - it->start);
			currentColor[0][0] = 0.0f;
			currentColor[1][1] = 0.0f;
			currentColor[2][2] = 0.0f;
			// For drawing the parts that don't have textures
			// Update uniform
			uniforms.UpdateUniform("color", &currentColor[0][0]);
			glDrawRangeElements(GL_PATCHES, it->start, it->end, count, GL_UNSIGNED_SHORT, (void*)offset);
		}
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

    // Swap front and back rendering buffers
    glfwSwapBuffers(window);
	glfwPollEvents();
}

// Window resized. Tell OpenGL to map a all of the window space
void onResize(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, w, h);

    // Recreate the projection matrix based off window height and width
	const float aspect = (float)w / (float)h;
	const float near1 = 0.1f, far1 = 1500.0f;
	ProjectionTransform = { {1/(aspect * glm::tan(glm::radians(45.0f)/2)), 0.0f, 0.0f, 0.0f},
							{0.0f, 1 / glm::tan(glm::radians(45.0f) / 2), 0.0f, 0.0f },
							{0.0f, 0.0f, -((far1 + near1) / (far1 - near1)), -1.0f},
							{0.0f, 0.0f, -((2 * far1 * near1) / (far1 - near1)), 1.0f} };

	// Make the lemons
	MVP = ProjectionTransform * View * ModelMatrix;
	// Update MVP uniform
	uniforms.UpdateUniform("MVP", &MVP[0][0]);

    // Always redraw
    render();
}

// Key pressed
void onKey(GLFWwindow* window, int keycode, int scancode, int keystate, int modifiers)
{
    // Only worry when released
    if(keystate == GLFW_PRESS)
        return;

	switch (keycode)
	{
		case GLFW_KEY_ESCAPE:
		{
			cleanup();
			exit(EXIT_SUCCESS);
			break;
		}
		case GLFW_KEY_LEFT:
		{
			glm::mat4 count = { {glm::cos(0.2f), glm::sin(0.2f), 0.0f, 0.0f},
								{-glm::sin(0.2f), glm::cos(0.2f), 0.0f, 0.0f},
								{0.0f, 0.0f, 1.0f, 0.0f},
								{0.0f, 0.0f, 0.0f, 1.0f} };

			count = RotateModelTransform * count;
			ModelMatrix = ModelMatrix * RotateModelTransform * count;
			MVP = ProjectionTransform * View * ModelMatrix;

			uniforms.UpdateUniform("MVP", &MVP[0][0]);
			uniforms.UpdateUniform("M", &ModelMatrix[0][0]);
			break;
		}
		case GLFW_KEY_RIGHT:
		{
			glm::mat4 count = { { glm::cos(-0.2f), glm::sin(-0.2f), 0.0f, 0.0f },
								{ -glm::sin(-0.2f), glm::cos(-0.2f), 0.0f, 0.0f },
								{ 0.0f, 0.0f, 1.0f, 0.0f },
								{ 0.0f, 0.0f, 0.0f, 1.0f } };

			count = RotateModelTransform * count;
			ModelMatrix = ModelMatrix * RotateModelTransform * count;
			MVP = ProjectionTransform * View * ModelMatrix;

			uniforms.UpdateUniform("MVP", &MVP[0][0]);
			uniforms.UpdateUniform("M", &ModelMatrix[0][0]);
			break;
		}
		case GLFW_KEY_UP:
		{
			glm::mat4 count = Ident;
			count[3][1] = -5.0f;

			count = ModelTransform * count;
			ModelMatrix = ModelMatrix * ModelTransform * count;
			MVP = ProjectionTransform * View * ModelMatrix;

			uniforms.UpdateUniform("MVP", &MVP[0][0]);
			uniforms.UpdateUniform("M", &ModelMatrix[0][0]);
			break;
		}
		case GLFW_KEY_DOWN:
		{
			glm::mat4 count = Ident;
			count[3][1] = 5.0f;

			count = ModelTransform * count;
			ModelMatrix = ModelMatrix * ModelTransform * count;
			MVP = ProjectionTransform * View * ModelMatrix;

			uniforms.UpdateUniform("MVP", &MVP[0][0]);
			uniforms.UpdateUniform("M", &ModelMatrix[0][0]);
			break;
		}
		case GLFW_KEY_H:
		{
			ProjectionTransform = glm::perspective(glm::radians(45.0f), 1024.0f / 600.0f, 0.1f, 1500.0f);

			ModelTransform = Ident;
			RotateModelTransform = Ident;

			ModelMatrix = ModelTransform * RotateModelTransform;

			View = glm::lookAt(
				glm::vec3(0, -500, 500), 
				glm::vec3(0, 0, 0), 
				glm::vec3(0, 1, 0) 
			);

			MVP = ProjectionTransform * View * ModelMatrix;

			uniforms.UpdateUniform("MVP", &MVP[0][0]);
			uniforms.UpdateUniform("M", &ModelMatrix[0][0]);
			break;
		}
		case GLFW_KEY_EQUAL:
		{
			tesslevelInner += 1.0f;
			uniforms.AddUniform("TessLevelInner", tesslevelInner);
			break;
		}
		case GLFW_KEY_MINUS:
		{
			if (tesslevelInner > 1.0f)
				tesslevelInner -= 1.0f;
			uniforms.AddUniform("TessLevelInner", tesslevelInner);
			break;
		}

		case GLFW_KEY_RIGHT_BRACKET:
		{
			tesslevelOuter += 1.0f;
			uniforms.AddUniform("TessLevelOuter", tesslevelOuter);
			break;
		}
		case GLFW_KEY_LEFT_BRACKET:
		{
			if (tesslevelOuter > 1.0f)
				tesslevelOuter -= 1.0f;
			uniforms.AddUniform("TessLevelOuter", tesslevelOuter);
			break;
		}

		case GLFW_KEY_W:
		{
			isWireframe = !isWireframe;
			break;
		}
	}
    // Always redraw
    render();
}

// Entry point
int main(int argc, char** argv) 
{
    // Initialize OpenGL and create our window
    init();
    
    // Main loop
    int running = GL_TRUE; 
	double lastTime = glfwGetTime();
	int nbFrames = 1;
    while(running) 
    {
        // Render the scene 
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0)
		{
			cout << "ms/frames " << 1000.0 / double(nbFrames) << "\r";
			nbFrames = 0;
			lastTime += 1.0;
		}
        render();

        // Check if window was closed
        glfwPollEvents();  
        running = !glfwGetKey(window, GLFW_KEY_ESCAPE) &&
                  !glfwWindowShouldClose(window);
    }

    // Exit program
    cleanup();
    return EXIT_SUCCESS;
}