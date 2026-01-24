#include "TextureHolder.h"
// Подключаем функцию assert
#include <assert.h>
TextureHolder* TextureHolder::m_s_Instance = nullptr;
TextureHolder::TextureHolder()
{
	assert(m_s_Instance == nullptr);
	m_s_Instance = this;
}
Texture& TextureHolder::GetTexture(string const& filename)
{
	// Получаем ссылку на m_Textures, используя m_s_Instance
	auto& m = m_s_Instance->m_Textures;
	// auto эквивалентно map<string, Texture>
	// Создаем итератор для хранения пары "ключ — значение" (kvp)
	// и ищем нужную пару kvp,
	// используя переданное имя файла
	auto keyValuePair = m.find(filename);
	// auto эквивалентно map<string, Texture>::iterator
	// Мы нашли совпадение?
	if (keyValuePair != m.end())
	{
		// Да
		// Возвращаем текстуру,
		// вторую часть пары "ключ — значение" — текстуру
		return keyValuePair->second;
	}
	else
	{
		// Имя файла не найдено
		// Создаем новую пару "ключ — значение", используя имя файла
		auto& texture = m[filename];
		// Загружаем текстуру из файла как обычно
		texture.loadFromFile(filename);
		// Возвращаем текстуру в вызывающий код
		return texture;
	}
}