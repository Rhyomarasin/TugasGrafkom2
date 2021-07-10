 #include "Demo.h"



Demo::Demo() {

}


Demo::~Demo() {
}



void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("Cube.vert", "Cube.frag", nullptr);

//	BuildColoredRefrigerator();

//	BuildColoredPaint();

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		//Lukisan
	// front
		-3.8,  1,  0.25, 0, 0,  // 0 <- 4
		-0.7,  1, 0.25, 1, 0,   // 1 <- 5
		-0.7, 3, 0.25, 1, 1,   // 2 <- 1
		-3.8, 3, 0.25, 0, 1,   // 3 <- 0

		// right
		-0.7, 3, 0.25,  0, 0,  // 4 = 2
		-0.7,  3, 0.05, 1, 0,  // 5 = 10
		-0.7, 1, 0.05, 1, 1,  // 6 = 9
		-0.7,  1, 0.25, 0, 1,  // 7 = 1

		// back
		-3.8, 1,  0.05, 0, 0,  // 8 <- 7
		-0.7, 1, 0.05, 1, 0, // 9 <- 6
		-0.7,  3, 0.05, 1, 1, // 10 <- 2
		-3.8,  3, 0.05, 0, 1,  // 11 <- 3

		// left
		-3.8, 1,  0.05, 0, 0, // 12 = 8
		-3.8, 1,  0.25, 1, 0, // 13 = 0
		-3.8, 3, 0.25, 1, 1, // 14 = 3
		-3.8, 3, 0.05, 0, 1, // 15 = 11

		// upper
		-0.7, 3, 0.25, 0, 0,   // 16 = 2
		-3.8, 3, 0.25, 1, 0,  // 17 = 3
		-3.8,  3, 0.05,  1, 1,  // 18 = 11
		-0.7,  3, 0.05, 0, 1,   // 19 = 10

		// bottom
		-3.8, 1,  0.05, 0, 0, // 20 = 8
		-0.7, 1, 0.05, 1, 0,  // 21 = 9
		-0.7,  1, 0.25, 1, 1,  // 22 = 1
		-3.8,  1,  0.25, 0, 1, // 23 = 0


	};
	unsigned int indices[] = {  // note that we start from 0!
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22,   // bottom
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define color pointer layout 1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Demo::Update(double deltaTime) {

}

void Demo::Render() {
	// render
	// ------
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_CULL_FACE);

	// draw our first triangle
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(0, 1, 2), glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	// glBindVertexArray(0); // no need to unbind it every time
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start("Primitive: Draw a Cube using Triangles", 800, 600, false, true);
} 