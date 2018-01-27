#include "HexGrid.h"


sf::VertexArray HexGrid::GenerateHexGrid(sf::Vector2f startCoord, int r, int c, float s)
{
	sf::VertexArray gridVertices;
	gridVertices.setPrimitiveType(primitiveType);

	float height = s * 2;
	float width = sqrt(3) / 2 * height;

	float vertDist = height * 0.75;

	int curRow = 0;
	int curCol = 0;
	bool offsetRow = false;
	while (curRow < r)
	{
		curCol = 0;
		while (curCol < c)
		{
			sf::VertexArray newHex(primitiveType, 7);
			sf::Vector2f thisCoord;

			thisCoord.x = startCoord.x + (curCol * width);
			if (offsetRow)
				thisCoord.x -= (width / 2);
			thisCoord.y = startCoord.y + (curRow * vertDist);

			newHex = GenerateHex(thisCoord, s, offsetRow);
			
			for (int i = 0; i < newHex.getVertexCount(); i++)
			{
				gridVertices.append(newHex[i]);
			}
			
			curCol++;
		}
		curRow++;
		offsetRow = !offsetRow;
	}
	int num = gridVertices.getVertexCount();
	return gridVertices;
}

sf::VertexArray HexGrid::GenerateHex(sf::Vector2f center, float size, bool offset = false)
{

	sf::VertexArray hexVertices(primitiveType, 7);
	
	for (int i = 0; i < 7; i++)
	{
		hexVertices[i].position = hexCorner(center, size, i);
		hexVertices[i].color = sf::Color::Red;

		if (i == 0 || i == 6)
			hexVertices[i].color = sf::Color::Black;

	}

	return hexVertices;
}



sf::VertexArray HexGrid::GenerateHexGrid()
{	
	return GenerateHexGrid(origin, rows, columns, cellSize);
}


sf::Vector2f HexGrid::hexCorner(sf::Vertex center, float size, int i)
{
	sf::Vector2f corner;
	int angle_deg = 60 * i + 30;
	float angle_rad = M_PI / 180 * angle_deg;
	corner.x = (center.position.x + size * cos(angle_rad));
	corner.y = (center.position.y + size * sin(angle_rad));

	return corner;
}

sf::Vector2f HexGrid::cube_to_offset(sf::Vector3f cube)
{
    sf::Vector2f evenr;

    evenr.x = cube.x + (cube.z + ((int)cube.z & 1)) / 2;
    evenr.y = cube.z;
	
    return evenr;
}

sf::Vector3f HexGrid::offset_to_cube(sf::Vector2f evenr) // even-r
{
    sf::Vector3f cube;

    cube.x = evenr.x - (evenr.y + ((int)evenr.y&1)) / 2;
    cube.z = evenr.y;
    cube.y = -cube.x - cube.z;

	return cube;
}

sf::Vector2f HexGrid::offset_to_pixel(sf::Vector2f offset)
{
    sf::Vector2f coord;

    coord.x = cellSize * sqrt(3) * (offset.x - 0.5 * ( (int) offset.y & 1));
    coord.y = cellSize * 3.0 / 2.0 * offset.y;

    return coord;

}

sf::Vector3f HexGrid::cube_round(sf::Vector3f to_round)
{
    float rx = round(to_round.x);
    float ry = round(to_round.y);
    float rz = round(to_round.z);

    float x_diff = abs(rx - to_round.x);
    float y_diff = abs(ry - to_round.y);
    float z_diff = abs(rz - to_round.z);

    if (x_diff > y_diff && x_diff > z_diff)
        rx = -ry - rz;
    else if (y_diff > z_diff)
        ry = -rx - rz;
    else
        rz = -rx - ry;

    return sf::Vector3f(rx, ry, rz);
}

sf::Vector2f HexGrid::hex_round(sf::Vector2f to_round)
{
    return cube_to_offset(cube_round(offset_to_cube(to_round)));
}

sf::Vector2f HexGrid::pixel_to_offset(sf::Vector2f pixel)
{
    sf::Vector2f offset;
    
    offset.x = (pixel.x * sqrt(3.0)/3.0 - pixel.y / 3.0) / cellSize;
    offset.y = pixel.y * (2.0/3.0) / cellSize;

    return cube_to_offset(cube_round(sf::Vector3f(offset.x, -offset.x-offset.y, offset.y)));
}

sf::Vector2f HexGrid::getOrigin()
{
    return origin;
}

	HexGrid::HexGrid()
{
		origin.x = 0;
		origin.y = 0;
		rows = 0;
		columns = 0;
		cellSize = 0;
		primitiveType = sf::Points;
}
	
	HexGrid::HexGrid(int x, int y, int r, int c, float s, sf::PrimitiveType pt)
{
		origin.x = x;
		origin.y = y;

		rows = r;
		columns = c;

		cellSize = s;

		primitiveType = pt;
}


void HexGrid::SetPrimitiveType(sf::PrimitiveType pt)
{
	primitiveType = pt;
	return;
}

sf::PrimitiveType HexGrid::GetPrimitiveType()
{
	return primitiveType;
}

int HexGrid::getRows()
{
    return this->rows;
}

int HexGrid::getCols()
{
    return this->columns;
}