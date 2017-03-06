#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <random>  
#include <cstdlib>
#include <string>
#include <chrono>
#include <thread>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

int randmoNumber (int intervalMin, int intervalMax);
void wait (unsigned int millisecondsTime);

int main (int argc, char * argv[])
{
	const int WIDTH = 700, HEIGHT = 400, DEPTH_OF_COLOR = 32, MAX_FPS = 60, PADDLE_WIDTH = 20, PADDLE_HEIGHT = 90;
	int ballSpeedX = 3,	ballSpeedY = -1;
	const std::string SEPARATION = "|";
	int scorePlayerFirst = 0, scorePlayerSecond = 0;
	std::string scorePlayerFirstConversion = std::to_string (scorePlayerFirst);
	std::string scorePlayerSecondConversion = std::to_string (scorePlayerSecond);
	std::string scoreGame = scorePlayerFirstConversion + SEPARATION + scorePlayerSecondConversion;
	float ballRadius = 10.f;
	enum MoveBall { up, down, right, left };
	sf::Vector2f paddleSize (PADDLE_WIDTH, PADDLE_HEIGHT);

	//Load font
	sf::Font mainFont;
	if (!mainFont.loadFromFile ("resources/fontComic.ttf"))
		return EXIT_FAILURE;
	
	//Load music
	sf::Music mainMusic;
	if (!mainMusic.openFromFile ("resources/soundtrack.wav"))
		return EXIT_FAILURE;
	mainMusic.setLoop (true);
	mainMusic.setVolume (1);
	//mainMusic.play ();

	// Create the left paddle
	sf::RectangleShape leftPaddle;
	leftPaddle.setSize (paddleSize - sf::Vector2f (3, 3));
	leftPaddle.setOutlineThickness (3);
	leftPaddle.setOutlineColor (sf::Color::White);
	leftPaddle.setFillColor (sf::Color::Black);
	leftPaddle.setOrigin (paddleSize / 2.f);
	leftPaddle.setPosition (20, 200);

	// Create the right paddle
	sf::RectangleShape rightPaddle;
	rightPaddle.setSize (paddleSize - sf::Vector2f (3, 3));
	rightPaddle.setOutlineThickness (3);
	rightPaddle.setOutlineColor (sf::Color::White);
	rightPaddle.setFillColor (sf::Color::Black);
	rightPaddle.setOrigin (paddleSize / 2.f);
	rightPaddle.setPosition (680, 200);

	// Create the ball
	sf::CircleShape ball;
	ball.setRadius (ballRadius - 3);
	ball.setOutlineThickness (3);
	ball.setOutlineColor (sf::Color::White);
	ball.setFillColor (sf::Color::Black);
	ball.setOrigin (ballRadius / 2, ballRadius / 2);
	ball.setPosition (350, 200);
	
	//TODO: Score 
	sf::Text scoreText ("st", mainFont);
	scoreText.setCharacterSize (30);
	scoreText.setStyle (sf::Text::Bold);
	scoreText.setPosition (325, 10);

	//Main settings of windows
	sf::RenderWindow mainWindow (sf::VideoMode (WIDTH, HEIGHT, DEPTH_OF_COLOR), "PING-PONG", sf::Style::Titlebar | sf::Style::Close);
	mainWindow.setFramerateLimit (MAX_FPS);

	//Main loop game
	while (mainWindow.isOpen ())
	{
		sf::Event eventCloseMainWindow;
		while (mainWindow.pollEvent (eventCloseMainWindow))
		{
			if (eventCloseMainWindow.type == sf::Event::Closed)
				mainWindow.close ();
			
		}

		//Limit move right paddle
		if (rightPaddle.getPosition ().y < 0)
		{
			rightPaddle.setPosition (680, 0);
		}
		else if (rightPaddle.getPosition ().y > 400)
		{
			rightPaddle.setPosition (680, 400);
		}
		//Limit move left paddle
		if (leftPaddle.getPosition ().y < 0)
		{
			leftPaddle.setPosition (20, 0);
		}
		else if (leftPaddle.getPosition ().y > 400)
		{
			leftPaddle.setPosition (20, 400);
		}

		//Move left paddles
		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Up))
		{
			leftPaddle.move (0, -3);
		}
		else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Down))
		{
			leftPaddle.move (0, 3);
		}
		//Move right paddles
		if (sf::Keyboard::isKeyPressed (sf::Keyboard::W))
		{
			rightPaddle.move (0, -3);
		}
		else if (sf::Keyboard::isKeyPressed (sf::Keyboard::S))
		{
			rightPaddle.move (0, 3);
		}
		
		//Colision bounds
		sf::FloatRect leftPaddleCollision = leftPaddle.getGlobalBounds ();
		sf::FloatRect rightPaddleCollision = rightPaddle.getGlobalBounds ();
		sf::FloatRect ballCollision = ball.getGlobalBounds ();
		
		//TODO: Collision action
		ball.move (ballSpeedX, ballSpeedY);
		if (ballCollision.intersects (rightPaddleCollision))
		{
			ballSpeedX = -3;
			ball.move (ballSpeedX, ballSpeedY);
		}
		else if (ballCollision.intersects (leftPaddleCollision))
		{
			ballSpeedX = 3;
			ball.move (ballSpeedX, ballSpeedY);
		}
		else if (ball.getPosition ().x > WIDTH)
		{
			scorePlayerFirst++;
			ball.setPosition (350, 200);
			rightPaddle.setPosition (680, 200);
			leftPaddle.setPosition (20, 200);
			wait (500);
		}
		else if (ball.getPosition ().x < 0)
		{
			scorePlayerSecond++;
			ball.setPosition (350, 200);
			rightPaddle.setPosition (680, 200);
			leftPaddle.setPosition (20, 200);
			wait (500);
		}
		else if (ball.getPosition ().y <= 0)
		{
			ballSpeedY = 2;
			ball.move (ballSpeedX, ballSpeedY);
		}
		else
		{ 
			ballSpeedY = -2;
			ball.move (ballSpeedX, ballSpeedY);
		}
		
		mainWindow.clear (sf::Color::Black);
		mainWindow.draw (scoreText);
		mainWindow.draw (leftPaddle);
		mainWindow.draw (rightPaddle);
		mainWindow.draw (ball);
		mainWindow.display ();
	}

	return 0;
}
void wait (unsigned int millisecondsTime)
{
	std::this_thread::sleep_for (std::chrono::milliseconds (millisecondsTime));
}

int randmoNumber (int intervalMin, int intervalMax)
{
	std::random_device random;
	std::mt19937 gen (random ());
	std::uniform_int_distribution<> randomNumber (intervalMin, intervalMax);
	return randomNumber (gen);
}