#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <string>
#include <windows.h>
#include <cstdio>

struct Color {
	int r, g, b;
} myColor[7] = {{51, 135, 148}, //LightBlue
				{51, 148, 74}, //Green
				{148, 51, 51}, //Red
				{126, 51, 148}, //Violet
				{148, 109, 51}, //Orange
				{51, 96 , 148}, //Blue
				{48, 137, 51}}; //Yellow
const int sqrSize = 25;
const int Height = 20, Width = 10;
int bottom[Width][Height] = {0};
int shape[7][4] = {
	1, 3, 5, 7,	
	2, 4, 5, 7,			//  0 | 1
	3, 4, 5, 6,			//  2 | 3
	3, 5, 4, 7,			//  4 | 5
	2, 3, 5, 7,			//  6 | 7
	3, 5, 7, 6,
	2, 3, 4, 5,
};

struct Point {
	int x, y;
} a[4] = { {-1, -1} , {-1, -1} , {-1, -1} , {-1, -1} },
	b[4] = { {-1, -1} , {-1, -1} , {-1, -1} , {-1, -1} };

void PlayGame(sf::RenderWindow &window) {
	sf::RectangleShape sqr(sf::Vector2f(sqrSize, sqrSize));
	sqr.setOutlineThickness(-1.f);
	sqr.setOutlineColor(sf::Color::Black);
	sf::Font font;
	font.loadFromFile("Resource/Font/arial.ttf");
	sf::Text text;
	text.setFont(font);
	text.setFillColor(sf::Color::White);

	bool rotate = false;
	for (int i = 0; i <= Height; i++)
		for (int j = 0; j <= Width; j++)
			bottom[i][j] = 0;
	int newN = rand() % 7;
	for (int i = 0; i < 4; i++) {
		b[i].x = shape[newN][i] % 2 + 12;
		b[i].y = shape[newN][i] / 2 + 6;
	}
	int n = newN;
	bool endGame = false;
	int lines = 0;

	sf::Clock clock;
	float timer = 0;
	float delayTime = 0.2f;

	while (window.isOpen())
	{

		if (endGame) break;
		int dx = 0;
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (event.type == sf::Event::KeyPressed)
				if (event.key.code == sf::Keyboard::Up) rotate = true;
				else if (event.key.code == sf::Keyboard::Right) dx = 1;
				else if (event.key.code == sf::Keyboard::Left) dx = -1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) delayTime = 0.05f; else delayTime = 0.2f;
		// Moving
		bool moveAble = true;
		for (int i = 0; i < 4; i++) {
			if ((a[i].x < 2 && dx == -1) || (a[i].x >= Width && dx == 1) ||
				(bottom[a[i].y][a[i].x - 1] && dx == -1) ||
				(bottom[a[i].y][a[i].x + 1] && dx == 1)) moveAble = false;
		}

		if (moveAble)
			for (int i = 0; i < 4; i++)
				a[i].x += dx;

		if (rotate)
			for (int i = 0; i < 4; i++) {
				Point p = { a[1].y - a[i].y , a[1].x - a[i].x };
				a[i].x = a[1].x + p.x;
				a[i].y = a[1].y - p.y;
			}
		rotate = false;

		// New block
		if (timer >= delayTime) {
			for (int i = 0; i < 4; i++) {
				a[i].y += 1;
				timer = 0;
			}

			for (int j = 0; j < 4; j++) {
				if ((a[j].y <= 0) || (a[j].y > Height - 1) || (bottom[a[j].y + 1][a[j].x])) {
					for (int i = 0; i < 4; i++) bottom[a[i].y][a[i].x] = n + 1;
					n = newN;
					for (int i = 0; i < 4; i++) {
						a[i].x = shape[n][i] % 2 + 5;
						a[i].y = shape[n][i] / 2;
					}
					newN = rand() % 7;
					for (int i = 0; i < 4; i++) {
						b[i].x = shape[newN][i] % 2 + 13;
						b[i].y = shape[newN][i] / 2 + 6;
					}

					break;
				}
			}
		}
		//Check done lines
		int k = Height;
		for (int i = Height; i > 0; i--) {
			int count = 0;
			for (int j = 1; j <= Width; j++) {
				if (bottom[i][j]) count++;
				bottom[k][j] = bottom[i][j];
				if (bottom[i][j] && i <= 2) endGame = true;
			}
			if (count < Width) k--; else lines++;
		}
		//Drawing grid
		window.clear(sf::Color::Black);
		for (int i = 1; i <= Width + 1; i++) {
			for (int j = 1; j <= Height + 1; j++) {
				sf::Vertex line[] =
				{
					sf::Vertex(sf::Vector2f(i * sqrSize, sqrSize), sf::Color::Color(50, 50, 50)),
					sf::Vertex(sf::Vector2f(i * sqrSize, (Height + 1) * sqrSize), sf::Color::Color(50, 50, 50))
				};
				window.draw(line, 2, sf::Lines);

				sf::Vertex line2[] =
				{
					sf::Vertex(sf::Vector2f(sqrSize, j * sqrSize), sf::Color::Color(50, 50, 50)),
					sf::Vertex(sf::Vector2f((Width + 1) * sqrSize , j * sqrSize), sf::Color::Color(50, 50, 50))
				};
				window.draw(line2, 2, sf::Lines);

			}
		}
		//Drawing next block
		for (int i = 0; i < 4; i++)
		{
			sqr.setPosition(b[i].x * sqrSize, b[i].y * sqrSize);
			sqr.setFillColor(sf::Color::Color(myColor[newN].r, myColor[newN].g, myColor[newN].b));
			window.draw(sqr);
		}

		text.setCharacterSize(sqrSize - 5);
		text.setString("NEXT");
		text.setPosition((Width + 3) * sqrSize, 4 * sqrSize);
		window.draw(text);

		text.setCharacterSize(sqrSize - 5);
		text.setString(" LINES");
		text.setPosition((Width + 3) * sqrSize, (Height - 5) * sqrSize);
		window.draw(text);

		
		text.setCharacterSize(sqrSize);
		text.setString(std::to_string(lines));
		text.setPosition((Width + 4) * sqrSize, (Height - 3.5) * sqrSize);
		window.draw(text);

		for (int i = 0; i < 4; i++)
			if (a[i].x > 0 && a[i].x <= Width && a[i].y > 0)
			{
				sqr.setPosition(a[i].x * sqrSize, a[i].y * sqrSize);
				sqr.setFillColor(sf::Color::Color(myColor[n].r, myColor[n].g, myColor[n].b));
				window.draw(sqr);
			}

		for (int i = 0; i <= Height; i++) {
			for (int j = 0; j <= Width; j++) {
				if (bottom[i][j] != 0) {
					sqr.setPosition(j * sqrSize, i * sqrSize);
					sqr.setFillColor(sf::Color::Color(myColor[bottom[i][j] - 1].r, myColor[bottom[i][j] - 1].g, myColor[bottom[i][j] - 1].b));
					window.draw(sqr);
				}
			}
		}

		window.display();
	}
	sf::Texture backgroundTexture;
	backgroundTexture.loadFromFile("Resource/Image/Gameover.png");

	sf::Sprite background;
	background.setTexture(backgroundTexture);
	text.setString("Your score : " + std::to_string(lines));
	text.setPosition(4 * sqrSize, Height / 4 * 3 * sqrSize);
	text.setCharacterSize(40);
	text.setFillColor(sf::Color::Color(130, 130, 130));
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{

			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		window.clear();
		window.draw(background);
		window.draw(text);
		window.display();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) return;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::Mouse::getPosition(window).x >= 0 && sf::Mouse::getPosition(window).x <= 380 && sf::Mouse::getPosition(window).y >= 0 && sf::Mouse::getPosition(window).y <= 550) return;
	}
}

