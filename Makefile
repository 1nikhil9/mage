all: mage

mage: main.cpp
mage: game.cpp
mage: shader.cpp
mage: texture.cpp
mage: resource_manager.cpp
mage: sprite_renderer.cpp
mage: entity.cpp
mage: level.cpp
mage: text_renderer.cpp
mage: ball.cpp
mage: utility.cpp
mage: glad.c
	g++ -o $@ $^ -lGL -lglfw -ldl -lSOIL -lfreetype -std=c++11 -g

clean:
	rm mage
