#include <iostream>
#include <SFML/Graphics.hpp>
#define TINYC2_IMPL
#include <tinyc2.h>
#include <AnimatedSprite.h>
#include <Player.h>
#include <Input.h>
#include <string.h>
#include <Debug.h>

using namespace std;


int main()
{
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

	// Load a sprite to display
	sf::Texture sprite_sheet;
	if (!sprite_sheet.loadFromFile("assets\\grid.png")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}

	// Load a mouse texture to display
	sf::Texture mouse_texture;
	if (!mouse_texture.loadFromFile("assets\\mouse.png")) {
		DEBUG_MSG("Failed to load file");
		return EXIT_FAILURE;
	}

	int mouseShape = 0;
	int playerShape = 0;

	enum shapes
	{
		VECTOR,
		CIRCLE,
		CAPSULE,
		POLY,
		RAY
	};

	// Setup a mouse Sprite
	sf::Sprite mouse;
	mouse.setTexture(mouse_texture);

	//Setup mouse AABB
	c2AABB aabb_mouse;
	aabb_mouse.min = c2V(mouse.getPosition().x, mouse.getPosition().y);
	aabb_mouse.max = c2V(mouse.getGlobalBounds().width, mouse.getGlobalBounds().width);

	//Setup mouse Circle
	//c2Circle circle_mouse;
	//circle_mouse.p = { mouse.getPosition().x, mouse.getPosition().y };
	//circle_mouse.r = 42.f;

	////Setup mouse Ray
	//c2Ray ray_mouse;
	//ray_mouse.p = c2V(mouse.getPosition().x, mouse.getPosition().y);
	//ray_mouse.d = c2V(mouse.getPosition().x +84, mouse.getPosition().y+84);
	//ray_mouse.t = 5.f;


	// Setup Players Default Animated Sprite
	AnimatedSprite animated_sprite(sprite_sheet);
	animated_sprite.addFrame(sf::IntRect(3, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(88, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(173, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(258, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(343, 3, 84, 84));
	animated_sprite.addFrame(sf::IntRect(428, 3, 84, 84));

	AnimatedSprite circle_sprite(sprite_sheet);
	circle_sprite.addFrame(sf::IntRect(3, 88, 84, 84));
	circle_sprite.addFrame(sf::IntRect(88, 88, 84, 84));
	circle_sprite.addFrame(sf::IntRect(173, 88, 84, 84));
	circle_sprite.addFrame(sf::IntRect(258, 88, 84, 84));
	circle_sprite.addFrame(sf::IntRect(343, 88, 84, 84));
	circle_sprite.addFrame(sf::IntRect(428, 88, 84, 84));

	AnimatedSprite capsule_sprite(sprite_sheet);
	capsule_sprite.addFrame(sf::IntRect(3, 173, 84, 84));
	capsule_sprite.addFrame(sf::IntRect(88, 173, 84, 84));
	capsule_sprite.addFrame(sf::IntRect(173, 173, 84, 84));
	capsule_sprite.addFrame(sf::IntRect(258, 173, 84, 84));
	capsule_sprite.addFrame(sf::IntRect(343, 173, 84, 84));
	capsule_sprite.addFrame(sf::IntRect(428, 173, 84, 84));

	// Setup Players AABB
	c2AABB aabb_player;
	aabb_player.min = c2V(animated_sprite.getPosition().x, animated_sprite.getPosition().y);
	aabb_player.max = c2V(animated_sprite.getGlobalBounds().width / animated_sprite.getFrames().size(), 
	animated_sprite.getGlobalBounds().height / animated_sprite.getFrames().size());

	c2Circle circle_player;
	circle_player.p = { circle_sprite.getPosition().x+42,circle_sprite.getPosition().y+42 };
	circle_player.r = 42.f;

	c2Capsule capsule_player;
	capsule_player.a = c2V(capsule_sprite.getPosition().x, capsule_sprite.getPosition().y);
	capsule_player.b = c2V(capsule_sprite.getPosition().x, capsule_sprite.getPosition().y); 
	capsule_player.r = 1.f;

	//c2Ray ray_player;
	//ray_player.p = c2V(0, 0);
	//ray_player.d = c2V(1, 1);
	//ray_player.t = 5.f;

	/*c2Poly poly_player;
	poly_player.count = 6;*/
	//poly_player.norms = 6;
	//poly_player.verts = 6;

	// Setup the Player
	Player player(animated_sprite);
	Player circle(circle_sprite);
	Player capsule(capsule_sprite);
//	Player ray();
	//Input input;

	// Collision result
	int result = 0;
	
	// Start the game loop
	while (window.isOpen())
	{
		// Move Sprite Follow Mouse
		mouse.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));

		// Update mouse AABB
		aabb_mouse.min = c2V(mouse.getPosition().x, mouse.getPosition().y);
		aabb_mouse.max = c2V(mouse.getGlobalBounds().width, mouse.getGlobalBounds().width);

		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				// Close window : exit
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					//input.setCurrent(Input::Action::LEFT);
					if (playerShape > 0 && playerShape <= 4)
					{
						playerShape--;
					}
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					//input.setCurrent(Input::Action::RIGHT);
					if (playerShape >= 0 && playerShape < 4)
					{
						playerShape++;
					}
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					//input.setCurrent(Input::Action::UP);
					if (playerShape >= 0 && playerShape < 4)
					{
						mouseShape++;
					}
				}
				else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				{
					if (playerShape > 0 && playerShape <= 4)
					{
						mouseShape--;
					}
				}
				break;
			default:
				//input.setCurrent(Input::Action::IDLE);
				break;
			}
		}

		// Handle input to Player
		//player.handleInput(input);						//STUFF FOR CHANGING STATE

		// Update the Player
		player.update();

		if (playerShape == VECTOR && mouseShape == VECTOR)
		{
			//Check for collisions between 2 vectors
			result = c2AABBtoAABB(aabb_mouse, aabb_player);
		}
		else if(playerShape == VECTOR && mouseShape == CIRCLE)
		{
			// Check for collisions between a vector and a circle
			result = c2CircletoAABB(circle_player, aabb_mouse);
		}
		else if (playerShape == VECTOR && mouseShape == CAPSULE)
		{
			//Check for collisions between a vector and a capsule
			result = c2AABBtoCapsule(aabb_mouse, capsule_player);
		}
		else if (playerShape == VECTOR && mouseShape == POLY)
		{
			//Check for collisions between a vector and a poly
			//result = c2AABBtoPoly();
		}
		else if (playerShape == VECTOR && mouseShape == RAY)
		{
			//Check for collisions between a vector and a ray
			//result = c2RaytoAABB();
		}
		else if (playerShape == CIRCLE && mouseShape == CIRCLE)
		{
			//Check for collisions between a circle and a circle
			//result = c2CircletoCircle();
		}
		else if (playerShape == CIRCLE && mouseShape == CAPSULE)
		{
			//Check for collisions between a circle and a capsule
			//result = c2CircletoCapsule();
		}
		else if (playerShape == CIRCLE && mouseShape == POLY)
		{
			//Check for collisions between a circle and a poly
			//result = c2CircletoPoly();
		}
		else if (playerShape == CIRCLE && mouseShape == RAY)
		{
			//Check for collisions a circle and a ray
			//result = c2RaytoCircle();
		}
		else if (playerShape == CAPSULE && mouseShape == RAY)
		{ 
			//Check for collisions a capsule and a ray
			//result = c2RaytoCapsule();
		}
		else if (playerShape == POLY && mouseShape == RAY)
		{
			//Check for collisions a poly and a ray
			//result = c2RaytoPoly();
		}
		
		

		cout << ((result != 0) ? ("Collision") : "") << endl;

		if (result)
		{	
			
			player.getAnimatedSprite().setColor(sf::Color(255,0,0));
			circle.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
			capsule.getAnimatedSprite().setColor(sf::Color(255, 0, 0));
		}
		else 
		{
			player.getAnimatedSprite().setColor(sf::Color(0, 255, 0)); 
			circle.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
			capsule.getAnimatedSprite().setColor(sf::Color(0, 255, 0));
		}


	

		// Clear screen
		window.clear(sf::Color(255,255,255));

		// Draw the Players Current Animated Sprite
		if (playerShape == 0)
		{
			window.draw(player.getAnimatedSprite());
		}
		else if (playerShape == 1)
		{
			window.draw(circle.getAnimatedSprite());
		}
		else if (playerShape == 2)
		{
			window.draw(capsule.getAnimatedSprite());
		}
		
		//Will be changed later
		window.draw(mouse);

		// Update the window
		window.display();
	}

	return EXIT_SUCCESS;
};