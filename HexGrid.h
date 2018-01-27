#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <math.h>


#ifndef HEXGRID_H
#define HEXGRID_H

class HexGrid
{
public:
	HexGrid();
	HexGrid(int, int, int, int, float, sf::PrimitiveType);

	sf::PrimitiveType GetPrimitiveType();
	void SetPrimitiveType(sf::PrimitiveType pt);

	sf::VertexArray GenerateHex(sf::Vector2f center, float size, bool);
	sf::VertexArray GenerateHexGrid();
	sf::VertexArray GenerateHexGrid(sf::Vector2f, int, int, float);
    sf::Vector2f offset_to_pixel(sf::Vector2f);
    sf::Vector2f pixel_to_offset(sf::Vector2f);
    
    int getRows();
    int getCols();
    sf::Vector2f getOrigin();
    
private:
    sf::Vector2f cube_to_offset(sf::Vector3f);
    sf::Vector3f offset_to_cube(sf::Vector2f); // even-r

    sf::Vector2f origin;

	int rows;
	int columns;

	sf::PrimitiveType primitiveType;

	
	float cellSize;

	sf::Vector2f hexCorner(sf::Vertex center, float size, int i);

    sf::Vector2f hex_round(sf::Vector2f to_round);
    sf::Vector3f cube_round(sf::Vector3f to_round);




};
#endif // !HEXGRID_H
