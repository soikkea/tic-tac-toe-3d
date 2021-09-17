#include "Cell.h"

Cell::Cell() : content_{ EMPTY }
{
}

char Cell::GetContent() const
{
	return content_;
}

void Cell::SetContent(char c)
{
	content_ = c;
}

bool Cell::IsEmpty() const
{
	return content_ == EMPTY;
}
