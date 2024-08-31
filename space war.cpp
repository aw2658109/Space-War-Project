#include<iostream>
#include<SFML/Audio.hpp>
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/System.hpp>
#include<math.h>
#include<cstdlib>
#include<vector>

using namespace sf;
using namespace std;

class Bullet {
public:
	Sprite shape;
	Bullet(Texture* texture, Vector2f pos)
	{
		this->shape.setTexture(*texture);
		this->shape.setScale(0.07f, 0.07f);
		this->shape.setPosition(pos);
	}
	~Bullet()
	{

	}
};

class Player {
public:
	Sprite shape;
	Texture* texture;
	int HP;
	int HPMax;
	std::vector <Bullet> bullets;

	Player(Texture* texture)
	{
		this->HPMax = 5;
		this->HP = this->HPMax;
		this->texture = texture;
		this->shape.setTexture(*texture);
		this->shape.setScale(0.1f, 0.1f);
	}

	void shootBullet(Texture& bulletObj) {
		this->bullets.push_back(Bullet(&bulletObj, this->shape.getPosition()));
	}
};

class Enemy {

public:

	int shootTimer; // Timer for enemy bullet shooting
	int shootInterval; // Interval between shots
	Sprite shape;
	int HP;
	int HPMax;
	Vector2f veloctiy;
	std::vector <Bullet> bullets;

	Enemy(Texture* texture, float pos_x, float pos_y, float vel_x, float vel_y, int HPMax, int shootInterval) : shootTimer(0)
	{
		this->HPMax = HPMax;
		this->HP = this->HPMax;
		this->shape.setTexture(*texture);
		this->shape.setScale(0.1f, 0.1f);
		this->shape.setPosition(pos_x, pos_y);
		this->veloctiy = Vector2f(vel_x, vel_y);
		this->shootInterval = shootInterval;
	}

	Enemy(Texture* texture, Vector2u windowSize, int HPMax, Vector2f velocity) : shootTimer(0)
	{
		this->HPMax = HPMax;
		this->HP = this->HPMax;
		this->veloctiy = velocity;
		this->shape.setTexture(*texture);
		this->shape.setScale(0.1f, 0.1f);
		this->shape.setPosition(rand() % (size_t)(windowSize.x - this->shape.getGlobalBounds().width), 0);
		this->shootInterval = 1000;
	}

	void updateShootTimer() {
		if (shootTimer < shootInterval) {
			shootTimer++;
		}
	}

	// Method to shoot bullets randomly
	void shootRandomBullet(Texture& bulletObj) {
		int randNum = rand() % 500;
		if (randNum == 1)
		{
			bullets.push_back(Bullet(&bulletObj, this->shape.getPosition()));
		}
	}

	void updateBullets(RenderWindow& window, Player& player) {
		// Move and check collision for each bullet
		for (size_t j = 0; j < this->bullets.size(); ) {
			this->bullets[j].shape.move(0.f, 2.f); // Adjust bullet speed as needed

			// Remove bullets that go out of bounds
			if (this->bullets[j].shape.getPosition().y > window.getSize().y) {
				this->bullets.erase(this->bullets.begin() + j);
			}
			else if (this->bullets[j].shape.getGlobalBounds().intersects(player.shape.getGlobalBounds()))
			{
				this->bullets.erase(this->bullets.begin() + j);
				player.HP--; // Player Got Damage
			}
			else {
				// Increment i if the bullet is still in bounds
				j++;
			}
		}
	}

};