void MenuGame(sf::RenderWindow &window) {
	sf::Texture menuBG, playBtn, exitBtn;
	menuBG.loadFromFile("Resource/Image/Menu.png");
	playBtn.loadFromFile("Resource/Image/Playbtn.png");
	exitBtn.loadFromFile("Resource/Image/Exitbtn.png");
	sf::Sprite MenuBG(menuBG), PlayBtn(playBtn), ExitBtn(exitBtn);
	MenuBG.setPosition(0, 0);
	PlayBtn.setPosition(165, 240);
	ExitBtn.setPosition(165, 340);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		window.clear();
		window.draw(MenuBG);
		window.draw(PlayBtn);
		window.draw(ExitBtn);
		window.display();

		if (sf::Mouse::getPosition(window).x >= 165 &&
			sf::Mouse::getPosition(window).x <= 165 + 123 &&
			sf::Mouse::getPosition(window).y >= 240 &&
			sf::Mouse::getPosition(window).y <= 240 + 68) {
			PlayBtn.setColor(sf::Color::Color(80, 80, 80));
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				window.clear();
				PlayGame(window);
			}
		}
		else PlayBtn.setColor(sf::Color::Color(195, 195, 195));

		if (sf::Mouse::getPosition(window).x >= 165 &&
			sf::Mouse::getPosition(window).x <= 165 + 123 &&
			sf::Mouse::getPosition(window).y >= 340 &&
			sf::Mouse::getPosition(window).y <= 340 + 68) {
			ExitBtn.setColor(sf::Color::Color(80, 80, 80));
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) window.close();
		}
		else ExitBtn.setColor(sf::Color::Color(195, 195, 195));
	}

}

void GameIntro(sf::RenderWindow &window) {
	sf::Texture introTexture;
	introTexture.loadFromFile("Resource/Image/Begin.png");
	sf::Sprite introImg;
	introImg.setTexture(introTexture);
	introImg.setPosition(0, 0);
	sf::Clock clock;
	float timer = 0.f;
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		window.clear();
		window.draw(introImg);
		window.display();
		if (timer > 2.f)
		{
			window.clear();
			MenuGame(window);
		}
	}
}


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char*, int nShowCmd)
{
	srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(sqrSize * 18,  sqrSize * 22), "Tetris", sf::Style::Close);
	sf::Image icon;
	icon.loadFromFile("Resource/Image/icon.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	GameIntro(window);
    return 0;
}