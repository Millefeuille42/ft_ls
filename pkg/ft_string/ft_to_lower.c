//
// Created by millefeuille on 4/24/23.
//

int	ft_tolower(int c) {
	if (c >= 'A' && c <= 'Z')
		return ('a' + (c - 'A'));
	return (c);
}
