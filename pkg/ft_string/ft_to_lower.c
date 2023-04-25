//
// Created by millefeuille on 4/24/23.
//

int	ft_tolower(int c) {
    int condition = c >= 'A' && c <= 'Z';
	return condition * (c | 0x20) + !condition * c;
}
