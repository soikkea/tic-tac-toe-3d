#pragma once
class Cell
{
private:
	char content_;
	static const char EMPTY = ' ';
public:
	Cell();
	char GetContent() const;
	void SetContent(char c);
	bool IsEmpty();
};

