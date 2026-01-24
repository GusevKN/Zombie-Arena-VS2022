#include "ZombieArena.h"
int createBackground(VertexArray& rVA, IntRect arena)
{
	// Все, что мы делаем с rVA, мы в действительности делаем
	// с background (в функции function)
	// Какого размера каждая плитка/текстура
	const int TILE_SIZE = 50;
	const int TILE_TYPES = 3;
	const int VERTS_IN_QUAD = 4;
	int worldWidth = arena.width / TILE_SIZE;
	int worldHeight = arena.height / TILE_SIZE;
	// Какой тип примитива мы используем?
	rVA.setPrimitiveType(Quads);
	// Устанавливаем размер массива вершин
	rVA.resize(worldWidth * worldHeight * VERTS_IN_QUAD);
	// Стартуем с начала массива вершин
	int currentVertex = 0;
	for (int w = 0; w < worldWidth; w++)
	{
		for (int h = 0; h < worldHeight; h++)
		{
			// Устанавливаем позиции каждой вершины в текущем четырехугольнике
			rVA[currentVertex + 0].position =
				Vector2f(w * TILE_SIZE, h * TILE_SIZE);
			rVA[currentVertex + 1].position =
				Vector2f((w * TILE_SIZE) + TILE_SIZE, h * TILE_SIZE);
			rVA[currentVertex + 2].position =
				Vector2f((w * TILE_SIZE) + TILE_SIZE, (h * TILE_SIZE)
					+ TILE_SIZE);
			rVA[currentVertex + 3].position =
				Vector2f((w * TILE_SIZE), (h * TILE_SIZE)
					+ TILE_SIZE);
			// Определяем позицию в текстуре для текущего четырехугольника
			// Это могут быть трава, камень, куст или стена
			if (h == 0 || h == worldHeight - 1 ||
				w == 0 || w == worldWidth - 1)
			{
				// Используем текстуру стены
				rVA[currentVertex + 0].texCoords =
					Vector2f(0, 0 + TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 1].texCoords =
					Vector2f(TILE_SIZE, 0 + TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 2].texCoords =
					Vector2f(TILE_SIZE, TILE_SIZE + TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 3].texCoords =
					Vector2f(0, TILE_SIZE + TILE_TYPES * TILE_SIZE);
			}
			else
			{
				// Используем случайную текстуру пола
				srand((int)time(0) + h * w - h);
				int mOrG = (rand() % TILE_TYPES);
				int verticalOffset = mOrG * TILE_SIZE;
				rVA[currentVertex + 0].texCoords =
					Vector2f(0, 0 + verticalOffset);
				rVA[currentVertex + 1].texCoords =
					Vector2f(TILE_SIZE, 0 + verticalOffset);
				rVA[currentVertex + 2].texCoords =
					Vector2f(TILE_SIZE, TILE_SIZE + verticalOffset);
				rVA[currentVertex + 3].texCoords =
					Vector2f(0, TILE_SIZE + verticalOffset);
			}
			// Позиция, готовая для следующих четырех вершин
			currentVertex = currentVertex + VERTS_IN_QUAD;
		}
	}

	return TILE_SIZE;
}