int main()
{
	srand(time(NULL));

	VideoMode desktopMode = VideoMode::getDesktopMode();

	RenderWindow window(VideoMode(desktopMode.width, desktopMode.height), "Space Wars", Style::Fullscreen);
	window.setFramerateLimit(100);
	Texture menuPhoto;
	menuPhoto.loadFromFile("Textures/menubg.jpeg");
	Sprite menuBackground(menuPhoto);
	menuBackground.setScale(window.getSize().x / menuBackground.getLocalBounds().width, window.getSize().y / menuBackground.getLocalBounds().height);

	Texture gameScreenLevel;
	gameScreenLevel.loadFromFile("Textures/Photo1.jpg");
	Sprite gameBg(gameScreenLevel);
	gameBg.setScale(window.getSize().x / gameBg.getLocalBounds().width, window.getSize().y / gameBg.getLocalBounds().height);

	Texture gameScreenLevel1;
	gameScreenLevel1.loadFromFile("Textures/Photo1.jpg");

	Texture gameScreenLevel2;
	gameScreenLevel2.loadFromFile("Textures/Photo2.jpg");

	Texture gameOverPhoto;
	gameOverPhoto.loadFromFile("Textures/gameover2.jpeg");
	Sprite gameOverBg(gameOverPhoto);
	gameOverBg.setScale(window.getSize().x / gameOverBg.getLocalBounds().width, window.getSize().y / gameOverBg.getLocalBounds().height);

	// Init Text
	Font font;
	font.loadFromFile("Fonts/DeathStar-VmWB.ttf");

	// Init Textures
	Texture playerTex;
	playerTex.loadFromFile("Textures/Spaceship1.png");

	Texture enemyTex1;
	enemyTex1.loadFromFile("Textures/ship2.png");

	Texture enemyTex2;
	enemyTex2.loadFromFile("Textures/Spaceship2.png");

	Texture enemyTex3;
	enemyTex3.loadFromFile("Textures/Spaceship3.png");

	Texture bulletTex;
	bulletTex.loadFromFile("Textures/bullet.png");


	// Init Player
	Player player(&playerTex);

	int shootTimer = 15;
	Text hpText;
	hpText.setFont(font);
	hpText.setCharacterSize(20);
	hpText.setFillColor(Color::White);
	hpText.setPosition(desktopMode.width - 160.f, 10.f);




	int score = 0;
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(20);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(10.f, 10.f);

	Text spaceToContinue;
	spaceToContinue.setFont(font);
	spaceToContinue.setCharacterSize(20);
	spaceToContinue.setFillColor(Color::Cyan);
	spaceToContinue.setPosition(((window.getSize().x / 2) - 200), window.getSize().y - 100);
	spaceToContinue.setString("Press   SPACE   key for Main Menu...");

	int gameLavel = 1;
	int kills = 0;
	Text levelText;
	levelText.setFont(font);
	levelText.setCharacterSize(30);
	levelText.setFillColor(Color::Cyan);
	levelText.setPosition(((desktopMode.width - static_cast<float>(100)) / 2), (desktopMode.height / 2) - 30);

	Text aboutContent;
	aboutContent.setFont(font);
	aboutContent.setCharacterSize(20);
	aboutContent.setFillColor(Color::Cyan);
	aboutContent.setPosition(300, 300);
	aboutContent.setString("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tAbout the Game \n Welcome to Space Wars our space shooter game, developed as a project using C++ and the SFML library.\n In this action - packed adventure, you'll navigate through space, battling enemy ships and dodging asteroids. Developers:\nMuhammad Bachal : Implemented the game using the SFML library, ensuring smooth graphics and gameplay.\nMuhammad Shafique : Handled the core coding, bringing the game's mechanics and features to life.\nHammad Asghar : Conceived the original idea, providing the creative vision for the project. \nEnjoy an exciting journey through the cosmos in our meticulously crafted space�shooter�game");

	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(30);
	gameOverText.setFillColor(Color::Cyan);
	gameOverText.setPosition(((desktopMode.width - static_cast<float>(250)) / 2), (desktopMode.height - static_cast<float>(210)) / 2);
	gameOverText.setString("Game Over");


	Texture buttonBg;
	buttonBg.loadFromFile("Textures/buttonbg2.png");
	RectangleShape playRect;
	playRect.setSize(Vector2f(230.f, 60.f));
	playRect.setTexture(&buttonBg);

	Texture healthBarPhoto;
	healthBarPhoto.loadFromFile("Textures/healthBar.png");

	RectangleShape healthBar;
	healthBar.setSize(Vector2f(200.f, 60.f));
	healthBar.setTexture(&healthBarPhoto);
	healthBar.setPosition(Vector2f(6.f, window.getSize().y - 60.f));

	RectangleShape healthLine(Vector2f(200.f, 60.f));
	healthLine.setPosition(healthBar.getPosition().x + 17, healthBar.getPosition().y + 17);
	healthLine.setFillColor(Color::Green);



	playRect.setPosition(((desktopMode.width - static_cast<float>(230)) / 2), (desktopMode.height - static_cast<float>(210)) / 2);

	RectangleShape aboutRect;
	aboutRect.setSize(Vector2f(230.f, 60.f));
	aboutRect.setTexture(&buttonBg);
	aboutRect.setPosition(((desktopMode.width - static_cast<float>(230)) / 2), (desktopMode.height - static_cast<float>(90)) / 2);

	RectangleShape resumeRect;
	resumeRect.setSize(Vector2f(230.f, 60.f));
	resumeRect.setTexture(&buttonBg);
	resumeRect.setPosition(((desktopMode.width - static_cast<float>(230)) / 2), (desktopMode.height - static_cast<float>(210)) / 2);

	RectangleShape restartRect;
	restartRect.setSize(Vector2f(230.f, 60.f));
	restartRect.setTexture(&buttonBg);
	restartRect.setPosition(((desktopMode.width - static_cast<float>(230)) / 2), (desktopMode.height - static_cast<float>(90)) / 2);

	RectangleShape exitRect;
	exitRect.setSize(Vector2f(230.f, 60.f));
	exitRect.setTexture(&buttonBg);
	exitRect.setPosition(((desktopMode.width - static_cast<float>(230)) / 2), (desktopMode.height + static_cast<float>(30)) / 2);

	float playRectCenterX = playRect.getPosition().x + playRect.getLocalBounds().width / 2;

	Text playButtonText;
	playButtonText.setFont(font);
	playButtonText.setCharacterSize(30);
	playButtonText.setFillColor(Color::Cyan);
	playButtonText.setPosition(playRect.getPosition().x + 52, playRect.getPosition().y + 10);
	playButtonText.setString("    Play");

	float resumeRectCenterX = resumeRect.getPosition().x + resumeRect.getLocalBounds().width / 2;
	Text resumeButtonText;
	resumeButtonText.setFont(font);
	resumeButtonText.setCharacterSize(30);
	resumeButtonText.setFillColor(Color::Cyan);
	resumeButtonText.setPosition(resumeRect.getPosition().x + 52, resumeRect.getPosition().y + 10);
	resumeButtonText.setString(" Resume");

	float restartRectCenterX = aboutRect.getPosition().x + aboutRect.getLocalBounds().width / 2;
	Text newGameButtonText;
	newGameButtonText.setFont(font);
	newGameButtonText.setCharacterSize(30);
	newGameButtonText.setFillColor(Color::Cyan);
	newGameButtonText.setPosition(restartRect.getPosition().x + 52, restartRect.getPosition().y + 10);
	newGameButtonText.setString("Restart");

	float exitRectCenterX = exitRect.getPosition().x + exitRect.getLocalBounds().width / 2;
	Text exitButtonText;
	exitButtonText.setFont(font);
	exitButtonText.setCharacterSize(30);
	exitButtonText.setFillColor(Color::Cyan);
	exitButtonText.setPosition(exitRect.getPosition().x + 52, exitRect.getPosition().y + 10);
	exitButtonText.setString("      Exit");

	// Sounds

	SoundBuffer playerBulletSound;
	playerBulletSound.loadFromFile("Textures/bulletSound1.wav");
	Sound bulletSound;
	bulletSound.setBuffer(playerBulletSound);

	SoundBuffer gameOverSound;
	gameOverSound.loadFromFile("Textures/gameOver.wav");
	Sound gameSound;
	gameSound.setBuffer(gameOverSound);


	// Enemy Init
	int enemySpawnTimer = 0;
	std::vector <Enemy> enemies;

	Text eHpText;
	eHpText.setFont(font);
	eHpText.setCharacterSize(20);
	eHpText.setFillColor(Color::White);

	player.shape.setPosition(desktopMode.width, window.getSize().y - player.shape.getGlobalBounds().height);

	levelText.setString("Level  " + std::to_string(gameLavel));
	// Game State Variables 
	bool isPaused = false; // Add this variable outside of your main loop
	bool isRestartGame = true;
	bool loadNextLevel = false;
	int bulletLoadTime = 15;
	int enemyCounter = 0;
	bool firstKill = true;
	int wave = 1;
	bool isAbout = false;
	bool firstLoad = true;
	int waitWave = 20;
	Clock clock;
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
			{
				isPaused = !isPaused;
			}

			if (event.type == Event::MouseButtonPressed) {
				if (playButtonText.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
					// Handle play button click
					isPaused = false;
					isRestartGame = false;
				}
				if (resumeButtonText.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
					// Handle play button click
					isPaused = false;
				}
				if (newGameButtonText.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
					if (isRestartGame)
					{
						isAbout = true;
					}
					else {
						isRestartGame = true;
						isAbout = false;
					}
				}

				if (exitButtonText.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
					// Handle exit button click
					window.close();
				}

			}
		}

		// Draw Background Image if needed

		if (isAbout)
		{
			if (Keyboard::isKeyPressed(Keyboard::Space)) {
				isAbout = false;
			}
			window.clear();
			window.draw(aboutContent);
			window.draw(spaceToContinue);
			window.display();
			continue;
		}

		if (isRestartGame)
		{
			gameLavel = 1;
			kills = 0;
			score = 0;
			enemyCounter = 0;
			player.HP = player.HPMax;
			player.shape.setPosition(desktopMode.width, window.getSize().y - player.shape.getGlobalBounds().height);
			player.bullets.clear();
			enemies.clear();

			newGameButtonText.setString("  About");
			// Draw Background at menuus
			window.draw(menuBackground);
			gameScreenLevel = gameScreenLevel1;

			// Draw ButtonTexts
			window.draw(playRect);
			window.draw(exitRect);

			window.draw(aboutRect);
			window.draw(playButtonText);
			window.draw(newGameButtonText);
			window.draw(exitButtonText);
			window.display();
			isPaused = true;
			continue;

		}

		// PLayer Controls
		if (Keyboard::isKeyPressed(Keyboard::W))
			player.shape.move(0.f, -10.f);
		if (Keyboard::isKeyPressed(Keyboard::A))
			player.shape.move(-10.f, 0.f);
		if (Keyboard::isKeyPressed(Keyboard::S))
			player.shape.move(0.f, 10.f);
		if (Keyboard::isKeyPressed(Keyboard::D))
			player.shape.move(10.f, 0.f);

		// Get the current mouse position relative to the window
		Vector2i mousePosition = Mouse::getPosition(window);

		// Get the center position of the player
		Vector2i playerCenter = window.mapCoordsToPixel(player.shape.getPosition());

		// Calculate the direction vector from player to mouse position
		Vector2i direction = mousePosition - playerCenter;

		// Calculate the distance between player and mouse position
		float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

		// Define a minimum distance threshold
		float minDistance = 50.0f;  // Adjust this value as needed

		// Only move the player if the mouse is beyond the minimum distance threshold
		if (distance > minDistance) {
			// Normalize the direction vector
			float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
			Vector2f normalizedDirection(direction.x / magnitude, direction.y / magnitude);

			// Move the player shape towards the mouse position
			player.shape.move(normalizedDirection * 20.f);
		}


		// Collision with window
		if (player.shape.getPosition().x <= 0) // Left
			player.shape.setPosition(0.f, player.shape.getPosition().y);
		if (player.shape.getPosition().x >= window.getSize().x - player.shape.getGlobalBounds().width) // Right
			player.shape.setPosition(window.getSize().x - player.shape.getGlobalBounds().width, player.shape.getPosition().y);
		if (player.shape.getPosition().y <= 0) // Top
			player.shape.setPosition(player.shape.getPosition().x, 0.f);
		if (player.shape.getPosition().y >= window.getSize().y - player.shape.getGlobalBounds().height) // Bottom
			player.shape.setPosition(player.shape.getPosition().x, window.getSize().y - player.shape.getGlobalBounds().height);

		if (Keyboard::isKeyPressed(Keyboard::N)) {
			enemyCounter = 40;
		}


		// Levels
		if (loadNextLevel)
		{
			gameLavel++;
			wave = 1;
			enemyCounter = 0;
			if (gameLavel == 1)
			{
				firstKill = true;
				bulletLoadTime = 15;
			}
			if (gameLavel == 2)
			{
				enemies.clear();
				firstKill = true;
				gameScreenLevel = gameScreenLevel2;
				bulletLoadTime = 10;
			}
			if (gameLavel == 2)
			{
				enemies.clear();
				firstKill = true;
				gameScreenLevel = gameScreenLevel2;
				bulletLoadTime = 10;
			}
			levelText.setString("Level  " + std::to_string(gameLavel));
			loadNextLevel = false;
		}


		if (!isPaused && player.HP > 0)
		{
			//	cout << "Size" << enemies.size() << endl;
			window.clear();
			window.draw(gameBg);

			// Update
			if (shootTimer < bulletLoadTime)
				shootTimer++;

			if (Mouse::isButtonPressed(Mouse::Left) && shootTimer >= bulletLoadTime)
			{
				player.shootBullet(bulletTex);
				bulletSound.play();
				shootTimer = 0;
			}
			// Bullet
			for (size_t i = 0; i < player.bullets.size(); )
			{
				// Move
				player.bullets[i].shape.move(0.f, -20.f);

				// Out of Window bounds
				if (player.bullets[i].shape.getPosition().y > window.getSize().y)
				{
					player.bullets.erase(player.bullets.begin() + i);
					// Don't increment i here
				}
				else
				{
					// Enemy Collision
					bool bulletRemoved = false;
					for (size_t k = 0; k < enemies.size(); k++)
					{
						if (player.bullets[i].shape.getGlobalBounds().intersects(enemies[k].shape.getGlobalBounds()))
						{
							if (enemies[k].HP <= 1)
							{
								score += enemies[k].HPMax;
								enemies.erase(enemies.begin() + k);
								kills++;
							}
							else
								enemies[k].HP--;

							player.bullets.erase(player.bullets.begin() + i);
							bulletRemoved = true;
							break;
						}
					}
					if (!bulletRemoved)
					{
						// Increment i only if the bullet wasn't removed due to collision
						i++;
					}
				}
			}
			// Enemies
			if (gameLavel == 1)
			{
				if (enemySpawnTimer < 70)
				{
					enemySpawnTimer++;
				}
				if (enemySpawnTimer >= 70)
				{
					if (wave == 1)
					{
						if (enemyCounter > 3)
						{
							levelText.setString("");
						}
						enemies.push_back(Enemy(&enemyTex1, window.getSize(), 1, Vector2f(0.f, 1.f)));

						if (enemyCounter == 10)
						{
							wave = 2;
						}
					}

					else if (wave == 2)
					{
						enemies.push_back(Enemy(&enemyTex2, window.getSize(), 2, Vector2f(0.f, 1.f)));
						if (enemyCounter == 20)
						{
							wave = 3;
						}
					}
					else if (wave == 3)
					{
						enemies.push_back(Enemy(&enemyTex3, window.getSize(), 3, Vector2f(0.f, 1.f)));
						if (enemyCounter == 30)
						{
							wave = 4;
						}
					}
					else if (wave == 4)
					{
						int randNum = rand() % 4;
						if (randNum == 1)
						{
							enemies.push_back(Enemy(&enemyTex1, window.getSize(), 1, Vector2f(0.f, 1.f)));
						}
						else if (randNum == 2)
						{
							enemies.push_back(Enemy(&enemyTex2, window.getSize(), 2, Vector2f(0.f, 1.f)));
						}
						else if (randNum == 3)
						{
							enemies.push_back(Enemy(&enemyTex3, window.getSize(), 3, Vector2f(0.f, 1.f)));
						}
					}

					if (enemyCounter == 40)
					{
						loadNextLevel = true;
					}

					enemyCounter++;
					enemySpawnTimer = 0; // Reset enemy respawn timer
				}

				for (size_t i = 0; i < enemies.size(); )
				{
					enemies[i].shape.move(enemies[i].veloctiy);

					if (enemies[i].shape.getPosition().y - enemies[i].shape.getGlobalBounds().height > (window.getSize().y))
					{
						enemies.erase(enemies.begin() + i);
					}
					else if (enemies[i].shape.getGlobalBounds().intersects(player.shape.getGlobalBounds()))
					{
						enemies.erase(enemies.begin() + i);
						kills++;
						player.HP--; // Player Got Damage
					}
					else
					{
						i++;
					}
				}
			}
			else if (gameLavel == 2)
			{
				if (enemyCounter > 3)
				{
					levelText.setString("");
				}
				if (enemySpawnTimer < 20)
				{
					enemySpawnTimer++;
				}
				if (enemySpawnTimer >= 20)
				{
					if (wave == 1)
					{
						if (enemyCounter < 15)
						{
							enemies.push_back(Enemy(&enemyTex1, (desktopMode.width / 2) - (enemyCounter) * (45.f), 0.f, 0.f, 0.5f, 1, 0));
							enemies.push_back(Enemy(&enemyTex1, (desktopMode.width / 2) + (enemyCounter) * (45.f), 0.f, 0.f, 0.5f, 1, 0));
							enemyCounter += 2;
							if (enemyCounter == 16)
							{
								enemyCounter = 15;
							}
						}

						if (firstKill)
						{
							kills++;
							firstKill = false;
						}
						if (enemyCounter == 15)
						{
							wave = 2;
							clock.restart();
							waitWave = 20;
							cout << "Reset: " << clock.getElapsedTime().asSeconds() << endl;
						}
					}
					if (wave == 2)
					{
						if (enemyCounter < 30)
						{
							if (enemies.size() == 0)
							{
								waitWave = 3;
							}
							if (clock.getElapsedTime().asSeconds() >= waitWave) {
								enemies.push_back(Enemy(&enemyTex2, (desktopMode.width / 2) - (enemyCounter - 15) * (45.f), 0, 0.f, 0.5f, 2, 0));
								enemies.push_back(Enemy(&enemyTex2, (desktopMode.width / 2) + (enemyCounter - 15) * (45.f), 0, 0.f, 0.5f, 2, 0));
								enemyCounter += 2;
								if (enemyCounter == 31)
								{
									enemyCounter = 30;
								}
							}

						}

						if (firstKill)
						{
							kills++;
							firstKill = false;
						}
						if (enemyCounter == 30)
						{
							wave = 3;
							waitWave = 20;
							clock.restart();
						}
					}
					if (wave == 3)
					{
						if (enemyCounter < 45)
						{
							if (enemies.size() == 0)
							{
								waitWave = 3;
							}
							if (clock.getElapsedTime().asSeconds() >= waitWave) {
								enemies.push_back(Enemy(&enemyTex3, (desktopMode.width / 2) - (enemyCounter - 30) * (45.f), 0.f, 0.f, 0.5f, 3, 0));
								enemies.push_back(Enemy(&enemyTex3, (desktopMode.width / 2) + (enemyCounter - 30) * (45.f), 0.f, 0.f, 0.5f, 3, 0));
								enemyCounter++;
								enemyCounter++;
								if (enemyCounter == 46)
								{
									enemyCounter = 45;
								}
							}
						}

						if (firstKill)
						{
							kills++;
							firstKill = false;
						}
						if (enemyCounter == 45)
						{
							waitWave = 20;
							wave = 4;
							clock.restart();
						}
					}

					if (wave == 4)
					{
						if (enemySpawnTimer < 80)
						{
							enemySpawnTimer++;
						}
						if (enemySpawnTimer >= 80)
						{
							if (enemyCounter < 100)
							{
								if (enemies.size() == 0)
								{
									waitWave = 3;
								}
								if (clock.getElapsedTime().asSeconds() >= waitWave) {
									int randNum = rand() % 4;
									if (randNum == 1)
									{
										enemies.push_back(Enemy(&enemyTex1, rand() % (size_t)(window.getSize().x - 4), 0.f, 0.f, 0.5f, randNum, 0));

									}
									else if (randNum == 2)
									{
										enemies.push_back(Enemy(&enemyTex2, rand() % (size_t)(window.getSize().x - 4), 0.f, 0.f, 0.5f, randNum, 0));

									}
									else if (randNum == 3)
									{
										enemies.push_back(Enemy(&enemyTex3, rand() % (size_t)(window.getSize().x - 4), 0.f, 0.f, 0.5f, randNum, 0));
									}
									enemyCounter++;
									if (enemyCounter > 101)
									{
										enemyCounter = 100;
									}
								}
							}
							if (firstKill)
							{
								kills++;
								firstKill = false;
							}
							enemySpawnTimer = 0; // Reset enemy respawn timer
						}
						if (enemyCounter == 200)
						{
							loadNextLevel = true;
						}
					}

				}

				for (size_t i = 0; i < enemies.size(); )
				{
					enemies[i].shape.move(enemies[i].veloctiy.x, enemies[i].veloctiy.y);

					if (enemies[i].shape.getPosition().y <= 0 - enemies[i].shape.getGlobalBounds().height)
					{
						enemies.erase(enemies.begin() + i);
					}
					else if (enemies[i].shape.getGlobalBounds().intersects(player.shape.getGlobalBounds()))
					{
						enemies.erase(enemies.begin() + i);
						kills++;
						player.HP--; // Player Got Damage
					}
					else
					{
						i++;
					}
				}

				for (size_t i = 0; i < enemies.size(); i++) {
					enemies[i].updateBullets(window, player);
					// Update shoot timer for each enemy
					enemies[i].updateShootTimer();
					// Shoot bullets randomly for each enemy
					enemies[i].shootRandomBullet(bulletTex);

				}

			}
			else if (gameLavel == 3) {
				levelText.setString("To Be Continued....");
			}

			// UI

			scoreText.setString("Score: " + std::to_string(score));

			// Draw ===================

			// PLayer
			window.draw(player.shape);
			window.draw(healthBar);

			// Bullets
			for (size_t i = 0; i < player.bullets.size(); i++)
			{
				window.draw(player.bullets[i].shape);
			}

			for (size_t i = 0; i < enemies.size(); i++) {
				// Draw each enemy's bullets
				for (size_t j = 0; j < enemies[i].bullets.size(); j++) {
					window.draw(enemies[i].bullets[j].shape);
				}
			}
			// Enemies
			for (size_t i = 0; i < enemies.size(); i++)
			{
				window.draw(enemies[i].shape);
			}
			if (player.HP == player.HPMax)
			{
				healthLine.setFillColor(Color::Green);
			}
			if (player.HP < player.HPMax)
			{
				healthLine.setFillColor(Color::Yellow);
			}
			if (player.HP < 2)
				healthLine.setFillColor(Color::Red);
			healthLine.setSize(Vector2f((3300 / 100) * player.HP, 24.f));
			window.draw(healthLine);
			window.draw(scoreText);
			window.draw(levelText);
			if (player.HP <= 0)
			{
				gameSound.play();
			}
			window.display();
		}
		else
		{
			// If the game is paused or player's HP is zero, only draw UI elements like score, health, etc.
			newGameButtonText.setString("Restart");
			if (player.HP <= 0)
			{
				window.draw(gameOverBg);
				window.draw(scoreText);
				window.draw(spaceToContinue);
				window.draw(levelText);
				window.display();

				if (Keyboard::isKeyPressed(Keyboard::Space)) {
					isRestartGame = true;
				}

				continue;
			}

			// Draw Background at menuu
			window.draw(menuBackground);
			window.draw(resumeRect);
			window.draw(restartRect);
			window.draw(exitRect);

			window.draw(resumeButtonText);
			window.draw(newGameButtonText);
			window.draw(exitButtonText);
			window.display();
			continue;
		}

	}
	return 0;
}



