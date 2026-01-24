#pragma once
#ifndef TEXTURE_HOLDER_H
#define TEXTURE_HOLDER_H
#include <SFML/Graphics.hpp>
#include <map>
using namespace sf;
using namespace std;
class TextureHolder
{
private:
	// Контейнер map из STL,
	// который хранит связанные пары String и Texture
	map<string, Texture> m_Textures;
	// Указатель того же типа, что и сам класс,
	// один-единственный экземпляр
	static TextureHolder* m_s_Instance;
public:
	TextureHolder();
	static Texture& GetTexture(string const& filename);
};
#endif