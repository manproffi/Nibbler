#include "Glfw3Wrapper.h"
#include "../../Snake/Snake.h"
#include <cmath>
#include <string>

/*
 *	A method that renders a game over screen
 */

int Glfw3Wrapper::_key = Directions::NOTHING_PRESSED;
int         	lastDirection = Directions::NOTHING_PRESSED;

void key_callback(GLFWwindow* , int key, int , int action, int )
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		Glfw3Wrapper::_key = Directions::QUIT;
		return ;
	}
	else if (action == GLFW_PRESS && key == GLFW_KEY_SPACE)
	{
		Glfw3Wrapper::_key = Directions::PAUSE;
		return ;
	}
	else if (action == GLFW_PRESS && key == GLFW_KEY_UP)
	{
		Glfw3Wrapper::_key = Directions::UP;
		return ;
	}
	else if (action == GLFW_PRESS && key == GLFW_KEY_DOWN)
	{
		Glfw3Wrapper::_key = Directions::DOWN;
		return ;
	}
	else if (action == GLFW_PRESS && key == GLFW_KEY_LEFT)
	{
		Glfw3Wrapper::_key = Directions::LEFT;
		return ;
	}
	else if (action == GLFW_PRESS && key == GLFW_KEY_RIGHT)
	{
		Glfw3Wrapper::_key = Directions::RIGHT;
		return ;
	}
	else if (action == GLFW_PRESS && key == GLFW_KEY_KP_1)
	{
		Glfw3Wrapper::_key = Directions::SDL_LIB;
		return ;
	}
	else if (action == GLFW_PRESS && key == GLFW_KEY_KP_2)
	{
		Glfw3Wrapper::_key = Directions::SFML_LIB;
		return ;
	}
}

void	Glfw3Wrapper::RenderGameOverScreen(void)
{
	font->FaceSize(width / 8);

	std::string str_data = "GAME OVER";
	glRasterPos2f(50, height / 4);
	font->Render(str_data.c_str());
}

/*
 *	A function that renders a side menu in a simple manner
 */

void	Glfw3Wrapper::RenderSideMenu(int w, int , size_t score, float time_left, std::vector<int> score_data)
{
	font->FaceSize(50);

	std::string str_data = "Score " + std::to_string(score);
	glColor3f(1.0, 1.0, 1.0);

	glRasterPos2f(w + 10, 50);
	font->Render(str_data.c_str());

	str_data = "Time left: " + std::to_string(time_left);

	glRasterPos2f(w + 10, 100);
	font->Render(str_data.c_str());

	str_data = "BEST SCORES:";

	glRasterPos2f(w + 10, 150);
	font->Render(str_data.c_str());


	int k = 200;
	int step = 50;

	for (size_t i = 0; i < score_data.size(); i++)
	{
		str_data = std::to_string(i + 1) + ": " + std::to_string(score_data[i]);

		glRasterPos2f(w + 10, k);
		font->Render(str_data.c_str());
		k += step;
	}
}

/*
 *	Method that clears an image
 */

void	Glfw3Wrapper::ClearImage(void)
{
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glClear( GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
}

/*
 *	Method that puts image to the window
 */

void	Glfw3Wrapper::RenderImage(void)
{
	glfwSwapBuffers(window);
}

/*
 *	Method that renders a Snake
 */

void 			Glfw3Wrapper::RenderSnake(const std::vector<std::pair<int, int>> &snake_parts, int)
{
	int x;
	int y;

	for (size_t i = 1; i < snake_parts.size(); i++)
	{
		x = snake_parts[i].second * 32;
		y = snake_parts[i].first * 32;
		glColor3f(0.18, 0.54, 0.32);
		glRecti(x, y, x + 32, y + 32);
	}
	glColor3f(0.26, 0.91, 0.52);
	x = snake_parts[0].second * 32;
	y = snake_parts[0].first * 32;
	glRecti(x, y, x + 32, y + 32);
}


/*
 *	Key hook handler for user input
 *	Movement of the snake UP, DOWN, LEFT, RIGHT and ESC
 */

int 			Glfw3Wrapper::HandleInput(void)
{
	glfwPollEvents();
	
	int temp = Glfw3Wrapper::_key;
	
	Glfw3Wrapper::_key = Directions::NOTHING_PRESSED;
	
	return (temp);
}


/*
 *	A method to render a map
 */

void 		 Glfw3Wrapper::RenderMap(const std::vector<std::vector<int>> &game_map)
{
	int x;
	int y;

	x = 0;
	y = 0;
	glRecti(x, y, x + 32, y + 32);
	for (size_t i = 0; i < game_map.size(); i++)
	{
		for (size_t j = 0; j < game_map[i].size(); j++)
		{
			x = j * 32;
			y = i * 32;
			if (game_map[i][j])
				glColor3f(0.73, 0.68, 0.49);
			else
				glColor3f(0.96, 0.9, 0.63);
			glRecti(x, y, x + 32, y + 32);
		}
	}
}

/*
 *	Default constructor for the SDL wrap
 */

Glfw3Wrapper::Glfw3Wrapper(int w, int h)
{
	width = w * 32 + SIDE_MENU_WIDTH;
	height = h * 32;

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(width, height, "Nibbler OpenGl", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwMakeContextCurrent(window);

	/*
	 *	Initialize callback function for the input handler
	 */
	glfwSetKeyCallback(window, key_callback);

	glOrtho(0, width, height, 0, 0, 1);

	font = nullptr;

	std::ifstream checker;

	checker.open("libs/glfw3/10.ttf");

	if (!checker.is_open())
	{
		std::cout << "COULD NOT OPEN THE FONT\n";
		exit(1);
	}

	font = new FTGLPixmapFont("libs/glfw3/10.ttf");

	if (!font)
    {
        std::cout << "COULD NOT OPEN THE FONT\n";
        exit(1);
    }

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	checker.close();
}

/*
 *	Method that renders a fruit
 */

void	Glfw3Wrapper::RenderFood(int i_pos, int j_pos, bool isBonusFruit)
{
	int x;
	int y;

	x = j_pos * 32;
	y = i_pos * 32;
	if (isBonusFruit) {
		glColor3f(0.38, 0.21, 0.56);
	}
	else {
		glColor3f(1.0, 0.47, 0.47);
	}
	glRecti(x, y, x + 32, y + 32);
}

/*
 *	Copy constructor
 */

Glfw3Wrapper::Glfw3Wrapper(const Glfw3Wrapper &glfw3)
{
    window = glfw3.window;
	width = glfw3.width;
	height = glfw3.height;
	font = glfw3.font;
}

/*
 *	Copy assignment operator
 */

Glfw3Wrapper 	&Glfw3Wrapper::operator=(const Glfw3Wrapper &glfw3)
{
    window = glfw3.window;
	width = glfw3.width;
	height = glfw3.height;
	font = glfw3.font;
	return (*this);
}

/*
 *	Destructor
 */

Glfw3Wrapper::~Glfw3Wrapper()
{
	delete (font);
    glfwDestroyWindow(window);
    glfwTerminate();
}

extern "C" InterfaceLibrary      *createWrapper(int w, int h)
{
	return (new Glfw3Wrapper(w, h));
}
extern "C" void                deleteWrapper(InterfaceLibrary *lib)
{
	delete lib;
}
